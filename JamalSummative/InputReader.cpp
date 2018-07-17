// includes
#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <sstream>
// include other files
#include "NetwokMaker.h"
#include "NNTraining.h"
#include "InputReader.h"

using namespace std;




void InputReader::getCurrentInput(InputReader *reader)
{
    // find the current board for training
    string currentGame;
    svec games;
    currentGame = reader->allInput[0]; //read in the first line which contains the input and expected output
    reader->allInput.erase(reader->allInput.begin());// delete that from the list
    currentGame.erase(currentGame.size());// delete the return at the start of the input
    while (true)
    {
        if (currentGame[0]==',')// if the current letter is a comma, it is the end of the input, so exit
        {
            currentGame.erase(currentGame.begin() );
            break;
        }
        reader->currentInput+=currentGame[0];// add the current char to the string
        currentGame.erase(currentGame.begin());// delete that char from the original string
    }
    reader->currentAnswer=currentGame;// the rest is the answer

}


void InputReader::readAllInput(InputReader *input)
{
    string str;
    ifstream infile;
    //open the file
    infile.open("Sudoku.txt", ifstream::in);
    if (!infile.is_open())
    {
        cout << "Could not open file" << endl;// if the file cannot be opened, return nothing
        return;
    }


    while (infile.good() && input->allInput.size()<300000)// read in the first 3000000 cases instead of 1 million to save time
    {
        getline (infile, str);
        input->allInput.push_back(str);
    }
    input->allInput.erase(input->allInput.begin());
    infile.close();

}



void InputReader::deletePastInput (InputReader *reader)
{
    // delete the past input values
    reader->currentInput.clear();
    reader->currentAnswer.clear();
}






