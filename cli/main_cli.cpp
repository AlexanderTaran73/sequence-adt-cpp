#include <iostream>
#include <functional>
#include <string>
#include <sstream>
#include <memory>
#include <limits>

#include "mutable_array_sequence.hpp"
#include "immutable_array_sequence.hpp"
#include "mutable_list_sequence.hpp"
#include "immutable_list_sequence.hpp"
#include "queue.hpp"

bool safeReadInt(int& val) {
    std::string line;
    std::getline(std::cin, line);

    std::stringstream ss(line);
    if ((ss >> val) && ss.eof()) {
        return true;
    }

    return false;
}


bool safeReadDouble(double& val) {
    std::string line;
    std::getline(std::cin, line);

    std::stringstream ss(line);
    if ((ss >> val) && ss.eof()) {
        return true;
    }

    return false;
}


void safeReadString(std::string& val) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, val);
}

template <typename T>
class SequenceCLI {
private:
    Sequence<T>* sequence;

public:
    SequenceCLI(Sequence<T>* seq) : sequence(seq) {}
    ~SequenceCLI() { delete sequence; }

    void show() const {
        std::cout << "Sequence: [ ";
        for (int i = 0; i < sequence->getLength(); ++i)
            std::cout << sequence->get(i) << ", ";
        std::cout << "]\n";
    }

    void getFirst() const {
        std::cout << "First element: " << sequence->getFirst() << "\n";
    }

    void getLast() const {
        std::cout << "Last element: " << sequence->getLast() << "\n";
    }

    void getByIndex() const {
        int index;
        std::cout << "Enter index: ";
        if (!safeReadInt(index)) {
            std::cout << "Invalid input. Index must be an integer.\n";
            return;
        }
        std::cout << "Element: " << sequence->get(index) << "\n";
    }

    void getLength() const {
        std::cout << "Length: " << sequence->getLength() << "\n";
    }

    void append() {
        T value;
        std::cout << "Enter value to append: ";
        if (!readValue(value)) {
            std::cout << "Invalid input format for value.\n";
            return;
        }
        Sequence<T>* newSeq = sequence->append(value);
        sequence = newSeq;
    }

    void prepend() {
        T value;
        std::cout << "Enter value to prepend: ";
        if (!readValue(value)) {
            std::cout << "Invalid input format for value.\n";
            return;
        }
        Sequence<T>* newSeq = sequence->prepend(value);
        sequence = newSeq;
    }

