#include "catch.hpp"
#include "linked_list.hpp"

TEST_CASE("LinkedList Constructors", "[LinkedList]") {
    SECTION("Default constructor") {
        LinkedList<int> list;
        REQUIRE(list.getLength() == 0);
        REQUIRE_THROWS_WITH(list.getFirst(), Catch::Matchers::Contains("Empty list"));
    }

    SECTION("Array constructor") {
        int init[] = {1, 2, 3, 4, 5};
        LinkedList<int> list(init, 5);
        
        REQUIRE(list.getLength() == 5);
        for (int i = 0; i < 5; ++i) {
            REQUIRE(list[i] == i+1);
        }
    }

    SECTION("Copy constructor") {
        int init[] = {10, 20, 30};
        LinkedList<int> list1(init, 3);
        LinkedList<int> list2(list1);
        
        REQUIRE(list2.getLength() == 3);
        REQUIRE(list2[0] == 10);
        REQUIRE(list2[1] == 20);
        REQUIRE(list2[2] == 30);
    }

    SECTION("Move constructor") {
        LinkedList<int> list1;
        list1.append(1);
        list1.append(2);
        LinkedList<int> list2(std::move(list1));
        
        REQUIRE(list2.getLength() == 2);
        REQUIRE(list1.getLength() == 0);
    }
}

TEST_CASE("LinkedList Operations", "[LinkedList]") {
    LinkedList<int> list;
    list.append(1);
    list.append(2);
    list.append(3);

    SECTION("Element access") {
        REQUIRE(list.get(0) == 1);
        REQUIRE(list[1] == 2);
        REQUIRE_THROWS_WITH(list.get(3), Catch::Matchers::Contains("Index out of range"));
    }

    SECTION("Size and length") {
        REQUIRE(list.getLength() == 3);
    }

    SECTION("Modification methods") {
        SECTION("Append") {
            list.append(4);
            REQUIRE(list.getLength() == 4);
            REQUIRE(list.getLast() == 4);
        }

        SECTION("Prepend") {
            list.prepend(0);
            REQUIRE(list.getLength() == 4);
            REQUIRE(list.getFirst() == 0);
        }

        SECTION("InsertAt") {
            list.insertAt(99, 1);
            REQUIRE(list.getLength() == 4);
            REQUIRE(list[1] == 99);
            REQUIRE(list[2] == 2);
        }

        SECTION("Remove") {
            list.remove(1);
            REQUIRE(list.getLength() == 2);
            REQUIRE(list[0] == 1);
            REQUIRE(list[1] == 3);
        }

        SECTION("Clear") {
            list.clear();
            REQUIRE(list.getLength() == 0);
            REQUIRE_THROWS_WITH(list.getFirst(), Catch::Matchers::Contains("Empty list"));
        }
    }

    SECTION("SubList creation") {
        auto sub = list.getSubList(1, 2);
        REQUIRE(sub->getLength() == 2);
        REQUIRE((*sub)[0] == 2);
        REQUIRE((*sub)[1] == 3);
        delete sub;
    }

    SECTION("Concatenation") {
        LinkedList<int> other;
        other.append(4);
        other.append(5);
        
        auto concated = list.concat(&other);
        REQUIRE(concated->getLength() == 5);
        REQUIRE((*concated)[3] == 4);
        delete concated;
    }
}

TEST_CASE("LinkedList Edge Cases", "[LinkedList]") {
    SECTION("Empty list") {
        LinkedList<std::string> list;
        
        REQUIRE(list.getLength() == 0);
        REQUIRE_THROWS_WITH(list.get(0), Catch::Matchers::Contains("Index out of range"));
        REQUIRE_THROWS_WITH(list.remove(0), Catch::Matchers::Contains("Index out of range"));
    }

    SECTION("Negative size") {
        int arr[] = {1};
        REQUIRE_THROWS_WITH(LinkedList<int>(arr, -1), Catch::Matchers::Contains("Negative count"));
    }

    SECTION("Invalid indices") {
        LinkedList<float> list;
        list.append(1.1f);
        
        REQUIRE_THROWS_WITH(list.get(-1), Catch::Matchers::Contains("Index out of range"));
        REQUIRE_THROWS_WITH(list.insertAt(2.2f, 10), Catch::Matchers::Contains("Index out of range"));
        REQUIRE_THROWS_WITH(list.getSubList(0, 1), Catch::Matchers::Contains("Invalid indices"));
    }
}

TEST_CASE("LinkedList Operators", "[LinkedList]") {
    int init1[] = {1, 2, 3};
    int init2[] = {1, 2, 3};
    int init3[] = {4, 5, 6};
    
    LinkedList<int> list1(init1, 3);
    LinkedList<int> list2(init2, 3);
    LinkedList<int> list3(init3, 3);

    SECTION("Assignment operator") {
        LinkedList<int> list4 = list1;
        REQUIRE(list4.getLength() == 3);
        REQUIRE(list4[0] == 1);
    }

    SECTION("Move assignment") {
        LinkedList<int> list4 = std::move(list1);
        REQUIRE(list4.getLength() == 3);
        REQUIRE(list1.getLength() == 0);
    }

    SECTION("Equality operator") {
        REQUIRE(list1 == list2);
        REQUIRE_FALSE(list1 == list3);
    }

    SECTION("Inequality operator") {
        REQUIRE(list1 != list3);
        REQUIRE_FALSE(list1 != list2);
    }

    SECTION("Index operator") {
        REQUIRE(list1[0] == 1);
        list1[0] = 10;
        REQUIRE(list1[0] == 10);
        REQUIRE_THROWS_WITH(list1[-1], Catch::Matchers::Contains("Index out of range"));
    }
}

TEST_CASE("LinkedList Special Cases", "[LinkedList]") {
    SECTION("Single element list") {
        LinkedList<int> list;
        list.append(42);
        
        REQUIRE(list.getLength() == 1);
        REQUIRE(list.getFirst() == 42);
        REQUIRE(list.getLast() == 42);
        
        list.remove(0);
        REQUIRE(list.getLength() == 0);
    }

    SECTION("Concat with empty list") {
        LinkedList<int> list1;
        list1.append(1);
        LinkedList<int> list2;
        
        auto result = list1.concat(&list2);
        REQUIRE(result->getLength() == 1);
        REQUIRE((*result)[0] == 1);
        delete result;
    }

    SECTION("Insert at boundaries") {
        LinkedList<int> list;
        list.insertAt(1, 0);  // insert at start
        list.insertAt(2, 1);  // insert at end
        list.insertAt(3, 1);  // insert in middle
        
        REQUIRE(list.getLength() == 3);
        REQUIRE(list[0] == 1);
        REQUIRE(list[1] == 3);
        REQUIRE(list[2] == 2);
    }
}