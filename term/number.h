#ifndef NUMBER_H
#define NUMBER_H

#include <string>
#include "term.h"

class Number: public Term
{
public:
    Number(std::string strNumber);

    void eval(std::stack<double> &evalStack) const override;

private:
    double number_;
};

#endif // NUMBER_H
