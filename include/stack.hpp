#pragma once

#include "mutable_list_sequence.hpp"
#include <functional>
#include <utility>
#include <stdexcept>
#include <memory>

template <typename T>
class Stack {
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
    Stack() = default;

    explicit Stack(const Sequence<T>* other) : sequence(convertSequence(other)) {}

    explicit Stack(Sequence<T>*&& other) noexcept {
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

    Stack(const Stack<T>& other) = default;
    Stack(Stack<T>&& other) noexcept = default;

    Stack<T>& operator=(const Stack<T>& other) = default;
    Stack<T>& operator=(Stack<T>&& other) noexcept = default;

    void push(const T& item) {
        sequence.append(item);
    }

    T pop() {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        T item = sequence.getLast();
        sequence.remove(sequence.getLength() - 1);
        return item;
    }

    T& top() {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        return sequence[sequence.getLength() - 1];
    }

    const T& top() const {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
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
            pop();
        }
    }

    Stack<T> map(std::function<T(T)> f) const {
        return Stack<T>(sequence.map(f));
    }

    Stack<T> where(std::function<bool(T)> predicate) const {
        return Stack<T>(sequence.where(predicate));
    }

    T reduce(std::function<T(T, T)> reducer, T initial) const {
        return sequence.reduce(reducer, initial);
    }

    Stack<T> concat(const Stack<T>& other) const {
        auto new_seq = sequence.clone();
        for (int i = 0; i < other.size(); ++i) {
            new_seq->append(other.sequence.get(i));
        }
        return Stack<T>(new_seq);
    }

    Stack<T> getSubsequence(int startIndex, int endIndex) const {
        return Stack<T>(sequence.getSubsequence(startIndex, endIndex));
    }

    bool contains(const T& item) const {
        for (int i = 0; i < sequence.getLength(); ++i) {
            if (sequence.get(i) == item) {
                return true;
            }
        }
        return false;
    }

    bool containsSubsequence(const Stack<T>& sub) const {
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
    Stack<std::pair<T, U>> zip(const Stack<U>& other) const {
        Stack<std::pair<T, U>> result;
        int minSize = std::min(size(), other.size());
        for (int i = 0; i < minSize; ++i) {
            result.push(std::make_pair(get(i), other.get(i)));
        }
        return result;
    }

    std::pair<Stack<T>, Stack<T>> split(std::function<bool(T)> predicate) const {
        Stack<T> left, right;
        for (int i = 0; i < sequence.getLength(); ++i) {
            T item = sequence.get(i);
            if (predicate(item)) {
                left.push(item);
            } else {
                right.push(item);
            }
        }
        return {left, right};
    }

    bool operator==(const Stack<T>& other) const {
        if (size() != other.size()) return false;
        for (int i = 0; i < size(); ++i) {
            if (sequence.get(i) != other.sequence.get(i)) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Stack<T>& other) const {
        return !(*this == other);
    }
};