#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "LibraryItem.h"
#include "Book.h"
#include "Magazine.h"
#include "Member.h"
#include "LinkedList.h"
#include "Algorithms.h"

// ============================================================
//  Library — mengelola koleksi item dan member
//
//  Menggunakan LinkedList<LibraryItem*> manual untuk koleksi
//  dan LinkedList<Member*> untuk member.
//
//  Big-O ringkasan:
//    addItem()          : O(1)
//    addMember()        : O(1)
//    findItemById()     : O(n)
//    findMemberById()   : O(n)
//    searchItems()      : O(n log n) sort + O(log n) binary search
//    borrowItem()       : O(n)
//    returnItem()       : O(n)
//    listAll()          : O(n)
// ============================================================
class Library {
private:
    LinkedList<LibraryItem*> items;    // BONUS: manual linked list
    LinkedList<Member*>      members;
    int nextItemId;
    int nextMemberId;

    // Cari item berdasarkan ID — O(n)
    LibraryItem* findItemById(int id) const {
        for (LibraryItem* item : items)
            if (item->getId() == id) return item;
        return nullptr;
    }

    // Cari member berdasarkan ID — O(n)
    Member* findMemberById(int id) const {
        for (Member* m : members)
            if (m->getMemberId() == id) return m;
        return nullptr;
    }

public:
    Library() : nextItemId(1), nextMemberId(1) {
        // Seed data awal
        addBook("Laskar Pelangi", "Andrea Hirata", 2005,
                "978-979-1260-00-1", "Fiksi");
        addBook("Bumi Manusia", "Pramoedya Ananta Toer", 1980,
                "978-979-407-481-1", "Fiksi Sejarah");
        addBook("Filosofi Teras", "Henry Manampiring", 2018,
                "978-602-03-8035-7", "Non-fiksi");
        addBook("Atomic Habits", "James Clear", 2018,
                "978-0-7352-1129-5", "Pengembangan Diri");
        addBook("Clean Code", "Robert C. Martin", 2008,
                "978-0-13-235088-4", "Teknologi");
        addMagazine("National Geographic", "NGS", 2024,
                    "National Geographic Society", 201);
        addMagazine("Popular Science", "Various", 2023,
                    "Bonnier Corp", 180);
        addMember("Alice Wijaya",  "alice@email.com");
        addMember("Budi Santoso",  "budi@email.com");
        addMember("Citra Dewi",    "citra@email.com");
    }

    ~Library() {
        for (LibraryItem* item : items) delete item;
        for (Member* m : members)       delete m;
    }

    // ---- CRUD Item ----------------------------------------

    int addBook(const std::string& title, const std::string& author,
                int year, const std::string& isbn,
                const std::string& genre) {
        int id = nextItemId++;
        items.pushBack(new Book(id, title, author, year, isbn, genre));
        return id;
    }

    int addMagazine(const std::string& title, const std::string& author,
                    int year, const std::string& publisher, int edition) {
        int id = nextItemId++;
        items.pushBack(new Magazine(id, title, author, year,
                                    publisher, edition));
        return id;
    }

    bool removeItem(int id) {
        LibraryItem* target = findItemById(id);
        if (!target) return false;
        if (!target->getAvailable()) return false; // sedang dipinjam
        items.removeIf([id](LibraryItem* it){ return it->getId() == id; });
        delete target;
        return true;
    }

    // ---- CRUD Member --------------------------------------

    int addMember(const std::string& name, const std::string& email) {
        int id = nextMemberId++;
        members.pushBack(new Member(id, name, email));
        return id;
    }

    // ---- Transaksi ----------------------------------------

    // Pinjam buku — O(n)
    std::string borrowItem(int memberId, int itemId) {
        Member*      m = findMemberById(memberId);
        LibraryItem* it = findItemById(itemId);
        if (!m)              return "ERROR: Member tidak ditemukan";
        if (!it)             return "ERROR: Item tidak ditemukan";
        if (!it->getAvailable()) return "ERROR: Item sedang dipinjam";
        if (m->hasBorrowed(itemId)) return "ERROR: Sudah meminjam item ini";
        it->setAvailable(false);
        m->addBorrowedId(itemId);
        return "OK: " + m->getName() + " berhasil meminjam \"" +
               it->getTitle() + "\"";
    }

    // Kembalikan buku — O(n)
    std::string returnItem(int memberId, int itemId) {
        Member*      m  = findMemberById(memberId);
        LibraryItem* it = findItemById(itemId);
        if (!m)  return "ERROR: Member tidak ditemukan";
        if (!it) return "ERROR: Item tidak ditemukan";
        if (!m->hasBorrowed(itemId)) return "ERROR: Member tidak meminjam item ini";
        it->setAvailable(true);
        m->removeBorrowedId(itemId);
        return "OK: \"" + it->getTitle() + "\" berhasil dikembalikan";
    }

    // ---- Pencarian ----------------------------------------

    // Cari berdasarkan judul (substring, linear search) — O(n)
    std::vector<LibraryItem*> searchByTitle(const std::string& query) const {
        auto vec = items.toVector();
        return Algorithms::linearSearchByTitle(vec, query);
    }

    // Cari persis + terurut (binary search, setelah quick sort) — O(n log n)
    LibraryItem* searchExact(const std::string& title) const {
        auto vec = items.toVector();
        Algorithms::sortByTitle(vec);   // O(n log n)
        int idx = Algorithms::binarySearch(vec, title);  // O(log n)
        return (idx >= 0) ? vec[idx] : nullptr;
    }

    // Ambil semua item sebagai vector (untuk dikirim via JSON) — O(n)
    std::vector<LibraryItem*> getAllItems() const {
        return items.toVector();
    }

    // Ambil semua item terurut — O(n log n)
    std::vector<LibraryItem*> getAllItemsSorted() const {
        auto vec = items.toVector();
        Algorithms::sortByTitle(vec);
        return vec;
    }

    // Ambil semua member — O(n)
    std::vector<Member*> getAllMembers() const {
        return members.toVector();
    }

    // Info member — O(n)
    Member* getMemberById(int id) const { return findMemberById(id); }
    LibraryItem* getItemById(int id)    const { return findItemById(id); }

    int getItemCount()   const { return items.getSize(); }
    int getMemberCount() const { return members.getSize(); }
};
