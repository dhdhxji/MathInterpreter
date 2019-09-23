#ifndef SPECIALTERM_H
#define SPECIALTERM_H

#include "term.h"

class SpecialTerm: public Term
{
public:
    void eval(std::stack<double> &evalStack) const override;
};

#endif // SPECIALTERM_H