    void insertAt() {
        T value;
        int index;
        std::cout << "Enter value and index: ";
        if (!readValue(value)) {
            std::cout << "Invalid input format for value.\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }
        if (!safeReadInt(index)) {
            std::cout << "Invalid input. Index must be an integer.\n";
            return;
        }
        Sequence<T>* newSeq = sequence->insertAt(value, index);
        delete sequence;
        sequence = newSeq;
    }

    void remove() {
        int index;
        std::cout << "Enter index to remove: ";
        if (!safeReadInt(index)) {
            std::cout << "Invalid input. Index must be an integer.\n";
            return;
        }
        Sequence<T>* newSeq = sequence->remove(index);
        sequence = newSeq;
    }

    void getSubsequence() const {
        int start, end;
        std::cout << "Enter start and end indices: ";
        if (!safeReadInt(start) || !safeReadInt(end)) {
            std::cout << "Invalid input. Indices must be integers.\n";
            return;
        }
        Sequence<T>* sub = sequence->getSubsequence(start, end);
        SequenceCLI<T> cli(sub);
        cli.show();
        delete sub;
    }

    void slice() const {
        int start, end;
        std::cout << "Enter start, end: ";
        if (!safeReadInt(start) || !safeReadInt(end)) {
            std::cout << "Invalid input. Indices must be integers.\n";
            return;
        }
        Sequence<T>* sub = sequence->slice(start, end);
        SequenceCLI<T> cli(sub);
        cli.show();
        delete sub;
    }

    void map() const {
        if constexpr (std::is_same_v<T, int>) {
            auto mapped = sequence->map([](T x) { return x * 2; });
            SequenceCLI<T> cli(mapped);
            std::cout << "Mapped (x * 2): ";
            cli.show();
            delete mapped;
        } else if constexpr (std::is_same_v<T, double>) {
            auto mapped = sequence->map([](T x) { return x * 1.5; });
            SequenceCLI<T> cli(mapped);
            std::cout << "Mapped (x * 1.5): ";
            cli.show();
            delete mapped;
        } else {
            std::cout << "Map not supported for this type\n";
        }
    }

    void where() const {
        if constexpr (std::is_same_v<T, int>) {
            auto filtered = sequence->where([](T x) { return x % 2 == 0; });
            SequenceCLI<T> cli(filtered);
            std::cout << "Filtered (even): ";
            cli.show();
            delete filtered;
        } else if constexpr (std::is_same_v<T, double>) {
            auto filtered = sequence->where([](T x) { return x > 0; });
            SequenceCLI<T> cli(filtered);
            std::cout << "Filtered (x > 0): ";
            cli.show();
            delete filtered;
        } else {
            std::cout << "Where not supported for this type\n";
        }
    }

    void reduce() const {
        if constexpr (std::is_same_v<T, int>) {
            auto result = sequence->reduce([](T a, T b) { return a + b; }, 0);
            std::cout << "Reduced sum: " << result << "\n";
        } else if constexpr (std::is_same_v<T, double>) {
            auto result = sequence->reduce([](T a, T b) { return a + b; }, 0.0);
            std::cout << "Reduced sum: " << result << "\n";
        } else {
            std::cout << "Reduce not supported for this type\n";
        }
    }

    void run() {
        while (true) {
            std::cout << "\n==== Sequence CLI Menu ====\n";
            std::cout << "1. Show sequence\n";
            std::cout << "2. Get first\n";
            std::cout << "3. Get last\n";
            std::cout << "4. Get by index\n";
            std::cout << "5. Length\n";
            std::cout << "6. Append\n";
            std::cout << "7. Prepend\n";
            std::cout << "8. Insert at index\n";
            std::cout << "9. Remove at index\n";
            std::cout << "10. Subsequence\n";
            std::cout << "11. Slice\n";
            std::cout << "12. Map\n";
            std::cout << "13. Where\n";
            std::cout << "14. Reduce\n";
            std::cout << "0. Exit\n";
            std::cout << "Choose option: ";

            int option;
            if (!safeReadInt(option)) {
                std::cout << "Invalid input. Please enter a number from 0 to 14.\n";
                continue;
            }

            try {
                switch (option) {
                    case 1: show(); break;
                    case 2: getFirst(); break;
                    case 3: getLast(); break;
                    case 4: getByIndex(); break;
                    case 5: getLength(); break;
                    case 6: append(); break;
                    case 7: prepend(); break;
                    case 8: insertAt(); break;
                    case 9: remove(); break;
                    case 10: getSubsequence(); break;
                    case 11: slice(); break;
                    case 12: map(); break;
                    case 13: where(); break;
                    case 14: reduce(); break;
                    case 0: return;
                    default: std::cout << "Invalid option\n";
                }
            } catch (const BaseError& e) {
                std::cout << "[Error] " << e.what() << "\n";
            } catch (...) {
                std::cout << "[Unknown Error] Something went wrong.\n";
            }
        }
    }

private:
    bool readValue(int& v) {
        return safeReadInt(v);
    }
    bool readValue(double& v) {
        return safeReadDouble(v);
    }
    bool readValue(std::string& v) {
        safeReadString(v);
        return !v.empty();
    }
};

template <typename T>
class QueueCLI {
private:
    Queue<T> queue;

public:
    QueueCLI() = default;

    void show() const {
        std::cout << "Queue: [ ";
        for (int i = 0; i < queue.size(); ++i) {
            std::cout << queue.get(i) << ", ";
        }
        std::cout << "]\n";
    }

    void enqueue() {
        T value;
        std::cout << "Enter value to enqueue: ";
        if (!readValue(value)) {
            std::cout << "Invalid input format for value.\n";
            return;
        }
        queue.enqueue(value);
    }

