#pragma once
#include <string>
#include <iostream>

// ============================================================
//  ABSTRAKSI (Abstract Class)
//  LibraryItem adalah abstract class — tidak bisa diinstansiasi
//  langsung. Setiap item perpustakaan HARUS mengimplementasikan
//  method virtual murni getInfo() dan getType().
//
//  Big-O:
//    getInfo(), getType() : O(1)  — hanya return string
// ============================================================
class LibraryItem {
protected:
    // ENKAPSULASI: atribut disembunyikan di protected/private
    int         id;
    std::string title;
    std::string author;
    int         year;
    bool        isAvailable;

public:
    LibraryItem(int id, const std::string& title,
                const std::string& author, int year)
        : id(id), title(title), author(author),
          year(year), isAvailable(true) {}

    virtual ~LibraryItem() = default;

    // --- Pure virtual (wajib di-override oleh child class) ---
    virtual std::string getInfo() const = 0;   // ABSTRAKSI
    virtual std::string getType() const = 0;   // ABSTRAKSI

    // --- POLIMORFISME: virtual method dengan default behaviour ---
    virtual void display() const {
        std::cout << "[" << getType() << "] "
                  << "ID:" << id << " | " << title
                  << " (" << year << ") by " << author
                  << " | " << (isAvailable ? "Tersedia" : "Dipinjam")
                  << "\n";
    }

    // --- ENKAPSULASI: getter/setter ---
    int         getId()        const { return id; }
    std::string getTitle()     const { return title; }
    std::string getAuthor()    const { return author; }
    int         getYear()      const { return year; }
    bool        getAvailable() const { return isAvailable; }

    void setAvailable(bool val) { isAvailable = val; }
};
