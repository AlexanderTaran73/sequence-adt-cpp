#include "catch.hpp"
#include "mutable_list_sequence.hpp"
#include <memory>

TEST_CASE("MutableListSequence Basic Operations", "[MutableListSequence]") {
    SECTION("Default constructor creates empty sequence") {
        MutableListSequence<int> seq;
        REQUIRE(seq.getLength() == 0);
        REQUIRE_THROWS(seq.getFirst());
    }

    SECTION("Create from array") {
        int data[] = {1, 2, 3};
        MutableListSequence<int> seq(data, 3);

        REQUIRE(seq.getLength() == 3);
        REQUIRE(seq[0] == 1);
        REQUIRE(seq[1] == 2);
        REQUIRE(seq[2] == 3);
    }
}

TEST_CASE("MutableListSequence Element Access", "[MutableListSequence]") {
    MutableListSequence<std::string> seq;
    seq.append("first");
    seq.append("second");

    SECTION("Valid access") {
        REQUIRE(seq.get(0) == "first");
        REQUIRE(seq[1] == "second");
    }

    SECTION("Invalid access throws") {
        REQUIRE_THROWS(seq.get(-1));
        REQUIRE_THROWS(seq.get(2));
    }
}

TEST_CASE("MutableListSequence Modification", "[MutableListSequence]") {
    MutableListSequence<double> seq;

    SECTION("Append elements") {
        seq.append(1.1);
        seq.append(2.2);
        REQUIRE(seq.getLength() == 2);
        REQUIRE(seq.getLast() == Approx(2.2));
    }

    SECTION("Prepend elements") {
        seq.prepend(3.3);
        seq.prepend(4.4);
        REQUIRE(seq.getLength() == 2);
        REQUIRE(seq.getFirst() == Approx(4.4));
    }

    SECTION("Insert at position") {
        seq.append(1.0);
        seq.append(3.0);
        seq.insertAt(2.0, 1);

        REQUIRE(seq.getLength() == 3);
        REQUIRE(seq[1] == Approx(2.0));
    }
}

TEST_CASE("MutableListSequence Advanced Operations", "[MutableListSequence]") {
    MutableListSequence<int> seq;
    seq.append(1);
    seq.append(2);
    seq.append(3);

    SECTION("Get subsequence") {
        auto sub = std::unique_ptr<Sequence<int>>(seq.getSubsequence(1, 2));
        REQUIRE(sub->getLength() == 2);
        REQUIRE(sub->get(0) == 2);
    }

    SECTION("Concatenate sequences") {
        MutableListSequence<int> other;
        other.append(4);

        auto combined = std::unique_ptr<Sequence<int>>(seq.concat(&other));
        REQUIRE(combined->getLength() == 4);
        REQUIRE(combined->get(3) == 4);
    }

    SECTION("Map operation") {
        auto doubled = std::unique_ptr<Sequence<int>>(
            seq.map([](int x) { return x * 2; }));
        REQUIRE(doubled->get(1) == 4);
    }

    SECTION("Zip operation") {
        MutableListSequence<int> other;
        other.append(10);
        other.append(20);
        other.append(30);
        auto zipped = std::unique_ptr<Sequence<int>>(
            seq.zip(&other, [](int a, int b) { return a + b; }));
        REQUIRE(zipped->get(1) == 2 + 20);
    }
}

TEST_CASE("MutableListSequence Edge Cases", "[MutableListSequence]") {
    SECTION("Empty sequence operations") {
        MutableListSequence<float> empty;

        REQUIRE_THROWS(empty.remove(0));
        REQUIRE_THROWS(empty.getLast());
    }

    SECTION("Single element sequence") {
        MutableListSequence<char> single;
        single.append('A');

        REQUIRE(single.getLength() == 1);
        REQUIRE(single.getFirst() == 'A');
        REQUIRE(single.getLast() == 'A');

        single.remove(0);
        REQUIRE(single.getLength() == 0);
    }

    SECTION("Slice returns correct subsequence") {
        MutableListSequence<int> seq;
        for (int i = 0; i < 5; ++i) seq.append(i);

        auto sliced = std::unique_ptr<Sequence<int>>(seq.slice(1, 4));
        REQUIRE(sliced->getLength() == 3);
        REQUIRE(sliced->get(0) == 1);
        REQUIRE(sliced->get(2) == 3);
    }
}
