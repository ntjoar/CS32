//
//  testSet.cpp
//  CS32_HW1
//
//  Created by Nathan Tjoar on 1/17/19.
//  Copyright © 2019 Nathan Tjoar. All rights reserved.
//

//#include "Set.h" //Run with type alias string
//#include <iostream>
//#include <string>
//#include <cassert>
//using namespace std;
//
//int main()
//{
//    Set s;
//    assert(s.empty());
//    ItemType x = "arepa";
//    assert( !s.get(42, x)  &&  x == "arepa"); // x unchanged by get failure
//    s.insert("chapati");
//    assert(s.size() == 1);
//    assert(s.get(0, x)  &&  x == "chapati");
//    cout << "Passed all tests" << endl;
//}
//
//#include "Set.h" //Run with type alias unsigned long
//#include <iostream>
//#include <cassert>
//using namespace std;
//
//int main()
//{
//    Set s;
//    assert(s.empty());
//    ItemType x = 9876543;
//    assert( !s.get(42, x)  &&  x == 9876543); // x unchanged by get failure
//    s.insert(123456789);
//    assert(s.size() == 1);
//    assert(s.get(0, x)  &&  x == 123456789);
//    cout << "Passed all tests" << endl;
//}
//#include "Set.h" //Run with type alias string
//#include <string>
//#include <iostream>
//#include <cassert>
//using namespace std;
//
//void test()
//{
//    Set ss;
//    assert(ss.insert("roti"));
//    assert(ss.insert("pita"));
//    assert(ss.size() == 2);
//    assert(ss.contains("pita"));
//    ItemType x = "laobing";
//    assert(ss.get(0, x)  &&  x == "pita");
//    assert(ss.get(1, x)  &&  x == "roti");
//}
//
//int main()
//{
//    test();
//    cout << "Passed all tests" << endl;
//}
//#include "Set.h" //Run with unsigned long
//#include <iostream>
//#include <cassert>
//using namespace std;
//
//void test()
//{
//    Set uls;
//    assert(uls.insert(20));
//    assert(uls.insert(10));
//    assert(uls.size() == 2);
//    assert(uls.contains(10));
//    ItemType x = 30;
//    assert(uls.get(0, x)  &&  x == 10);
//    assert(uls.get(1, x)  &&  x == 20);
//}
//
//int main()
//{
//    test();
//    cout << "Passed all tests" << endl;
//}
