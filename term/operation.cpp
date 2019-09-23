#include "operation.h"

Operation::Operation(int priority, double (*func)(const double& p1, const double& p2),
                     bool prefix)
{
    priority_ = priority;
    func_ = func;

    isCanBePrefix_ = prefix;
}

void Operation::eval(std::stack<double>& evalStack) const
{
    double operand2 = evalStack.top();
    evalStack.pop();
    double operand1 = evalStack.top();
    evalStack.pop();

    evalStack.push(func_(operand1, operand2));
}

int Operation::priority() const
{
    return priority_;
}

bool Operation::isCanBePrefix() const
{
    return isCanBePrefix_;
}
