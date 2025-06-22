#pragma once

#include "mutable_list_sequence.hpp"
#include <functional>
#include <utility>
#include <stdexcept>
#include <memory>

template <typename T>
class Deque {
private:
    MutableListSequence<T> sequence;

    static MutableListSequence<T> convertSequence(const Sequence<T>* seq) {
        if (auto derived = dynamic_cast<const MutableListSequence<T>*>(seq)) {
            return *derived;
        }
        
        MutableListSequence<T> result;
        for (int i = 0; i < seq->getLength(); ++i) {
            result.append(seq->get(i));
        }
        return result;
    }

public:
    Deque() = default;

    explicit Deque(const Sequence<T>* other) : sequence(convertSequence(other)) {}

    explicit Deque(Sequence<T>*&& other) noexcept {
        if (auto derived = dynamic_cast<MutableListSequence<T>*>(other)) {
            sequence = std::move(*derived);
            delete other;
            other = nullptr;
        } else {
            sequence = convertSequence(other);
            delete other;
            other = nullptr;
        }
    }

    Deque(const Deque<T>& other) = default;
    Deque(Deque<T>&& other) noexcept = default;

    Deque<T>& operator=(const Deque<T>& other) = default;
    Deque<T>& operator=(Deque<T>&& other) noexcept = default;

    void pushFront(const T& item) {
        sequence.prepend(item);
    }

    void pushBack(const T& item) {
        sequence.append(item);
    }

    T popFront() {
        if (isEmpty()) {
            throw std::runtime_error("Deque is empty");
        }
        T item = sequence.getFirst();
        sequence.remove(0);
        return item;
    }

    T popBack() {
        if (isEmpty()) {
            throw std::runtime_error("Deque is empty");
        }
        T item = sequence.getLast();
        sequence.remove(sequence.getLength() - 1);
        return item;
    }

    T& front() {
        if (isEmpty()) {
            throw std::runtime_error("Deque is empty");
        }
        return sequence[0];
    }

    T& back() {
        if (isEmpty()) {
            throw std::runtime_error("Deque is empty");
        }
        return sequence[sequence.getLength() - 1];
    }

    const T& front() const {
        if (isEmpty()) {
            throw std::runtime_error("Deque is empty");
        }
        return sequence[0];
    }

    const T& back() const {
        if (isEmpty()) {
            throw std::runtime_error("Deque is empty");
        }
        return sequence[sequence.getLength() - 1];
    }

    T get(int index) const {
        return sequence.get(index);
    }

    int size() const {
        return sequence.getLength();
    }

    bool isEmpty() const {
        return sequence.getLength() == 0;
    }

    void clear() {
        while (!isEmpty()) {
            popBack();
        }
    }

    Deque<T> map(std::function<T(T)> f) const {
        return Deque<T>(sequence.map(f));
    }

    Deque<T> where(std::function<bool(T)> predicate) const {
        return Deque<T>(sequence.where(predicate));
    }

    T reduce(std::function<T(T, T)> reducer, T initial) const {
        return sequence.reduce(reducer, initial);
    }

    Deque<T> concat(const Deque<T>& other) const {
        auto new_seq = sequence.clone();
        for (int i = 0; i < other.size(); ++i) {
            new_seq->append(other.sequence.get(i));
        }
        return Deque<T>(new_seq);
    }

    Deque<T> getSubsequence(int startIndex, int endIndex) const {
        return Deque<T>(sequence.getSubsequence(startIndex, endIndex));
    }

    bool contains(const T& item) const {
        for (int i = 0; i < sequence.getLength(); ++i) {
            
            if (sequence.get(i) == item) {
                return true;
            }
        }
        return false;
    }

    bool containsSubsequence(const Deque<T>& sub) const {
        if (sub.isEmpty()) return true;
        if (size() < sub.size()) return false;

        for (int i = 0; i <= size() - sub.size(); ++i) {
            bool match = true;
            for (int j = 0; j < sub.size(); ++j) {
                if (get(i + j) != sub.get(j)) {
                    match = false;
                    break;
                }
            }
            if (match) return true;
        }
        return false;
    }

    void sort(std::function<bool(const T&, const T&)> comparator) {
        T* items = new T[sequence.getLength()];
        for (int i = 0; i < sequence.getLength(); ++i) {
            items[i] = sequence.get(i);
        }

        if (comparator) {
            std::sort(items, items + sequence.getLength(), comparator);
        } else {
            std::sort(items, items + sequence.getLength());
        }

        sequence = MutableListSequence<T>(items, sequence.getLength());
        delete[] items;
    }

    void sort() {
        sort([](const T& a, const T& b) { return a < b; });
    }

    template <typename U>
    Deque<std::pair<T, U>> zip(const Deque<U>& other) const {
        Deque<std::pair<T, U>> result;
        int minSize = std::min(size(), other.size());
        for (int i = 0; i < minSize; ++i) {
            result.pushBack(std::make_pair(get(i), other.get(i)));
        }
        return result;
    }

    std::pair<Deque<T>, Deque<T>> split(std::function<bool(T)> predicate) const {
        Deque<T> left, right;
        for (int i = 0; i < sequence.getLength(); ++i) {
            T item = sequence.get(i);
            if (predicate(item)) {
                left.pushBack(item);
            } else {
                right.pushBack(item);
            }
        }
        return {left, right};
    }

    bool operator==(const Deque<T>& other) const {
        if (size() != other.size()) return false;
        for (int i = 0; i < size(); ++i) {
            if (sequence.get(i) != other.sequence.get(i)) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Deque<T>& other) const {
        return !(*this == other);
    }
};