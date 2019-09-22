#include <iostream>
#include <cmath>
#include "mathinterpreter.h"



using namespace std;



double opPlus(const double& a, const double& b)
{
    return a+b;
}

double opMinus(const double& a, const double& b)
{
    return a-b;
}

double opMultiply(const double& a, const double& b)
{
    return a*b;
}

double opDIvision(const double& a, const double& b)
{
    return a/b;
}

double opPower(const double& a, const double& power)
{
    return pow(a, power);
}


int main()
{
    MathInterpretProvider interpreter;

    interpreter.addOperation('+', 0, &opPlus);
    interpreter.addOperation('-', 0, &opMinus);
    interpreter.addOperation('*', 1, &opMultiply);
    interpreter.addOperation('/', 1, &opDIvision);
    interpreter.addOperation('^', 2, &opPower);

    cout << "Available operations : + - * / ^, ( )" << endl;

    while(true)
    {
        cout << '>';

        string input;
        getline(cin, input);

        cout << "    = " << interpreter.eval(input) << endl;
    }

    return 0;
}
