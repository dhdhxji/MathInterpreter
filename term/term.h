#ifndef TERM_H
#define TERM_H

#include<stack>

class Term
{
public:
    virtual void eval(std::stack<double>& evalStack) const = 0;
    int type() const;

    void setTermType(int type);

protected:
    int termType_;
};

#endif // TERM_H
