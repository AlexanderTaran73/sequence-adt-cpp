#include "catch.hpp"
#include "immutable_list_sequence.hpp"
#include <memory>

TEST_CASE("ImmutableListSequence Basic Operations", "[ImmutableListSequence]") {
    SECTION("Default constructor creates empty sequence") {
        ImmutableListSequence<int> seq;
        REQUIRE(seq.getLength() == 0);
        REQUIRE_THROWS(seq.getFirst());
        REQUIRE_THROWS(seq.getLast());
    }

    SECTION("Create from array") {
        int data[] = {1, 2, 3};
        ImmutableListSequence<int> seq(data, 3);

        REQUIRE(seq.getLength() == 3);
        
        REQUIRE(seq.get(0) == 1);
        REQUIRE(seq.get(1) == 2);
        REQUIRE(seq.get(2) == 3);
    }
}

TEST_CASE("ImmutableListSequence Access", "[ImmutableListSequence]") {
    ImmutableListSequence<std::string> seq;
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

TEST_CASE("ImmutableListSequence Modification Returns New", "[ImmutableListSequence]") {
    ImmutableListSequence<double> seq;

    SECTION("Append returns new object") {
        auto seq2 = std::unique_ptr<Sequence<double>>(seq.append(1.1));
        REQUIRE(seq2->getLength() == 1);
        REQUIRE(seq.getLength() == 0);
    }

    SECTION("Prepend returns new object") {
        auto seq2 = std::unique_ptr<Sequence<double>>(seq.prepend(2.2));
        REQUIRE(seq2->getLength() == 1);
        REQUIRE(seq.getLength() == 0);
    }

    SECTION("Insert returns new object") {
        auto seq1 = std::unique_ptr<Sequence<double>>(seq.append(1.0)->append(3.0));
        auto inserted = std::unique_ptr<Sequence<double>>(seq1->insertAt(2.0, 1));
        REQUIRE(inserted->get(1) == Approx(2.0));
    }
}

TEST_CASE("ImmutableListSequence Advanced Operations", "[ImmutableListSequence]") {
    int items[] = {1, 2, 3};
    ImmutableListSequence<int> seq(items, 3);

    SECTION("Map doubles values") {
        auto mapped = std::unique_ptr<Sequence<int>>(seq.map([](int x) { return x * 2; }));
        REQUIRE(mapped->get(0) == 2);
        REQUIRE(mapped->get(1) == 4);
        REQUIRE(mapped->get(2) == 6);
    }

    SECTION("Where filters even numbers") {
        auto filtered = std::unique_ptr<Sequence<int>>(seq.where([](int x) { return x % 2 == 0; }));
        REQUIRE(filtered->getLength() == 1);
        REQUIRE(filtered->get(0) == 2);
    }

    SECTION("Reduce sums elements") {
        int result = seq.reduce([](int a, int b) { return a + b; }, 0);
        REQUIRE(result == 6);
    }

    SECTION("Concat with other ImmutableListSequence") {
        int more[] = {4, 5};
        ImmutableListSequence<int> other(more, 2);
        auto combined = std::unique_ptr<Sequence<int>>(seq.concat(&other));
        REQUIRE(combined->getLength() == 5);
        REQUIRE(combined->get(4) == 5);
    }

    SECTION("Zip combines two sequences") {
        int b[] = {10, 20, 30};
        ImmutableListSequence<int> other(b, 3);
        auto zipped = std::unique_ptr<Sequence<int>>(seq.zip(&other, [](int a, int b) { return a + b; }));
        REQUIRE(zipped->getLength() == 3);
        REQUIRE(zipped->get(1) == 2 + 20);
    }
}

TEST_CASE("ImmutableListSequence Edge Cases", "[ImmutableListSequence]") {
    ImmutableListSequence<char> seq;

    SECTION("Remove from empty throws") {
        REQUIRE_THROWS(seq.remove(0));
    }

    SECTION("Slice returns correct subsequence") {
        for (char c = 'a'; c <= 'e'; ++c)
            seq = *dynamic_cast<ImmutableListSequence<char>*>(seq.append(c));

        auto sliced = std::unique_ptr<Sequence<char>>(seq.slice(1, 4));
        REQUIRE(sliced->getLength() == 3);
        REQUIRE(sliced->get(0) == 'b');
        REQUIRE(sliced->get(2) == 'd');
    }
}
