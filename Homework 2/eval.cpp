
#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

int compare(char op1, char op2);
string convert(string infix);
bool valid(string x_infix);
bool valuesMap(string infix, const Map& values);
bool calc(const Map& values, string& post_fix, int& result);
int calcOp(string op, int v1, int v2);
bool isOperator(char op);
int evaluate(string infix, const Map& values, string& postfix, int& result);

int evaluate(string infix, const Map& values, string& postfix, int& result)
// Evaluates an integer arithmetic expression
//   If infix is a syntactically valid infix integer expression whose
//   only operands are single lower case letters (whether or not they
//   appear in the values map), then postfix is set to the postfix
//   form of the expression; otherwise postfix may or may not be
//   changed, result is unchanged, and the function returns 1.  If
//   infix is syntactically valid but contains at least one lower
//   case letter operand that does not appear in the values map, then
//   result is unchanged and the function returns 2.  If infix is
//   syntactically valid and all its lower case operand letters
//   appear in the values map, then if evaluating the expression
//   (using for each letter in the expression the value in the map
//   that corresponds to it) attempts to divide by zero, then result
//   is unchanged and the function returns 3; otherwise, result is
//   set to the value of the expression and the function returns 0.
{
    if(!valid(infix))
        return 1;
    else
    {
        string post_fix = convert(infix);
        postfix = post_fix;
        if(!valuesMap(infix, values))
            return 2;
        else
        {
            if(calc(values, postfix, result))
                return 0;
            else
                return 3;
        }
    }
}

int calcOp(char op, int v1, int v2)
{
    if(op == '+')
        return v1 + v2;
    if(op == '*')
        return v1 * v2;
    if(op == '/')
        return v1 / v2;
    if(op == '-')
        return v1 - v2;
    return -1;
}

bool calc(const Map& values, string& post_fix, int& result)
{
    stack<int> s_stack;
    for(int i = 0; i < post_fix.length(); i++)
    {
        if(islower(post_fix[i]))
        {
            int res;
            values.get(post_fix[i], res);
            s_stack.push(res);
        }
        if(post_fix[i] == '+' || post_fix[i] == '-' || post_fix[i] == '*' || post_fix[i] == '/')
        {
            int v2 = s_stack.top();
            s_stack.pop();
            int v1 = s_stack.top();
            s_stack.pop();
            if(v2 == 0 && post_fix[i] == '/')
                return false;
            int res = calcOp(post_fix[i], v1, v2);
            s_stack.push(res);
        }
    }
    result = s_stack.top();
    return true;
}

bool isOperator(char op)
{
    switch(op)
    {
        case '+':
        case '-':
        case '*':
        case '/': return true;
        default: return false;
    }
}

bool valid(string infix)
{
    //"y +o *(   a-u)  "
    int numLetters = 0;
    int numOperators = 0;
    int numBrace1 = 0;
    int numBrace2 = 0;
    for(int i = 0; i < infix.size(); i++)
    {
        if(infix[i] == ' ')
            continue;
        if(!islower(infix[i]))
        {
            if(isOperator(infix[i]))
            {
               numOperators++;
               if(numLetters == 0)
                   return false;
            }
            else
            {
                if(infix[i] != '(' && infix[i] != ')')
                    return false;
                else if(infix[i] == '(')
                {
                    if(i != 0)
                    {
                        if(numOperators < numLetters)
                            return false;
                    }
                    numBrace1++;
                }
                else if(infix[i] == ')')
                    numBrace2++;
            }
        }
        
        if(islower(infix[i]))
        {
           if(i != infix.size() - 1)
           {
               if(islower(infix[i + 1]))
                   return false;
           }
            numLetters++;
        }
        
        
    }
    if(numLetters == 0 && numOperators == 0)
        return false;
    if(numLetters == 1 && numOperators == 1)
        return false;
    if(numLetters > 1 && numOperators == 0)
        return false;
    if(numLetters == 0 && numOperators > 0)
        return false;
    if(numBrace1 != numBrace2)
        return false;
    return true;
}

//returns true if all lower case operands are contained in the values map
//returns false if at least one lower case operand is not found in the values map
bool valuesMap(string infix, const Map& values)
{
    stack<char> x_stack;
    for(int i = 0; i < infix.length(); i++)
    {
        if(islower(infix[i]))
            x_stack.push(infix[i]);
    }
    while(!x_stack.empty())
    {
        if(!values.contains(x_stack.top()))
            return false;
        x_stack.pop();
    }
    return true;
}

//returns 0 if op1 has greater or equal precedence than op2
//returns 1 if op1 has less precedence than op2
int compare(char op1, char op2)
{
    if(op1 == '+')
    {
        if(op2 == '-' || op2 == '+')
            return 0;
        return 1;
    }
    if(op1 == '-')
    {
        if(op2 == '+' || op2 == '-')
            return 0;
        return 1;
    }
    if(op1 == '*')
    {
        if(op2 == '/' || op2 == '+' || op2 == '-' || op2 == '*')
            return 0;
        return 1;
    }
    if(op1 == '/')
    {
        if(op2 == '*' || op2 == '+' || op2 == '-' || op2 == '/')
            return 0;
        return 1;
    }
    return -1;
}

//convert infix string to postfix string
string convert(string infix)
{
    std::stack<string> s_stack;
    string postfix = "";
    //1. Begin at left-most Infix token
    for(int i = 0; i < infix.length(); i++)
    {
        //if it's a number, append it to end of postfix string followed by a space
        if(islower(infix[i]))
        {
            postfix += infix.substr(i, 1);
        }
        //if it's a (, push it onto the stack
        if(infix.substr(i, 1) == "(")
        {
            s_stack.push("(");
        }
        //if it's an operator
        if(infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/')
        {
            //and the stack is empty, push the operator on the stack
            if(s_stack.empty())
            {
                s_stack.push(infix.substr(i, 1));
            }
            else
            {
                //if the stack is not empty
                //pop all operators with greater or equal precedence off the stack
                //and append them on the postfix string
                while(!s_stack.empty() && s_stack.top() != "(" && compare(s_stack.top()[0], infix[i]) == 0)
                {
                    postfix += s_stack.top();
                    s_stack.pop();
                }
                //stop when you reach an operator with lower precedence of a (
                //push the new operator on the stack
                s_stack.push(infix.substr(i, 1));
            }
        }
        //if it's a ), pop operators off the stack and append them onto the postfix string until you pop a matching (
        if(infix.substr(i, 1) == ")")
        {
            //cout << ")" << endl;
            string s = s_stack.top();
            while(s != "(")
            {
                postfix += s_stack.top();
                s_stack.pop();
                s = s_stack.top();
            }
            s_stack.pop();
        }
    }
    //when all infix tokens are gone, pop each operator and append it to the postfix string
    while(!s_stack.empty())
    {
        postfix += s_stack.top();
        s_stack.pop();
    }
    return postfix;
}


