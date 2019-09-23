#ifndef MATHINTERPRETER_H
#define MATHINTERPRETER_H

#include <map>
#include <string>
#include <stack>
#include <queue>

#include "operation.h"




class MathInterpretProvider
{
public:
    MathInterpretProvider();
    MathInterpretProvider(char openBracket, char closeBracket);

    void addOperation(char symbol, int priority,
                      double (*evalFunction)(const double& op1, const double& op2),
                      bool prefix = false);


    double eval(std::string input);

private:
    enum termType {NONE, NUMBER, OPERATION, OPEN_BRACKET, CLOSE_BRACKET};
    std::map<char, Operation> opset_;

    char openingBracket_;
    char closingBracket_;



    void checkSyntax(std::string input);        //check for unacceptable symbols
                                                //and brackets corresponding
                                                //throw exception if not

    std::string prepare(std::string input);     //remove space characters, place operands
                                                //in prefix form of notation

    void parseTerms(std::string input, std::queue<std::string>& values);    //transform
                                                //input string to queue of terms

    void toPolishReverseNotation(std::queue<std::string>& input,
                                 std::queue<std::string>& output);
    double evalPolishReverseNotation(std::queue<std::string>& input);

    termType symbolType(char c);
    termType symbolType(std::string in);

    int getOperationPriority(char op);
    const Operation& getOperation(char c);
};

#endif // MATHINTERPRETER_H
