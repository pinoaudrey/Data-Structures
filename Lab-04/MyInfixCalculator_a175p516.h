#ifndef __MYINFIXCALCULATOR_H__
#define __MYINFIXCALCULATOR_H__

#include <algorithm>
#include <string>

#include "MyStack_a175p516.h"
#include "MyVector_a175p516.h"

class MyInfixCalculator{

  public:
    
    MyInfixCalculator()
    {

    }

    ~MyInfixCalculator()
    {
     
    }

    double calculate(const std::string& s)
    {
        // code begins
        MyVector<std::string> infix_tokens; // stores the infix tokens
        MyVector<std::string> postfix_tokens; // stores the postfix tokens
        tokenize(s, infix_tokens); // tokenize the infix string
        infixToPostfix(infix_tokens, postfix_tokens); // convert infix to postfix
        return calPostfix(postfix_tokens); // calculate the postfix expression and return the result
        // code ends
    }

  private:

    // returns operator precedance; the smaller the number the higher precedence
    // returns -1 if the operator is invalid
    // does not consider parenthesis
    int operatorPrec(const char c) const
    {
        switch(c)
        {
            case '*':
                return 2;
            case '/':
                return 2;
            case '+':
                return 3;
            case '-':
                return 3;
            default:
                return -1;
        }
    }

    // checks if a character corresponds to a valid parenthesis
    bool isValidParenthesis(const char c) const
    {
        switch(c)
        {
            case '(':
                return true;
            case ')':
                return true;
            default:
                return false;
        }
    }

    // checks if a character corresponds to a valid digit
    bool isDigit(const char c) const
    {
        if(c >= '0' && c <= '9')
            return true;
        return false;
    }

    // computes binary operation given the two operands and the operator in their string form
    double computeBinaryOperation(const std::string& ornd1, const std::string& ornd2, const std::string& opt) const
    {
        double o1 = std::stod(ornd1);
        double o2 = std::stod(ornd2);
        switch(opt[0])
        {
            case '+':
                return o1 + o2;
            case '-':
                return o1 - o2;
            case '*':
                return o1 * o2;
            case '/':
                return o1 / o2;
            default:
                std::cout << "Error: unrecognized operator: " << opt << std::endl;
                return 0.0;
        }
    }


    // tokenizes an infix string s into a set of tokens (operands or operators)
 void tokenize(const std::string &s, MyVector<std::string> &tokens)
    {
        // code begins
        for (size_t i = 0; i < s.size(); i++)
        {                         // loop through the string
            // fix the bitwise OR operator to a logical OR operator
            if (isDigit(s[i]) || (s[i] == '-' && isDigit(s[i + 1]) && (i == 0 || (!isDigit(s[i - 1]) && s[i - 1] != ')'))))
            {
                std::string num; // create a string to store the number
                num += s[i];     // add the character to the string
                i++;             // move to the next character
                while (i < s.size() && (isDigit(s[i]) || s[i] == '.'))
                {                // while the current character is a digit or a decimal point
                    num += s[i]; // add the current character to the string
                    i++;         // move to the next character
                }

                if (i < s.size() && isDigit(s[i]) == false) // if the current character is not a digit
                {        
                    i--; // move back to the previous character
                }
                tokens.push_back(num); // add the number to the vector
            }
            else if (isValidParenthesis(s[i]))
            {                             // if the current character is a parenthesis
                std::string par(1, s[i]); // create a string to store the parenthesis and add the parenthesis to the string
                tokens.push_back(par);    // add the parenthesis to the vector
            }
            else
            {                            // if the current character is an operator (not a digit or a parenthesis)
                std::string op(1, s[i]); // create a string to store the operator and add the operator to the string
                tokens.push_back(op);    // add the operator to the vector
            }
        }
        // code ends
    }

    // converts a set of infix tokens to a set of postfix tokens
    void infixToPostfix(MyVector<std::string>& infix_tokens, MyVector<std::string>& postfix_tokens)
    {
        // code begins
        MyStack<std::string> stack; // create a stack to store the operators
        for(const auto &token : infix_tokens){
            if(isDigit(token[0]) || (token[0] == '-' && isDigit(token[1]))){ // if the token is a number
                postfix_tokens.push_back(token); // add the number to the postfix vector
            }
            else if(isValidParenthesis(token[0])){
                if(token[0] == '('){ // if the token is a left parenthesis
                    stack.push(token); // push the left parenthesis to the stack
                }
                else{ // if the token is a right parenthesis
                    while(!stack.empty() && stack.top()[0] != '('){ // while the top of the stack is not a left parenthesis
                        postfix_tokens.push_back(stack.top()); // add the top of the stack to the postfix vector
                        stack.pop(); // pop the top of the stack
                    }
                    stack.pop(); // pop the left parenthesis
                }
            }
            else{
                while(!stack.empty() && (stack.top()[0] != '(' && operatorPrec(token[0]) >= operatorPrec(stack.top()[0]))){ // while the top of the stack is not a left parenthesis and the precedence of the operator is less than or equal to the precedence of the top of the stack
                    postfix_tokens.push_back(stack.top()); // add the top of the stack to the postfix vector
                    stack.pop(); // pop the top of the stack
                }
                stack.push(token); // push the operator to the stack
            }
        }
        while(!stack.empty()) // while the stack is not empty
        { 
            postfix_tokens.push_back(stack.top()); // add the top of the stack to the postfix vector
            stack.pop(); // pop the top of the stack
        }
        // code ends
    }

    // calculates the final result from postfix tokens
    double calPostfix(const MyVector<std::string>& postfix_tokens) const
    {
        // code begins
        MyStack<double> stack; // create a stack to store the operands
        for(const auto &token : postfix_tokens)
        {
            if(isDigit(token[0]) || (token.size() > 1 && token[0] == '-' && isDigit(token[1]))) // if the token is a number
            { 
                stack.push(std::stod(token)); // push the number to the stack
            }
            else{ // if the token is an operator
                double o2 = stack.top(); // get the second operand
                stack.pop(); // pop the second operand from the stack
                double o1 = stack.top(); // get the first operand
                stack.pop(); // pop the first operand from the stack
                double result = computeBinaryOperation(std::to_string(o1), std::to_string(o2), token); // compute the result of the operation between the two operands and the operator 
                stack.push(result); // push the result to the stack
            }
        }
        return stack.top(); // return the result
        // code ends
    }
};

#endif // __MYINFIXCALCULATOR_H__