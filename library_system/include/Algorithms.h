#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "LibraryItem.h"

// ============================================================
//  ALGORITMA MANUAL — tanpa fungsi bawaan library
//
//  1. Quick Sort  — mengurutkan koleksi berdasarkan judul
//     Big-O:
//       Best case    : O(n log n)  — pivot selalu membagi rata
//       Average case : O(n log n)
//       Worst case   : O(n²)       — pivot selalu terkecil/terbesar
//       Space        : O(log n)    — stack rekursi
//
//  2. Binary Search — mencari item berdasarkan judul (persis)
//     Prasyarat: array sudah terurut ascending by title
//     Big-O:
//       Best case    : O(1)        — target ada di tengah
//       Average case : O(log n)
//       Worst case   : O(log n)
//       Space        : O(1)        — iteratif, bukan rekursif
// ============================================================

namespace Algorithms {

// ----------------------------------------------------------
//  QUICK SORT — partisi Lomuto
//  Mengurutkan vector<LibraryItem*> berdasarkan title asc.
//  Big-O: avg O(n log n), worst O(n²), space O(log n)
// ----------------------------------------------------------
inline int partition(std::vector<LibraryItem*>& arr, int low, int high) {
    // Pilih pivot = elemen paling kanan
    std::string pivot = arr[high]->getTitle();
    int i = low - 1;

    for (int j = low; j < high; j++) {
        // Bandingkan title secara lexicographic (case-insensitive)
        std::string t = arr[j]->getTitle();
        std::string tl = t, pl = pivot;
        std::transform(tl.begin(), tl.end(), tl.begin(), ::tolower);
        std::transform(pl.begin(), pl.end(), pl.begin(), ::tolower);

        if (tl <= pl) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

inline void quickSort(std::vector<LibraryItem*>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);   // rekursi kiri
        quickSort(arr, pi + 1, high);  // rekursi kanan
    }
}

// Entry point untuk quick sort
// Big-O: avg O(n log n), worst O(n²)
inline void sortByTitle(std::vector<LibraryItem*>& items) {
    if (items.size() > 1)
        quickSort(items, 0, (int)items.size() - 1);
}

// ----------------------------------------------------------
//  BINARY SEARCH — iteratif, array sudah terurut by title asc
//  Mengembalikan index jika ditemukan, -1 jika tidak.
//  Big-O: O(log n) time, O(1) space
// ----------------------------------------------------------
inline int binarySearch(const std::vector<LibraryItem*>& sortedItems,
                        const std::string& query) {
    int low = 0, high = (int)sortedItems.size() - 1;

    // Lowercase query untuk perbandingan case-insensitive
    std::string q = query;
    std::transform(q.begin(), q.end(), q.begin(), ::tolower);

    while (low <= high) {
        int mid = low + (high - low) / 2;  // hindari overflow vs (low+high)/2

        std::string t = sortedItems[mid]->getTitle();
        std::transform(t.begin(), t.end(), t.begin(), ::tolower);

        if (t == q)      return mid;   // ditemukan
        else if (t < q)  low = mid + 1;
        else             high = mid - 1;
    }
    return -1; // tidak ditemukan
}

// ----------------------------------------------------------
//  LINEAR SEARCH — untuk pencarian substring (partial match)
//  Dipakai ketika query tidak persis sama dengan judul.
//  Big-O: O(n) time, O(1) space
// ----------------------------------------------------------
inline std::vector<LibraryItem*> linearSearchByTitle(
        const std::vector<LibraryItem*>& items,
        const std::string& query) {

    std::vector<LibraryItem*> results;
    std::string q = query;
    std::transform(q.begin(), q.end(), q.begin(), ::tolower);

    for (LibraryItem* item : items) {
        std::string t = item->getTitle();
        std::transform(t.begin(), t.end(), t.begin(), ::tolower);
        // O(m) per item, m = panjang judul
        if (t.find(q) != std::string::npos)
            results.push_back(item);
    }
    return results;  // total: O(n*m) ≈ O(n) jika m konstan
}

} // namespace Algorithms
