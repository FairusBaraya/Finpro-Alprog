#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "LibraryItem.h"


namespace Algorithms {

inline int partition(std::vector<LibraryItem*>& arr, int low, int high) {
    std::string pivot = arr[high]->getTitle();
    int i = low - 1;

    for (int j = low; j < high; j++) {
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
        quickSort(arr, low, pi - 1);  
        quickSort(arr, pi + 1, high); 
    }
}


inline void sortByTitle(std::vector<LibraryItem*>& items) {
    if (items.size() > 1)
        quickSort(items, 0, (int)items.size() - 1);
}

inline int binarySearch(const std::vector<LibraryItem*>& sortedItems,
                        const std::string& query) {
    int low = 0, high = (int)sortedItems.size() - 1;

    std::string q = query;
    std::transform(q.begin(), q.end(), q.begin(), ::tolower);

    while (low <= high) {
        int mid = low + (high - low) / 2;  

        std::string t = sortedItems[mid]->getTitle();
        std::transform(t.begin(), t.end(), t.begin(), ::tolower);

        if (t == q)      return mid;  
        else if (t < q)  low = mid + 1;
        else             high = mid - 1;
    }
    return -1; 
}


inline std::vector<LibraryItem*> linearSearchByTitle(
        const std::vector<LibraryItem*>& items,
        const std::string& query) {

    std::vector<LibraryItem*> results;
    std::string q = query;
    std::transform(q.begin(), q.end(), q.begin(), ::tolower);

    for (LibraryItem* item : items) {
        std::string t = item->getTitle();
        std::transform(t.begin(), t.end(), t.begin(), ::tolower);
    
        if (t.find(q) != std::string::npos)
            results.push_back(item);
    }
    return results;  
}

} 
