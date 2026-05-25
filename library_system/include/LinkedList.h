#pragma once
#include <iostream>
#include <stdexcept>

template <typename T>
class LinkedList {
private:
    struct Node {
        T     data;
        Node* next;
        Node(T d) : data(d), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int   size;

public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~LinkedList() { clear(); }

    void pushBack(T item) {
        Node* n = new Node(item);
        if (!tail) { head = tail = n; }
        else       { tail->next = n; tail = n; }
        size++;
    }

    void pushFront(T item) {
        Node* n = new Node(item);
        n->next = head;
        head = n;
        if (!tail) tail = head;
        size++;
    }

    template <typename Pred>
    int removeIf(Pred pred) {
        int removed = 0;
        Node* cur  = head;
        Node* prev = nullptr;
        while (cur) {
            if (pred(cur->data)) {
                Node* del = cur;
                if (prev) prev->next = cur->next;
                else      head       = cur->next;
                if (cur == tail) tail = prev;
                cur = cur->next;
                delete del;
                size--;
                removed++;
            } else {
                prev = cur;
                cur  = cur->next;
            }
        }
        return removed;
    }

    T& operator[](int index) {
        if (index < 0 || index >= size)
            throw std::out_of_range("LinkedList index out of range");
        Node* cur = head;
        for (int i = 0; i < index; i++) cur = cur->next;
        return cur->data;
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= size)
            throw std::out_of_range("LinkedList index out of range");
        Node* cur = head;
        for (int i = 0; i < index; i++) cur = cur->next;
        return cur->data;
    }

    std::vector<T> toVector() const {
        std::vector<T> result;
        Node* cur = head;
        while (cur) { result.push_back(cur->data); cur = cur->next; }
        return result;
    }

    void fromVector(const std::vector<T>& vec) {
        clear();
        for (const T& item : vec) pushBack(item);
    }

    int  getSize() const { return size; }
    bool isEmpty() const { return size == 0; }

    void clear() {
        Node* cur = head;
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
        head = tail = nullptr;
        size = 0;
    }

    struct Iterator {
        Node* cur;
        Iterator(Node* n) : cur(n) {}
        T&          operator*()  { return cur->data; }
        Iterator&   operator++() { cur = cur->next; return *this; }
        bool operator!=(const Iterator& o) const { return cur != o.cur; }
    };
    Iterator begin() const { return Iterator(head); }
    Iterator end()   const { return Iterator(nullptr); }
};
