#ifndef NNTRAINING_H_INCLUDED
#define NNTRAINING_H_INCLUDED

class NNTrainer;



// includes
#include <vector>
#include "NetwokMaker.h"
#include "InputReader.h"


using namespace std;

class NNTrainer
{
    friend class NeuralNetwork;
    friend class InputReader;
    friend main();
protected:
    double corretAnswer[9];// use to update weights
    double momentum;
    double learningRate;
    int numCheck;


public:
    void runNN(vector <NeuralNetwork*> network, InputReader *input);
    void fowardProp (NeuralNetwork *network);
    void backProp (vector<NeuralNetwork*> network, NNTrainer training);
    NNTrainer findCorrectAnswer (NNTrainer hiddenLayer, InputReader *input);
    double findError (double output, double desiredOutput);
    double findDeltaHidden (double network,  double sum);
    double updateWeights (double weight, double deltaWeight);

};

#endif // NNTRAINING_H_INCLUDED
