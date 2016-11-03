/*
 * Jonah Zamora Epstein
 * This program created randomnly generated text in the style of a given file. It does this
 * by ordering the file into a hashmap with "n - 1" words as the key and then the likely words
 * to follow as the value. Then it generates a given number of words by randomnly selected a
 * key and then following the chain by randomnly selected one of the values and using that
 * to create a new key
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "random.h"
#include "vector.h"
#include "hashmap.h"

using namespace std;

//prototypes
static void introduction();
static HashMap<Vector<string>, Vector<string>> readInFile(int& n);
static void getFileAndN(string& fileName, int& n);
static void generateText(HashMap<Vector<string>, Vector<string>>& ngrams, int& n);

int main() {
    introduction();
    int n;
    HashMap<Vector<string>, Vector<string>> ngrams = readInFile(n);
    generateText(ngrams, n);
    cout << "Exiting." << endl;
    return 0;
}

/*this function introdcues the user to the progrm and gives them information about it*/
static void introduction() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups of words, and I'll create " << endl;
    cout << "random text for you." << endl;
    cout << endl;
}

/*this functions asks the user for a filename, checks to see that it exists, and the creats the
 * n-gram hashmap with this file. It does this by also asking the user for an integer greater than 2
 * and using that to define the n - 1 number of words in the key*/
static HashMap<Vector<string>, Vector<string>> readInFile(int& n) {
    HashMap<Vector<string>, Vector<string>> ngrams;
    string fileName;
    getFileAndN(fileName, n);
    ifstream input;
    openFile(input, fileName);
    string word;
    Vector<string> window;
    //this creates the beginning window, it assumes that the text file has enough words for 1 window
    for (int i = 0; i < n - 1; i++) {
        input >> word;
        window.add(word);
    }
    while (true) {
        input >> word;
        if (input.fail()) {
            break;
        }
        Vector<string> values = ngrams[window];
        values.add(word);
        ngrams[window] = values;
        window.add(word);
        Vector<string> newWindow;
        for (int i = 0; i < window.size() - 1; i ++) {
            newWindow.add(window[i+1]);
        }
        window = newWindow;
    }
    //open the file again so that you can wrap the map around to the beginning
    ifstream repeated;
    openFile(repeated, fileName);
    for (int i = 0; i < n - 1; i++) {
        repeated >> word;
        Vector<string> values = ngrams[window];
        values.add(word);
        ngrams[window] = values;
        window.add(word);
        Vector<string> newWindow;
        for (int i = 0; i < window.size() - 1; i ++) {
            newWindow.add(window[i+1]);
        }
        window = newWindow;
    }
    return ngrams;
}

/*this function gets the file name from the user and checks to make sure it exists,
 * along with the n value*/
static void getFileAndN(string& fileName, int& n) {
    fileName = getLine("Input File? ");
    while (!fileExists(fileName)) {
        cout << "file does not exist" << endl;
        fileName = getLine("Input File? ");
    }
    n = getInteger("Value of n? ");
    while (n < 3) {
        cout << "n must be an integer greater than 2" << endl;
        n = getInteger("Value of n? ");
    }
    cout << endl;
}

/*this function generates the text from the given ngrams hashmap. It asks the user for the
 * number of words it wants in the text and then uses a random number generate and a vector
 * of all the keys to get the starting key. From there it prints the starting key and then uses it
 * to find the next word, where it then creates a new key and repeats the process until it gets
 * the correct amount of words*/
static void generateText(HashMap<Vector<string>, Vector<string>>& ngrams, int& n) {
    while (true) {
        int numWords = getInteger("# of random words to generate (0 to quit)? ");
        if (numWords == 0) {
            return;
        }
        Vector<Vector<string>> prefixes = ngrams.keys();
        Vector<string> key = prefixes[randomInteger(0, prefixes.size() - 1)];
        string startWords;
        for (int i = 0; i < n - 1; i++) {
            startWords += key[i];
            startWords += " ";
        }
        cout << "... " << startWords;
        //n + 1 to take into account the key used at the beginning
        for (int i = 0; i < numWords - n + 1; i++) {
            Vector<string> values = ngrams[key];
            string wordToAdd = values[randomInteger(0, values.size() - 1)];
            key.add(wordToAdd);
            Vector<string> newKey;
            for (int i = 0; i < key.size() - 1; i ++) {
                newKey.add(key[i+1]);
            }
            key = newKey;
            wordToAdd += " ";
            cout << wordToAdd;
        }
        cout << "..." << endl;
        cout << endl;
    }
}
