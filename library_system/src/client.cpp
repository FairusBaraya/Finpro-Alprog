#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <string>
#include <iomanip>

#include "../json/json.hpp"
#include "../include/Protocol.h"

using json = nlohmann::json;

SOCKET g_sock = INVALID_SOCKET;

std::string sendRequest(const json& req) {
    std::string msg = req.dump() + Protocol::DELIMITER;
    if (send(g_sock, msg.c_str(), (int)msg.size(), 0) == SOCKET_ERROR)
        return "";

    std::string buf;
    char c;
    while (true) {
        int n = recv(g_sock, &c, 1, 0);
        if (n <= 0) break;
        if (c == Protocol::DELIMITER) break;
        buf += c;
    }
    return buf;
}

void printResponse(const std::string& raw) {
    if (raw.empty()) { std::cout << "[!] Tidak ada response dari server\n"; return; }
    try {
        json resp = json::parse(raw);
        std::string status = resp.value("status", "?");
        std::string msg    = resp.value("message", "");

        if (status == "ok")
            std::cout << "\n[OK] " << msg << "\n";
        else
            std::cout << "\n[ERR] " << msg << "\n";

        if (resp.contains("data") && resp["data"].is_array()) {
            std::cout << std::string(60, '-') << "\n";
            for (auto& item : resp["data"]) {
                if (item.contains("title")) {
                    
                    std::cout << std::setw(4) << item.value("id", 0)
                              << " | " << std::left << std::setw(8)
                              << item.value("type", "")
                              << " | " << std::setw(35)
                              << item.value("title", "")
                              << " | " << std::setw(22)
                              << item.value("author", "")
                              << " | " << item.value("year", 0)
                              << " | " << (item.value("available", true)
                                           ? "Tersedia " : "Dipinjam ")
                              << "\n";
                } else if (item.contains("name")) {
                    
                    std::cout << std::setw(4) << item.value("id", 0)
                              << " | " << std::left << std::setw(25)
                              << item.value("name", "")
                              << " | " << std::setw(28)
                              << item.value("email", "")
                              << " | Pinjaman: "
                              << item.value("borrowed_count", 0)
                              << "\n";
                }
            }
            std::cout << std::string(60, '-') << "\n";
        }
    } catch (...) {
        std::cout << "[RAW] " << raw << "\n";
    }
}


void menuListItems() {
    json req = { {"action", "list_items"} };
    printResponse(sendRequest(req));
}

void menuSearch() {
    std::string q;
    std::cout << "Masukkan judul/kata kunci: ";
    std::cin.ignore();
    std::getline(std::cin, q);
    json req = { {"action", "search"}, {"data", { {"query", q} }} };
    printResponse(sendRequest(req));
}

void menuBorrow() {
    int mid, iid;
    std::cout << "ID Member : "; std::cin >> mid;
    std::cout << "ID Item   : "; std::cin >> iid;
    json req = {
        {"action", "borrow"},
        {"data", { {"member_id", mid}, {"item_id", iid} }}
    };
    printResponse(sendRequest(req));
}

void menuReturn() {
    int mid, iid;
    std::cout << "ID Member : "; std::cin >> mid;
    std::cout << "ID Item   : "; std::cin >> iid;
    json req = {
        {"action", "return_item"},
        {"data", { {"member_id", mid}, {"item_id", iid} }}
    };
    printResponse(sendRequest(req));
}

void menuListMembers() {
    json req = { {"action", "list_members"} };
    printResponse(sendRequest(req));
}

void menuAddBook() {
    std::string title, author, isbn, genre;
    int year;
    std::cin.ignore();
    std::cout << "Judul  : "; std::getline(std::cin, title);
    std::cout << "Penulis: "; std::getline(std::cin, author);
    std::cout << "Tahun  : "; std::cin >> year;
    std::cin.ignore();
    std::cout << "ISBN   : "; std::getline(std::cin, isbn);
    std::cout << "Genre  : "; std::getline(std::cin, genre);
    json req = {
        {"action", "add_book"},
        {"data", {
            {"title",  title},  {"author", author},
            {"year",   year},   {"isbn",   isbn},
            {"genre",  genre}
        }}
    };
    printResponse(sendRequest(req));
}

void menuAddMember() {
    std::string name, email;
    std::cin.ignore();
    std::cout << "Nama  : "; std::getline(std::cin, name);
    std::cout << "Email : "; std::getline(std::cin, email);
    json req = {
        {"action", "add_member"},
        {"data", { {"name", name}, {"email", email} }}
    };
    printResponse(sendRequest(req));
}

void menuRemoveItem() {
    int iid;
    std::cout << "ID Item yang akan dihapus: "; std::cin >> iid;
    json req = {
        {"action", "remove_item"},
        {"data", { {"item_id", iid} }}
    };
    printResponse(sendRequest(req));
}


int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    g_sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(Protocol::SERVER_PORT);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    std::cout << "=== Library Client ===\n";
    std::cout << "Menghubungkan ke server...\n";

    if (connect(g_sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "Gagal terhubung ke server. Pastikan server.exe sudah berjalan.\n";
        WSACleanup();
        return 1;
    }
    std::cout << "Terhubung!\n\n";

    json ping = { {"action", "ping"} };
    printResponse(sendRequest(ping));

    bool running = true;
    while (running) {
        std::cout << "\n+==================================+\n";
        std::cout <<   "|  SISTEM MANAJEMEN PERPUSTAKAAN  |\n";
        std::cout <<   "+==================================+\n";
        std::cout <<   "|  1. Lihat semua koleksi         |\n";
        std::cout <<   "|  2. Cari koleksi                |\n";
        std::cout <<   "|  3. Pinjam item                 |\n";
        std::cout <<   "|  4. Kembalikan item             |\n";
        std::cout <<   "|  5. Daftar member               |\n";
        std::cout <<   "|  6. Tambah buku (admin)         |\n";
        std::cout <<   "|  7. Tambah member               |\n";
        std::cout <<   "|  8. Hapus item (admin)          |\n";
        std::cout <<   "|  0. Keluar                      |\n";
        std::cout <<   "+==================================+\n";
        std::cout <<   "Pilihan: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: menuListItems();   break;
            case 2: menuSearch();      break;
            case 3: menuBorrow();      break;
            case 4: menuReturn();      break;
            case 5: menuListMembers(); break;
            case 6: menuAddBook();     break;
            case 7: menuAddMember();   break;
            case 8: menuRemoveItem();  break;
            case 0: running = false;   break;
            default: std::cout << "Pilihan tidak valid\n";
        }
    }

    closesocket(g_sock);
    WSACleanup();
    std::cout << "\nTerima kasih!\n";
    return 0;
}
