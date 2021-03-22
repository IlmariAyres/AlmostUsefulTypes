#include <catch2/catch.hpp>

#include "types/directionaliterator.h"

using namespace Types;

int test_array[] = { 1, 2, 3, 4, 5 };

int* test_begin = test_array;
int* test_middle = test_array + 2;
int* test_end = test_array + 4;

TEST_CASE("Iterator dereferencing")
{
    DirectionalIterator<int> forward(test_begin, true);
    DirectionalIterator<int> backward(test_end, false);

    REQUIRE(*forward == 1);
    REQUIRE(*backward == 5);
}

TEST_CASE("Iterator increment")
{
    DirectionalIterator<int> forward(test_begin, true);
    DirectionalIterator<int> backward(test_end, false);

    forward++;
    REQUIRE(*forward == 2);

    ++forward;
    REQUIRE(*forward == 3);

    backward++;
    REQUIRE(*backward == 4);

    ++backward;
    REQUIRE(*backward == 3);

    REQUIRE(*forward++ == 3);
    REQUIRE(*forward == 4);
    REQUIRE(*++forward == 5);

    REQUIRE(*backward++ == 3);
    REQUIRE(*backward == 2);
    REQUIRE(*++backward == 1);
}

TEST_CASE("Iterator decrement")
{
    DirectionalIterator<int> forward(test_end, true);
    DirectionalIterator<int> backward(test_begin, false);

    forward--;
    REQUIRE(*forward == 4);

    --forward;
    REQUIRE(*forward == 3);

    backward--;
    REQUIRE(*backward == 2);

    --backward;
    REQUIRE(*backward == 3);

    REQUIRE(*forward-- == 3);
    REQUIRE(*forward == 2);
    REQUIRE(*--forward == 1);

    REQUIRE(*backward-- == 3);
    REQUIRE(*backward == 4);
    REQUIRE(*--backward == 5);
}

TEST_CASE("Iterator asign and compare equality")
{
    DirectionalIterator<int> forward1(test_begin, true), forward2(test_middle, true);
    DirectionalIterator<int> backward1(test_end, false), backward2(test_middle, false);

    REQUIRE(forward1 != forward2);
    REQUIRE(forward2 == backward2);

    forward1 = forward2;
    REQUIRE(*forward1 == 3);
    REQUIRE(forward1 == forward2);
}

TEST_CASE("Iterator member access")
{
    std::pair<int, int> testpair[] = { {1, 2}, {3, 4} };
    DirectionalIterator<std::pair<int, int>> pair_iterator(testpair);

    REQUIRE(pair_iterator[0].first == 1);
    REQUIRE(pair_iterator[1].first == 3);

    REQUIRE(pair_iterator->first == 1);
    REQUIRE((++pair_iterator)->first == 3);
}

TEST_CASE("Iterator writable member access")
{
    int mod_array[] = { 1, 2, 3, 4, 5 };
    DirectionalIterator<int> mod_iterator(mod_array, true);

    REQUIRE(*mod_iterator == 1);

    *mod_iterator = 6;
    REQUIRE(*mod_iterator == 6);
    REQUIRE(mod_iterator[0] == 6);

    REQUIRE(mod_iterator[1] == 2);

    mod_iterator[1] = 7;
    REQUIRE(mod_iterator[1] == 7);
    REQUIRE(*(++mod_iterator) == 7);

}

TEST_CASE("Iterator multipass")
{
    DirectionalIterator<int> forward1(test_begin, true);
    DirectionalIterator<int> forward2 = forward1;

    REQUIRE(*forward1 == *forward2);
    REQUIRE(*++forward1 == *++forward2);
}

TEST_CASE("Iterator arithmetics and assignment")
{
    SECTION("Addition")
    {
        DirectionalIterator<int> forward(test_begin, true);
        DirectionalIterator<int> backward(test_end, false);

        REQUIRE(*(forward + 1) == 2);
        REQUIRE(*(backward + 1) == 4);

        forward += 3;
        backward += 3;

        REQUIRE(*forward  == 4);
        REQUIRE(*backward == 2);
    }

    SECTION("Subtraction")
    {
        DirectionalIterator<int> forward(test_end, true);
        DirectionalIterator<int> backward(test_begin, false);

        REQUIRE(*(forward - 1) == 4);
        REQUIRE(*(backward - 1) == 2);

        forward -= 3;
        backward -= 3;

        REQUIRE(*forward  == 2);
        REQUIRE(*backward == 4);
    }

    SECTION("Pointer difference")
    {
        DirectionalIterator<int> forward_begin(test_begin, true);
        DirectionalIterator<int> forward_end(test_end, true);

        DirectionalIterator<int> backward_begin(test_end, false);
        DirectionalIterator<int> backward_end(test_begin, false);

        REQUIRE(forward_end - forward_begin == 4);
        REQUIRE(forward_begin - forward_end == -4);

        REQUIRE(backward_end - backward_begin == 4);
        REQUIRE(backward_begin - backward_end == -4);
    }
}

TEST_CASE("Iterator comparisom")
{
    DirectionalIterator<int> forward1(test_middle - 1, true), forward2(test_middle, true), forward3(test_middle + 1, true);
    DirectionalIterator<int> backward1(test_middle - 1, false), backward2(test_middle, false), backward3(test_middle + 1, false);

    SECTION("Forward")
    {
        REQUIRE(forward1 < forward2);
        REQUIRE(!(forward3 < forward2));

        REQUIRE(forward3 > forward2);
        REQUIRE(!(forward1 > forward2));

        REQUIRE(forward1 <= forward2);
        REQUIRE(forward2 <= forward2);
        REQUIRE(!(forward3 <= forward2));

        REQUIRE(forward3 >= forward2);
        REQUIRE(forward2 >= forward2);
        REQUIRE(!(forward1 >= forward2));
    }

    SECTION("Backward")
    {
        REQUIRE(backward1 > backward2);
        REQUIRE(!(backward3 > backward2));

        REQUIRE(backward3 < backward2);
        REQUIRE(!(backward1 < backward2));

        REQUIRE(backward1 >= backward2);
        REQUIRE(backward2 >= backward2);
        REQUIRE(!(backward3 >= backward2));

        REQUIRE(backward3 <= backward2);
        REQUIRE(backward2 <= backward2);
        REQUIRE(!(backward1 <= backward2));
    }
}
