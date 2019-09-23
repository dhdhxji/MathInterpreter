#include "operation.h"

Operation::Operation(int priority, double (*func)(const double& p1, const double& p2),
                     bool prefix)
{
    priority_ = priority;
    func_ = func;

    isCanBePrefix_ = prefix;
}

int Operation::getPriority() const
{
    return priority_;
}

double Operation::eval(const double& op1, const double& op2)const
{
    return func_(op1, op2);
}
bool Operation::isCanBePrefix() const
{
    return isCanBePrefix_;
}
