
// (c) Jamal Nur
// includes
#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
// include other files
#include "NetwokMaker.h"
#include "NetwokMaker.cpp"
#include "NNTraining.h"
#include "NNTraining.cpp"
#include "InputReader.h"
#include "InputReader.cpp"
/*
********************************
This code does not give the optimal result due to a problem with the back prop functions
********************************
*/
using namespace std;
typedef vector <double> dvec;
typedef vector <dvec> ddvec;
typedef vector <string> svec;


int main()
{
    int count=5;
    InputReader *reader;
    reader = new InputReader;
    reader->readAllInput(reader);
    NeuralNetwork *nnPointer;// make a pointer to pass through the functions
    NNTrainer netTrainer;// make the netTrainerer
    netTrainer.momentum=1;
    netTrainer.learningRate=1;
    vector <NeuralNetwork*>  point;
    NeuralNetwork *temp;
    vector <NeuralNetwork> network;

    network= network[0].makeNNTrain(reader);
// ask if user wants to run program or netTrainering
    while (count!=1 && count!=0)
    {
        cout<<"Do you want to run training (this doesn't work) 1 for yes 0 for no"<<endl;
        cin>>count;
    }
    for (int i=0; i<int (network.size()); i++)// make the vector of pointers of NN point at the actual NN
    {
        temp = & network[i];

        point.push_back(temp);
    }

    if (count==0)// if the user doesn't want to run netTrainering get the board to be tested
    {
        while (reader->currentInput.length()!=81)
        {
            cout<<"Please enter the sudoku board, check the sudoku file for an example"<<endl;
            cin>>reader->currentInput;// make that board the input
        }
        network[0].runRealNN(point, reader);// run it through the NN to find the output
        return 0;// end the program
    }
    do
    {
        cout<<"How many training sets do you want to run through (max 300,000)"<<endl;
        cin>>count;
    }
    while (count>300000);
// otherwise go through netTrainering
    for (int end=1; end<count; end++)// set the max number of training sets
    {
        reader->getCurrentInput(reader);// get the current input
        nnPointer = &network[0];
        network[0].getFirstInput(nnPointer,reader->currentInput);// turn the input into doubles, within


        if (reader->allInput.size()<=1)// if there are no more training sets left, exit training
            break;

        netTrainer.runNN(point, reader);// run forward and back prop
        for (int i=0; i<int (network.size()); i++)
        {
            nnPointer=&network[i];// delete the inputs
            reader->deletePastInput(reader);
            network[i].input.clear();

        }

    }
    for (int i=0; i<int (network.size()); i++)// save the new weights
    {
        nnPointer = & network[i];
        network[i].saveWeights(nnPointer);
    }
    while (reader->currentInput.length()!=81)// find value with updated weights
    {
        cout<<"Please enter the sudoku board, check the sudoku file for an example"<<endl;
        cin>>reader->currentInput;// make that board the input
    }
    network[0].runRealNN(point, reader);// run it through the NN to find the output
    return 0;// end the program
}







