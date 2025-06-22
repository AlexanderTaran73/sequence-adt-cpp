#pragma once

#include "sequence.hpp"
#include "dynamic_array.hpp"

#include <functional>
#include <stdexcept>
#include <algorithm>
#include <utility>

template <typename T>
class MutableArraySequence : public Sequence<T> {
private:
    DynamicArray<T>* items;
    int length;  
    static constexpr double GROWTH_FACTOR = 1.5;

    void ensureCapacity(int requiredCapacity) {
        if (items->getSize() >= requiredCapacity) return;
        
        int newCapacity = std::max(requiredCapacity, 
                                  static_cast<int>(items->getSize() * GROWTH_FACTOR) + 1);
        items->resize(newCapacity);
    }

public:
    MutableArraySequence() : items(new DynamicArray<T>(1)), length(0) {} 

    explicit MutableArraySequence(T* array, int count)
        : items(new DynamicArray<T>(count)), length(count) {
        for (int i = 0; i < count; ++i) {
            items->set(i, array[i]);
        }
    }

    explicit MutableArraySequence(const DynamicArray<T>& array)
        : items(new DynamicArray<T>(array)), length(array.getSize()) {}

    MutableArraySequence(const MutableArraySequence<T>& other)
        : items(new DynamicArray<T>(*other.items)), length(other.length) {}

    MutableArraySequence(MutableArraySequence<T>&& other) noexcept
        : items(other.items), length(other.length) {
        other.items = nullptr;
        other.length = 0;
    }

    MutableArraySequence<T>& operator=(const MutableArraySequence<T>& other) {
        if (this != &other) {
            delete items;
            items = new DynamicArray<T>(*other.items);
            length = other.length;
        }
        return *this;
    }

    MutableArraySequence<T>& operator=(MutableArraySequence<T>&& other) noexcept {
        if (this != &other) {
            delete items;
            items = other.items;
            length = other.length;
            other.items = nullptr;
            other.length = 0;
        }
        return *this;
    }

    ~MutableArraySequence() override {
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

    T& operator[](int index) override {
        if (index < 0 || index >= length) throw Errors::indexOutOfRange();
        return (*items)[index];
    }

    const T& operator[](int index) const override {
        if (index < 0 || index >= length) throw Errors::indexOutOfRange();
        return (*items)[index];
    }

    Sequence<T>* getSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || endIndex >= length || startIndex > endIndex) 
            throw Errors::invalidIndices();

        DynamicArray<T>* sub = items->getSubArray(startIndex, endIndex);
        auto* result = new MutableArraySequence<T>(*sub);
        delete sub;
        return result;
    }

    Sequence<T>* append(T item) override {
        ensureCapacity(length + 1);
        items->set(length, item);
        length++;
        return this;
    }

    Sequence<T>* prepend(T item) override {
        ensureCapacity(length + 1);
        for (int i = length; i > 0; --i) {
            items->set(i, items->get(i - 1));
        }
        items->set(0, item);
        length++;
        return this;
    }

    Sequence<T>* insertAt(T item, int index) override {
        if (index < 0 || index > length) throw Errors::indexOutOfRange();
        
        ensureCapacity(length + 1);
        for (int i = length; i > index; --i) {
            items->set(i, items->get(i - 1));
        }
        items->set(index, item);
        length++;
        return this;
    }

    Sequence<T>* remove(int index) override {
        if (length == 0) throw Errors::emptyArray();
        if (index < 0 || index >= length) throw Errors::indexOutOfRange();
        
        for (int i = index; i < length - 1; ++i) {
            items->set(i, items->get(i + 1));
        }
        length--;
        return this;
    }

    Sequence<T>* concat(const Sequence<T>* other) const override {
        const auto* otherArray = dynamic_cast<const MutableArraySequence<T>*>(other);
        if (!otherArray) throw Errors::incompatibleTypes();

        int totalSize = length + otherArray->length;
        DynamicArray<T> combined(totalSize);

        for (int i = 0; i < length; ++i)
            combined.set(i, get(i));
        for (int j = 0; j < otherArray->length; ++j)
            combined.set(j + length, otherArray->get(j));

        auto* result = new MutableArraySequence<T>();
        delete result->items;  // Удаляем пустой массив
        result->items = new DynamicArray<T>(combined);
        result->length = totalSize;
        return result;
    }

    Sequence<T>* clone() const override {
        return new MutableArraySequence<T>(*this);
    }

    Sequence<T>* map(std::function<T(T)> f) const override {
        DynamicArray<T> mapped(length);
        for (int i = 0; i < length; ++i)
            mapped.set(i, f(get(i)));
        auto* result = new MutableArraySequence<T>(mapped);
        result->length = length;
        return result;
    }

    Sequence<T>* where(std::function<bool(T)> predicate) const override {
        DynamicArray<T> filtered(length);
        int count = 0;
        for (int i = 0; i < length; ++i)
            if (predicate(get(i)))
                filtered.set(count++, get(i));

        DynamicArray<T> result(count);
        for (int i = 0; i < count; ++i)
            result.set(i, filtered.get(i));
        auto* seq = new MutableArraySequence<T>(result);
        seq->length = count;
        return seq;
    }

    T reduce(std::function<T(T, T)> reducer, T initial) const override {
        T acc = initial;
        for (int i = 0; i < length; ++i)
            acc = reducer(acc, get(i));
        return acc;
    }

    Sequence<T>* zip(const Sequence<T>* other, std::function<T(T, T)> combiner) const override {
        int len = std::min(length, other->getLength());
        DynamicArray<T> result(len);
        for (int i = 0; i < len; ++i)
            result.set(i, combiner(get(i), other->get(i)));
        auto* seq = new MutableArraySequence<T>(result);
        seq->length = len;
        return seq;
    }

    Sequence<T>* slice(int start, int end) const override {
        if (start < 0) start = 0;
        if (end > length) end = length;
        if (start >= end) return new MutableArraySequence<T>();
        return getSubsequence(start, end - 1);
    }
};