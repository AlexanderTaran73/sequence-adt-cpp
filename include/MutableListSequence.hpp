#pragma once

#include "Sequence.hpp"
#include "LinkedList.hpp"

#include <functional>
#include <stdexcept>
#include <utility>
#include <iterator>
#include <algorithm>

template <typename T>
class MutableListSequence : public Sequence<T> {
protected:
    LinkedList<T>* list;

public:
    MutableListSequence() : list(new LinkedList<T>()) {}

    explicit MutableListSequence(T* items, int count)
        : list(new LinkedList<T>(items, count)) {}

    explicit MutableListSequence(const LinkedList<T>& source)
        : list(new LinkedList<T>(source)) {}

    MutableListSequence(const MutableListSequence<T>& other)
        : list(new LinkedList<T>(*other.list)) {}

    MutableListSequence(MutableListSequence<T>&& other) noexcept
        : list(other.list) {
        other.list = nullptr;
    }

    MutableListSequence<T>& operator=(const MutableListSequence<T>& other) {
        if (this != &other) {
            delete list;
            list = new LinkedList<T>(*other.list);
        }
        return *this;
    }

    MutableListSequence<T>& operator=(MutableListSequence<T>&& other) noexcept {
        if (this != &other) {
            delete list;
            list = other.list;
            other.list = nullptr;
        }
        return *this;
    }

    ~MutableListSequence() override {
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

    T& operator[](int index) override {
        return (*list)[index];
    }

    const T& operator[](int index) const override {
        return (*list)[index];
    }

    int getLength() const override {
        return list->getLength();
    }

    Sequence<T>* getSubsequence(int startIndex, int endIndex) const override {
        LinkedList<T>* sub = list->getSubList(startIndex, endIndex);
        auto* result = new MutableListSequence<T>(*sub);
        delete sub;
        return result;
    }

    Sequence<T>* append(T item) override {
        list->append(item);
        return this;
    }

    Sequence<T>* prepend(T item) override {
        list->prepend(item);
        return this;
    }

    Sequence<T>* insertAt(T item, int index) override {
        list->insertAt(item, index);
        return this;
    }

    Sequence<T>* remove(int index) override {
        if (list->getLength() == 0) throw Errors::emptyList();
        list->remove(index);
        return this;
    }

    Sequence<T>* concat(const Sequence<T>* other) const override {
        const auto* otherList = dynamic_cast<const MutableListSequence<T>*>(other);
        if (!otherList) throw Errors::incompatibleTypes();

        LinkedList<T>* resultList = list->concat(otherList->list);
        auto* result = new MutableListSequence<T>(*resultList);
        delete resultList;
        return result;
    }

    Sequence<T>* clone() const override {
        return new MutableListSequence<T>(*this);
    }

    Sequence<T>* map(std::function<T(T)> f) const override {
        LinkedList<T> result;
        for (int i = 0; i < getLength(); ++i)
            result.append(f(get(i)));
        return new MutableListSequence<T>(result);
    }

    Sequence<T>* where(std::function<bool(T)> predicate) const override {
        LinkedList<T> result;
        for (int i = 0; i < getLength(); ++i)
            if (predicate(get(i)))
                result.append(get(i));
        return new MutableListSequence<T>(result);
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
        return new MutableListSequence<T>(result);
    }

    // Sequence<Sequence<T>*>* split(std::function<bool(T)> condition) const override {
    //     auto* result = new MutableListSequence<Sequence<T>*>();
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
        if (start >= end) return new MutableListSequence<T>();
        return getSubsequence(start, end - 1);
    }
};