    void dequeue() {
        try {
            T val = queue.dequeue();
            std::cout << "Dequeued value: " << val << "\n";
        } catch (const std::runtime_error& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    void front() const {
        try {
            std::cout << "Front element: " << queue.front() << "\n";
        } catch (const std::runtime_error& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    void size() const {
        std::cout << "Size: " << queue.size() << "\n";
    }

    void isEmpty() const {
        std::cout << "Queue is " << (queue.isEmpty() ? "empty" : "not empty") << "\n";
    }

    void clear() {
        queue.clear();
        std::cout << "Queue cleared\n";
    }

    void map() const {
        if constexpr (std::is_same_v<T, int>) {
            auto mapped = queue.map([](T x) { return x * 2; });
            QueueCLI<T> cli;
            cli.queue = mapped;
            std::cout << "Mapped (x * 2): ";
            cli.show();
        } else if constexpr (std::is_same_v<T, double>) {
            auto mapped = queue.map([](T x) { return x * 1.5; });
            QueueCLI<T> cli;
            cli.queue = mapped;
            std::cout << "Mapped (x * 1.5): ";
            cli.show();
        } else {
            std::cout << "Map not supported for this type\n";
        }
    }

    void where() const {
        if constexpr (std::is_same_v<T, int>) {
            auto filtered = queue.where([](T x) { return x % 2 == 0; });
            QueueCLI<T> cli;
            cli.queue = filtered;
            std::cout << "Filtered (even): ";
            cli.show();
        } else if constexpr (std::is_same_v<T, double>) {
            auto filtered = queue.where([](T x) { return x > 0; });
            QueueCLI<T> cli;
            cli.queue = filtered;
            std::cout << "Filtered (x > 0): ";
            cli.show();
        } else {
            std::cout << "Where not supported for this type\n";
        }
    }

    void reduce() const {
        if constexpr (std::is_same_v<T, int>) {
            auto result = queue.reduce([](int a, int b) { return a + b; }, 0);
            std::cout << "Reduced sum: " << result << "\n";
        } else if constexpr (std::is_same_v<T, double>) {
            auto result = queue.reduce([](double a, double b) { return a + b; }, 0.0);
            std::cout << "Reduced sum: " << result << "\n";
        } else {
            std::cout << "Reduce not supported for this type\n";
        }
    }


    void run() {
        while (true) {
            std::cout << "\n==== Queue CLI Menu ====\n";
            std::cout << "1. Show queue\n";
            std::cout << "2. Enqueue\n";
            std::cout << "3. Dequeue\n";
            std::cout << "4. Front element\n";
            std::cout << "5. Size\n";
            std::cout << "6. Check empty\n";
            std::cout << "7. Clear\n";
            std::cout << "8. Map\n";
            std::cout << "9. Where\n";
            std::cout << "10. Reduce\n";              
            std::cout << "0. Exit\n";
            std::cout << "Choose option: ";

            int option;
            if (!safeReadInt(option)) {
                std::cout << "Invalid input. Please enter a number from 0 to 10.\n";
                continue;
            }

            try {
                switch (option) {
                    case 1: show(); break;
                    case 2: enqueue(); break;
                    case 3: dequeue(); break;
                    case 4: front(); break;
                    case 5: size(); break;
                    case 6: isEmpty(); break;
                    case 7: clear(); break;
                    case 8: map(); break;
                    case 9: where(); break;
                    case 10: reduce(); break;
                    case 0: return;
                    default: std::cout << "Invalid option\n";
                }
            } catch (const std::exception& e) {
                std::cout << "[Error] " << e.what() << "\n";
            } catch (...) {
                std::cout << "[Unknown Error] Something went wrong.\n";
            }
        }
    }

private:
    bool readValue(int& v) { return safeReadInt(v); }
    bool readValue(double& v) { return safeReadDouble(v); }
    bool readValue(std::string& v) { safeReadString(v); return !v.empty(); }
};

template <typename T>
void runSequenceCLI(int implChoice) {
    Sequence<T>* seq = nullptr;
    switch (implChoice) {
        case 1: seq = new MutableArraySequence<T>(); break;
        case 2: seq = new ImmutableArraySequence<T>(); break;
        case 3: seq = new MutableListSequence<T>(); break;
        case 4: seq = new ImmutableListSequence<T>(); break;
        default:
            std::cout << "Invalid choice, using MutableArraySequence\n";
            seq = new MutableArraySequence<T>();
    }
    SequenceCLI<T> cli(seq);
    cli.run();
}

template <typename T>
void runQueueCLI() {
    QueueCLI<T> cli;
    cli.run();
}

int main() {
    while (true) {
        std::cout << "\n==== Main Menu ====\n";
        std::cout << "1. Work with Sequence\n";
        std::cout << "2. Work with Queue\n";
        std::cout << "0. Exit\n";
        std::cout << "Your choice: ";

        int mainChoice;
        if (!safeReadInt(mainChoice)) {
            std::cout << "Invalid input\n";
            continue;
        }

        if (mainChoice == 0) break;

        std::cout << "\nChoose data type:\n";
        std::cout << "1. int\n2. double\n3. string\n";
        std::cout << "Your choice: ";
        int typeChoice;
        if (!safeReadInt(typeChoice) || typeChoice < 1 || typeChoice > 3) {
            std::cout << "Invalid data type choice\n";
            continue;
        }

        if (mainChoice == 1) {
            std::cout << "\nChoose sequence implementation:\n";
            std::cout << "1. MutableArraySequence\n2. ImmutableArraySequence\n";
            std::cout << "3. MutableListSequence\n4. ImmutableListSequence\n";
            std::cout << "Your choice: ";
            int implChoice;
            if (!safeReadInt(implChoice) || implChoice < 1 || implChoice > 4) {
                std::cout << "Invalid implementation choice\n";
                continue;
            }

            switch (typeChoice) {
                case 1: runSequenceCLI<int>(implChoice); break;
                case 2: runSequenceCLI<double>(implChoice); break;
                case 3: runSequenceCLI<std::string>(implChoice); break;
            }
        }
        else if (mainChoice == 2) {
            switch (typeChoice) {
                case 1: runQueueCLI<int>(); break;
                case 2: runQueueCLI<double>(); break;
                case 3: runQueueCLI<std::string>(); break;
            }
        }
    }

    return 0;
}