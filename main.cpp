#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <algorithm>

using namespace std;
int patternCount = 0;

struct Node {
    bool isFinal;
    bool toRemove;
    unordered_map<char, Node *> nextNodes;
    int length;
    Node() : isFinal(false), toRemove(false), length(0) {

    }
};

string * readPatterns() {
    ifstream patterns("patterns.txt");
    patterns >> patternCount;
    string *sPatterns = new string[patternCount];
    string skip;
    std::getline(patterns, skip);
    for (int i = 0; i < patternCount; i++) {
        std::getline(patterns, sPatterns[i]);
    }
    return sPatterns;
}

string readText() {
    ifstream textFile("text.txt");
    string text;
    std::getline(textFile, text);
    return text;
}

bool keyExists(unordered_map<char, Node *> m, char key)
{
    return m.find(key) != m.end();
}

Node createStateMachine(string * s, int size) {
    Node root;
    Node * currentNode = &root;
    root.isFinal = false;
    for (int i = 0; i < size; i++) {
        string & pattern = s[i];
        currentNode = &root;
        for (int j = 0; j < pattern.length(); j++) {
            if (keyExists(currentNode->nextNodes, pattern[j])) {
                currentNode = currentNode->nextNodes[pattern[j]];
                if(j == pattern.length() - 1) {
                    currentNode->isFinal = true;
                }
            }
            else {
                Node *newNode = new Node();
                newNode->length = currentNode->length + 1;
                if(j == pattern.length() - 1) {
                   newNode->isFinal = true;
                }
                currentNode->nextNodes[pattern[j]] = newNode;
                currentNode = newNode;
            }
        }
    }
    return root;
}

void matchString(Node &machine, string text) {
    vector<Node *> states;
    for(int i=0; i < text.length(); i++) {
        machine.toRemove = false;
        states.push_back(&machine);
        int statesCurrentSize = states.size();

        for(int j=0; j < statesCurrentSize; j++) {
            if(keyExists(states[j]->nextNodes, text[i])) {
                Node *nextState = states[j]->nextNodes[text[i]];
                nextState->toRemove = false;
                if(nextState->isFinal) {
                    cout << "Found: \"" << text.substr(i - nextState->length + 1, nextState->length) << "\" at: " << i - nextState->length + 1 << endl;
                }
                states.push_back(nextState);
            }
            states[j]->toRemove = true;
        }
        states.erase(std::remove_if(states.begin(), states.end(), [](Node * obj) { return obj->toRemove; } ), states.end());
    }
}

int main() {
    string * patterns = readPatterns();
    string text = readText();
    Node root = createStateMachine(patterns, patternCount);
    matchString(root, text);
}