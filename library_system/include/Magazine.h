#pragma once
#include "LibraryItem.h"

// ============================================================
//  PEWARISAN (Inheritance) — child class kedua dari LibraryItem
//  Magazine menambah atribut edition dan publisher.
//
//  POLIMORFISME: override getInfo(), getType(), display()
//  Fungsi yang sama (display) menghasilkan output berbeda
//  tergantung tipe objek sebenarnya → dynamic binding.
//
//  Big-O:
//    getInfo(), getType(), display() : O(1)
// ============================================================
class Magazine : public LibraryItem {
private:
    std::string publisher;
    int         edition;

public:
    Magazine(int id, const std::string& title, const std::string& author,
             int year, const std::string& publisher, int edition)
        : LibraryItem(id, title, author, year),
          publisher(publisher), edition(edition) {}

    std::string getType() const override { return "Majalah"; }

    std::string getInfo() const override {
        return "Majalah | " + title + " | " + author +
               " | " + publisher +
               " | Edisi:" + std::to_string(edition) +
               " | " + std::to_string(year);
    }

    void display() const override {
        std::cout << "[Majalah] ID:" << id
                  << " | " << title
                  << " Edisi " << edition
                  << " (" << year << ")"
                  << " by " << author
                  << " | Penerbit: " << publisher
                  << " | " << (isAvailable ? "Tersedia" : "Dipinjam")
                  << "\n";
    }

    std::string getPublisher() const { return publisher; }
    int         getEdition()   const { return edition; }
};
