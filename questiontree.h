/* Nikita Lilichenko
 * HW 5 - Tiles
 * Section Leader - Eli Echt-Wilson
 * This is the header file for the program 20 Questions. This file establishes the QuestionTree class
 * by defining all of the relevant public variables that wil be used in the class during the progam.
 * It also defines several private member variables that will be used to initialize the QuestionTree,
 * as well as many recursive helper functions for publicly defined functions.
 */

#ifndef _questiontree_h
#define _questiontree_h

#include <iostream>
#include <string>
#include "questionnode.h"
#include "ui.h"

class QuestionTree {
public:
    QuestionTree();
    ~QuestionTree();
    void answerRange(int& minDepth, int& maxDepth) const;
    int countAnswers() const;
    int countQuestions() const;
    int gamesLost() const;
    int gamesWon() const;
    void mergeData(UI& ui, std::istream& input);
    bool playGame(UI& ui);
    void readData(std::istream& input);
    void writeData(std::ostream& output);

private:
    // root, gamesWonCounter, and gamesLostCounter are all defined here as private member variables
    QuestionNode* root;
    int gamesWonCounter;
    int gamesLostCounter;
    // helper functions are defined below to serve as recursive helpers for public functions
    void destructNode(QuestionNode* node);
    bool playGameHelper(UI& ui, QuestionNode*& node);
    int countAnswersHelper(QuestionNode* root) const;
    int countQuestionsHelper(QuestionNode* root) const;
    void answerRangeHelper(QuestionNode* node, int& minDepth, int& maxDepth, int currentDepth) const;
    void readDataHelper(std::istream& input, QuestionNode* node);
    void writeDataHelper(std::ostream& output, QuestionNode* node) const;
    // clear() function used to free allocated memory for a tree and all of its nodes in several functions
    void clear();
    // function checks if passed in node is a leaf
    bool isLeaf(QuestionNode* node) const;

};

#endif // _questiontree_h
