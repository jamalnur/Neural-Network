#ifndef INPUTREADER_H_INCLUDED
#define INPUTREADER_H_INCLUDED

using namespace std;

class InputReader;

#include "NetwokMaker.h"

class InputReader
{
    friend class NNTrainer;
    friend class NeuralNetwork;
    friend  main();
protected:
    int currentCheck;
    vector <string> allInput;
    string currentInput;
    string currentAnswer;


public:
    void readAllInput (InputReader *reader);
    void getCurrentInput (InputReader *reader);
    void deletePastInput (InputReader *reader);
};




#endif // INPUTREADER_H_INCLUDED
