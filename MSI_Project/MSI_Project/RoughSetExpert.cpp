#include "RoughSetExpert.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

using node = std::tuple<std::string, int, unsigned char>;

RoughSetExpert::RoughSetExpert() : ExpertInterface{}
{
    Answers = Questions = std::vector<node>();
    Table = std::vector<std::vector<int>>();
    if (!ReadTable()) {
        throw std::exception("Invalid table");
    }
    GetMaxVals();
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
    std::string tmp, val;
    std::stringstream ss;
    std::ifstream fin("QnA.csv");
    // WARNING - there is no check for uniqueness of questions and answers, we
    // assume user provides valid data and only check if file format is correct
    getline(fin, tmp, ',');
    if (tmp != "\"QnA\"")
        return false;
    // now parse questions
    getline(fin, tmp);
    ss = std::move(std::stringstream(tmp));
    while (getline(ss, val, ',')) {
        val = val.substr(1);
        val.pop_back();
        Questions.emplace_back(node{ val, q++, 0 });
        Table.emplace_back(std::vector<int>());
        // Table[q][a]
    }
    // now parse answers and values
    while (getline(fin, tmp)) {
        ss = std::move(std::stringstream(tmp));
        getline(ss, val, ',');
        val = val.substr(1);
        val.pop_back();
        Answers.emplace_back(node{ val, a++, 0 });
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
