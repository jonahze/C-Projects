/*
 * Jonah Zamora Epstein
 * CS106B
 * Section Leader: Kensen Shi
 * This program generates word ladders by taking one word, and creating new words by changing
 * one letter at a time with the restriction that they must be valid words. It checks this by first
 * reading in a dictionary and checking all words against that, then taking two words from the user
 * and using a breadth search with a queue of stacks to find the ladder. This works by changing one
 * letter at a time, checking if it is a word, and creating its own stack.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "lexicon.h"
#include "simpio.h"
#include "queue.h"
#include "filelib.h"

using namespace std;

//prototypes
static void welcomeMessage();
static bool createLexicon(Lexicon& dictionary, string& fileName);
static bool wordsAreLegal(string& word1, string& word2, Lexicon& dictionary);
static bool getWords(string& word1, string& word2, Lexicon& dictionary);
static void giveLadder(string& word1, string& word2, Lexicon& dictionary);
static Stack<string> findLadder(Queue<Stack<string>>& queue, string& word2, Lexicon& usedWords, Lexicon& dictionary);
static void printLadder(Stack<string>& ladder, string& word2, string& word1);
static void playWordLadder(Lexicon& dictionary);

int main() {
    welcomeMessage();
    string fileName = getLine("Dictionary file name? ");
    Lexicon dictionary;
    while (!createLexicon(dictionary, fileName)) {
        cout << "Unable to open that file. Try again" << endl;
        fileName = getLine("Dictionary file name? ");
    }
    playWordLadder(dictionary);
    cout << "Have a nice day." << endl;
    return 0;
}

/*This function prints the welcome message and gives the user info about the program*/
static void welcomeMessage() {
    cout << "Welcome to CS106B Word Ladder" << endl;
    cout << "Please give me two English words, and I will change the first into" << endl;
    cout << "the second by changing one letter at a time." << endl;
    cout << endl;
}

/*this function creates the dictionary lexicon from the given filename and returns a boolean based
 * on its success*/
static bool createLexicon(Lexicon& dictionary, string& fileName) {
    try { dictionary.addWordsFromFile(fileName); }
    catch (...) { return false; }
    return true;
}

/*this function gets the two words that the user wishes to enter and returns a boolean to the user
 * based on wether or not the user enters a blank value and therefore wishes to exit the game*/
static bool getWords(string& word1, string& word2, Lexicon& dictionary) {
    word1 = getLine("Word #1 (or Enter to quit): ");
    if (word1 == "") {
        return false;
    }
    word2 = getLine("Word #2 (or Enter to quit): ");
    if (word2 == "") {
        return false;
    }
    while (!wordsAreLegal(word1, word2, dictionary)) {
        cout << endl;
        word1 = getLine("Word #1 (or Enter to quit): ");
        if (word1 == "") {
            return false;
        }
        word2 = getLine("Word #2 (or Enter to quit): ");
        if (word2 == "") {
            return false;
        }
    }
    return true;
}

/*this function is the overall wrapper function for the word ladder game, it asks for
 * two words from the user and uses them to find a connection between the two*/
static void playWordLadder(Lexicon& dictionary) {
    while(true) {
        cout << endl;
        string word1, word2;
        bool prepareWordLadder = getWords(word1, word2, dictionary);
        if(!prepareWordLadder) {
            break;
        }
        giveLadder(word1, word2, dictionary);
    }
}

/*this function checks to see whether the words given by the user are correct english words, are
 * the ssme length, and are different. It also makes the words lowercae, and returns a boolean
 * based on the outcome and prints out a message for the user*/
static bool wordsAreLegal(string& word1, string& word2, Lexicon& dictionary) {
    toLowerCase(word1);
    toLowerCase(word2);
    if (!dictionary.contains(word1) || !dictionary.contains(word2)) {
        cout << "The two words must be found in the dictionary" << endl;
        return false;
    }
    if (word1.size() != word2.size()) {
        cout << "The two words must be the same length" << endl;
        return false;
    }
    if (word1 == word2) {
        cout << "The two words must be different" << endl;
        return false;
    }
    return true;
}

/*this function serves as the wrapper function for finding and printing the word ladder
 * it initializes the que and stack and the used words lexicon, and contains two seperate
 * functions, one to find the ladder, and one to print it to the user*/
static void giveLadder(string& word1, string& word2, Lexicon& dictionary) {
    string result;
    Queue<Stack<string>> queue;
    Stack<string> first;
    first.push(word1);
    queue.enqueue(first);
    Lexicon usedWords;
    usedWords.add(word1);
    Stack<string> ladder = findLadder(queue, word2, usedWords, dictionary);
    printLadder(ladder, word2, word1);
}

/*this function finds the correct stack which corresponds the correct word ladder and returns it to the user. It does
 * this by using a queue of stacks, and starting off with the word at the top of one stack. It then pops that word and
 * goes through with two for loops and changes every letter and cycles through all 26 letters. Each time it finds a correct
 * english word, it makes a copy of the stack with that word pushed on top.*/
static Stack<string> findLadder(Queue<Stack<string>>& queue, string& word2, Lexicon& usedWords, Lexicon& dictionary) {
    Stack<string> currentStack;
    Stack<string> newStack;
    string currentWord;
    while (queue.size() > 0) {
        currentStack = queue.dequeue();
        string topWord = currentStack.peek();
        for (int i = 0; i < (int)topWord.size(); i++) {
            for (char ch = 'a'; ch < 'z'; ch++) {
                currentWord = topWord;
                currentWord[i] = ch;
                if (dictionary.contains(currentWord) && !usedWords.contains(currentWord)) {
                    newStack = currentStack;
                    newStack.push(currentWord);
                    queue.enqueue(newStack);
                    usedWords.add(currentWord);
                    //this is so that it stops when it reaches its target word
                    if (currentWord == word2) {
                        return newStack;
                    }
                }
            }
        }
    }
    //this is the stack that will be returned if it never finds the target word
    return newStack;
}

/*this is the function the prints out the final word ladder if the correct one was found,
 * or informs the user that there is no correct word ladder*/
static void printLadder(Stack<string>& ladder, string& word2, string& word1) {
    //this is to check that the correct ladder was given
    if (ladder.size() == 0) {
        cout << "No possible ladder" << endl;
        return;
    }
    //this also checks that the correct ladder was given
    if (ladder.peek() != word2) {
        cout << "No possible ladder" << endl;
        return;
    }
    cout << "A ladder from " << word2 << " back to " << word1 << endl;
    cout << ladder.pop();
    while (ladder.size() > 0) {
        cout << " - " << ladder.pop();
    }
    cout << endl;
}

