#include <catch2/catch.hpp>

#include "types/tree.h"

#include <vector>

using namespace Types;

TEST_CASE("Tree memeber access")
{
    Tree<int, int> t(1);

    REQUIRE(*t == 1);

    t.addChild(2);
    t.setChild(3, 100);
    t.addChild(4);

    SECTION("Iterate")
    {
        auto it = t.begin();
        REQUIRE(**it == 2);
        it++;
        REQUIRE(**it == 3);

        it++; it++;
        REQUIRE(it.getPtr() == nullptr);

        it--; it--;
        REQUIRE(**it == 3);

        auto ite = t.end();
        REQUIRE(ite.getPtr() == nullptr);
        ite--;
        REQUIRE(**ite == 4);
    }

    SECTION("Reverse iterate")
    {
        auto it = t.rbegin();
        REQUIRE(**it == 4);
        it--;
        REQUIRE(**it == 3);

        auto ite = t.rend();
        ite++;
        REQUIRE(**ite == 2);
        ite++;
        REQUIRE(**ite == 3);
    }

    SECTION("Overiteration")
    {
        auto it = t.begin();
        it--; it--; it--; it++;
        REQUIRE(**it == 2);


        auto ite = t.end();
        ite++; ite++; ite++; ite--;
        REQUIRE(**ite == 4);
    }

    SECTION("Array style")
    {
        REQUIRE(*t[100] == 3);
        REQUIRE(t[123] == nullptr);
    }


    SECTION("Modify")
    {
        auto it = t.begin();
        it->setChild(5, 200);

        REQUIRE(*(*it)[200] == 5);
        REQUIRE(**it == 2);

        **it = 10;
        REQUIRE(*(*it)[200] == 5);
        REQUIRE(**it == 10);
    }

    SECTION("For each")
    {
        std::vector<int> v, v_comp = {2, 3, 4};
        for(auto a = t.begin(); a != t.end(); a++) v.push_back(**a);

        REQUIRE(v == v_comp);
    }
}

TEST_CASE("Tree deletion")
{
    Tree<int, int> t(1);
    t.addChild(2);
    t.addChild(3);
    t.addChild(4);

    auto it = t.begin();
    it++; delete it.getPtr();
    it = t.begin();

    REQUIRE(**it == 2); it++;
    REQUIRE(**it == 4); it++;
}

TEST_CASE("Tree comparisom")
{
    Tree<int, int> t1(1), t2(1), t3(2);

    REQUIRE(t1 == t2);
    REQUIRE(!(t1 != t2));
    REQUIRE(t1 != t3);
    REQUIRE(!(t1 == t3));

    t1.addChild(2);
    t1.addChild(3);

    t2.addChild(2);
    t2.addChild(3);

    REQUIRE(t1 == t2);
    REQUIRE(!(t1 != t2));
    REQUIRE(t1 != t3);
    REQUIRE(!(t1 == t3));

    t2.begin()->addChild(4);
    REQUIRE(t1 != t2);
    REQUIRE(!(t1 == t2));
}

TEST_CASE("Tree copy and move")
{
    Tree<int, int> t1(1), t2(1), t3(1);

    t1.addChild(2);
    t1.setChild(3, 100);

    t2 = t1;
    REQUIRE(t1 == t2);
    REQUIRE(t2[100]->operator*() == 3);

    REQUIRE(t3 != t1);
    t3 = std::move(t1);
    REQUIRE(t1 != t3);
    REQUIRE(t2 == t3);
    REQUIRE(t1.begin().getPtr() == nullptr);

}
