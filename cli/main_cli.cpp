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
void runCLIForTypeAndImpl(int implChoice) {
    Sequence<T>* seq = nullptr;
    switch (implChoice) {
        case 1: seq = new MutableArraySequence<T>(); break;
        case 2: seq = new ImmutableArraySequence<T>(); break;
        case 3: seq = new MutableListSequence<T>(); break;
        case 4: seq = new ImmutableListSequence<T>(); break;
        default:
            std::cout << "Invalid implementation choice, defaulting to MutableArraySequence\n";
            seq = new MutableArraySequence<T>();
            break;
    }
    SequenceCLI<T> cli(seq);
    cli.run();
}

int main() {
    std::cout << "Choose data type:\n";
    std::cout << "1. int\n2. double\n3. string\n";
    std::cout << "Your choice: ";
    int typeChoice;
    if (!safeReadInt(typeChoice) || typeChoice < 1 || typeChoice > 3) {
        std::cout << "Invalid data type choice\n";
        return 1;
    }

    std::cout << "Choose sequence implementation:\n";
    std::cout << "1. MutableArraySequence\n2. ImmutableArraySequence\n3. MutableListSequence\n4. ImmutableListSequence\n";
    std::cout << "Your choice: ";
    int implChoice;
    if (!safeReadInt(implChoice) || implChoice < 1 || implChoice > 4) {
        std::cout << "Invalid implementation choice\n";
        return 1;
    }

    switch (typeChoice) {
        case 1: runCLIForTypeAndImpl<int>(implChoice); break;
        case 2: runCLIForTypeAndImpl<double>(implChoice); break;
        case 3: runCLIForTypeAndImpl<std::string>(implChoice); break;
    }

    return 0;
}
