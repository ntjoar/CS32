#include "Set.h" //String
#include <iostream>
#include <cassert>
#include <type_traits>
using namespace std;

#define CHECKTYPE(f, t) { auto p = static_cast<t>(f); (void)p; }

static_assert(std::is_default_constructible<Set>::value,
              "Set must be default-constructible.");
static_assert(std::is_copy_constructible<Set>::value,
              "Set must be copy-constructible.");

void thisFunctionWillNeverBeCalled()
{
    CHECKTYPE(&Set::operator=, Set& (Set::*)(const Set&));
    CHECKTYPE(&Set::empty,     bool (Set::*)() const);
    CHECKTYPE(&Set::size,      int  (Set::*)() const);
    CHECKTYPE(&Set::insert,    bool (Set::*)(const ItemType&));
    CHECKTYPE(&Set::erase,     bool (Set::*)(const ItemType&));
    CHECKTYPE(&Set::contains,  bool (Set::*)(const ItemType&) const);
    CHECKTYPE(&Set::get,       bool (Set::*)(int, ItemType&) const);
    CHECKTYPE(&Set::swap,      void (Set::*)(Set&));
    CHECKTYPE(unite,    void (*)(const Set&, const Set&, Set&));
    CHECKTYPE(subtract, void (*)(const Set&, const Set&, Set&));
}

void test()
{
    Set ss;
    assert(ss.insert("roti"));
    assert(ss.insert("pita"));
    assert(ss.size() == 2);
    assert(ss.contains("pita"));
    ItemType x = "laobing";
    assert(ss.get(0, x)  &&  x == "pita");
    assert(ss.get(1, x)  &&  x == "roti");
}

void test2()
{
    Set s;
    assert(s.insert("roti"));
    assert(s.insert("pita"));
    assert(!s.erase("K"));
    assert(s.erase("roti"));
    assert(!s.contains("K"));
    assert(s.contains("pita"));
    ItemType n = "HI";
    assert(!s.get(1, n) && n == "HI");
}

void extra()
{
    Set s;
    assert(s.insert("Cabbage"));
    assert(!s.insert("Cabbage"));
    assert(s.insert("Tomato"));
    assert(s.insert("Lettuce"));
    assert(s.insert("Beef"));
    assert(s.insert("Cake"));
    assert(s.insert("Guava"));
    Set s2;
    assert(s2.empty());
    assert(s2.insert("Cabbage"));
    assert(!s2.insert("Cabbage"));
    assert(s2.size() == 1);
    assert(s2.insert("Chicken"));
    assert(s2.insert("Orange"));
    assert(s2.insert("Apple"));
    assert(s2.insert("Cake"));
    assert(s2.insert("Guava"));
    assert(s2.size() == 6);
    Set add;
    Set sub;
    unite(s, s2, add);
    assert(add.contains("Cabbage"));
    assert(add.contains("Tomato"));
    assert(add.contains("Chicken"));
    assert(add.contains("Orange"));
    assert(add.contains("Apple"));
    assert(add.contains("Cake"));
    subtract(s, s2, sub);
    assert(!sub.contains("Cabbage"));
    assert(sub.contains("Tomato"));
    assert(!sub.contains("Chicken"));
    assert(!sub.contains("Orange"));
    assert(!sub.contains("Apple"));
    assert(!sub.contains("Cake"));
}

void sw()
{
    Set s;
    assert(s.insert("100"));
    assert(!s.insert("100"));
    assert(s.insert("5081232"));
    assert(s.insert("911"));
    assert(s.insert("314159265"));
    assert(s.insert("27189234"));
    assert(s.insert("88"));
    Set s2;
    assert(s2.insert("2000"));
    assert(!s2.insert("2000"));
    assert(s2.insert("42069"));
    assert(s2.insert("911"));
    assert(s2.insert("5081232"));
    assert(s2.insert("100"));
    assert(s2.insert("42"));
    s.swap(s2);
    assert(s.contains("5081232"));
    assert(s.contains("2000"));
    assert(s.contains("42069"));
    assert(s2.contains("911"));
    assert(s2.contains("88"));
    assert(s2.contains("100"));
}

