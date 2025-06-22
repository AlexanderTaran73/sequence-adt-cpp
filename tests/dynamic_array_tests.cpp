#include "catch.hpp"
#include "dynamic_array.hpp"

TEST_CASE("DynamicArray Constructors", "[DynamicArray]") {
    SECTION("Default constructor") {
        DynamicArray<int> arr(5);
        REQUIRE(arr.getSize() == 5);
    }

    SECTION("Array constructor") {
        int init[] = {1, 2, 3, 4, 5};
        DynamicArray<int> arr(init, 5);
        
        REQUIRE(arr.getSize() == 5);
        for (int i = 0; i < 5; ++i) {
            REQUIRE(arr[i] == i+1);
        }
    }

    SECTION("Copy constructor") {
        int init[] = {10, 20, 30};
        DynamicArray<int> arr1(init, 3);
        DynamicArray<int> arr2(arr1);
        
        REQUIRE(arr2.getSize() == 3);
        REQUIRE(arr2[0] == 10);
        REQUIRE(arr2[1] == 20);
        REQUIRE(arr2[2] == 30);
    }

    SECTION("Move constructor") {
        DynamicArray<int> arr1(10);
        DynamicArray<int> arr2(std::move(arr1));
        
        REQUIRE(arr2.getSize() == 10);
        REQUIRE(arr1.getSize() == 0);
    }
}

TEST_CASE("DynamicArray Operations", "[DynamicArray]") {
    DynamicArray<int> arr(3);
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;

    SECTION("Element access") {
        REQUIRE(arr.get(0) == 1);
        REQUIRE(arr[1] == 2);
        REQUIRE_THROWS_WITH(arr.get(3), Catch::Matchers::Contains("Index out of range"));
    }

    SECTION("Size") {
        REQUIRE(arr.getSize() == 3);
    }

    SECTION("Resize operations") {
        arr.resize(5);
        REQUIRE(arr.getSize() == 5);
        
        arr.resize(2);
        REQUIRE(arr.getSize() == 5);
    }

    SECTION("Modification methods") {
        arr.set(1, 99);
        REQUIRE(arr[1] == 99);
        
        arr.remove(1);
        REQUIRE(arr.getSize() == 2);
        REQUIRE(arr[1] == 3);
        
        arr.clear();
        REQUIRE(arr.getSize() == 0);
    }

    SECTION("Subarray creation") {
        auto sub = arr.getSubArray(1, 2);
        REQUIRE(sub->getSize() == 2);
        REQUIRE((*sub)[0] == 2);
        REQUIRE((*sub)[1] == 3);
        delete sub;
    }
}

TEST_CASE("DynamicArray Edge Cases", "[DynamicArray]") {
    SECTION("Empty array") {
        DynamicArray<std::string> arr(0);
        
        REQUIRE(arr.getSize() == 0);
        REQUIRE_THROWS_WITH(arr.get(0), Catch::Matchers::Contains("Index out of range"));
        REQUIRE_THROWS_WITH(arr.remove(0), Catch::Matchers::Contains("Empty array"));
    }

    SECTION("Negative size") {
        REQUIRE_THROWS_WITH(DynamicArray<int>(-1), Catch::Matchers::Contains("Negative size not allowed"));
    }

    SECTION("Invalid indices") {
        DynamicArray<float> arr(5);
        REQUIRE_THROWS_WITH(arr.get(-1), Catch::Matchers::Contains("Index out of range"));
        REQUIRE_THROWS_WITH(arr.set(10, 3.14f), Catch::Matchers::Contains("Index out of range"));
        REQUIRE_THROWS_WITH(arr.getSubArray(3, 2), Catch::Matchers::Contains("Invalid indices"));
    }
}

TEST_CASE("DynamicArray Operators", "[DynamicArray]") {
    int init1[] = {1, 2, 3};
    int init2[] = {1, 2, 3};
    int init3[] = {4, 5, 6};
    
    DynamicArray<int> arr1(init1, 3);
    DynamicArray<int> arr2(init2, 3);
    DynamicArray<int> arr3(init3, 3);

    SECTION("Assignment operator") {
        DynamicArray<int> arr4 = arr1;
        REQUIRE(arr4.getSize() == 3);
        REQUIRE(arr4[0] == 1);
    }

    SECTION("Move assignment") {
        DynamicArray<int> arr4 = std::move(arr1);
        REQUIRE(arr4.getSize() == 3);
        REQUIRE(arr1.getSize() == 0);
    }

    SECTION("Equality operator") {
        REQUIRE(arr1 == arr2);
        REQUIRE_FALSE(arr1 == arr3);
    }

    SECTION("Index operator") {
        REQUIRE(arr1[0] == 1);
        arr1[0] = 10;
        REQUIRE(arr1[0] == 10);
        REQUIRE_THROWS_WITH(arr1[-1], Catch::Matchers::Contains("Index out of range"));
    }
}

TEST_CASE("DynamicArray size Management", "[DynamicArray]") {
    DynamicArray<int> arr(0);
    
    SECTION("Initial size") {
        REQUIRE(arr.getSize() == 0);
    }

    SECTION("Ensure size") {
        arr.resize(10);
        REQUIRE(arr.getSize() == 10);
        
        arr.resize(5);
        REQUIRE(arr.getSize() == 10);
    }

    SECTION("Automatic expansion") {
        DynamicArray<int> small(1);
        small[0] = 42;
        small.resize(10);
        REQUIRE(small[0] == 42);
    }
}