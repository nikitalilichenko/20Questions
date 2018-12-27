/* Nikita Lilichenko
 * HW 6 - 20 Questions
 * Section Leader - Eli Echt-Wilson
 * This program has several functions revolving around the functionality of a simple 20 Questions game.
 * The program can build a tree from scratch by repeatedly asking the player a series of yes or no
 * questions. The program is also able to create a tree by reading a file, output an existing tree, merge
 * exisitng trees, and print tree stats.
 * This is the main .cpp file for the QuestionTree class. It recieves the functions established in the
 * correspoding .h file and defines them here. It also passes in the private member recursive helper
 * functions and defines them here as well. This file also constructs and deconstructs the tree.
 */

#include "questiontree.h"
#include <climits>
#include "strlib.h"
#include "questionnode.h"
#include "istream"
using namespace std;

/* Constructor function: initializes values for the games won and games lost counters, as well as the root node
 * as an answer node with its data set to "computer"
 */
QuestionTree::QuestionTree() {
    gamesLostCounter = 0;
    gamesWonCounter = 0;
    root = new QuestionNode("computer", nullptr, nullptr);
}

/* Destructor function: calls the clear function
 */
QuestionTree::~QuestionTree() {
    clear();
}

/* function calls destructNode function to delete tree, then deletes the root. Used as a helper function
 * for several public functions, including the destructor and the readData function
 */
void QuestionTree::clear() {
    destructNode(root);
    delete root;
}

/* function serves as recursive helper function for clear(), starts at the bottom of the tree and deletes nodes
 * recursively if they are leaves.
 */
void QuestionTree::destructNode(QuestionNode* node){
    if (node->yes != nullptr) {
        destructNode(node->yes);
        delete node->yes;
    }
    if (node->no != nullptr) {
        destructNode(node->no);
        delete node->no;
    }
}

/* function counts the amount of answer nodes(leaves) are present in the tree. It calls its recursive helper
 * function, passing through the root node.
 */
int QuestionTree::countAnswers() const {
    return countAnswersHelper(root);
}

/* function recursively increments its counter variable everytime the node encountered is a leaf throughout the entire tree,
 * for yes and no nodes.
 */
int QuestionTree::countAnswersHelper(QuestionNode* node) const {
    if (isLeaf(node)) {
        return 1;
    }
    int counter = 0;
    counter += countAnswersHelper(node->yes);
    counter += countAnswersHelper(node->no);
    return counter;
}

/* function counts the amount of question nodes(non-leaves) are present in the tree. It calls its recursive helper
 * function, passing through the root node.
 */
int QuestionTree::countQuestions() const {
    return countQuestionsHelper(root);
}

/* function recursively increments its counter variable everytime the node encountered is not a leaf throughout
 * the entire tree, for both the yes and no nodes.
 */
int QuestionTree::countQuestionsHelper(QuestionNode* node) const {
    if (node->yes == nullptr || node->no == nullptr) {
        return 0;
    }
    int counter = 1;
    counter += countQuestionsHelper(node->yes);
    counter += countQuestionsHelper(node->no);
    return counter;
}

/* function returns the private member gamesLostCounter int variable, which is incrimented in the playGame function.
 * This function counts the games lost by the computer.
 */
int QuestionTree::gamesLost() const {
    return gamesLostCounter;
}

/* function returns the private member gamesWonCounter int variable, which is incrimented in the playGame function.
 * This function counts the games won by the computer.
 */
int QuestionTree::gamesWon() const {
    return gamesWonCounter;
}

/* function creates new QuestionTree and links it to an existing QuestionTree by creating a new root node that
 * combines the two trees.
 */
void QuestionTree::mergeData(UI& ui, std::istream& input) {
    /* function asks user for a question to create a new root node
     */
    string newRootQuestion = ui.readLine("Type a Y/N question to distinguish my data from this file:");
    if (newRootQuestion.find('?') == -1) {
        newRootQuestion = newRootQuestion + "?";
    }
    /* function creates new nodes for the new main tree root and the new file QuestionTree root, then the new tree
     * root yes and no pointers to either the new file root or the existing tree root based on what they answered for
     * the boolean newRootResult.
     */
    bool newRootResult = ui.readBoolean("And what is the answer for the new data?");
    QuestionNode* newTreeRoot = new QuestionNode(newRootQuestion);
    QuestionNode* newFileQuestionRoot = new QuestionNode;
    if (newRootResult) {
        newTreeRoot->yes = newFileQuestionRoot;
        newTreeRoot->no = root;
    }
    else {
        newTreeRoot->no = newFileQuestionRoot;
        newTreeRoot->yes = root;
    }
    /* function calls readDataHelper to create new root. The question node created at the top of the function is set
     * as the new tree root.
     */
    readDataHelper(input, newFileQuestionRoot);
    root = newTreeRoot;
}


/* function is used to cycle through existing binary tree, as well as generate new tree through questions asked to the
 * player. It calls its recursive helper function.
 */
bool QuestionTree::playGame(UI& ui) {
    return playGameHelper(ui, root);
}

/* function uses recursion to cycle through binary tree and to create new question/answer nodes when a computer loses a
 * game.
 */
