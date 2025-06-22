#pragma once

#include "sequence.hpp"
#include "dynamic_array.hpp"

#include <functional>
#include <stdexcept>
#include <utility>
#include <algorithm>

template <typename T>
class ImmutableArraySequence : public Sequence<T> {
private:
    DynamicArray<T>* items;
    int length;

    DynamicArray<T>* createWithCapacity(int capacity, int copyLength) const {
        auto* newArray = new DynamicArray<T>(capacity);
        for (int i = 0; i < copyLength; ++i) {
            newArray->set(i, items->get(i));
        }
        return newArray;
    }

public:
    ImmutableArraySequence() : items(new DynamicArray<T>(1)), length(0) {}

    explicit ImmutableArraySequence(T* arr, int count)
        : items(new DynamicArray<T>(count)), length(count) {
        for (int i = 0; i < count; ++i) {
            items->set(i, arr[i]);
        }
    }

    explicit ImmutableArraySequence(const DynamicArray<T>& array)
        : items(new DynamicArray<T>(array)), length(array.getSize()) {}

    ImmutableArraySequence(const ImmutableArraySequence<T>& other)
        : items(new DynamicArray<T>(*other.items)), length(other.length) {}

    ImmutableArraySequence(ImmutableArraySequence<T>&& other) noexcept
        : items(other.items), length(other.length) {
        other.items = nullptr;
        other.length = 0;
    }

    ImmutableArraySequence<T>& operator=(const ImmutableArraySequence<T>& other) {
        if (this != &other) {
            delete items;
            items = new DynamicArray<T>(*other.items);
            length = other.length;
        }
        return *this;
    }

    ImmutableArraySequence<T>& operator=(ImmutableArraySequence<T>&& other) noexcept {
        if (this != &other) {
            delete items;
            items = other.items;
            length = other.length;
            other.items = nullptr;
            other.length = 0;
        }
        return *this;
    }

    ~ImmutableArraySequence() override {
        delete items;
    }

    T getFirst() const override {
        if (length == 0) throw Errors::emptyArray();
        return items->get(0);
    }

    T getLast() const override {
        if (length == 0) throw Errors::emptyArray();
        return items->get(length - 1);
    }

    T get(int index) const override {
        if (index < 0 || index >= length) throw Errors::indexOutOfRange();
        return items->get(index);
    }

    int getLength() const override {
        return length;
    }

    T& operator[](int) override {
        throw Errors::immutable();
    }

    const T& operator[](int index) const override {
        if (index < 0 || index >= length) throw Errors::indexOutOfRange();
        return (*items)[index];
    }

    Sequence<T>* getSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || endIndex >= length || startIndex > endIndex)
            throw Errors::invalidIndices();

        int subLength = endIndex - startIndex + 1;
        auto* subArray = new DynamicArray<T>(subLength);
        for (int i = 0; i < subLength; ++i) {
            subArray->set(i, items->get(startIndex + i));
        }
        return new ImmutableArraySequence<T>(*subArray);
    }

    Sequence<T>* append(T item) override {
        auto* newArray = createWithCapacity(length + 1, length);
        newArray->set(length, item);
        auto* result = new ImmutableArraySequence<T>(*newArray);
        delete newArray;
        return result;
    }

    Sequence<T>* prepend(T item) override {
        auto* newArray = new DynamicArray<T>(length + 1);
        newArray->set(0, item);
        for (int i = 0; i < length; ++i) {
            newArray->set(i + 1, items->get(i));
        }
        auto* result = new ImmutableArraySequence<T>(*newArray);
        delete newArray;
        return result;
    }

    Sequence<T>* insertAt(T item, int index) override {
        if (index < 0 || index > length) throw Errors::indexOutOfRange();
        
        auto* newArray = new DynamicArray<T>(length + 1);
        for (int i = 0; i < index; ++i) {
            newArray->set(i, items->get(i));
        }
        newArray->set(index, item);
        for (int i = index; i < length; ++i) {
            newArray->set(i + 1, items->get(i));
        }
        auto* result = new ImmutableArraySequence<T>(*newArray);
        delete newArray;
        return result;
    }

    Sequence<T>* remove(int index) override {
        if (length == 0) throw Errors::emptyArray();
        if (index < 0 || index >= length) throw Errors::indexOutOfRange();
        
        auto* newArray = new DynamicArray<T>(length - 1);
        for (int i = 0; i < index; ++i) {
            newArray->set(i, items->get(i));
        }
        for (int i = index + 1; i < length; ++i) {
            newArray->set(i - 1, items->get(i));
        }
        auto* result = new ImmutableArraySequence<T>(*newArray);
        delete newArray;
        return result;
    }

    Sequence<T>* concat(const Sequence<T>* other) const override {
        const auto* otherArray = dynamic_cast<const ImmutableArraySequence<T>*>(other);
        if (!otherArray) throw Errors::incompatibleTypes();

        int totalSize = length + otherArray->length;
        auto* combined = new DynamicArray<T>(totalSize);

        for (int i = 0; i < length; ++i)
            combined->set(i, get(i));
        for (int j = 0; j < otherArray->length; ++j)
            combined->set(j + length, otherArray->get(j));

        auto* result = new ImmutableArraySequence<T>(*combined);
        delete combined;
        return result;
    }

    Sequence<T>* clone() const override {
        return new ImmutableArraySequence<T>(*this);
    }

    Sequence<T>* map(std::function<T(T)> f) const override {
        auto* mapped = new DynamicArray<T>(length);
        for (int i = 0; i < length; ++i) {
            mapped->set(i, f(get(i)));
        }
        auto* result = new ImmutableArraySequence<T>(*mapped);
        delete mapped;
        return result;
    }

    Sequence<T>* where(std::function<bool(T)> predicate) const override {
        int count = 0;
        
        // Сначала подсчитываем количество подходящих элементов
        for (int i = 0; i < length; ++i) {
            if (predicate(get(i))) count++;
        }
        
        auto* filtered = new DynamicArray<T>(count);
        int index = 0;
        for (int i = 0; i < length; ++i) {
            if (predicate(get(i))) {
                filtered->set(index++, get(i));
            }
        }
        auto* result = new ImmutableArraySequence<T>(*filtered);
        delete filtered;
        return result;
    }

    T reduce(std::function<T(T, T)> reducer, T initial) const override {
        T acc = initial;
        for (int i = 0; i < length; ++i) {
            acc = reducer(acc, get(i));
        }
        return acc;
    }

    Sequence<T>* zip(const Sequence<T>* other, std::function<T(T, T)> combiner) const override {
        int len = std::min(length, other->getLength());
        auto* resultArray = new DynamicArray<T>(len);
        for (int i = 0; i < len; ++i) {
            resultArray->set(i, combiner(get(i), other->get(i)));
        }
        auto* result = new ImmutableArraySequence<T>(*resultArray);
        delete resultArray;
        return result;
    }

    Sequence<T>* slice(int start, int end) const override {
        if (start < 0) start = 0;
        if (end > length) end = length;
        if (start >= end) return new ImmutableArraySequence<T>();
        return getSubsequence(start, end - 1);
    }
};