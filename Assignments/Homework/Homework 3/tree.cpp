//
//  main.cpp
//  tree.cpp
//
//  Created by Nathan Tjoar on 2/7/19.
//  Copyright © 2019 Nathan Tjoar. All rights reserved.
//

#include <iostream>
using namespace std;

// Return the number of ways that all n2 elements of a2 appear
// in the n1 element array a1 (IMPORTANT!) in the same order (though not
// necessarily consecutively).  The empty sequence appears in a
// sequence of length n1 in 1 way, even if n1 is 0.
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then for this value of a2     the function must return
//    10 20 40            1
//    10 40 30            2
//    20 10 40            0
//    50 40 30            3
int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
    if(n2 <= 0) // Any null set is a subset of all sets in at least one way
        return 1;
    if(n1 <= 1)
    {
        if(n2 == n1 && *a1 == *a2) //Are the sizes the same?
            return 1;
        else
            return 0;
    }
    if(*a1 == *a2) // a1 and a2 can be expressed as pointers, good practice to try here
        return countIncludes(a1 + 1, n1 - 1, a2, n2) +
        countIncludes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
    return countIncludes(a1 + 1, n1 - 1, a2, n2);  // Recursive search until match
}

// Exchange two doubles
void exchange(double& x, double& y) // This is fine
{
    double t = x;
    x = y;
    y = t;
}

// Rearrange the elements of the array so that all the elements
// whose value is > divider come before all the other elements,
// and all the elements whose value is < divider come after all
// the other elements.  Upon return, firstNotGreater is set to the
// index of the first element in the rearranged array that is
// <= divider, or n if there is no such element, and firstLess is
// set to the index of the first element that is < divider, or n
// if there is no such element.
// In other words, upon return from the function, the array is a
// permutation of its original value such that
//   * for 0 <= i < firstNotGreater, a[i] > divider
//   * for firstNotGreater <= i < firstLess, a[i] == divider
//   * for firstLess <= i < n, a[i] < divider
// All the elements > divider end up in no particular order.
// All the elements < divider end up in no particular order.
void divide(double a[], int n, double divider,
            int& firstNotGreater, int& firstLess) //Done for us. Thank you, Smallberg
{
    if (n < 0)
        n = 0;
    
    // It will always be the case that just before evaluating the loop
    // condition:
    //  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
    //  Every element earlier than position firstNotGreater is > divider
    //  Every element from position firstNotGreater to firstUnknown-1 is
    //    == divider
    //  Every element from firstUnknown to firstLess-1 is not known yet
    //  Every element at position firstLess or later is < divider
    
    firstNotGreater = 0;
    firstLess = n;
    int firstUnknown = 0;
    while (firstUnknown < firstLess)
    {
        if (a[firstUnknown] < divider)
        {
            firstLess--;
            exchange(a[firstUnknown], a[firstLess]);
        }
        else
        {
            if (a[firstUnknown] > divider)
            {
                exchange(a[firstNotGreater], a[firstUnknown]);
                firstNotGreater++;
            }
            firstUnknown++;
        }
    }
}

// Rearrange the elements of the array so that
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.
void order(double a[], int n) // TODO: Implement some sort
{
    if(n <= 1) // One value or less, no need to do anything
        return;
    int i, j; // i is for firstNotGreater, lazy typing
    divide(a, n, *a, i, j);
    order(a, i);
    order(a + j, n - j);
    return;
}

int main() {
    /*Declare test values*/
    const int n1 = 7;
    const int n2 = 2;
    double a[n1] = {10, 10, 10, 10, 10, 10, 10};
    double b[n2] = {10, 10};
    
/////////////////////////////////////////////////
    
    cout << "countIncludes returns the value: " <<
    countIncludes(a, n1, b, n2) << endl; // countIncludes tester
    
/////////////////////////////////////////////////
    
    order(a, n1); // order tester
    cout << "order gives you the sorted array: {";
    for(int i = 0; i < n1; i++)
    {
        cout << *(a + i);
        if(i != n1 - 1)
            cout << ", ";
    }
    cout << "}" << endl;
}
