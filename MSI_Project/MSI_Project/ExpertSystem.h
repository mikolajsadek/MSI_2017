#pragma once

#include <array>
#include <functional>
#include <map>

#include "Node.h"
constexpr int kQuestionsCount = 18;
constexpr int kAnswersCount = 13;

class ExpertSystem {
public:
    ExpertSystem();
    void Run();
    bool ifRepeat();
private:
    void Greet();
    void Setup();
    void AskQuestions();
    void CalculateRecommendations();
    void DFS(Node& node);
    void ShowRecommendations();

    void SetChildrenWeights(const std::string& which, double val);
    std::function<int(int)> TripleLogic;
    int TripleLogicA(int in);
    int TripleLogicB(int in);
	std::function<double(double, double)> Choice;

    std::array<QuestionNode, kQuestionsCount> Questions;
    std::array<AnswerNode, kAnswersCount> Answers;

    // maps from names describing nodes to their indices
    std::map<std::string, int> QIndex;
    std::map<std::string, int> AIndex;

    // decision tree
    std::array<Node, kQuestionsCount + 1> Tree;

};