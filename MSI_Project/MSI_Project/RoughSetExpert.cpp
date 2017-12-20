#include "RoughSetExpert.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

using node = std::tuple<std::string, int, char>;

RoughSetExpert::RoughSetExpert() : ExpertInterface{}
{
    Answers = Questions = std::vector<node>();
    Table = std::vector<std::vector<int>>();
    if (!ReadTable()) {
        throw std::exception("Invalid table");
    }
    GetMaxVals();
    FillDecisions();
    FindReduct();
}

void RoughSetExpert::Run()
{
    Greet();
    Ask();
    ComputeResult();
    ShowResult();
}

bool RoughSetExpert::ReadTable()
{
    int q = 0;
    int a = 0;
    int c = 0;
    std::string tmp, val;
    std::ifstream fin("QnA.csv");
    // WARNING - there is no check for uniqueness of questions and answers, we
    // assume user provides valid data and only check if file format is correct
    getline(fin, tmp, ',');
    if (tmp != "\"QnA\"")
        return false;
    // now parse questions
    getline(fin, tmp);
    std::stringstream ss(tmp);
    while (getline(ss, val, ',')) {
        val = val.substr(1);
        val.pop_back();
        Questions.emplace_back(std::make_tuple(val, q++, 0));
        Table.emplace_back(std::vector<int>());
        // Data for given question and answer are stored in Table[q][a]
    }
    // now parse answers and values
    while (getline(fin, tmp)) {
        std::stringstream ss(tmp);
        getline(ss, val, ',');
        val = val.substr(1);
        val.pop_back();
        Answers.emplace_back(std::make_tuple(val, a++, 0));
        int qi = 0;
        while (getline(ss, val, ',')) {
            Table[qi].emplace_back(stoi(val));
            ++qi;
        }
        if (qi != q)
            return false;
    }
    Na = a;
    Nq = q;
    return true;
}

void RoughSetExpert::GetMaxVals()
{
    for (int i = 0; i < Nq; ++i) {
        auto& column = Table[i];
        auto max = std::max_element(column.begin(), column.end());
        std::get<2>(Questions[i]) = static_cast<unsigned char>(*max);
    }
}

void RoughSetExpert::FillDecisions()
{
    for (auto& a : Answers) {
        auto Text = std::get<std::string>(a);
        auto it = std::find(Decisions.begin(), Decisions.end(), Text);
        if (it != Decisions.end())
            continue;
        Decisions.push_back(Text);
        ++Nd;
    }
}

void RoughSetExpert::FindReduct()
{
}

void RoughSetExpert::Greet()
{
}

void RoughSetExpert::Ask()
{
}

void RoughSetExpert::ComputeResult()
{
}

void RoughSetExpert::ShowResult()
{
}