int main()
{
    test();
    test2();
    extra();
    sw();
    Set ss;  // ItemType is std::string
    ss.insert("ccc");
    ss.insert("aaa");
    ss.insert("bbb");
    ItemType x = "xxx";
    assert(!ss.get(3, x)  &&  x == "xxx");  // x is unchanged
    assert(ss.get(1, x)  &&  x == "bbb");   // "bbb" is greater than
    // exactly 1 item
    cout << "Passed all tests" << endl;
}
///////////////////////////////////////
//#include "Set.h" //Unsigned longs
//#include <iostream>
//#include <cassert>
//using namespace std;
//
//void test()
//{
//    Set uls;
//    assert(uls.insert(10));
//    assert(uls.insert(20));
//    assert(uls.size() == 2);
//    assert(uls.contains(20));
//    ItemType x = 30;
//    assert(uls.get(0, x)  &&  x == 10);
//    assert(uls.get(1, x)  &&  x == 20);
//}
//
//void test2()
//{
//    Set s;
//    assert(s.insert(200));
//    assert(s.insert(1000));
//    assert(!s.erase(20));
//    assert(s.erase(200));
//    assert(!s.contains(999));
//    assert(s.contains(1000));
//    ItemType n = 10;
//    assert(!s.get(1, n) && n == 10);
//}
//
//void extra()
//{
//    Set s;
//    assert(s.insert(100));
//    assert(!s.insert(100));
//    assert(s.insert(5081232));
//    assert(s.insert(911));
//    assert(s.insert(314159265));
//    assert(s.insert(27189234));
//    assert(s.insert(88));
//    Set s2;
//    assert(s2.insert(2000));
//    assert(!s2.insert(2000));
//    assert(s2.insert(42069));
//    assert(s2.insert(911));
//    assert(s2.insert(5081232));
//    assert(s2.insert(100));
//    assert(s2.insert(42));
//    Set add;
//    Set sub;
//    unite(s, s2, add);
//    assert(add.contains(5081232));
//    assert(add.contains(100));
//    assert(add.contains(88));
//    assert(add.contains(911));
//    assert(add.contains(42069));
//    assert(add.contains(42));
//    subtract(s, s2, sub);
//    assert(!sub.contains(5081232));
//    assert(sub.contains(88));
//    assert(!sub.contains(100));
//    assert(!sub.contains(911));
//    assert(!sub.contains(2000));
//}
//
//void sw()
//{
//    Set s;
//    assert(s.insert(100));
//    assert(!s.insert(100));
//    assert(s.insert(5081232));
//    assert(s.insert(911));
//    assert(s.insert(314159265));
//    assert(s.insert(27189234));
//    assert(s.insert(88));
//    Set s2;
//    assert(s2.insert(2000));
//    assert(!s2.insert(2000));
//    assert(s2.insert(42069));
//    assert(s2.insert(911));
//    assert(s2.insert(5081232));
//    assert(s2.insert(100));
//    assert(s2.insert(42));
//    s.swap(s2);
//    assert(s.contains(5081232));
//    assert(s.contains(2000));
//    assert(s.contains(42069));
//    assert(s2.contains(911));
//    assert(s2.contains(88));
//    assert(s2.contains(100));
//}
//
//int main()
//{
//    test();
//    test2();
//    extra();
//    sw();
//    Set s1; //UNITE AND SUBTRACT FUNCTIONS ONLY
//    s1.insert(2);
//    s1.insert(8);
//    s1.insert(3);
//    s1.insert(9);
//    s1.insert(5);
//    s1.dump();
//    std::cout << std::endl;
//
//    Set s2;
//    s2.insert(6);
//    s2.insert(3);
//    s2.insert(8);
//    s2.insert(5);
//    s2.insert(10);
//    s2.dump();
//    std::cout << std::endl;
//
//    Set result;
//    subtract(s1, s2, result);
//    result.dump();
//    cout << "Passed all tests" << endl;
//}
