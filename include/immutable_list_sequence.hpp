#pragma once

#include "sequence.hpp"
#include "linked_list.hpp"

#include <functional>
#include <stdexcept>
#include <iterator>
#include <utility>
#include <algorithm>

template <typename T>
class ImmutableListSequence : public Sequence<T> {
private:
    LinkedList<T>* list;

public:
    ImmutableListSequence() : list(new LinkedList<T>()) {}

    explicit ImmutableListSequence(T* items, int count)
        : list(new LinkedList<T>(items, count)) {}

    explicit ImmutableListSequence(const LinkedList<T>& source)
        : list(new LinkedList<T>(source)) {}

    ImmutableListSequence(const ImmutableListSequence<T>& other)
        : list(new LinkedList<T>(*other.list)) {}

    ImmutableListSequence(ImmutableListSequence<T>&& other) noexcept
        : list(other.list) {
        other.list = nullptr;
    }

    ImmutableListSequence<T>& operator=(const ImmutableListSequence<T>& other) {
        if (this != &other) {
            delete list;
            list = new LinkedList<T>(*other.list);
        }
        return *this;
    }

    ImmutableListSequence<T>& operator=(ImmutableListSequence<T>&& other) noexcept {
        if (this != &other) {
            delete list;
            list = other.list;
            other.list = nullptr;
        }
        return *this;
    }

    ~ImmutableListSequence() override {
        delete list;
    }

    T getFirst() const override {
        return list->getFirst();
    }

    T getLast() const override {
        return list->getLast();
    }

    T get(int index) const override {
        return list->get(index);
    }

    int getLength() const override {
        return list->getLength();
    }

    T& operator[](int) override {
        throw Errors::immutable();
    }

    const T& operator[](int index) const override {
        return (*list)[index];
    }

    Sequence<T>* getSubsequence(int startIndex, int endIndex) const override {
        LinkedList<T>* sub = list->getSubList(startIndex, endIndex);
        auto* result = new ImmutableListSequence<T>(*sub);
        delete sub;
        return result;
    }

    Sequence<T>* append(T item) override {
        LinkedList<T> copy(*list);
        copy.append(item);
        return new ImmutableListSequence<T>(copy);
    }

    Sequence<T>* prepend(T item) override {
        LinkedList<T> copy(*list);
        copy.prepend(item);
        return new ImmutableListSequence<T>(copy);
    }

    Sequence<T>* insertAt(T item, int index) override {
        LinkedList<T> copy(*list);
        copy.insertAt(item, index);
        return new ImmutableListSequence<T>(copy);
    }

    Sequence<T>* remove(int index) override {
        LinkedList<T> copy(*list);
        copy.remove(index);
        return new ImmutableListSequence<T>(copy);
    }

    Sequence<T>* concat(const Sequence<T>* other) const override {
        const auto* otherList = dynamic_cast<const ImmutableListSequence<T>*>(other);
        if (!otherList) throw Errors::incompatibleTypes();
        LinkedList<T>* resultList = list->concat(otherList->list);
        auto* result = new ImmutableListSequence<T>(*resultList);
        delete resultList;
        return result;
    }

    Sequence<T>* clone() const override {
        return new ImmutableListSequence<T>(*this);
    }

    Sequence<T>* map(std::function<T(T)> f) const override {
        LinkedList<T> result;
        for (int i = 0; i < getLength(); ++i)
            result.append(f(get(i)));
        return new ImmutableListSequence<T>(result);
    }

    Sequence<T>* where(std::function<bool(T)> predicate) const override {
        LinkedList<T> result;
        for (int i = 0; i < getLength(); ++i)
            if (predicate(get(i)))
                result.append(get(i));
        return new ImmutableListSequence<T>(result);
    }

    T reduce(std::function<T(T, T)> reducer, T initial) const override {
        T acc = initial;
        for (int i = 0; i < getLength(); ++i)
            acc = reducer(acc, get(i));
        return acc;
    }

    Sequence<T>* zip(const Sequence<T>* other, std::function<T(T, T)> combiner) const override {
        int len = std::min(getLength(), other->getLength());
        LinkedList<T> result;
        for (int i = 0; i < len; ++i)
            result.append(combiner(get(i), other->get(i)));
        return new ImmutableListSequence<T>(result);
    }

    Sequence<T>* slice(int start, int end) const override {
        if (start < 0) start = 0;
        if (end > getLength()) end = getLength();
        if (start >= end) return new ImmutableListSequence<T>();
        return getSubsequence(start, end - 1);
    }
};
