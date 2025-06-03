#pragma once

#include "Sequence.hpp"
#include "DynamicArray.hpp"

#include <functional>
#include <stdexcept>
#include <algorithm>

template <typename T>
class MutableArraySequence : public Sequence<T> {
private:
    DynamicArray<T>* items;

public:
    MutableArraySequence() : items(new DynamicArray<T>(0)) {}

    explicit MutableArraySequence(T* array, int count)
        : items(new DynamicArray<T>(array, count)) {}

    explicit MutableArraySequence(const DynamicArray<T>& array)
        : items(new DynamicArray<T>(array)) {}

    MutableArraySequence(const MutableArraySequence<T>& other)
        : items(new DynamicArray<T>(*other.items)) {}

    MutableArraySequence(MutableArraySequence<T>&& other) noexcept
        : items(other.items) {
        other.items = nullptr;
    }

    MutableArraySequence<T>& operator=(const MutableArraySequence<T>& other) {
        if (this != &other) {
            delete items;
            items = new DynamicArray<T>(*other.items);
        }
        return *this;
    }

    MutableArraySequence<T>& operator=(MutableArraySequence<T>&& other) noexcept {
        if (this != &other) {
            delete items;
            items = other.items;
            other.items = nullptr;
        }
        return *this;
    }

    ~MutableArraySequence() override {
        delete items;
    }

    T getFirst() const override {
        if (getLength() == 0) throw Errors::emptyArray();
        return items->get(0);
    }

    T getLast() const override {
        if (getLength() == 0) throw Errors::emptyArray();
        return items->get(getLength() - 1);
    }

    T get(int index) const override {
        return items->get(index);
    }

    int getLength() const override {
        return items->getSize();
    }

    T& operator[](int index) override {
        return (*items)[index];
    }

    const T& operator[](int index) const override {
        return (*items)[index];
    }

    Sequence<T>* getSubsequence(int startIndex, int endIndex) const override {
        DynamicArray<T>* sub = items->getSubArray(startIndex, endIndex);
        auto* result = new MutableArraySequence<T>(*sub);
        delete sub;
        return result;
    }

    Sequence<T>* append(T item) override {
        int size = items->getSize();
        items->resize(size + 1);
        items->set(size, item);
        return this;
    }

    Sequence<T>* prepend(T item) override {
        int size = items->getSize();
        items->resize(size + 1);
        for (int i = size; i > 0; --i)
            items->set(i, items->get(i - 1));
        items->set(0, item);
        return this;
    }

    Sequence<T>* insertAt(T item, int index) override {
        if (index < 0 || index > items->getSize()) throw Errors::indexOutOfRange();
        items->resize(items->getSize() + 1);
        for (int i = items->getSize() - 1; i > index; --i)
            items->set(i, items->get(i - 1));
        items->set(index, item);
        return this;
    }

    Sequence<T>* remove(int index) override {
        if (items->getSize() == 0) throw Errors::emptyArray();
        items->remove(index);
        return this;
    }

    Sequence<T>* concat(const Sequence<T>* other) const override {
        const auto* otherArray = dynamic_cast<const MutableArraySequence<T>*>(other);
        if (!otherArray) throw Errors::incompatibleTypes();

        int totalSize = getLength() + otherArray->getLength();
        DynamicArray<T> combined(totalSize);

        for (int i = 0; i < getLength(); ++i)
            combined.set(i, get(i));
        for (int j = 0; j < otherArray->getLength(); ++j)
            combined.set(j + getLength(), otherArray->get(j));

        return new MutableArraySequence<T>(combined);
    }

    Sequence<T>* clone() const override {
        return new MutableArraySequence<T>(*this);
    }

    Sequence<T>* map(std::function<T(T)> f) const override {
        DynamicArray<T> mapped(getLength());
        for (int i = 0; i < getLength(); ++i)
            mapped.set(i, f(get(i)));
        return new MutableArraySequence<T>(mapped);
    }

    Sequence<T>* where(std::function<bool(T)> predicate) const override {
        DynamicArray<T> filtered(getLength());
        int count = 0;
        for (int i = 0; i < getLength(); ++i)
            if (predicate(get(i)))
                filtered.set(count++, get(i));

        DynamicArray<T> result(count);
        for (int i = 0; i < count; ++i)
            result.set(i, filtered.get(i));
        return new MutableArraySequence<T>(result);
    }

    T reduce(std::function<T(T, T)> reducer, T initial) const override {
        T acc = initial;
        for (int i = 0; i < getLength(); ++i)
            acc = reducer(acc, get(i));
        return acc;
    }

    Sequence<T>* zip(const Sequence<T>* other, std::function<T(T, T)> combiner) const override {
        int len = std::min(getLength(), other->getLength());
        DynamicArray<T> result(len);
        for (int i = 0; i < len; ++i)
            result.set(i, combiner(get(i), other->get(i)));
        return new MutableArraySequence<T>(result);
    }

    // Sequence<Sequence<T>*>* split(std::function<bool(T)> condition) const override {
    //     auto* result = new MutableArraySequence<Sequence<T>*>();
    //     int start = 0;
    //     for (int i = 0; i < getLength(); ++i) {
    //         if (condition(get(i))) {
    //             if (i > start)
    //                 result->append(getSubsequence(start, i - 1));
    //             start = i + 1;
    //         }
    //     }
    //     if (start < getLength())
    //         result->append(getSubsequence(start, getLength() - 1));
    //     return result;
    // }

    Sequence<T>* slice(int start, int end) const override {
        if (start < 0) start = 0;
        if (end > getLength()) end = getLength();
        if (start >= end) return new MutableArraySequence<T>();
        return getSubsequence(start, end - 1);
    }
};
