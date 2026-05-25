#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <string>
#include <sstream>

#include "../json/json.hpp"
#include "../include/Library.h"
#include "../include/Protocol.h"

using json = nlohmann::json;

std::string handleRequest(const std::string& raw, Library& lib) {
    json resp;
    try {
        json req = json::parse(raw);
        std::string action = req.value("action", "");

        if (action == "ping") {
            resp["status"]  = "ok";
            resp["message"] = "pong";
        }

        else if (action == "list_items") {
            auto items = lib.getAllItemsSorted();
            json arr = json::array();
            for (LibraryItem* it : items) {
                json obj;
                obj["id"]        = it->getId();
                obj["type"]      = it->getType();
                obj["title"]     = it->getTitle();
                obj["author"]    = it->getAuthor();
                obj["year"]      = it->getYear();
                obj["available"] = it->getAvailable();
                obj["info"]      = it->getInfo();
                arr.push_back(obj);
            }
            resp["status"] = "ok";
            resp["data"]   = arr;
            resp["message"] = std::to_string(arr.size()) + " item ditemukan";
        }

        else if (action == "search") {
            std::string query = req["data"].value("query", "");
            if (query.empty()) {
                resp["status"]  = "error";
                resp["message"] = "Query tidak boleh kosong";
            } else {
                auto results = lib.searchByTitle(query);
                json arr = json::array();
                for (LibraryItem* it : results) {
                    json obj;
                    obj["id"]        = it->getId();
                    obj["type"]      = it->getType();
                    obj["title"]     = it->getTitle();
                    obj["author"]    = it->getAuthor();
                    obj["year"]      = it->getYear();
                    obj["available"] = it->getAvailable();
                    arr.push_back(obj);
                }
                resp["status"]  = "ok";
                resp["data"]    = arr;
                resp["message"] = std::to_string(arr.size()) + " hasil untuk \"" + query + "\"";
            }
        }

        else if (action == "borrow") {
            int memberId = req["data"].value("member_id", -1);
            int itemId   = req["data"].value("item_id",   -1);
            std::string result = lib.borrowItem(memberId, itemId);
            resp["status"]  = (result.substr(0,2) == "OK") ? "ok" : "error";
            resp["message"] = result;
        }

        else if (action == "return_item") {
            int memberId = req["data"].value("member_id", -1);
            int itemId   = req["data"].value("item_id",   -1);
            std::string result = lib.returnItem(memberId, itemId);
            resp["status"]  = (result.substr(0,2) == "OK") ? "ok" : "error";
            resp["message"] = result;
        }

        else if (action == "list_members") {
            auto mems = lib.getAllMembers();
            json arr = json::array();
            for (Member* m : mems) {
                json obj;
                obj["id"]    = m->getMemberId();
                obj["name"]  = m->getName();
                obj["email"] = m->getEmail();
                obj["borrowed_count"] = (int)m->getBorrowedIds().size();
                json bids = json::array();
                for (int bid : m->getBorrowedIds()) bids.push_back(bid);
                obj["borrowed_ids"] = bids;
                arr.push_back(obj);
            }
            resp["status"]  = "ok";
            resp["data"]    = arr;
            resp["message"] = std::to_string(arr.size()) + " member terdaftar";
        }

        else if (action == "add_book") {
            auto& d = req["data"];
            int id = lib.addBook(
                d.value("title",  "Unknown"),
                d.value("author", "Unknown"),
                d.value("year",   2024),
                d.value("isbn",   "-"),
                d.value("genre",  "Umum")
            );
            resp["status"]  = "ok";
            resp["message"] = "Buku ditambahkan dengan ID " + std::to_string(id);
            resp["data"]    = { {"id", id} };
        }

        else if (action == "add_member") {
            auto& d = req["data"];
            int id = lib.addMember(
                d.value("name",  "Anonymous"),
                d.value("email", "")
            );
            resp["status"]  = "ok";
            resp["message"] = "Member ditambahkan dengan ID " + std::to_string(id);
            resp["data"]    = { {"id", id} };
        }

        else if (action == "remove_item") {
            int itemId = req["data"].value("item_id", -1);
            bool ok    = lib.removeItem(itemId);
            resp["status"]  = ok ? "ok" : "error";
            resp["message"] = ok ? "Item dihapus" : "Gagal menghapus item (tidak ada atau sedang dipinjam)";
        }

        else {
            resp["status"]  = "error";
            resp["message"] = "Action tidak dikenal: " + action;
        }

    } catch (const json::parse_error& e) {
        resp["status"]  = "error";
        resp["message"] = std::string("JSON parse error: ") + e.what();
    } catch (const std::exception& e) {
        resp["status"]  = "error";
        resp["message"] = std::string("Server error: ") + e.what();
    }

    return resp.dump() + Protocol::DELIMITER;
}

std::string recvLine(SOCKET sock) {
    std::string buf;
    char c;
    while (true) {
        int n = recv(sock, &c, 1, 0);
        if (n <= 0) break;
        if (c == Protocol::DELIMITER) break;
        buf += c;
    }
    return buf;
}

int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup gagal\n";
        return 1;
    }

    SOCKET serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock == INVALID_SOCKET) {
        std::cerr << "Gagal membuat socket\n";
        WSACleanup();
        return 1;
    }

    int opt = 1;
    setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR,
               (char*)&opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(Protocol::SERVER_PORT);

    if (bind(serverSock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "Bind gagal\n";
        closesocket(serverSock);
        WSACleanup();
        return 1;
    }

    listen(serverSock, 1);
    std::cout << "=== Library Server (Elektro - 1 Client) ===\n";
    std::cout << "Listening on port " << Protocol::SERVER_PORT << "...\n\n";

    Library lib;
    std::cout << "Database siap: " << lib.getItemCount()
              << " item, " << lib.getMemberCount() << " member\n\n";

    while (true) {
        sockaddr_in clientAddr{};
        int addrLen = sizeof(clientAddr);
        SOCKET clientSock = accept(serverSock,
                                   (sockaddr*)&clientAddr, &addrLen);
        if (clientSock == INVALID_SOCKET) continue;

        char ipBuf[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, ipBuf, sizeof(ipBuf));
        std::cout << "[+] Client terhubung dari " << ipBuf << "\n";

        while (true) {
            std::string line = recvLine(clientSock);
            if (line.empty()) break;

            std::cout << "[REQ] " << line << "\n";
            std::string response = handleRequest(line, lib);
            std::cout << "[RES] " << response;

            int sent = send(clientSock, response.c_str(),
                            (int)response.size(), 0);
            if (sent == SOCKET_ERROR) break;
        }

        std::cout << "[-] Client disconnect\n\n";
        closesocket(clientSock);
    }

    closesocket(serverSock);
    WSACleanup();
    return 0;
}
