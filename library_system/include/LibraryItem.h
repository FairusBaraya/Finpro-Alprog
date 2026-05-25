#pragma once
#include <string>
#include <iostream>


class LibraryItem {
protected:
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

    virtual std::string getInfo() const = 0;   
    virtual std::string getType() const = 0;   

    virtual void display() const {
        std::cout << "[" << getType() << "] "
                  << "ID:" << id << " | " << title
                  << " (" << year << ") by " << author
                  << " | " << (isAvailable ? "Tersedia" : "Dipinjam")
                  << "\n";
    }

    int         getId()        const { return id; }
    std::string getTitle()     const { return title; }
    std::string getAuthor()    const { return author; }
    int         getYear()      const { return year; }
    bool        getAvailable() const { return isAvailable; }

    void setAvailable(bool val) { isAvailable = val; }
};
