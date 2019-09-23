#include "number.h"

Number::Number(std::string strNumber)
{
    number_ = stod(strNumber);
}

void Number::eval(std::stack<double> &evalStack) const
{
    evalStack.push(number_);
}
