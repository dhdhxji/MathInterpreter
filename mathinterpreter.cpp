#include "mathinterpreter.h"

MathInterpretProvider::MathInterpretProvider()
{
    openingBracket_ = '(';
    closingBracket_ = ')';
}

MathInterpretProvider::MathInterpretProvider(char openBracket, char closeBracket)
{
    openingBracket_ = openBracket;
    closingBracket_ = closeBracket;
}

void MathInterpretProvider::addOperation(char symbol, int priority,
                                         double (*evalFunction)(const double& op1, const double& op2),
                                         bool prefix)
{
    Operation current(priority, evalFunction, prefix);

    opset_.emplace(symbol, current);
}

double MathInterpretProvider::eval(std::string input)
{
    checkSyntax(input);
    std::string pureMath = prepare(input);

    std::queue<std::string> termsToEval;
    parseTerms(pureMath, termsToEval);

    std::queue<std::string> polishReverseQueueTerms;
    toPolishReverseNotation(termsToEval, polishReverseQueueTerms);

    return evalPolishReverseNotation(polishReverseQueueTerms);
}

void MathInterpretProvider::checkSyntax(std::string input)
{
    for(auto iter = input.begin(); iter != input.end(); ++iter)
    {
        if(*iter == ' ')
            continue;

        termType type = symbolType(*iter);

        if(type == OPEN_BRACKET || type == CLOSE_BRACKET)
            continue;

        if(symbolType(*iter) == NONE)
        {
            std::string err = "Syntax error: invalid character: ";
            err += *iter;

            throw err.c_str();
        }
    }
}

std::string MathInterpretProvider::prepare(std::string input)
{
    std::string result;

    termType prevType = NONE;
    for(auto iter = input.begin(); iter != input.end(); ++iter)
    {
        char current = *iter;
        termType currentType = symbolType(current);

        if(current == ' ')
            continue;

        if(currentType == OPERATION)
        {
            if(prevType == OPERATION || prevType == OPEN_BRACKET || prevType == NONE)
            {
                if(getOperation(current).isCanBePrefix())
                {
                    result += '0';
                }
                else throw "Syntax error : operation can not have prefix form";
            }
            result += current;
        }
        else if(currentType == NUMBER
                || currentType == OPEN_BRACKET || currentType == CLOSE_BRACKET)
            result.push_back(current);

        prevType = currentType;
    }

    return result;
}

void MathInterpretProvider::parseTerms(std::string input, std::queue<std::string>& terms)
{
    std::string buffer;

    for(auto iter = input.begin(); iter != input.end(); ++iter)
    {
        termType currentType = symbolType(*iter);

        if(currentType == NUMBER)
        {
            buffer += *iter;
            termType nextType = symbolType(*(iter + 1));
            if((iter+1) == input.end() || nextType == OPERATION
                    || nextType == OPEN_BRACKET || nextType == CLOSE_BRACKET)
            {
                terms.push(buffer);
                buffer.clear();
            }
        }
        else if(currentType == OPERATION
                || currentType == OPEN_BRACKET || currentType == CLOSE_BRACKET)
        {
            std::string temp;
            temp += *iter;
            terms.push(temp);
        }
    }
}


void MathInterpretProvider::toPolishReverseNotation(std::queue<std::string>& input,
                                                    std::queue<std::string>& output)
{
    std::stack<std::string>operationTransferStack;

    while(!input.empty())      //transform to polish reverse notation
    {
        std::string current = input.front();
        input.pop();

        if(symbolType(current) == NUMBER)
        {
            output.push(current);
            continue;
        }

        if(symbolType(current) == OPEN_BRACKET)
        {
            operationTransferStack.push(current);
            continue;
        }

        if(symbolType(current) == CLOSE_BRACKET)
        {
            while(symbolType(operationTransferStack.top()) != OPEN_BRACKET)
            {
                output.push(operationTransferStack.top());
                operationTransferStack.pop();
            }
            operationTransferStack.pop();      //remove '('

            continue;
        }

        if(symbolType(current) == OPERATION)
        {
            int currentPriority = getOperationPriority(current[0]);

            while(!operationTransferStack.empty() &&
                    getOperationPriority(operationTransferStack.top()[0]) >= currentPriority)
            {
                output.push(operationTransferStack.top());
                operationTransferStack.pop();
            }

            operationTransferStack.push(current);
        }
    }

    while(!operationTransferStack.empty())
    {
        output.push(operationTransferStack.top());
        operationTransferStack.pop();
    }
}

double MathInterpretProvider::evalPolishReverseNotation(std::queue<std::string> &input)
{
    std::stack<double> evaluateStack;
    while(!input.empty())
    {
        std::string current = input.front();
        input.pop();

        if(symbolType(current) == NUMBER)
        {
            evaluateStack.push(std::stod(current));
        }

        else if(symbolType(current) == OPERATION)
        {
            double operand2 = evaluateStack.top();
            evaluateStack.pop();
            double operand1 = evaluateStack.top();
            evaluateStack.pop();


            char chOp = current[0];
            double resultValue = opset_.at(chOp).eval(operand1, operand2);

            evaluateStack.push(resultValue);
        }
    }

    if(evaluateStack.size() != 1)
        throw "Calculation error: argument count disparity";

    double result = evaluateStack.top();
    evaluateStack.pop();

    return result;
}

MathInterpretProvider::termType MathInterpretProvider::symbolType(char c)
{
    if(c >= '0' && c <= '9')
        return NUMBER;
    else if(opset_.find(c) != opset_.end())
        return OPERATION;
    else if(c == openingBracket_)
        return OPEN_BRACKET;
    else if(c == closingBracket_)
        return CLOSE_BRACKET;

    else return NONE;
}

MathInterpretProvider::termType MathInterpretProvider::symbolType(std::string in)
{
    char oneCh = in[0];
    return symbolType(oneCh);
}

int MathInterpretProvider::getOperationPriority(char op)
{
    if(op == openingBracket_ || op == closingBracket_)
        return -1;
    else return opset_.at(op).getPriority();
}

const Operation& MathInterpretProvider::getOperation(char c)
{
    return opset_.at(c);
}
