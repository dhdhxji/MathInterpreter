#include "mathinterpreter.h"

void MathInterpretProvider::addOperation(char symbol, int priority,
                                            double (*evalFunction)(const double& op1, const double& op2))
{
    Operation current(priority, evalFunction);

    opset_.emplace(symbol, current);
}

double MathInterpretProvider::eval(std::string input)
{
    std::string pureMath = process(input);

    if(!isExpressionCorrect(pureMath))
        throw "Parsing Error: invalid syntax";

    std::stack<double> values;
    std::stack<char> operations;

    parseTerms(pureMath, values, operations);

    return evalWithPolishReverseNotation(values, operations);
}

bool MathInterpretProvider::isExpressionCorrect(std::string input)
{
    int openedBr = 0;
    int closedBr = 0;

    char prev = '\0';
    for(auto iter = input.begin(); iter != input.end(); ++iter)
    {
        char current = *iter;

        if(current == '(')
        {
            openedBr++;
            continue;
        }
        else if(current == ')')
        {
            closedBr++;
        }

        else if(current >= '0' && current <= '9'){}
        else return false;
    }

    if(closedBr - openedBr != 0)
        return false;



    return true;
}
