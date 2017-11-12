#include "Node.h"

#include <iomanip>
#include <iostream>
#include <string>

QuestionNode::QuestionNode(std::string node_question, bool yesno) : 
    question(node_question),
    answer(-1),
    isYesNo(yesno) {}

void QuestionNode::Ask() {
    std::cout << question << std::endl;
    int min = 0;
    int max = isYesNo ? 1 : 10;
    std::string temp;
    int val = -1;
    while(true) {
        std::cin >> temp;
        try {
            val = std::stoi(temp);
        }
        catch(...) {
            std::cout << "Niezrozumia³a odpowiedŸ, spróbuj jeszcze raz." << std::endl;
            continue;
        }
        if ((val < min) || (max < val)) {
            std::cout << "Podaj odpowiedŸ z w zakresie " << min << "-" << max << "." << std::endl;
            continue;
        }
        break;
    }
    answer = val;
}

AnswerNode::AnswerNode(std::string node_answer) :
    answer(node_answer),
    value(0),
    final(false) {}

void AnswerNode::Show() {
    std::cout << std::setw(12) << answer << "\t" << value << std::endl;
}

Node::Node(std::string node_name, std::string left_name, std::string right_name, std::string mid_name) :
    name(node_name), left(left_name), right(right_name), mid(mid_name) {}

Node::Node(std::string node_name, std::string left_name, std::string right_name) :
    Node(node_name, left_name, right_name, "") {}
