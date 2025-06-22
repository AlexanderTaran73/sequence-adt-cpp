#pragma once

#include "mutable_list_sequence.hpp"
#include <functional>
#include <utility>
#include <stdexcept>
#include <memory>

template <typename T>
class Queue {
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
    Queue() = default;

    explicit Queue(const Sequence<T>* other) : sequence(convertSequence(other)) {}

    explicit Queue(Sequence<T>*&& other) noexcept {
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

    Queue(const Queue<T>& other) = default;
    Queue(Queue<T>&& other) noexcept = default;

    Queue<T>& operator=(const Queue<T>& other) = default;
    Queue<T>& operator=(Queue<T>&& other) noexcept = default;

    void enqueue(const T& item) {
        sequence.append(item);
    }

    void enqueue(T&& item) {
        sequence.append(std::move(item));
    }

    T dequeue() {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        T item = sequence.getFirst();
        sequence.remove(0);
        return item;
    }

    T& front() {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        return sequence[0];
    }

    T get(int index) const {
        return sequence.get(index);
    }

    const T& front() const {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        return sequence[0];
    }

    int size() const {
        return sequence.getLength();
    }

    bool isEmpty() const {
        return sequence.getLength() == 0;
    }

    void clear() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    Queue<T> map(std::function<T(T)> f) const {
        return Queue<T>(sequence.map(f));
    }

    Queue<T> where(std::function<bool(T)> predicate) const {
        return Queue<T>(sequence.where(predicate));
    }

    T reduce(std::function<T(T, T)> reducer, T initial) const {
        return sequence.reduce(reducer, initial);
    }

    Queue<T> concat(const Queue<T>& other) const {
        auto new_seq = sequence.clone();
        for (int i = 0; i < other.size(); ++i) {
            new_seq->append(other.sequence.get(i));
        }
        return Queue<T>(new_seq);
    }

    Queue<T> getSubsequence(int startIndex, int endIndex) const {
        return Queue<T>(sequence.getSubsequence(startIndex, endIndex));
    }

    bool contains(const T& item) const {
        for (int i = 0; i < sequence.getLength(); ++i) {
            if (sequence.get(i) == item) {
                return true;
            }
        }
        return false;
    }

    bool containsSubsequence(const Queue<T>& sub) const {
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

    template <typename U>
    Queue<std::pair<T, U>> zip(const Queue<U>& other) const {
        Queue<std::pair<T, U>> result;
        int minSize = std::min(size(), other.size());
        for (int i = 0; i < minSize; ++i) {
            result.enqueue(std::make_pair(get(i), other.get(i)));
        }
        return result;
    }

    std::pair<Queue<T>, Queue<T>> split(std::function<bool(T)> predicate) const {
        Queue<T> left, right;
        for (int i = 0; i < sequence.getLength(); ++i) {
            T item = sequence.get(i);
            if (predicate(item)) {
                left.enqueue(item);
            } else {
                right.enqueue(item);
            }
        }
        return {left, right};
    }

    bool operator==(const Queue<T>& other) const {
        if (size() != other.size()) return false;
        for (int i = 0; i < size(); ++i) {
            if (sequence.get(i) != other.sequence.get(i)) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Queue<T>& other) const {
        return !(*this == other);
    }
};