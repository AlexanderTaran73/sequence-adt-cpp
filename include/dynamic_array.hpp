#pragma once

#include <algorithm>
#include <stdexcept>
#include "errors.hpp"

template <class T>
class DynamicArray {
private:
    T* data;
    int size;
    int capacity;

public:
    DynamicArray(int size);
    DynamicArray(T* items, int count);
    DynamicArray(const DynamicArray<T>& other);
    DynamicArray(DynamicArray<T>&& other) noexcept;
    DynamicArray<T>& operator=(const DynamicArray<T>& other);
    DynamicArray<T>& operator=(DynamicArray<T>&& other) noexcept;
    ~DynamicArray();

    T get(int index) const;
    void set(int index, T value);

    int getSize() const;
    int getCapacity() const;

    void resize(int newSize);
    void ensureCapacity(int newCapacity);
    void remove(int index);
    void clear();

    DynamicArray<T>* getSubArray(int startIndex, int endIndex) const;
    DynamicArray<T>* clone() const;

    T& operator[](int index);
    const T& operator[](int index) const;

    template <typename U>
    friend bool operator==(const DynamicArray<U>& lhs, const DynamicArray<U>& rhs);
};


template <class T>
DynamicArray<T>::DynamicArray(int size) {
    if (size < 0) throw Errors::negativeSize();
    this->size = size;
    capacity = size;
    data = new T[capacity];
}

template <class T>
DynamicArray<T>::DynamicArray(T* items, int count) {
    if (count < 0) throw Errors::negativeCount();
    size = count;
    capacity = count;
    data = new T[capacity];
    std::copy(items, items + size, data);
}

template <class T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other) {
    size = other.size;
    capacity = other.capacity;
    data = new T[capacity];
    std::copy(other.data, other.data + size, data);
}

template <class T>
DynamicArray<T>::DynamicArray(DynamicArray<T>&& other) noexcept
    : data(other.data), size(other.size), capacity(other.capacity) {
    other.data = nullptr;
    other.size = 0;
    other.capacity = 0;
}

template <class T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& other) {
    if (this != &other) {
        delete[] data;
        size = other.size;
        capacity = other.capacity;
        data = new T[capacity];
        std::copy(other.data, other.data + size, data);
    }
    return *this;
}

template <class T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray<T>&& other) noexcept {
    if (this != &other) {
        delete[] data;
        data = other.data;
        size = other.size;
        capacity = other.capacity;

        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }
    return *this;
}

template <class T>
DynamicArray<T>::~DynamicArray() {
    delete[] data;
}


template <class T>
T DynamicArray<T>::get(int index) const {
    if (index < 0 || index >= size)
        throw Errors::indexOutOfRange("Index: " + std::to_string(index));
    return data[index];
}

template <class T>
void DynamicArray<T>::set(int index, T value) {
    if (index < 0 || index >= size)
        throw Errors::indexOutOfRange("Index: " + std::to_string(index));
    data[index] = value;
}

template <class T>
int DynamicArray<T>::getSize() const {
    return size;
}

template <class T>
int DynamicArray<T>::getCapacity() const {
    return capacity;
}

template <class T>
void DynamicArray<T>::ensureCapacity(int newCapacity) {
    if (newCapacity < 0)
        throw Errors::negativeSize();
    if (newCapacity <= capacity)
        return;

    T* newData = new T[newCapacity];
    std::copy(data, data + size, newData);
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

template <class T>
void DynamicArray<T>::resize(int newSize) {
    if (newSize < 0)
        throw Errors::negativeSize();
    ensureCapacity(newSize);
    size = newSize;
}

template <class T>
void DynamicArray<T>::remove(int index) {
    if (size == 0)
        throw Errors::emptyArray();
    if (index < 0 || index >= size)
        throw Errors::indexOutOfRange("Remove index: " + std::to_string(index));

    for (int i = index; i < size - 1; ++i)
        data[i] = data[i + 1];
    --size;
}

template <class T>
void DynamicArray<T>::clear() {
    size = 0;
}

template <class T>
DynamicArray<T>* DynamicArray<T>::getSubArray(int startIndex, int endIndex) const {
    if (startIndex < 0 || endIndex >= size || startIndex > endIndex)
        throw Errors::invalidIndices("From " + std::to_string(startIndex) + " to " + std::to_string(endIndex));
    int count = endIndex - startIndex + 1;
    DynamicArray<T>* result = new DynamicArray<T>(count);
    for (int i = 0; i < count; ++i)
        result->set(i, data[startIndex + i]);
    return result;
}

template <class T>
DynamicArray<T>* DynamicArray<T>::clone() const {
    return new DynamicArray<T>(*this);
}


template <class T>
T& DynamicArray<T>::operator[](int index) {
    if (index < 0 || index >= size)
        throw Errors::indexOutOfRange("[] index: " + std::to_string(index));
    return data[index];
}

template <class T>
const T& DynamicArray<T>::operator[](int index) const {
    if (index < 0 || index >= size)
        throw Errors::indexOutOfRange("[] index: " + std::to_string(index));
    return data[index];
}

template <typename T>
bool operator==(const DynamicArray<T>& lhs, const DynamicArray<T>& rhs) {
    if (lhs.getSize() != rhs.getSize())
        return false;
    for (int i = 0; i < lhs.getSize(); ++i)
        if (lhs.get(i) != rhs.get(i))
            return false;
    return true;
}
