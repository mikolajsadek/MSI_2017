#pragma once

#include "ExpertInterface.h"

#include <functional>
#include <tuple>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <unordered_map>

class RoughSetExpert : public ExpertInterface {
public:
    RoughSetExpert();
    void Prepare();
    void Run() override;
private:
    // tuple is a node consisting of text, index, and maxval
    // (maxval is uchar, we assume minval for all questions to be 0)
    std::vector<std::tuple<std::string, int, char>> Input;
    std::vector<std::tuple<std::string, int, char>> Questions;
    std::set<std::string> Decisions;
    std::vector<std::vector<int>> Table;
    std::vector<std::vector<std::set<int>>> IndistinguishableMatrix;
    std::set<std::vector<int>> Inconsistencies;
	// tuple consists of a decision and a map with questions and answers
	std::vector<std::tuple<std::unordered_map<int, char>, std::string>> Rules;
	std::unordered_map<int, char> Answers;
    int Na, Nq, Nd;
    bool verbose;

    bool ReadTable();
    void GetMaxVals();
    void FillDecisions();
    void Setup();

    void FindInconsistencies();
    std::function<void(void)> GetRidOfInconsistency;
    void Generalize();
    void DeleteLowerScore();
    void RemoveDuplicates();
	void BuildIndistinguishableMatrix();
    void FindRules();

    void Greet();
    void AskAndShowAnswer();

    int LowerBound(const std::string&);
    int UpperBound(const std::string&);
};