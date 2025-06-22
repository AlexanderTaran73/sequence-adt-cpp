#include "catch.hpp"
#include "stack.hpp"
#include <memory>
#include <string>

TEST_CASE("Stack Basic Operations", "[Stack]") {
    SECTION("Default constructor creates empty stack") {
        Stack<int> s;
        REQUIRE(s.size() == 0);
        REQUIRE(s.isEmpty());
        REQUIRE_THROWS(s.top());
        REQUIRE_THROWS(s.pop());
    }

    SECTION("Push and pop operations") {
        Stack<std::string> s;
        s.push("first");
        s.push("second");

        REQUIRE(s.size() == 2);
        REQUIRE(s.top() == "second");
        
        std::string item = s.pop();
        REQUIRE(item == "second");
        REQUIRE(s.size() == 1);
        REQUIRE(s.top() == "first");
    }
}

TEST_CASE("Stack Element Access", "[Stack]") {
    Stack<double> s;
    s.push(1.1);
    s.push(2.2);
    s.push(3.3);

    SECTION("Top access") {
        REQUIRE(s.top() == Approx(3.3));
        s.pop();
        REQUIRE(s.top() == Approx(2.2));
    }

    SECTION("Get by index") {
        REQUIRE(s.get(0) == Approx(1.1));
        REQUIRE(s.get(2) == Approx(3.3));
        REQUIRE_THROWS(s.get(-1));
        REQUIRE_THROWS(s.get(3));
    }
}

TEST_CASE("Stack Functional Operations", "[Stack]") {
    Stack<int> s;
    for (int i = 1; i <= 5; ++i) s.push(i);

    SECTION("Map operation") {
        auto doubled = s.map([](int x) { return x * 2; });
        REQUIRE(doubled.size() == 5);
        REQUIRE(doubled.get(0) == 2);
        REQUIRE(doubled.top() == 10);
    }

    SECTION("Where operation") {
        auto evens = s.where([](int x) { return x % 2 == 0; });
        REQUIRE(evens.size() == 2);
        REQUIRE(evens.get(0) == 2);
        REQUIRE(evens.top() == 4);
    }

    SECTION("Reduce operation") {
        int sum = s.reduce([](int a, int b) { return a + b; }, 0);
        REQUIRE(sum == 15);
    }
}

TEST_CASE("Stack Combination Operations", "[Stack]") {
    SECTION("Concatenation") {
        Stack<int> s1;
        s1.push(1);
        s1.push(2);

        Stack<int> s2;
        s2.push(3);

        auto combined = s1.concat(s2);
        REQUIRE(combined.size() == 3);
        REQUIRE(combined.top() == 3);
        REQUIRE(combined.get(0) == 1);
    }

    SECTION("Subsequence extraction") {
        Stack<char> s;
        for (char c = 'a'; c <= 'e'; ++c) s.push(c);

        auto sub = s.getSubsequence(1, 3);
        REQUIRE(sub.size() == 3);
        REQUIRE(sub.get(0) == 'b');
        REQUIRE(sub.top() == 'd');
    }

    SECTION("Contains operations") {
        Stack<int> s;
        s.push(10);
        s.push(20);
        s.push(30);
        s.push(40);

        REQUIRE(s.contains(20));
        REQUIRE_FALSE(s.contains(50));

        Stack<int> sub;
        sub.push(20);
        sub.push(30);
        REQUIRE(s.containsSubsequence(sub));
    }
}

TEST_CASE("Stack Advanced Operations", "[Stack]") {
    Stack<std::string> s;
    s.push("apple");
    s.push("banana");
    s.push("cherry");

    SECTION("Zip operation") {
        Stack<int> nums;
        nums.push(1);
        nums.push(2);

        auto zipped = s.zip(nums);
        REQUIRE(zipped.size() == 2);
        REQUIRE(zipped.top().first == "banana");
        REQUIRE(zipped.top().second == 2);
    }

    SECTION("Split operation") {
        auto [short_words, long_words] = s.split([](const std::string& str) { 
            return str.length() < 6; 
        });

        REQUIRE(short_words.size() == 1);
        REQUIRE(long_words.size() == 2);
        REQUIRE(short_words.contains("apple"));
        REQUIRE(long_words.contains("banana"));
    }

    SECTION("Equality comparison") {
        Stack<std::string> s2;
        s2.push("apple");
        s2.push("banana");

        REQUIRE(s != s2);
        s2.push("cherry");
        REQUIRE(s == s2);
    }
}

TEST_CASE("Stack Edge Cases", "[Stack]") {
    SECTION("Empty stack operations") {
        Stack<float> empty;

        REQUIRE_THROWS(empty.pop());
        REQUIRE_THROWS(empty.top());
    }

    SECTION("Single element stack") {
        Stack<char> single;
        single.push('X');

        REQUIRE(single.top() == 'X');
        REQUIRE(single.pop() == 'X');
        REQUIRE(single.isEmpty());
    }

    SECTION("Clear operation") {
        Stack<int> s;
        s.push(1);
        s.push(2);
        s.clear();

        REQUIRE(s.isEmpty());
        REQUIRE(s.size() == 0);
    }
}

TEST_CASE("Stack Order Preservation", "[Stack]") {
    SECTION("LIFO order verification") {
        Stack<int> s;
        s.push(1);
        s.push(2);
        s.push(3);

        REQUIRE(s.pop() == 3);
        REQUIRE(s.pop() == 2);
        REQUIRE(s.pop() == 1);
        REQUIRE(s.isEmpty());
    }

    SECTION("Reverse order operations") {
        Stack<std::string> s;
        s.push("first");
        s.push("last");

        REQUIRE(s.top() == "last");
        s.pop();
        REQUIRE(s.top() == "first");
    }
}