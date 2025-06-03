#pragma once

#include "Errors.hpp"
#include <stdexcept>
#include <utility>

template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(T value, Node* nextNode = nullptr)
            : data(value), next(nextNode) {}
    };

    Node* root;
    Node* tail;
    int size;

    void copyFrom(const LinkedList<T>& other);
    void moveFrom(LinkedList<T>&& other) noexcept;

public:
    LinkedList();
    LinkedList(T* items, int count);
    LinkedList(const LinkedList<T>& other);
    LinkedList(LinkedList<T>&& other) noexcept;

    LinkedList<T>& operator=(const LinkedList<T>& other);
    LinkedList<T>& operator=(LinkedList<T>&& other) noexcept;

    ~LinkedList();

    T getFirst() const;
    T getLast() const;
    T get(int index) const;

    T& operator[](int index);
    const T& operator[](int index) const;

    int getLength() const;

    void append(T item);
    void prepend(T item);
    void insertAt(T item, int index);
    void remove(int index);
    void clear();

    LinkedList<T>* getSubList(int startIndex, int endIndex) const;
    LinkedList<T>* clone() const;
    LinkedList<T>* concat(const LinkedList<T>* other) const;

    bool operator==(const LinkedList<T>& other) const;
    bool operator!=(const LinkedList<T>& other) const;
};

template <typename T>
LinkedList<T>::LinkedList() : root(nullptr), tail(nullptr), size(0) {}

template <typename T>
LinkedList<T>::LinkedList(T* items, int count) : LinkedList() {
    if (count < 0) throw Errors::negativeCount();
    for (int i = 0; i < count; ++i)
        append(items[i]);
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& other) : LinkedList() {
    copyFrom(other);
}

template <typename T>
LinkedList<T>::LinkedList(LinkedList<T>&& other) noexcept : LinkedList() {
    moveFrom(std::move(other));
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other) {
    if (this != &other) {
        clear();
        copyFrom(other);
    }
    return *this;
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList<T>&& other) noexcept {
    if (this != &other) {
        clear();
        moveFrom(std::move(other));
    }
    return *this;
}

template <typename T>
LinkedList<T>::~LinkedList() {
    clear();
}

template <typename T>
void LinkedList<T>::copyFrom(const LinkedList<T>& other) {
    Node* current = other.root;
    while (current) {
        append(current->data);
        current = current->next;
    }
}

template <typename T>
void LinkedList<T>::moveFrom(LinkedList<T>&& other) noexcept {
    root = other.root;
    tail = other.tail;
    size = other.size;
    other.root = other.tail = nullptr;
    other.size = 0;
}

template <typename T>
T LinkedList<T>::getFirst() const {
    if (!root) throw Errors::emptyList();
    return root->data;
}

template <typename T>
T LinkedList<T>::getLast() const {
    if (!tail) throw Errors::emptyList();
    return tail->data;
}

template <typename T>
T LinkedList<T>::get(int index) const {
    if (index < 0 || index >= size)
        throw Errors::indexOutOfRange("Index: " + std::to_string(index));
    Node* current = root;
    for (int i = 0; i < index; ++i)
        current = current->next;
    return current->data;
}

template <typename T>
T& LinkedList<T>::operator[](int index) {
    if (index < 0 || index >= size)
        throw Errors::indexOutOfRange("[] index: " + std::to_string(index));
    Node* current = root;
    for (int i = 0; i < index; ++i)
        current = current->next;
    return current->data;
}

template <typename T>
const T& LinkedList<T>::operator[](int index) const {
    if (index < 0 || index >= size)
        throw Errors::indexOutOfRange("[] index: " + std::to_string(index));
    Node* current = root;
    for (int i = 0; i < index; ++i)
        current = current->next;
    return current->data;
}

template <typename T>
int LinkedList<T>::getLength() const {
    return size;
}

template <typename T>
void LinkedList<T>::append(T item) {
    Node* newNode = new Node(item);
    if (!root) {
        root = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    ++size;
}

template <typename T>
void LinkedList<T>::prepend(T item) {
    Node* newNode = new Node(item, root);
    root = newNode;
    if (!tail) tail = root;
    ++size;
}

template <typename T>
void LinkedList<T>::insertAt(T item, int index) {
    if (index < 0 || index > size)
        throw Errors::indexOutOfRange("Insert at index: " + std::to_string(index));
    if (index == 0) {
        prepend(item);
        return;
    }
    if (index == size) {
        append(item);
        return;
    }

    Node* prev = root;
    for (int i = 0; i < index - 1; ++i)
        prev = prev->next;
    Node* newNode = new Node(item, prev->next);
    prev->next = newNode;
    ++size;
}

template <typename T>
void LinkedList<T>::remove(int index) {
    if (index < 0 || index >= size)
        throw Errors::indexOutOfRange("Remove index: " + std::to_string(index));

    Node* toDelete = nullptr;

    if (index == 0) {
        toDelete = root;
        root = root->next;
        if (size == 1) tail = nullptr;
    } else {
        Node* prev = root;
        for (int i = 0; i < index - 1; ++i)
            prev = prev->next;
        toDelete = prev->next;
        prev->next = toDelete->next;
        if (toDelete == tail) tail = prev;
    }

    delete toDelete;
    --size;
}

template <typename T>
void LinkedList<T>::clear() {
    Node* current = root;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    root = tail = nullptr;
    size = 0;
}

template <typename T>
LinkedList<T>* LinkedList<T>::getSubList(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex >= size || startIndex > endIndex)
        throw Errors::invalidIndices("Start: " + std::to_string(startIndex) +
                                     ", End: " + std::to_string(endIndex));

    LinkedList<T>* result = new LinkedList<T>();
    Node* current = root;
    for (int i = 0; i < startIndex; ++i)
        current = current->next;
    for (int i = startIndex; i <= endIndex; ++i) {
        result->append(current->data);
        current = current->next;
    }
    return result;
}

template <typename T>
LinkedList<T>* LinkedList<T>::clone() const {
    return new LinkedList<T>(*this);
}

template <typename T>
LinkedList<T>* LinkedList<T>::concat(const LinkedList<T>* other) const {
    if (!other) throw Errors::nullList();
    LinkedList<T>* result = new LinkedList<T>(*this);
    Node* current = other->root;
    while (current) {
        result->append(current->data);
        current = current->next;
    }
    return result;
}

template <typename T>
bool LinkedList<T>::operator==(const LinkedList<T>& other) const {
    if (size != other.size) return false;
    Node* a = root;
    Node* b = other.root;
    while (a && b) {
        if (a->data != b->data) return false;
        a = a->next;
        b = b->next;
    }
    return true;
}

template <typename T>
bool LinkedList<T>::operator!=(const LinkedList<T>& other) const {
    return !(*this == other);
}
