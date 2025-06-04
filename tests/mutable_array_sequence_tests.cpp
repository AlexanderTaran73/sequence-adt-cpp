#include "catch.hpp"
#include "mutable_array_sequence.hpp"
#include <memory>

TEST_CASE("MutableArraySequence Basic Operations", "[MutableArraySequence]") {
    SECTION("Default constructor creates empty sequence") {
        MutableArraySequence<int> seq;
        REQUIRE(seq.getLength() == 0);
        REQUIRE_THROWS_WITH(seq.getFirst(), Catch::Matchers::Contains("Empty array"));
    }

    SECTION("Create from array") {
        int data[] = {1, 2, 3};
        MutableArraySequence<int> seq(data, 3);
        
        REQUIRE(seq.getLength() == 3);
        REQUIRE(seq[0] == 1);
        REQUIRE(seq[1] == 2);
        REQUIRE(seq[2] == 3);
    }
}

TEST_CASE("MutableArraySequence Element Access", "[MutableArraySequence]") {
    MutableArraySequence<std::string> seq;
    seq.append("first");
    seq.append("second");

    SECTION("Valid access") {
        REQUIRE(seq.get(0) == "first");
        REQUIRE(seq[1] == "second");
    }

    SECTION("Invalid access throws") {
        REQUIRE_THROWS_WITH(seq.get(-1), Catch::Matchers::Contains("Index out of range"));
        REQUIRE_THROWS_WITH(seq.get(2), Catch::Matchers::Contains("Index out of range"));
    }
}

TEST_CASE("MutableArraySequence Modification", "[MutableArraySequence]") {
    MutableArraySequence<double> seq;
    
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


TEST_CASE("MutableArraySequence Advanced Operations", "[MutableArraySequence]") {
    MutableArraySequence<int> seq;
    seq.append(1);
    seq.append(2);
    seq.append(3);

    SECTION("Get subsequence") {
        auto sub = std::unique_ptr<Sequence<int>>(seq.getSubsequence(1, 2));
        REQUIRE(sub->getLength() == 2);
        REQUIRE(sub->get(0) == 2);
    }

    SECTION("Concatenate sequences") {
        MutableArraySequence<int> other;
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
}

TEST_CASE("MutableArraySequence Edge Cases", "[MutableArraySequence]") {
    SECTION("Empty sequence operations") {
        MutableArraySequence<float> empty;
        
        REQUIRE_THROWS_WITH(empty.remove(0), Catch::Matchers::Contains("Empty array"));
        REQUIRE_THROWS_WITH(empty.getLast(), Catch::Matchers::Contains("Empty array"));
    }

    SECTION("Single element sequence") {
        MutableArraySequence<char> single;
        single.append('A');
        
        REQUIRE(single.getLength() == 1);
        REQUIRE(single.getFirst() == 'A');
        REQUIRE(single.getLast() == 'A');
        
        single.remove(0);
        REQUIRE(single.getLength() == 0);
    }
}