bool QuestionTree::playGameHelper(UI& ui, QuestionNode*& node){
    if (isLeaf(node)) {
        /* if the encountered node is a leaf, the computer has reached an answer node, and the player is asked if the data
         * at the node is their object. If it is, the games won counter is incrimented and the function returns true.
         */
        bool answer = ui.readBoolean("Are you thinking of: " + node->data + "?");
        if (answer) {
            gamesWonCounter++;
            ui.print("Hooray, I win!");
            return true;
        }
        else {
            /* if the encountered node's answer is not the correct node, the function asks several follow up questions to the
             * player and collects their answers in strings or booleans.
             */
            gamesLostCounter++;
            string currentData = node->data;
            string actualAnswer = ui.readLine("Drat, I lost. What was your object?");
            string newQuestion = ui.readLine("Type a Y/N question to distinguish " + actualAnswer + " from " + currentData + ":");
            if (newQuestion.find('?') == -1) {
                newQuestion = newQuestion + "?";
            }
            bool newQuestionAnswer = ui.readBoolean("And what is the answer for " + actualAnswer + "?");
            /* function then creates new nodes for the new differentiating question, the actual answer, and the wrong answer that the
             * user denied as their object. The new question node is then has its yes and no pointers set to either the actual answer or
             * the wrong answer based on what they answered for the boolean newQuestionAnswer. The function then returns false.
             */
            QuestionNode* newQuestionNode = new QuestionNode(newQuestion);
            QuestionNode* actualAnswerNode = new QuestionNode(actualAnswer);
            QuestionNode* wrongAnswerNode = new QuestionNode(currentData);
            node = newQuestionNode;
            if (newQuestionAnswer) {
                node->yes = actualAnswerNode;
                node->no = wrongAnswerNode;
            }
            else {
                node->yes = wrongAnswerNode;
                node->no = actualAnswerNode;
            }
            return false;
        }
    }
    /* if the encountered node is not a leaf, the function asks the user if the answer to question node is
     * yes or no. If yes, the function calls itself and passes through the node that is pointed to by the yes
     * pointer of the current node. The same happens for no, but with the no pointer.
     */
    bool currentAnswer = ui.readBoolean(node->data);
    if (currentAnswer){
        return playGameHelper(ui, node->yes);
    }
    else{
        return playGameHelper(ui, node->no);
    }
}


/* function determines the height of both the shallowest and deepest answer nodes in the tree. the function calls
 * its helper recursive function, passing through the root node, the min and max depths, and 1 as the current depth.
 */
void QuestionTree::answerRange(int& minDepth, int& maxDepth) const {
    answerRangeHelper(root, minDepth, maxDepth, 1);
}

/* function is the recursive helper of the answerRange function
 */
void QuestionTree::answerRangeHelper(QuestionNode* node, int& minDepth, int& maxDepth, int currentDepth) const {
    /* if encountered node is a leaf, the min depth is set to the current depth only if the mindepth is 0 or if it is
     * greater than the current depth. The max depth is also set to current depth if the max depth is less than the current
     * depth.
     */
    if (isLeaf(node)) {
        if (minDepth == 0 || minDepth > currentDepth) {
            minDepth = currentDepth;
        }
        if (maxDepth < currentDepth) {
            maxDepth = currentDepth;
        }
        return;
    }
    /* if the node is not a leaf, the function calls itself with both yes and no pointer, incrementing the current depth
     * both times.
     */
    answerRangeHelper(node->yes, minDepth, maxDepth, currentDepth + 1);
    answerRangeHelper(node->no, minDepth, maxDepth, currentDepth + 1);
}

/* function reads a ceratin text file and puts the data into a binary tree. The function first calls the clear function
 * to clear any exisitng tree nodes, then creates a new root node and calls its recursive helper function.
 */
void QuestionTree::readData(std::istream& input) {
    clear();
    root = new QuestionNode;
    readDataHelper(input, root);
}

/* function serves as recursive helper for readData function
 */
void QuestionTree::readDataHelper(std::istream& input, QuestionNode* node){
    /* function reads the current line of the file, then trims it to exclude the identifier prefix and and sets the current
     * node data to the trimmed string. If the line contains an answer, the funciton returns.
     */
    string line;
    getline(input, line);
    string value = line.substr(2);
    node->data = value;
    if (line[0] == 'A') {
        return;
    }
    /* function creates new nodes for the yes and no pointers of the current node, then calls itself to continue cycling
     * through the tree
     */
    node->yes = new QuestionNode;
    readDataHelper(input, node->yes);
    node->no = new QuestionNode;
    readDataHelper(input, node->no);
}

/* function outputs data of a current tree into a text file. It calls its recursive helper function
 */
void QuestionTree::writeData(std::ostream& output) {
    writeDataHelper(output, root);
}

/* function serves as recursive helper of the writeData function
 */
void QuestionTree::writeDataHelper(std::ostream& output, QuestionNode* node) const {
    /* if the current node is a leaf, the function outputs the current node data with the
     * answer prefix. If the node is not a leaf, the function outputs the current node data with the
     * question prefix.
     */
    if (isLeaf(node)) {
        output << "A:" << node->data << endl;
        return;
    }
    else {
        output << "Q:" << node->data << endl;
    }
    /* function calls itself and passes through its yes and no pointers
     */
    writeDataHelper(output, node->yes);
    writeDataHelper(output, node->no);
}

/* function determines if passed through node is a leaf by checking if its yes and no pointers are
 * null pointers. Function was made for simplicity, and is called frequently in other functions.
 */
bool QuestionTree::isLeaf(QuestionNode* node) const {
    if (node->yes == nullptr && node->no == nullptr) {
        return true;
    }
    else{
        return false;
    }
}

