#pragma once

#include <string>

struct QuestionNode {
public:
    QuestionNode(std::string node_question, bool yesno);
    void Ask();
    int GetAnswer() { return answer; }
    const bool isYesNo;
private:
    std::string question;
    int answer;
};

struct AnswerNode {
public:
    AnswerNode(std::string node_answer);
    void Show();
    double GetValue() { return value; }
    void IncValueBy(double val) { value += val; }
private:
    std::string answer;
    double value;
    bool final;
};

struct Node {
public:
    Node(std::string node_name, std::string left_name, std::string right_name, std::string mid_name);
    Node(std::string node_name, std::string left_name, std::string right_name);

    double GetWeight() { return weight; }
    void SetWeight(double newval) { weight = newval; }

    const std::string name;
    const std::string left;
    const std::string mid;
    const std::string right;
private:
    double weight = 1;
};
