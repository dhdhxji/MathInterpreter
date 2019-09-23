#ifndef OPERATION_H
#define OPERATION_H

#include "term.h"

class Operation: public Term
{
public:
    Operation(int priority, double (*func)(const double& p1, const double& p2),
              bool prefix = false);

    void eval(std::stack<double>&evalStack) const override;
    int priority() const;
    bool isCanBePrefix() const;

private:
    int priority_;
    double (*func_)(const double& p1, const double& p2);

    bool isCanBePrefix_;
};

#endif // OPERATION_H
