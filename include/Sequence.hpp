#pragma once

#include <functional>
#include <stdexcept>
#include <iterator>

template <typename T>
class Sequence {
public:
    virtual ~Sequence() = default;

    virtual T getFirst() const = 0;
    virtual T getLast() const = 0;
    virtual T get(int index) const = 0;
    virtual int getLength() const = 0;
    virtual Sequence<T>* getSubsequence(int startIndex, int endIndex) const = 0;

    virtual Sequence<T>* append(T item) = 0;
    virtual Sequence<T>* prepend(T item) = 0;
    virtual Sequence<T>* insertAt(T item, int index) = 0;
    virtual Sequence<T>* remove(int index) = 0;

    virtual Sequence<T>* concat(const Sequence<T>* other) const = 0;
    virtual Sequence<T>* clone() const = 0;

    virtual Sequence<T>* map(std::function<T(T)> func) const = 0;
    virtual Sequence<T>* where(std::function<bool(T)> predicate) const = 0;
    virtual T reduce(std::function<T(T, T)> func, T initial) const = 0;
    virtual Sequence<T>* zip(const Sequence<T>* other, std::function<T(T, T)> combiner) const = 0;
    virtual Sequence<T>* slice(int startIndex, int endIndex) const = 0;

    virtual T& operator[](int index) = 0;
    virtual const T& operator[](int index) const = 0;

    class SequenceIterator {
    private:
        const Sequence<T>* sequence;
        int index;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        SequenceIterator(const Sequence<T>* seq, int idx) : sequence(seq), index(idx) {}

        T operator*() const { return sequence->get(index); }

        SequenceIterator& operator++() {
            ++index;
            return *this;
        }

        bool operator!=(const SequenceIterator& other) const {
            return index != other.index || sequence != other.sequence;
        }
    };

    SequenceIterator begin() const {
        return SequenceIterator(this, 0);
    }

    SequenceIterator end() const {
        return SequenceIterator(this, getLength());
    }
};
