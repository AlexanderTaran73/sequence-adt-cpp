#pragma once

#include "sequence.hpp"
#include "dynamic_array.hpp"

#include <functional>
#include <stdexcept>
#include <utility>
#include <iterator>
#include <algorithm>

template <typename T>
class ImmutableArraySequence : public Sequence<T> {
private:
    DynamicArray<T>* items;

public:
    ImmutableArraySequence() : items(new DynamicArray<T>(0)) {}

    explicit ImmutableArraySequence(T* arr, int count)
        : items(new DynamicArray<T>(arr, count)) {}

    explicit ImmutableArraySequence(const DynamicArray<T>& array)
        : items(new DynamicArray<T>(array)) {}

    ImmutableArraySequence(const ImmutableArraySequence<T>& other)
        : items(new DynamicArray<T>(*other.items)) {}

    ImmutableArraySequence(ImmutableArraySequence<T>&& other) noexcept
        : items(other.items) {
        other.items = nullptr;
    }

    ImmutableArraySequence<T>& operator=(const ImmutableArraySequence<T>& other) {
        if (this != &other) {
            delete items;
            items = new DynamicArray<T>(*other.items);
        }
        return *this;
    }

    ImmutableArraySequence<T>& operator=(ImmutableArraySequence<T>&& other) noexcept {
        if (this != &other) {
            delete items;
            items = other.items;
            other.items = nullptr;
        }
        return *this;
    }

    ~ImmutableArraySequence() override {
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

    T& operator[](int) override {
        throw Errors::immutable();
    }

    const T& operator[](int index) const override {
        return (*items)[index];
    }

    Sequence<T>* getSubsequence(int startIndex, int endIndex) const override {
        auto* sub = items->getSubArray(startIndex, endIndex);
        auto* result = new ImmutableArraySequence<T>(*sub);
        delete sub;
        return result;
    }

    Sequence<T>* append(T item) override {
        int n = getLength();
        DynamicArray<T> newArray(n + 1);
        for (int i = 0; i < n; ++i)
            newArray.set(i, items->get(i));
        newArray.set(n, item);
        return new ImmutableArraySequence<T>(newArray);
    }

    Sequence<T>* prepend(T item) override {
        int n = getLength();
        DynamicArray<T> newArray(n + 1);
        newArray.set(0, item);
        for (int i = 0; i < n; ++i)
            newArray.set(i + 1, items->get(i));
        return new ImmutableArraySequence<T>(newArray);
    }

    Sequence<T>* insertAt(T item, int index) override {
        if (index < 0 || index > getLength()) throw Errors::indexOutOfRange();
        int n = getLength();
        DynamicArray<T> newArray(n + 1);
        for (int i = 0; i < index; ++i)
            newArray.set(i, items->get(i));
        newArray.set(index, item);
        for (int i = index; i < n; ++i)
            newArray.set(i + 1, items->get(i));
        return new ImmutableArraySequence<T>(newArray);
    }

    Sequence<T>* remove(int index) override {
        if (getLength() == 0) throw Errors::emptyArray();
        if (index < 0 || index >= getLength()) throw Errors::indexOutOfRange();
        int n = getLength();
        DynamicArray<T> newArray(n - 1);
        for (int i = 0; i < index; ++i)
            newArray.set(i, items->get(i));
        for (int i = index + 1; i < n; ++i)
            newArray.set(i - 1, items->get(i));
        return new ImmutableArraySequence<T>(newArray);
    }

    Sequence<T>* concat(const Sequence<T>* other) const override {
        const auto* otherArray = dynamic_cast<const ImmutableArraySequence<T>*>(other);
        if (!otherArray) throw Errors::incompatibleTypes();

        int totalSize = getLength() + otherArray->getLength();
        DynamicArray<T> combined(totalSize);

        for (int i = 0; i < getLength(); ++i)
            combined.set(i, get(i));
        for (int j = 0; j < otherArray->getLength(); ++j)
            combined.set(j + getLength(), otherArray->get(j));

        return new ImmutableArraySequence<T>(combined);
    }

    Sequence<T>* clone() const override {
        return new ImmutableArraySequence<T>(*this);
    }

    Sequence<T>* map(std::function<T(T)> f) const override {
        DynamicArray<T> mapped(getLength());
        for (int i = 0; i < getLength(); ++i)
            mapped.set(i, f(get(i)));
        return new ImmutableArraySequence<T>(mapped);
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
        return new ImmutableArraySequence<T>(result);
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
        return new ImmutableArraySequence<T>(result);
    }

    Sequence<T>* slice(int start, int end) const override {
        if (start < 0) start = 0;
        if (end > getLength()) end = getLength();
        if (start >= end) return new ImmutableArraySequence<T>();
        return getSubsequence(start, end - 1);
    }
};
