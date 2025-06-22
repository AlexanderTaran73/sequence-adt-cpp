#include "catch.hpp"
#include "deque.hpp"
#include <string>

TEST_CASE("Deque Basic Operations", "[Deque]") {
    SECTION("Default constructor creates empty deque") {
        Deque<int> d;
        REQUIRE(d.size() == 0);
        REQUIRE(d.isEmpty());
        REQUIRE_THROWS(d.front());
        REQUIRE_THROWS(d.back());
        REQUIRE_THROWS(d.popFront());
        REQUIRE_THROWS(d.popBack());
    }

    SECTION("Push and pop operations") {
        Deque<std::string> d;
        d.pushFront("first");
        d.pushBack("second");

        REQUIRE(d.size() == 2);
        REQUIRE(d.front() == "first");
        REQUIRE(d.back() == "second");
        
        std::string frontItem = d.popFront();
        REQUIRE(frontItem == "first");
        REQUIRE(d.size() == 1);
        
        std::string backItem = d.popBack();
        REQUIRE(backItem == "second");
        REQUIRE(d.isEmpty());
    }
}

TEST_CASE("Deque Element Access", "[Deque]") {
    Deque<double> d;
    d.pushBack(1.1);
    d.pushFront(2.2);
    d.pushBack(3.3);

    SECTION("Front and back access") {
        REQUIRE(d.front() == Approx(2.2));
        REQUIRE(d.back() == Approx(3.3));
        d.popFront();
        REQUIRE(d.front() == Approx(1.1));
    }

    SECTION("Get by index") {
        REQUIRE(d.get(0) == Approx(2.2));
        REQUIRE(d.get(1) == Approx(1.1));
        REQUIRE(d.get(2) == Approx(3.3));
        REQUIRE_THROWS(d.get(-1));
        REQUIRE_THROWS(d.get(3));
    }
}

TEST_CASE("Deque Functional Operations", "[Deque]") {
    Deque<int> d;
    for (int i = 1; i <= 5; ++i) d.pushBack(i);

    SECTION("Map operation") {
        auto doubled = d.map([](int x) { return x * 2; });
        REQUIRE(doubled.size() == 5);
        REQUIRE(doubled.get(0) == 2);
        REQUIRE(doubled.get(4) == 10);
    }

    SECTION("Where operation") {
        auto evens = d.where([](int x) { return x % 2 == 0; });
        REQUIRE(evens.size() == 2);
        REQUIRE(evens.get(0) == 2);
        REQUIRE(evens.get(1) == 4);
    }

    SECTION("Reduce operation") {
        int sum = d.reduce([](int a, int b) { return a + b; }, 0);
        REQUIRE(sum == 15);
    }
}

TEST_CASE("Deque Combination Operations", "[Deque]") {
    SECTION("Concatenation") {
        Deque<int> d1;
        d1.pushBack(1);
        d1.pushBack(2);

        Deque<int> d2;
        d2.pushBack(3);

        auto combined = d1.concat(d2);
        REQUIRE(combined.size() == 3);
        REQUIRE(combined.front() == 1);
        REQUIRE(combined.back() == 3);
    }

    SECTION("Subsequence extraction") {
        Deque<char> d;
        for (char c = 'a'; c <= 'e'; ++c) d.pushBack(c);

        auto sub = d.getSubsequence(1, 3);
        REQUIRE(sub.size() == 3);
        REQUIRE(sub.get(0) == 'b');
        REQUIRE(sub.get(2) == 'd');
    }

    SECTION("Contains operations") {
        Deque<int> d;
        d.pushBack(10);
        d.pushBack(20);
        d.pushBack(30);
        d.pushBack(40);

        REQUIRE(d.contains(20));
        REQUIRE_FALSE(d.contains(50));

        Deque<int> sub;
        sub.pushBack(20);
        sub.pushBack(30);
        REQUIRE(d.containsSubsequence(sub));
    }
}

TEST_CASE("Deque Advanced Operations", "[Deque]") {
    Deque<std::string> d;
    d.pushBack("apple");
    d.pushBack("banana");
    d.pushBack("cherry");

    SECTION("Zip operation") {
        Deque<int> nums;
        nums.pushBack(1);
        nums.pushBack(2);

        auto zipped = d.zip(nums);
        REQUIRE(zipped.size() == 2);
        REQUIRE(zipped.get(0).first == "apple");
        REQUIRE(zipped.get(0).second == 1);
    }

    SECTION("Split operation") {
        auto [short_words, long_words] = d.split([](const std::string& str) { 
            return str.length() < 6; 
        });

        REQUIRE(short_words.size() == 1);
        REQUIRE(long_words.size() == 2);
        REQUIRE(short_words.contains("apple"));
        REQUIRE(long_words.contains("banana"));
    }

    SECTION("Equality comparison") {
        Deque<std::string> d2;
        d2.pushBack("apple");
        d2.pushBack("banana");

        REQUIRE(d != d2);
        d2.pushBack("cherry");
        REQUIRE(d == d2);
    }
}

TEST_CASE("Deque Edge Cases", "[Deque]") {
    SECTION("Empty deque operations") {
        Deque<float> empty;

        REQUIRE_THROWS(empty.popFront());
        REQUIRE_THROWS(empty.popBack());
        REQUIRE_THROWS(empty.front());
        REQUIRE_THROWS(empty.back());
    }

    SECTION("Single element deque") {
        Deque<char> single;
        single.pushFront('X');

        REQUIRE(single.front() == 'X');
        REQUIRE(single.back() == 'X');
        REQUIRE(single.popFront() == 'X');
        REQUIRE(single.isEmpty());
    }

    SECTION("Clear operation") {
        Deque<int> d;
        d.pushBack(1);
        d.pushFront(2);
        d.clear();

        REQUIRE(d.isEmpty());
        REQUIRE(d.size() == 0);
    }
}

TEST_CASE("Deque Order Preservation", "[Deque]") {
    SECTION("FIFO order verification") {
        Deque<int> d;
        d.pushBack(1);
        d.pushBack(2);
        d.pushBack(3);

        REQUIRE(d.popFront() == 1);
        REQUIRE(d.popFront() == 2);
        REQUIRE(d.popFront() == 3);
        REQUIRE(d.isEmpty());
    }

    SECTION("LIFO order verification") {
        Deque<std::string> d;
        d.pushFront("first");
        d.pushFront("last");

        REQUIRE(d.front() == "last");
        d.popFront();
        REQUIRE(d.front() == "first");
    }
}

TEST_CASE("Deque Sorting", "[Deque]") {
    SECTION("Sort numbers") {
        Deque<int> d;
        d.pushBack(3);
        d.pushFront(1);
        d.pushBack(4);
        d.pushFront(2);

        d.sort();
        REQUIRE(d.get(0) == 1);
        REQUIRE(d.get(1) == 2);
        REQUIRE(d.get(2) == 3);
        REQUIRE(d.get(3) == 4);
    }

    SECTION("Sort strings by length") {
        Deque<std::string> d;
        d.pushBack("banana");
        d.pushFront("apple");
        d.pushBack("kiwi");
        d.pushFront("pear");

        d.sort([](const std::string& a, const std::string& b) {
            return a.length() < b.length();
        });
        
        REQUIRE(d.get(0).length() <= d.get(1).length());
        REQUIRE(d.get(1).length() <= d.get(2).length());
        REQUIRE(d.get(2).length() <= d.get(3).length());
    }
}