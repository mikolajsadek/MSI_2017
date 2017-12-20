#pragma once

#include "ExpertInterface.h"

#include <functional>
#include <tuple>
#include <string>
#include <map>
#include <vector>
#include <set>

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
    std::vector<std::string> Decisions;
    std::vector<std::vector<int>> Table;
    std::vector<std::vector<int>> IndistinguishableMatrix;
    std::set<std::vector<int>> Inconsistencies;
    std::vector<bool> Core;
    std::set<std::set<int>> Reducts;
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
    void FindCore();
    void FindReducts();

    void Greet();
    void Ask();
    void ComputeResult();
    void ShowResult();

    int LowerBound(const std::string&);
    int UpperBound(const std::string&);
};