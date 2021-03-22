#include <catch2/catch.hpp>

#include "types/stack.h"

using namespace Types;

TEST_CASE("Stack list constructor and basic member access")
{
    Stack<int> stack = {1, 2, 3, 4, 5};
    Stack<int> stackR({1, 2, 3, 4, 5}, false);

    for(int i = 1; i <= 5; i++)
    {
        REQUIRE(stack[i - 1] == 6 - i);
        REQUIRE(stackR[i - 1] == i);
    }

    REQUIRE(stack.size() == 5);
    REQUIRE(stackR.size() == 5);
}

TEST_CASE("Stack assignment")
{
    SECTION("Default")
    {
        Stack<int> stack;
        REQUIRE(stack.size() == 0);
    }

    SECTION("Copy")
    {
        Stack<int> stack1 = {1, 2, 3, 4, 5};
        Stack<int> stack2 = stack1;
        Stack<int> stack3;

        REQUIRE(stack1 == stack2);

        REQUIRE(stack1 != stack3);
        stack1 = stack3;
        REQUIRE(stack1 == stack3);
    }

    SECTION("Move")
    {
        Stack<int> stack1a = {1, 2, 3, 4, 5};
        Stack<int> stack1b = stack1a;
        Stack<int> stack2;

        stack2 = std::move(stack1a);
        REQUIRE(stack1a.size() == 0);
        REQUIRE(stack2 == stack1b);
    }
}

TEST_CASE("Stack iteration")
{
    Stack<int> stackF({2, 4, 6, 8, 10}, true);
    Stack<int> stackR({2, 4, 6, 8, 10}, false);

    SECTION("begin()")
    {
        REQUIRE(*stackF.begin() == 10);
        REQUIRE(*(stackF.begin() + 1) == 8);

        REQUIRE(*stackR.begin() == 2);
        REQUIRE(*(stackR.begin() + 1) == 4);
    }

    SECTION("rbegin()")
    {
        REQUIRE(*stackF.rbegin() == 2);
        REQUIRE(*(stackF.rbegin() + 1) == 4);

        REQUIRE(*stackR.rbegin() == 10);
        REQUIRE(*(stackR.rbegin() + 1) == 8);
    }

    SECTION("end()")
    {
        REQUIRE(*(stackF.end() - 1) == 2);
        REQUIRE(*(stackF.end() - 2) == 4);

        REQUIRE(*(stackR.end() - 1) == 10);
        REQUIRE(*(stackR.end() - 2) == 8);
    }

    SECTION("rend()")
    {
        REQUIRE(*(stackF.rend() - 1) == 10);
        REQUIRE(*(stackF.rend() - 2) == 8);

        REQUIRE(*(stackR.rend() - 1) == 2);
        REQUIRE(*(stackR.rend() - 2) == 4);
    }
}

TEST_CASE("Stack equality")
{
    Stack<int> stack1({1, 2, 3, 4, 5}, true);
    Stack<int> stack2({5, 4, 3, 2, 1}, true);
    Stack<int> stack3({5, 4, 3, 2, 1}, false);
    Stack<int> stack4({1, 2, 3, 4}, true);
    Stack<int> stack5({1, 2, 3, 4, 4}, true);

    REQUIRE(stack1 == stack1);
    REQUIRE(stack1 != stack2);

    REQUIRE(stack1 == stack3);
    REQUIRE(stack2 != stack3);

    REQUIRE(stack1 != stack4);
    REQUIRE(stack1 != stack5);

}

TEST_CASE("Stack push and pop")
{
    SECTION("Forward")
    {
        Stack<int> stack({1, 2, 3, 4, 5}, true);
        Stack<int> stack1({1, 2, 3, 4, 5}, true);
        Stack<int> stack2({1, 2, 3, 4, 5, 6}, true);
        Stack<int> stack3({7, 1, 2, 3, 4, 5, 6}, true);

        stack.push_top(6);
        REQUIRE(stack == stack2);

        stack.push_bottom(7);
        REQUIRE(stack == stack3);

        stack.pop_bottom();
        REQUIRE(stack == stack2);

        stack.pop_top();
        REQUIRE(stack == stack1);
    }

    SECTION("Backward")
    {
        Stack<int> stack({1, 2, 3, 4, 5}, false);
        Stack<int> stack1({1, 2, 3, 4, 5}, false);
        Stack<int> stack2({6, 1, 2, 3, 4, 5}, false);
        Stack<int> stack3({6, 1, 2, 3, 4, 5, 7}, false);

        stack.push_top(6);
        REQUIRE(stack == stack2);

        stack.push_bottom(7);
        REQUIRE(stack == stack3);

        stack.pop_bottom();
        REQUIRE(stack == stack2);

        stack.pop_top();
        REQUIRE(stack == stack1);
    }
}

TEST_CASE("Stack set direction and reverse")
{
    Stack<int> stack1({1, 2, 3, 4, 5}, true);
    Stack<int> stack2({1, 2, 3, 4, 5}, false);

    REQUIRE(stack1.getDirection() == true);
    REQUIRE(stack2.getDirection() == false);

    REQUIRE(stack1 != stack2);
    stack1.reverse();
    REQUIRE(stack1 == stack2);
}

TEST_CASE("Stack arithmetics")
{
    SECTION("Forward")
    {
        Stack<int> stack1({1, 2, 3}, true);
        Stack<int> stack2({4, 5, 6}, true);
        Stack<int> stackC({1, 2, 3, 4, 5, 6}, true);

        Stack<int> stack3 = stack1 + stack2;
        REQUIRE(stack3 == stackC);

        stack1 += stack2;
        REQUIRE(stack1 == stackC);
    }

}

TEST_CASE("Stack clear")
{
    Stack<int> stack({1, 2, 3}, true);

    stack.clear();
    REQUIRE(stack.size() == 0);

    stack.push_top(37);
    REQUIRE(stack[0] == 37);
}

TEST_CASE("Stack pull")
{
    Stack<int> stack1({1, 2, 3}, true);
    Stack<int> stack2({1, 2, 3}, true);

    REQUIRE(stack1.pull_top() == 3);
    REQUIRE(stack1.pull_top() == 2);
    REQUIRE(stack1.pull_top() == 1);
    REQUIRE(stack1.pull_top() == 0);
    REQUIRE(stack1.pull_top() == 0);

    REQUIRE(stack2.pull_bottom() == 1);
    REQUIRE(stack2.pull_bottom() == 2);
    REQUIRE(stack2.pull_bottom() == 3);
    REQUIRE(stack2.pull_bottom() == 0);
    REQUIRE(stack2.pull_bottom() == 0);
}
