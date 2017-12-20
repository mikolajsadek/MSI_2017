#pragma once

#include "ExpertInterface.h"

#include <tuple>
#include <string>
#include <map>
#include <vector>

class RoughSetExpert : public ExpertInterface {
public:
    RoughSetExpert();
    void Run() override;
private:
    // tuple is a node consisting of text, index, and maxval
    // (maxval is uchar, we assume minval for all questions to be 0)
    std::vector<std::tuple<std::string, int, char>> Answers;
    std::vector<std::tuple<std::string, int, char>> Questions;
    std::vector<std::string> Decisions;
    std::vector<std::vector<int>> Table;
    int Na, Nq, Nd;

    bool ReadTable();
    void GetMaxVals();
    void FillDecisions();
    void FindReduct();
    void Greet();
    void Ask();
    void ComputeResult();
    void ShowResult();
};