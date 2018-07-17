
#include <iostream>
#include "NetwokMaker.h"
#include <cstdlib>
#include <vector>
#include <math.h>
#include <sstream>
#include <cctype>
using namespace std;


typedef vector <double> dvec;
typedef vector <dvec > ddvec;
typedef vector <string> svec;

vector <NeuralNetwork> NeuralNetwork::makeNNTrain(InputReader *input)
{
    vector <NeuralNetwork> network;// make a network of 3 hidden layers
    NeuralNetwork *nnPointer;// make a pointer to pass through the functions
    for (int i=0; i<4; i++)// make a loop to create the weights/neurons
    {

        NeuralNetwork layer;// make a temporary variable to put into the vector
        nnPointer = &layer;// make the pointer point at the
        if (i>0)
        {
            layer.numOfInputs=network[i-1].weights.size();

        }
        else
        {
            // make the song lyrics the input for the first hidden layer
            layer.getFirstInput(nnPointer,input->currentInput);
            layer.numOfInputs=81;// set the size of the input to the correct value
        }
        if (i==3)
        {
            layer.makeOutput(nnPointer);
        }
        else
            layer.makeLayer(nnPointer);// make the hidden

        network.push_back(layer);// put it into the vector of layers
    }
    return network;
}

void NeuralNetwork::makeLayer(NeuralNetwork *network)
{
    ddvec *newWeight;
    newWeight = &network->weights;
    // set initial values of the neural network
    network->bias=-20;
    network->overallTotal=0;
    network->testNumber=0;
    network->numOfOutputs=9;
    // make the weights
    network->createWeights(newWeight, network);

}

void NeuralNetwork::createWeights(ddvec *newWeight, NeuralNetwork *hiddenLayer)
{

    int num=(hiddenLayer->numOfOutputs+hiddenLayer->numOfInputs )/2;// make a loop that finds the mean of the inputs and outputs

    for (int j=0; j<num; j++)// make a loop that holds makes each different neuron
    {
        dvec weight;// make a variable to hold each neuron's weights
        for (int i=0; i<hiddenLayer->numOfInputs; i++)// make a loop that makes (random number to start)
        {
            // create a weight for each input value
            double count = rand() % 999 + 1;
            weight.push_back(count/(1000));
        }
        newWeight->push_back(weight);// puts all the weights into the weight vector
    }
}

void NeuralNetwork::normalizeValue (NeuralNetwork *layer)
{
    //dvec derivative;
    for (int i=0; i<int (output.size()); i++)// normalize the values of each output using the sigmod normalization function
    {
        double newOutput=(layer->output[i])*(-1);

        layer->output[i]=1.f/(1.f+(exp(newOutput)));

    }
    for (int i=0; i<int (layer->output.size()); i++)// set the overall total for the layer
    {
        layer->overallTotal+=layer->output[i];

    }

}

void NeuralNetwork::getNextInput (NeuralNetwork lastLayer, NeuralNetwork *nextLayer)
{

    nextLayer->input=lastLayer.output;// set the current input to the previous output
}


void NeuralNetwork::getFirstInput (NeuralNetwork *network, string input)
{
    double newInput;// turn the input string into the different numbers
    for (int i=0; i<int (input.size()); i++)
    {
        if (!isdigit(input[i]))// if the input doesn't have the correct amount of inputs, exit
            return;
        newInput =double(input[i])-48;// run the sigmoid function on it to normalize the overall value
        newInput=1.f/(1.f+(exp(-newInput)));
        network->input.push_back(newInput);

    }
}


void NeuralNetwork::makeOutput( NeuralNetwork *hiddenLayer)
{
    // set starting weights
    hiddenLayer->bias=-3.0;
    hiddenLayer->overallTotal=0;
    hiddenLayer->testNumber=0;
    hiddenLayer->numOfOutputs=9;

    for (int j=0; j<9; j++)// make a loop that holds makes each different neuron
    {
        dvec weight;// make a variable to hold each neuron's weights
        for (int i=0; i<hiddenLayer->numOfInputs; i++)// make a loop that makes (random number to start)
        {
            // create a weight for each input value
            double count = rand() % 999 + 1;
            count=count/1000;
            weight.push_back(count);
        }
        hiddenLayer->weights.push_back(weight);// puts all the weights into the weight vector
    }

}


void NeuralNetwork::deleteValues (NeuralNetwork *network)
{
    // clear the previous values that are no longer needed
    network->errorCost.clear();
    network->output.clear();
    network->pastDelta.clear();
    network->deltaWeights.clear();

}

void NeuralNetwork::saveWeights(NeuralNetwork *network)
{
    ofstream outfile;// save the weights into a file
    // open the file

        outfile.open("weights.txt", ofstream::out);
        if (!outfile.is_open())
        {
            cout << "Could not open " << endl;
            return;
        }


    dvec weightSave;// output the weights
    for (int i=0; i<int (network->weights.size()); i++)
    {
        weightSave=network->weights[i];
        for (int j=0; j<int (weightSave.size()); j++)
        {
            outfile << weightSave[j]<< "   ";
        }

    }

    outfile.close();


}

void NeuralNetwork::runRealNN(vector <NeuralNetwork*> network, InputReader *inputer)
{
    // declare variables
    NNTrainer netTrainer;
    string finalOutput;


    network[0]->getFirstInput(network[0], inputer->currentInput);// get the input in the form of doubles

    if (network[0]->input.size()!=81)// if the input exited early and doesn't have enough inputs, don't run through the NN
    {
        cout<<"That is not valid, it has a non-degit input"<<endl;
        return;
    }
    for (int k=0; k<81; k++)// run for every non answered number
    {
        if (inputer->currentInput[k]!='0')
        {
            finalOutput+=inputer->currentInput[k];
            continue;
        }
        netTrainer=netTrainer.findCorrectAnswer(netTrainer, inputer);// get the correct answer for the current number
        netTrainer.fowardProp(network[0]);// run the inputs through the first hidden layer
        network[0]->normalizeValue(network[0]);
        for (int i=1; i<int(network.size()); i++)// run for the other layers
        {
            network[i]->input=network[i-1]->output;// set the input
            netTrainer.fowardProp(network[i]);
            network[i]->normalizeValue(network[i]);
        }
        int count=0;// output the number with the heighest odds according to the NN
        for (int i=0; i<9; i++)
        {
            if (network[3]->output[count]<network[3]->output[i])
                count=i;
        }
        finalOutput+=count+'1';
    }
    cout<<"The answer to this sudoku board is:"<<endl<<finalOutput<<endl;
}










