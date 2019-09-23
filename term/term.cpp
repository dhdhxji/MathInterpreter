#include "term.h"

int Term::type() const
{
    return termType_;
}

void Term::setTermType(int type)
{
    termType_ = type;
}
