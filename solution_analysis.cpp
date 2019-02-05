/*
 * The goal of this code is to run through superpermutations that have been previously generated, and represent it in
 * terms of cycles. I'm going to focus on n = 5, because it is shorter than n = 6 but also has superpermutations that
 * do not fit the palindromic pattern for the superpermutations of n < 5.
 */

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>

using Permutation = std::vector<int>;
using Cycle = std::set<Permutation>;

struct Transition {
  Permutation startPermutation;
  int startCycle = 0;
  Permutation endPermutation;
  int endCycle = 0;
  int weight = 0;
};

class SolutionAnalysis {
  public:
  std::vector<int> solution;
  int n;
  std::vector<Cycle> cycles;
  std::vector<Transition> transitions;
  SolutionAnalysis(std::vector<int> solution, int n);

  private:

  //Gives the cycle number for a given permutation
  std::map<Permutation, int> permutationCycleLookup;

  void generateCycles();
  void generateTransitions();
};

SolutionAnalysis::SolutionAnalysis(std::vector<int> solution, int n){
  this->solution = std::move(solution);
  this->n = n;
  generateCycles();
  generateTransitions();
}

int factorial(int n){
  int result = 0;
  if(n == 0){
    result = 1;
  }
  else if(n > 0){
    result = n * factorial(n - 1);
  }
  return result;
}

void SolutionAnalysis::generateCycles(){
  Permutation cycleStart(n);
  for(int i = 0; i < n; i++){
    cycleStart[i] = i + 1;
  }
  cycles.resize(factorial(n - 1));
  for(int i = 0; i < cycles.size(); i++){
    cycles[i].insert(cycleStart);
    Permutation p = cycleStart;
    permutationCycleLookup.emplace(cycleStart, i + 1);
    for(int j = 0; j < n - 1; j++){
      p.push_back(p.front());
      p.erase(p.begin());
      cycles[i].insert(p);
      permutationCycleLookup.emplace(p, i + 1);
    }
    for(int j = 2; j <= n; j++){
      std::vector<int> A(p.end() - n + j, p.end());
      std::vector<int> B(p.end() - n, p.end() - n + j);
      std::sort(B.begin(), B.end());
      do {
        Permutation AB = A;
        AB.insert(AB.end(), B.begin(), B.end());
        if(permutationCycleLookup.count(AB) == 0){
          cycleStart = AB;
        }
      } while (std::next_permutation(B.begin(), B.end()));
    }
  }
}

void SolutionAnalysis::generateTransitions(){
  std::vector<int> bufferA(solution.begin(), solution.begin() + n);
  std::vector<int> bufferB = bufferA;
  transitions.reserve(solution.size() - n);
  int weight = 0;
  for(int i = n; i < solution.size(); i++){
    bufferB.push_back(solution[i]);
    bufferB.erase(bufferB.begin());
    weight++;
    if(std::set<int>(bufferB.begin(), bufferB.end()).size() == bufferB.size()){
      Transition t;
      t.startPermutation = bufferA;
      t.startCycle = permutationCycleLookup[bufferA];
      t.endPermutation = bufferB;
      t.endCycle = permutationCycleLookup[bufferB];
      t.weight = weight;
      transitions.push_back(t);
      bufferA = bufferB;
      weight = 0;
    }
  }
  transitions.shrink_to_fit();
}

std::vector<int> toInts(std::string str){
  std::vector<int> ints;
  ints.reserve(str.size());
  for(char c : str){
    ints.push_back(c - '0');
  }
  return ints;
}

std::string toStr(std::vector<int> ints){
  std::string str;
  str.reserve(ints.size());
  for(int i : ints){
    str.push_back(i + '0');
  }
  return str;
}

std::string formatCycle(Cycle cycle){
  std::string result = "[ ";
  for(const Permutation& p : cycle){
    result += toStr(p) + " ";
  }
  result += "]";
  return result;
}

std::string formatTransition(Transition transition){
  std::string result;
  result += toStr(transition.startPermutation);
  result += " -> ";
  result += toStr(transition.endPermutation);
  result += ",   cycle ";
  result += std::to_string(transition.startCycle);
  result += " -> cycle ";
  result += std::to_string(transition.endCycle);
  if(transition.startCycle < 10 && transition.endCycle < 10){
    result += ",  ";
  }
  else if(transition.startCycle >= 10 && transition.endCycle >= 10){
    result += ",";
  }
  else {
    result += ", ";
  }
  result += "   weight = ";
  result += std::to_string(transition.weight);
  return result;
}

int main(){
  std::string solution =
    "1234513241532413524132541324513425134521345123415234125341235"
    "4123145231425314235142315421354215342154321453214352143251432"
    "1542312453124351243152431254312";
  int n = 5;
  SolutionAnalysis analysis(toInts(solution), n);
  std::cout << "Cycles:" << std::endl;
  for(const auto& c : analysis.cycles){
    std::cout << formatCycle(c) << std::endl;
  }
  std::cout << std::endl << std::endl;
  std::cout << "Transitions:" << std::endl;
  for(const auto& t : analysis.transitions){
    std::cout << formatTransition(t) << std::endl;
  }
}
