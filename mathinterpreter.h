#ifndef MATHINTERPRETER_H
#define MATHINTERPRETER_H

#include <map>
#include <string>
#include <stack>

#include "operation.h"




class MathInterpretProvider
{
public:
    void addOperation(char symbol, int priority,
                      double (*evalFunction)(const double& op1, const double& op2));

    bool isExpressionCorrect(std::string input);

    double eval(std::string input);

private:
    std::map<char, Operation> opset_;

    std::string process(std::string input);     //remove space characters, place operands
                                                //in prefix form of notation
    void parseTerms(std::string input,
                    std::stack<double>& values, std::stack<char>& operations);
    double evalWithPolishReverseNotation(std::stack<double>& values, std::stack<char>& operations);
};

#endif // MATHINTERPRETER_H
