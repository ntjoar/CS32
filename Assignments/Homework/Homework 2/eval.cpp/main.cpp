//
//  main.cpp
//  eval.cpp
//
//  Created by Nathan Tjoar on 1/31/19.
//  Copyright © 2019 Nathan Tjoar. All rights reserved.
//

#include "Set.h"
#include <iostream>
#include <string>
#include <cstring>
#include <stack>
#include <cassert>
using namespace std;

/*Function Declarations*/
int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result); //Evaluate
bool validity(string infix); //Check the validity of the string
void itp(string infix, string& postfix); //Convert infix to postfix
int prec(char a); //Precedence heirarchy

/*Evaluate function implementation*/
int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result)
{
    if(validity(infix)) //Check valid for infix
    {
        int j = -1;
        itp(infix, postfix);
        for(int i = 0; i < postfix.size(); i++)
        {
            if(isalpha(postfix[i]))
            {
                if(trueValues.contains(postfix[i]) &&
                   falseValues.contains(postfix[i])) //Default to 3, check for both
                    j = 3;
                if(!trueValues.contains(postfix[i]) &&
                   !falseValues.contains(postfix[i]) && j != 3) // Default at 3, check for neither
                    j = 2;
                if((trueValues.contains(postfix[i]) && !falseValues.contains(postfix[i])) ||
                   (!trueValues.contains(postfix[i]) && falseValues.contains(postfix[i]))) //XOR
                    if(j != 2 && j != 3) //Check if other two conditions have been done
                        j = 0;
            }
        }
        if(j == 0) //Now we can change the result based on it
        {
            stack<bool> s;
            for(int i = 0; i < postfix.size(); i++)
            {
                char c = postfix[i];
                if(islower(c)) //Operand
                {
                    if(trueValues.contains(c))
                        s.push(true);
                    else if(falseValues.contains(c))
                        s.push(false);
                    else
                        return 3;
                }
                else
                {
                    bool operand2 = s.top();
                    s.pop();
                    if(postfix[i] == '!')
                    {
                        s.push(!operand2);
                        continue;
                    }
                    bool operand1 = s.top();
                    s.pop();
                    if(postfix[i] == '|')
                    {
                        if(operand1 || operand2)
                            s.push(true);
                        else
                            s.push(false);
                    }
                    if(postfix[i] == '&')
                    {
                        if(operand1 && operand2)
                            s.push(true);
                        else
                            s.push(false);
                    }
                }
            }
            result = s.top();
            return 0;
        }
        return j;
    }
    return 1;
}

/*Helper Implementation*/
bool validity(string infix)
{
    string temp;
    for(int i = 0; i < infix.size(); i++) //Remove spaces
        if(infix[i] != ' ')
            temp += infix[i];
    if(!islower(temp[0]) && temp[0] != '(' && temp[0] != '!') //Validity test for 0
        return false;
    if(!islower(temp[temp.size()-1]) && temp[temp.size()-1] != ')') //Validity test for last
        return false;
    int j = 0; //Charcter counter
    int open = 0; //Parantheses
    int close = 0; //Parantheses
    for(int i = 0; i < temp.size(); i++) //Overall Validity
    {
        if(islower(temp[i]))
            j++;
        if(isupper(temp[i]) || isdigit(temp[i]))//No upper case allowed
            return false;
        if(temp[i] != '|' && temp[i] != '&' && !islower(temp[i]) &&
           temp[i] != '(' && temp[i] != '!' && temp[i] != ')') //Validity test
            return false;
        if(temp[i] == ')') // ) restrictor
            close++;
        if(temp[i] == '(') // ( restrictor
            open++;
    }
    if(open != close)
        return false;
    if(j <= 0 || (temp.size() == j && temp.size() != 1)) //Must have a character and operator
        return false;
    for(int i = 1; i < temp.size() - 1; i++)
    {
        if(temp[i] == '|' || temp[i] == '&') //OR, AND PRECEDED BY OR FOLLOWED BY
            if((!islower(temp[i+1]) && temp[i+1] != '(' && temp[i+1] != '!') ||
               (!islower(temp[i-1]) && temp[i-1] != ')'))
                return false;
        if(temp[i] == '!') //NOT PRECEDED BY OR FOLLOWED BY
            if((!islower(temp[i+1]) && temp[i+1] != '(' && temp[i+1] != '!') ||
               (temp[i-1] != '&' && temp[i-1] != '|' && temp[i-1] != '(' && temp[i-1] != '!'))
                return false;
        if(islower(temp[i])) //CHAR PRECEDED OR FOLLOWED BY
            if((temp[i-1] != '&' && temp[i-1] != '|' && temp[i-1] != '!' && temp[i-1] != '(') ||
               (temp[i+1] != '&' && temp[i+1] != '|' && temp[i+1] != '!' && temp[i+1] != ')'))
                return false;
        if(temp[i] == '(') // ( restrictor
            if((temp[i+1] != '!' && !islower(temp[i+1]) && temp[i+1] != '(') ||
                (temp[i-1] != '&' && temp[i-1] != '|' && temp[i-1] != '(' && temp[i-1] != '!'))
                return false;
        if(temp[i] == ')') // ) restrictor
            if((temp[i-1] != '!' && !islower(temp[i-1])&& temp[i-1] != ')') ||
               (temp[i+1] != '&' && temp[i+1] != '|' && temp[i+1] != ')'))
                return false;
    }
    return true;
}

