#ifndef OPERATION_H
#define OPERATION_H

class Operation
{
public:
    Operation(int priority, double (*func)(const double& p1, const double& p2),
              bool prefix = false);
    //Operation(const Operation& op);
    //Operation(Operation&& op);

    double eval(const double& p1, const double& p2) const;
    int getPriority() const;
    bool isCanBePrefix() const;

private:
    int priority_;
    double (*func_)(const double& p1, const double& p2);

    bool isCanBePrefix_;
};

#endif // OPERATION_H
