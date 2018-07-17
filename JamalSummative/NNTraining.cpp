
#include <iostream>
#include "NetwokMaker.h"
#include "NNTraining.h"
#include <cstdlib>
#include <vector>
#include <math.h>
using namespace std;


typedef vector <double> dvec;
typedef vector <dvec> ddvec;
typedef vector <string> svec;


void NNTrainer::fowardProp (NeuralNetwork *network)
{
    dvec currentWeight;
    for (int j=0; j<int (network->weights.size()); j++)
    {
        double total=network->bias;// set the total to the bias (instead of adding it later)
        double total2=0;
        currentWeight=network->weights[j];
        for (int i=0; i<int (currentWeight.size()); i++)// make a loop to multiply the weight by the input to find the output
        {
            total=total + (currentWeight[i]*network->input[i]);// find the sum of all the inputs by the weights
            total2+=(currentWeight[i]*network->input[i]);

        }
        if (network->weights.size()==9)// if this is the final output, use the second total
        {
            // put the total as the output for that neuron
            network->output.push_back(total2);
        }
        else
            network->output.push_back(total);
    }
}


void NNTrainer::runNN(vector <NeuralNetwork*> network, InputReader *input)
{

    for (int k=0; k<81; k++)// run for every non answered digit in the input
    {
        if (input->currentInput[k]!='0')
        {
            continue;
        }

        input->currentCheck=k;

        NNTrainer tester;
        tester=tester.findCorrectAnswer(tester, input);// get the correct answer for the current number
        tester.fowardProp(network[0]);// run the inputs through the first hidden layer
        network[0]->normalizeValue(network[0]);

        for (int i=1; i<int(network.size()); i++)// repeat above for the remaindeing layers
        {
            network[i]->input=network[i-1]->output;
            tester.fowardProp(network[i]);
            network[i]->normalizeValue(network[i]);
        }
// find the value chosen

        network[0]->input[k]=double (tester.numCheck)+1;// set the number to the correct answer
        tester.backProp(network, tester);// run through the back prop function
        for (int i=0; i<int (network.size()); i++)// delete all the unneeded data
        {
            network[i]->deleteValues(network[i]);
        }
    }
}



void NNTrainer::backProp (vector<NeuralNetwork*> network, NNTrainer training)
{
    double errorTemp=0;// total of the errors
    dvec placeHolder;
    dvec delta;
    double sum=0;
    double currentSum=0;
    // find the new weights for the output layer
    for (int i=0; i<int (network[3]->output.size()); i++)
    {
        delta.clear();
        if (network[3]->pastDelta.size()<1)// if this the first back prop, set the past delta to zero
        {
            placeHolder.clear();
            for (int count = 0; count< int (network[3]->input.size()); count++)
            {
                placeHolder.push_back(0);
            }// as the past delta is a vector of vectors, it needs to have a value for each spot
            for  (int count= 0; count <int (network[3]->output.size()); count++)
            {
                network[3]->pastDelta.push_back(placeHolder);
            }

        }
        errorTemp=training.findError(network[3]->output[i], training.corretAnswer[i]);//find the cost for each output
        network[3]->errorCost.push_back(errorTemp);
        errorTemp = 0;
        placeHolder = network[3]->pastDelta[i];
        for (int j=0; j<int (network[3]->input.size()); j++)// find the delta values of the weights
        {
            // find the delta weights and then update the according weight by adding them together
            errorTemp = training.learningRate * network[3]->input[j] * network[3]->errorCost[i] + (training.momentum * placeHolder[j]);
            network[3]->weights[i][j]= training.updateWeights(network[3]->weights[i][j], errorTemp);
            sum+=network[3]->weights[i][j]*network[3]->errorCost[i];// this will be used for the next layer
            delta.push_back(errorTemp);
        }
        network[3]->deltaWeights.push_back(delta);// put them into a vector
    }
    network[3]->pastDelta=network[3]->deltaWeights;
    // repeat above but instead for the other hidden layers
    for (int i=2; i>=0; i--)
    {
        if (network[i]->pastDelta.size()==0)// if this the first back prop, set the past delta to zero
        {
            placeHolder.clear();
            for (int count = 0; count< int (network[i]->input.size()); count++)
            {
                placeHolder.push_back(0);
            }
            for  (int count= 0; count <int (network[i]->output.size()); count++)
                network[i]->pastDelta.push_back(placeHolder);
        }

        for (int j=0; j<int (network[i]->output.size()); j++)
        {
            delta.clear();
            placeHolder = network[i]->pastDelta[j];
            errorTemp = training.findDeltaHidden(network[i]->output[j], sum);// find the error using the sum of the last layer
            network[i]->errorCost.push_back(errorTemp);

            for (int k=0; k<int (network[i]->input.size()); k++)
            {
                // update the delta and weights
                errorTemp=(training.learningRate * network[i]->input[k] * network[i]->errorCost[j]) + (placeHolder[k] * training.momentum);
                network[i]->weights[j][k]= training.updateWeights(network[i]->weights[j][k], errorTemp);
                delta.push_back(errorTemp);
                currentSum+=network[i]->weights[j][k]*network[i]->errorCost[j];
            }
            network[i]->deltaWeights.push_back(delta);
        }
        network[i]->pastDelta=network[i]->deltaWeights;
        sum=currentSum;// set the sum to the new sum value
        currentSum=0;
    }

}

double NNTrainer::findDeltaHidden (double output, double outputErrorCost)
{

    return (output * (1-output) * outputErrorCost);// multiply the error by the derivative
}


double NNTrainer::findError (double output, double desiredOutput)
{

    return output * (1-output) * ((desiredOutput)-output);// multiply the error by the derivative
}


double NNTrainer::updateWeights (double weight, double delta)
{
    return (weight+delta);// find the new weight value
}



NNTrainer NNTrainer::findCorrectAnswer(NNTrainer trainer, InputReader *input)
{
    trainer.numCheck=input->currentAnswer[input->currentCheck]-'1';// set the correct value for later
    for (int i=0; i<9; i++)
    {
        if (i==trainer.numCheck-1)
        {
            trainer.corretAnswer[i]=1;
        }
        else
            trainer.corretAnswer[i]=0;
    }



    return trainer;

}


