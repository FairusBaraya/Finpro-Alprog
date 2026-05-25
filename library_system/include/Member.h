#pragma once
#include <string>
#include <vector>
#include <iostream>


class Member {
private:
    int         memberId;
    std::string name;
    std::string email;
    std::vector<int> borrowedIds;   

public:
    Member(int memberId, const std::string& name,
           const std::string& email)
        : memberId(memberId), name(name), email(email) {}

    int         getMemberId() const { return memberId; }
    std::string getName()     const { return name; }
    std::string getEmail()    const { return email; }
    const std::vector<int>& getBorrowedIds() const { return borrowedIds; }

    void addBorrowedId(int bookId) {
        borrowedIds.push_back(bookId);
    }

    bool removeBorrowedId(int bookId) {
        for (auto it = borrowedIds.begin(); it != borrowedIds.end(); ++it) {
            if (*it == bookId) {
                borrowedIds.erase(it);
                return true;
            }
        }
        return false;
    }

    bool hasBorrowed(int bookId) const {
        for (int id : borrowedIds) if (id == bookId) return true;
        return false;
    }

    void display() const {
        std::cout << "Member ID:" << memberId
                  << " | " << name
                  << " | " << email
                  << " | Pinjaman aktif: " << borrowedIds.size()
                  << "\n";
    }
};
