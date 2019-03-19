//
//  main.cpp
//  linear.cpp
//
//  Created by Nathan Tjoar on 2/6/19.
//  Copyright © 2019 Nathan Tjoar. All rights reserved.
//

#include <iostream>
#include <cmath>
using namespace std;

bool somePredicate(double x) //Returns true if cos(x) > sin(x)
{
    return cos(x) > sin(x); //My predicate, check with a calculator
}

// Return true if the somePredicate function returns true for at
// least one of the array elements, false otherwise.
bool anyTrue(const double a[], int n)
{
    if(n > 0) //0-1 = -1, dummy, don't do it.
    {
        if(somePredicate(a[n - 1])) //Look at n-1 bc at n, array is undefined
            return true;
        else
            anyTrue(a, n - 1); //Check again
    }
    return false;
}

int factorial(int n)  //Test
{
    if(n > 1)
        return n * factorial(n - 1);
    else
        return 1;
}

// Return the number of elements in the array for which the
// somePredicate function returns true.
int countTrue(const double a[], int n) //Do similar to the last one, but add rather than returning
{
    if(n > 0) //0-1 = -1, dummy, don't do it.
    {
        if(somePredicate(a[n - 1]))
            return (1 + countTrue(a, n - 1)); //Count
        return countTrue(a, n - 1); //Check again
    }
    return 0; //No elements
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns true.  If there is no such
// element, return -1.
int firstTrue(const double a[], int n) //Same process, just keep adding for values not it.
{
    if(somePredicate(a[0])) //True found at first
        return 0;
    if(n == 0) //No trues can be found
        return -1;
    if(somePredicate(a[n - 1])) //Check n - 1 place
        return n - 1;
    return firstTrue(a, n - 1);
}

// Return the subscript of the smallest element in the array (i.e.,
// return the smallest subscript m such that a[m] <= a[k] for all
// k from 0 to n-1).  If the function is told to examine no
// elements, return -1.
int positionOfMin(const double a[], int n) //Similar to firstTrue
{
    if (n <= 0)
        return -1;
    if (a[0] <= a[1 + positionOfMin(a + 1, n - 1)]) //Check all until zero
        return 0;
    else
        return 1 + positionOfMin(a + 1, n - 1); //Return +1 because index will be at -1
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if(n2 > n1)
        return false;
    else if((n1 == 0 && n2 == 0) || (n2 == 0))
        return true;
    if(a1[0] == a2[0])
        return includes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
    else
        return includes(a1 + 1, n1 - 1, a2, n2);
}

int main() {
    const int n1 = 7; //Size of array Look into this for automation
    double a[n1] = {10, 50, 40, 20, 50, 40, 30}; //Adjust this array for testing purpose
    
///////////////////////////////////////
    
    if(anyTrue(a, n1)) //anyTrue tester - Good so far
            cout << "anyTrue returns the value: TRUE" << endl;
    else
        cout << "FALSE" << endl;
    
///////////////////////////////////////
    
    cout << "countTrue returns the value: " << countTrue(a, n1) << endl; //countTrue Tester - Done
    
///////////////////////////////////////
    
    cout << "firstTrue returns the value: " << firstTrue(a, n1) << endl; //firstTrue Tester - Done

///////////////////////////////////////
    
    cout << "positionOfMin returns the value: " << positionOfMin(a, n1) << endl; //positionOfMin Tester - Done
    
///////////////////////////////////////

    const int n2 = 3;
    double b[n2] = {50, 30, 20};
    if(includes(a, n1, b, n2))
        cout << "includes returns the value: TRUE" << endl;
    else
        cout << "includes returns the value: FALSE" << endl;
}
