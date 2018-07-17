#ifndef NETWOKMAKER_H_INCLUDED
#define NETWOKMAKER_H_INCLUDED

using namespace std;
class NeuralNetwork;
typedef vector <double> dvec;
typedef vector <dvec> ddvec;
typedef vector <int> ivec;
typedef vector <string> svec;
#include <vector>
#include "NNTraining.h"
#include "InputReader.h"

class NeuralNetwork
{

    // allow other classes to use the variables of this class
    friend class NNTrainer;
    friend class InputReader;
    friend main();
protected:
    ddvec weights;// all the weights for each input value at each neuron
    dvec output;
    dvec input;// all of the inputs
    // dvec oldInput;// save the previous input for reference
    double overallTotal; // the
    double bias;// the bias  that will be added to each neuron
    //  used to find the amount of neurons and hidden layers
    int numOfOutputs;
    int numOfInputs;
    int testNumber;
    dvec errorCost;
    ddvec deltaWeights;
    ddvec pastDelta;

public:
    vector <NeuralNetwork> makeNNTrain (InputReader *input);
    void createWeights(ddvec *newWeight, NeuralNetwork *hiddenLayer);// set the initial weights
    void makeLayer(NeuralNetwork *Network);// create each neuron in the network
    void normalizeValue (NeuralNetwork *layer);
    void getNextInput (NeuralNetwork lastLayer, NeuralNetwork *nextLayer);
    void getFirstInput (NeuralNetwork *network, string input);
    void makeOutput( NeuralNetwork *hiddenLayer);
    void deleteValues (NeuralNetwork *network);
    void saveWeights (NeuralNetwork *network);
    void runRealNN (vector <NeuralNetwork*> network, InputReader *input);

};

#endif // NETWOKMAKER_H_INCLUDED
