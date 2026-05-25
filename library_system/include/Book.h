#pragma once
#include "LibraryItem.h"


class Book : public LibraryItem {
private:
    std::string isbn;
    std::string genre;

public:
    Book(int id, const std::string& title, const std::string& author,
         int year, const std::string& isbn, const std::string& genre)
        : LibraryItem(id, title, author, year),
          isbn(isbn), genre(genre) {}

    std::string getType() const override { return "Buku"; }

    std::string getInfo() const override {
        return "Buku | " + title + " | " + author +
               " | " + genre + " | ISBN:" + isbn +
               " | " + std::to_string(year);
    }

    void display() const override {
        std::cout << "[Buku] ID:" << id
                  << " | " << title
                  << " (" << year << ")"
                  << " by " << author
                  << " | Genre: " << genre
                  << " | ISBN: " << isbn
                  << " | " << (isAvailable ? "Tersedia" : "Dipinjam")
                  << "\n";
    }

    std::string getIsbn()  const { return isbn; }
    std::string getGenre() const { return genre; }
};
