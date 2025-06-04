#include "catch.hpp"
#include "immutable_array_sequence.hpp"
#include <memory>

TEST_CASE("ImmutableArraySequence Basic Operations", "[ImmutableArraySequence]") {
    SECTION("Default constructor creates empty sequence") {
        ImmutableArraySequence<int> seq;
        REQUIRE(seq.getLength() == 0);
        REQUIRE_THROWS(seq.getFirst());
        REQUIRE_THROWS(seq.getLast());
    }

    SECTION("Create from array") {
        int data[] = {1, 2, 3};
        ImmutableArraySequence<int> seq(data, 3);

        REQUIRE(seq.getLength() == 3);
        REQUIRE(seq.get(0) == 1);
        REQUIRE(seq.get(1) == 2);
        REQUIRE(seq.get(2) == 3);
    }
}

TEST_CASE("ImmutableArraySequence Access and Safety", "[ImmutableArraySequence]") {
    ImmutableArraySequence<std::string> seq;
    auto extended = std::unique_ptr<Sequence<std::string>>(seq.append("hello")->append("world"));

    SECTION("Valid access") {
        REQUIRE(extended->get(0) == "hello");
        REQUIRE(extended->get(1) == "world");
    }

    SECTION("Invalid access throws") {
        REQUIRE_THROWS(extended->get(-1));
        REQUIRE_THROWS(extended->get(3));
    }

    SECTION("Mutable operator[] throws") {
        REQUIRE_THROWS((*const_cast<Sequence<std::string>*>(extended.get()))[0]);
    }
}

TEST_CASE("ImmutableArraySequence Modifications Return New", "[ImmutableArraySequence]") {
    ImmutableArraySequence<double> seq;

    SECTION("Append returns new sequence") {
        auto seq2 = std::unique_ptr<Sequence<double>>(seq.append(1.1));
        REQUIRE(seq2->getLength() == 1);
        REQUIRE(seq.getLength() == 0);
    }

    SECTION("Prepend returns new sequence") {
        auto seq2 = std::unique_ptr<Sequence<double>>(seq.prepend(2.2));
        REQUIRE(seq2->getLength() == 1);
        REQUIRE(seq.getLength() == 0);
    }

    SECTION("InsertAt works as expected") {
        auto seq1 = std::unique_ptr<Sequence<double>>(seq.append(1.0)->append(3.0));
        auto inserted = std::unique_ptr<Sequence<double>>(seq1->insertAt(2.0, 1));
        REQUIRE(inserted->get(1) == Approx(2.0));
    }
}

TEST_CASE("ImmutableArraySequence Functional Methods", "[ImmutableArraySequence]") {
    int data[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(data, 3);

    SECTION("Map doubles values") {
        auto mapped = std::unique_ptr<Sequence<int>>(seq.map([](int x) { return x * 2; }));
        REQUIRE(mapped->get(0) == 2);
        REQUIRE(mapped->get(1) == 4);
        REQUIRE(mapped->get(2) == 6);
    }

    SECTION("Where filters even values") {
        auto filtered = std::unique_ptr<Sequence<int>>(seq.where([](int x) { return x % 2 == 0; }));
        REQUIRE(filtered->getLength() == 1);
        REQUIRE(filtered->get(0) == 2);
    }

    SECTION("Reduce sums values") {
        int sum = seq.reduce([](int a, int b) { return a + b; }, 0);
        REQUIRE(sum == 6);
    }

    SECTION("Zip combines values") {
        int b[] = {10, 20, 30};
        ImmutableArraySequence<int> other(b, 3);
        auto zipped = std::unique_ptr<Sequence<int>>(seq.zip(&other, [](int a, int b) { return a + b; }));
        REQUIRE(zipped->getLength() == 3);
        REQUIRE(zipped->get(2) == 33);
    }
}

TEST_CASE("ImmutableArraySequence Subsequence and Slice", "[ImmutableArraySequence]") {
    int values[] = {10, 20, 30, 40, 50};
    ImmutableArraySequence<int> seq(values, 5);

    SECTION("GetSubsequence works") {
        auto sub = std::unique_ptr<Sequence<int>>(seq.getSubsequence(1, 3));
        REQUIRE(sub->getLength() == 3);
        REQUIRE(sub->get(0) == 20);
        REQUIRE(sub->get(2) == 40);
    }

    SECTION("Slice works correctly") {
        auto sliced = std::unique_ptr<Sequence<int>>(seq.slice(2, 4));
        REQUIRE(sliced->getLength() == 2);
        REQUIRE(sliced->get(0) == 30);
        REQUIRE(sliced->get(1) == 40);
    }

    SECTION("Slice with invalid range returns empty") {
        auto sliced = std::unique_ptr<Sequence<int>>(seq.slice(5, 5));
        REQUIRE(sliced->getLength() == 0);
    }
}
