#pragma once
#include <string>
#include <vector>
#include <iostream>

// ============================================================
//  ENKAPSULASI: Member menyembunyikan data via private +
//  menyediakan akses terkontrol lewat getter/setter.
//
//  Big-O:
//    addBorrowedId()    : O(1)
//    removeBorrowedId() : O(n) — scan borrowedIds
//    hasBorrowed()      : O(n)
// ============================================================
class Member {
private:
    int         memberId;
    std::string name;
    std::string email;
    std::vector<int> borrowedIds;   // ID buku yang sedang dipinjam

public:
    Member(int memberId, const std::string& name,
           const std::string& email)
        : memberId(memberId), name(name), email(email) {}

    // Getter
    int         getMemberId() const { return memberId; }
    std::string getName()     const { return name; }
    std::string getEmail()    const { return email; }
    const std::vector<int>& getBorrowedIds() const { return borrowedIds; }

    // Pinjam buku
    void addBorrowedId(int bookId) {
        borrowedIds.push_back(bookId);
    }

    // Kembalikan buku — O(n)
    bool removeBorrowedId(int bookId) {
        for (auto it = borrowedIds.begin(); it != borrowedIds.end(); ++it) {
            if (*it == bookId) {
                borrowedIds.erase(it);
                return true;
            }
        }
        return false;
    }

    // Cek apakah sudah meminjam bookId — O(n)
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
