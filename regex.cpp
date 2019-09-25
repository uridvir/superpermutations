#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <set>

struct NFA {
    //The first layer is a list of states
    //Each state has letters
    //Each letter has transitions to otther states, or is "empty"
    std::vector<std::vector<std::vector<int> > > transitions;

    std::vector<int> acceptStates;
};

//Helper functions
void addCycle(NFA* nfa, int n);
void addPermutation(NFA* nfa, std::vector<int> p, int n);
void printNFA(const NFA& nfa);

//Constructs an NFA that accepts if the string contains a permutation
NFA constructNFA(int n){
    std::vector<int> permutation(n);
    for (int i = 0; i < n; i++){
        permutation[i] = i; //Permutations start with 0, not 1
    }

    NFA nfa;
    nfa.transitions.push_back(std::vector<std::vector<int> >(n)); //Add state 0
    addCycle(&nfa, n);

    do {
        addPermutation(&nfa, permutation, n);
    } while(std::next_permutation(permutation.begin(), permutation.end()));
    return nfa;
}

//Adds a cycle to the last state on every digit
void addCycle(NFA* nfa, int n){
    for(int i = 0; i < n; i++){
        nfa->transitions.back()[i].push_back(nfa->transitions.size() - 1);
    }
}

//Adds each permutation to the NFA
void addPermutation(NFA* nfa, std::vector<int> p, int n){
    //Add a transition for state 0 on the first digit of p to the next state
    nfa->transitions[0][p[0]].push_back(nfa->transitions.size());

    for (int i = 1; i < n; i++){
        nfa->transitions.push_back(std::vector<std::vector<int> >(n)); //Add state
        nfa->transitions.back()[p[i]].push_back(nfa->transitions.size()); //Add transition on ith digit of p
        //Since the "letters" start at one and p is 0-indexed, subtract 1
    }

    nfa->transitions.push_back(std::vector<std::vector<int> >(n)); //Add accept state
    addCycle(nfa, n);
    nfa->acceptStates.push_back(nfa->transitions.size() - 1); //Record last state as accept state
}

struct DFA {
    //The first layer is a list of states
    //The second layer is a list of transitions to other states, indexed by letters
    std::vector<std::vector<int> > transitions;

    std::vector<int> acceptStates;
};

//Helper functions
std::set<int> move(const NFA& nfa, std::set<int> states, int letter);
void printDFA(const DFA& dfa);

// i: 0, j: 0

DFA constructDFA(const NFA& nfa, int n){
    DFA dfa;
    //e-closure is never called because there are no e-transitions
    std::vector<std::set<int> > unmarked = {std::set<int>({0})};
    //Maps a set of NFA states to its corresponding DFA state
    std::map<std::set<int>, int> stateMap = {{std::set<int>({0}), 0}};
    int i = 0;
    while (!unmarked.empty()){
        auto states = unmarked.front();
        unmarked.erase(unmarked.begin());
        dfa.transitions.push_back(std::vector<int>(n));
        for (int letter = 0; letter < n; letter++){
            auto newStates = move(nfa, states, letter);
            if (newStates.empty()){
                continue;
            }
            //If newStates has not been seen, add it to unmarked, stateMap, and the DFA
            if (stateMap.count(newStates) == 0){
                i++;
                unmarked.push_back(newStates);
                stateMap.emplace(newStates, i);
                dfa.transitions[stateMap.find(states)->second][letter] = i;
                //This is really important:
                //By setting the condition that any accept state in the DFA must correspond to all DFA accept states,
                //the regular language is changed. Instead of being accepting when one permutation is seen,
                //now the language only accepts when ALL permutations are seen (i.e. a superpermutation).
                if (std::includes(newStates.begin(), newStates.end(),
                 nfa.acceptStates.begin(), nfa.acceptStates.end())){
                    dfa.acceptStates.push_back(i);
                }
            }
            else {
                dfa.transitions[stateMap.find(states)->second][letter] = stateMap.find(newStates)->second;
            }
        }
    }
    return dfa;
}

//Gets the states reachable by transition on letter from states
std::set<int> move(const NFA& nfa, std::set<int> states, int letter){
    std::set<int> result;
    for (auto state : states){
        auto dest = nfa.transitions[state][letter];
        result.insert(dest.begin(), dest.end());
    }
    return result;
}

//Prints out the NFA for debugging purposes
void printNFA(const NFA& nfa){
    for (int i = 0; i < nfa.transitions.size(); i++){
        std::cout << "state " << i << ":\n";
        for (int j = 0; j < nfa.transitions[i].size(); j++){
            std::cout << "\tletter " << j + 1 << ":\n";
            for (int k = 0; k < nfa.transitions[i][j].size(); k++){
                std::cout << "\t\t-> state " << nfa.transitions[i][j][k] << "\n";
            }
        }
    }
    std::cout << "accept states: ";
    for (int i = 0; i < nfa.acceptStates.size(); i++){
        std::cout << nfa.acceptStates[i] << " ";
    }
    std::cout << "\n\n";
}

//Prints out the NFA for debugging purposes
void printDFA(const DFA& dfa){
    for (int i = 0; i < dfa.transitions.size(); i++){
        std::cout << "state " << i << ":\n";
        for (int j = 0; j < dfa.transitions[i].size(); j++){
            std::cout << "\tletter " << j + 1 << " -> state " << dfa.transitions[i][j] << "\n";
        }
    }
    std::cout << "accept states: ";
    for (int i = 0; i < dfa.acceptStates.size(); i++){
        std::cout << dfa.acceptStates[i] << " ";
    }
    std::cout << "\n";
}

void minimizeDFA(DFA* dfa){}

int main(){
    int n = 3;
    NFA nfa = constructNFA(n);
    DFA dfa = constructDFA(nfa, n);

    std::cout << "NFA:\n";
    printNFA(nfa);
    std::cout << "DFA:\n";
    printDFA(dfa);
}
