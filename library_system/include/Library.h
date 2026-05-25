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


class Library {
private:
    LinkedList<LibraryItem*> items;    
    LinkedList<Member*>      members;
    int nextItemId;
    int nextMemberId;

    LibraryItem* findItemById(int id) const {
        for (LibraryItem* item : items)
            if (item->getId() == id) return item;
        return nullptr;
    }

    Member* findMemberById(int id) const {
        for (Member* m : members)
            if (m->getMemberId() == id) return m;
        return nullptr;
    }

public:
    Library() : nextItemId(1), nextMemberId(1) {
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
        addMember("Fairus Baraya",  "faibaraya@email.com");
        addMember("jidan",  "bangkur@email.com");
        addMember("senna",  "sen@email.com");
    }

    ~Library() {
        for (LibraryItem* item : items) delete item;
        for (Member* m : members)       delete m;
    }


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
        if (!target->getAvailable()) return false;
        items.removeIf([id](LibraryItem* it){ return it->getId() == id; });
        delete target;
        return true;
    }


    int addMember(const std::string& name, const std::string& email) {
        int id = nextMemberId++;
        members.pushBack(new Member(id, name, email));
        return id;
    }


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


    std::vector<LibraryItem*> searchByTitle(const std::string& query) const {
        auto vec = items.toVector();
        return Algorithms::linearSearchByTitle(vec, query);
    }

    LibraryItem* searchExact(const std::string& title) const {
        auto vec = items.toVector();
        Algorithms::sortByTitle(vec);   
        int idx = Algorithms::binarySearch(vec, title);  
        return (idx >= 0) ? vec[idx] : nullptr;
    }

    std::vector<LibraryItem*> getAllItems() const {
        return items.toVector();
    }

    std::vector<LibraryItem*> getAllItemsSorted() const {
        auto vec = items.toVector();
        Algorithms::sortByTitle(vec);
        return vec;
    }

    std::vector<Member*> getAllMembers() const {
        return members.toVector();
    }

    Member* getMemberById(int id) const { return findMemberById(id); }
    LibraryItem* getItemById(int id)    const { return findItemById(id); }

    int getItemCount()   const { return items.getSize(); }
    int getMemberCount() const { return members.getSize(); }
};