int prec(char a)
{
    switch(a)
    {
        case '!':
            return 2;
        case '&':
            return 1;
        case '|':
            return 0;
        default:
            return -1;
    }
}

void itp(string infix, string& postfix)
{
    postfix = "";
    stack<char> s;
    string temp;
    for(int i = 0; i < infix.size(); i++) //Remove spaces
        if(infix[i] != ' ')
            temp += infix[i];
    char t;
    for(int i = 0; i < temp.size(); i++)
    {
        t = temp[i];
        switch(t)
        {
            case '(': // (
                s.push(t);
                break;
            case ')': // )
                while(s.top() != '(')
                {
                    postfix += s.top();
                    s.pop();
                }
                s.pop();
                break;
            case '!': //Operators
            case '&':
            case '|':
                while(!s.empty() && s.top() != '(' &&
                      prec(t) <= prec(s.top()) && postfix.size() >= 1)
                {
                    postfix += s.top();
                    s.pop();
                }
                s.push(t);
                break;
            default: //Operands
                postfix += t;
                break;
        }
    }
    while(!s.empty())
    {
        postfix += s.top();
        s.pop();
    }
}

/*main program*/
int main()
{
//    string ifix = "!!x"; //itp function tester
//    string pfix;
//    itp(ifix, pfix);
//    cout << pfix << endl;
    
////////////////////////////////////
    
//    string j = ""; // validity function tester
//    bool i = validity(j);
//    if(i)
//        cout << "TRUE" << endl;
//    else
//        cout << "FALSE" << endl;
    
////////////////////////////////////
    
//    char a = '!'; // prec function tester
//    cout << prec(a) << endl;
    
////////////////////////////////////
    
//    string trueChars  = "tywz"; //Full tester from the spec
//    string falseChars = "fnx";
//    Set trues;
//    Set falses;
//    for (int k = 0; k < trueChars.size(); k++)
//        trues.insert(trueChars[k]);
//    for (int k = 0; k < falseChars.size(); k++)
//        falses.insert(falseChars[k]);
//
//    string pf;
//    bool answer;
//    assert(evaluate("w| f", trues, falses, pf, answer) == 0  &&  pf == "wf|" /*&&  answer*/);
//    assert(evaluate("y|", trues, falses, pf, answer) == 1);
//    assert(evaluate("n t", trues, falses, pf, answer) == 1);
//    assert(evaluate("nt", trues, falses, pf, answer) == 1);
//    assert(evaluate("()", trues, falses, pf, answer) == 1);
//    assert(evaluate("y(n|y)", trues, falses, pf, answer) == 1);
//    assert(evaluate("t(&n)", trues, falses, pf, answer) == 1);
//    assert(evaluate("(n&(t|7)", trues, falses, pf, answer) == 1);
//    assert(evaluate("", trues, falses, pf, answer) == 1);
//    assert(evaluate("f  |  !f & (t&n) ", trues, falses, pf, answer) == 0
//           &&  pf == "ff!tn&&|"  &&  !answer);
//    assert(evaluate(" x  ", trues, falses, pf, answer) == 0  &&  pf == "x"  &&  !answer);
//    trues.insert('x');
//    assert(evaluate("((x))", trues, falses, pf, answer) == 3);
//    falses.erase('x');
//    assert(evaluate("((x))", trues, falses, pf, answer) == 0  &&  pf == "x"  &&  answer);
//    trues.erase('w');
//    assert(evaluate("w| f", trues, falses, pf, answer) == 2);
//    falses.insert('w');
//    assert(evaluate("w| f", trues, falses, pf, answer) == 0  &&  pf == "wf|" &&  !answer);
//    assert(evaluate("w|f &t |!x & n |!(f&t)", trues, falses, pf, answer) == 0  &&  pf == "wft&|x!n&|ft&!|" &&  answer); //Size data test
//    assert(evaluate("   !     x   ", trues, falses, pf, answer) == 0  &&  pf == "x!" && !answer); //Spaces test
//    assert(evaluate("ahdjsfkljahsjfahrgfpeurihr298orifjegro8ryfgah.dnl/fja'irhiour", trues, falses, pf, answer) == 1);
//    assert(evaluate("a|R & h | !r", trues, falses, pf, answer) == 1);
//    assert(evaluate("t|t & f | !x", trues, falses, pf, answer) == 0 && pf == "ttf&|x!|" && answer);
//    assert(evaluate("x|w| f", trues, falses, pf, answer) == 0 && pf == "xw|f|");
//    assert(evaluate("x|(w| f)", trues, falses, pf, answer) == 0 && pf == "xwf||");
//    assert(evaluate("x|(w| f", trues, falses, pf, answer) == 1);
//    assert(evaluate("x|w| f)", trues, falses, pf, answer) == 1);
//    assert(evaluate("(x|w| f", trues, falses, pf, answer) == 1);
//    assert(evaluate("((((((x|w| f)))", trues, falses, pf, answer) == 1);
//    assert(evaluate("!!x", trues, falses, pf, answer) == 0 && answer);
//    cout << "Passed all tests" << endl;
}
