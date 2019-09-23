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
    current.setTermType(OPERATION);

    opset_.emplace(symbol, current);
}

double MathInterpretProvider::eval(std::string input)
{
    checkSyntax(input);
    std::string pureMath = prepare(input);

    std::queue<Term*> termsToEval;
    parseTerms(pureMath, termsToEval);

    std::queue<Term*> polishReverseQueueTerms;
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

void MathInterpretProvider::parseTerms(std::string input, std::queue<Term*>& terms)
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
                Number* number = new Number(buffer);
                number->setTermType(NUMBER);
                terms.push(number);
                buffer.clear();
            }
        }
        else if(currentType == OPERATION)
        {
            Term* op = new Operation(getOperation(*iter));
            op->setTermType(OPERATION);

            terms.push(op);
        }
        else if(currentType == OPEN_BRACKET)
        {
            Term* openBracket = new Operation(-1, nullptr);
            openBracket->setTermType(OPEN_BRACKET);

            terms.push(openBracket);
        }
        else if(currentType == CLOSE_BRACKET)
        {
            Term* closeBracket = new Operation(-1, nullptr);
            closeBracket->setTermType(CLOSE_BRACKET);

            terms.push(closeBracket);
        }
    }
}


void MathInterpretProvider::toPolishReverseNotation(std::queue<Term*>& input,
                                                    std::queue<Term*>& output)
{
    std::stack<Operation*>operationTransferStack;

    while(!input.empty())      //transform to polish reverse notation
    {
        Term* current = input.front();
        input.pop();

        if(current->type() == NUMBER)
        {
            output.push(current);
            continue;
        }

        if(current->type() == OPEN_BRACKET)
        {
            operationTransferStack.push(dynamic_cast<Operation*>(current));
            continue;
        }

        if(current->type() == CLOSE_BRACKET)
        {
            while(operationTransferStack.top()->type() != OPEN_BRACKET)
            {
                output.push(operationTransferStack.top());
                operationTransferStack.pop();
            }
            operationTransferStack.pop();      //remove '('

            continue;
        }

        if(current->type() == OPERATION)
        {
            Operation* currentOp = dynamic_cast<Operation*>(current);
            int currentPriority = currentOp->priority();

            while(!operationTransferStack.empty() &&
                    dynamic_cast<Operation*>(operationTransferStack.top())->priority() >= currentPriority)
            {
                output.push(operationTransferStack.top());
                operationTransferStack.pop();
            }

            operationTransferStack.push(dynamic_cast<Operation*>(current));
        }
    }

    while(!operationTransferStack.empty())
    {
        output.push(operationTransferStack.top());
        operationTransferStack.pop();
    }
}

double MathInterpretProvider::evalPolishReverseNotation(std::queue<Term*> &input)
{
    std::stack<double> evaluateStack;
    while(!input.empty())
    {
        Term* current = input.front();
        current->eval(evaluateStack);

        delete current;

        input.pop();
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
    else return opset_.at(op).priority();
}

const Operation& MathInterpretProvider::getOperation(char c)
{
    return opset_.at(c);
}
