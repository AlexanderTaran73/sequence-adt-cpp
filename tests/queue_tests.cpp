#include "catch.hpp"
#include "queue.hpp"
#include <memory>
#include <string>

TEST_CASE("Queue Basic Operations", "[Queue]") {
    SECTION("Default constructor creates empty queue") {
        Queue<int> q;
        REQUIRE(q.size() == 0);
        REQUIRE(q.isEmpty());
        REQUIRE_THROWS(q.front());
        REQUIRE_THROWS(q.dequeue());
    }

    SECTION("Enqueue and dequeue operations") {
        Queue<std::string> q;
        q.enqueue("first");
        q.enqueue("second");

        REQUIRE(q.size() == 2);
        REQUIRE(q.front() == "first");
        
        std::string item = q.dequeue();
        REQUIRE(item == "first");
        REQUIRE(q.size() == 1);
        REQUIRE(q.front() == "second");
    }
}

TEST_CASE("Queue Element Access", "[Queue]") {
    Queue<double> q;
    q.enqueue(1.1);
    q.enqueue(2.2);
    q.enqueue(3.3);

    SECTION("Front access") {
        REQUIRE(q.front() == Approx(1.1));
        q.dequeue();
        REQUIRE(q.front() == Approx(2.2));
    }

    SECTION("Get by index") {
        REQUIRE(q.get(0) == Approx(1.1));
        REQUIRE(q.get(1) == Approx(2.2));
        REQUIRE_THROWS(q.get(-1));
        REQUIRE_THROWS(q.get(3));
    }
}

TEST_CASE("Queue Functional Operations", "[Queue]") {
    Queue<int> q;
    for (int i = 1; i <= 5; ++i) q.enqueue(i);

    SECTION("Map operation") {
        auto doubled = q.map([](int x) { return x * 2; });
        REQUIRE(doubled.size() == 5);
        REQUIRE(doubled.get(0) == 2);
        REQUIRE(doubled.get(4) == 10);
    }

    SECTION("Where operation") {
        auto evens = q.where([](int x) { return x % 2 == 0; });
        REQUIRE(evens.size() == 2);
        REQUIRE(evens.get(0) == 2);
        REQUIRE(evens.get(1) == 4);
    }

    SECTION("Reduce operation") {
        int sum = q.reduce([](int a, int b) { return a + b; }, 0);
        REQUIRE(sum == 15);
    }
}

TEST_CASE("Queue Combination Operations", "[Queue]") {
    SECTION("Concatenation") {
        Queue<int> q1;
        q1.enqueue(1);
        q1.enqueue(2);

        Queue<int> q2;
        q2.enqueue(3);

        auto combined = q1.concat(q2);
        REQUIRE(combined.size() == 3);
        REQUIRE(combined.get(2) == 3);
    }

    SECTION("Subsequence extraction") {
        Queue<char> q;
        for (char c = 'a'; c <= 'e'; ++c) q.enqueue(c);

        auto sub = q.getSubsequence(1, 3);
        REQUIRE(sub.size() == 3);
        REQUIRE(sub.get(0) == 'b');
        REQUIRE(sub.get(2) == 'd');
    }

    SECTION("Contains operations") {
        Queue<int> q;
        q.enqueue(10);
        q.enqueue(20);
        q.enqueue(30);
        q.enqueue(40);

        REQUIRE(q.contains(20));
        REQUIRE_FALSE(q.contains(50));

        Queue<int> sub;
        sub.enqueue(20);
        sub.enqueue(30);
        REQUIRE(q.containsSubsequence(sub));
    }
}

TEST_CASE("Queue Advanced Operations", "[Queue]") {
    Queue<std::string> q;
    q.enqueue("apple");
    q.enqueue("banana");
    q.enqueue("cherry");

    SECTION("Zip operation") {
        Queue<int> nums;
        nums.enqueue(1);
        nums.enqueue(2);

        auto zipped = q.zip(nums);
        REQUIRE(zipped.size() == 2);
        REQUIRE(zipped.get(0).first == "apple");
        REQUIRE(zipped.get(0).second == 1);
    }

    SECTION("Split operation") {
        auto [short_words, long_words] = q.split([](const std::string& s) { 
            return s.length() < 6; 
        });

        REQUIRE(short_words.size() == 1);
        REQUIRE(long_words.size() == 2);
        REQUIRE(short_words.contains("apple"));
        REQUIRE(long_words.contains("banana"));
    }

    SECTION("Equality comparison") {
        Queue<std::string> q2;
        q2.enqueue("apple");
        q2.enqueue("banana");

        REQUIRE(q != q2);
        q2.enqueue("cherry");
        REQUIRE(q == q2);
    }
}

TEST_CASE("Queue Edge Cases", "[Queue]") {
    SECTION("Empty queue operations") {
        Queue<float> empty;

        REQUIRE_THROWS(empty.dequeue());
        REQUIRE_THROWS(empty.front());
    }

    SECTION("Single element queue") {
        Queue<char> single;
        single.enqueue('X');

        REQUIRE(single.front() == 'X');
        REQUIRE(single.dequeue() == 'X');
        REQUIRE(single.isEmpty());
    }

    SECTION("Clear operation") {
        Queue<int> q;
        q.enqueue(1);
        q.enqueue(2);
        q.clear();

        REQUIRE(q.isEmpty());
        REQUIRE(q.size() == 0);
    }
}