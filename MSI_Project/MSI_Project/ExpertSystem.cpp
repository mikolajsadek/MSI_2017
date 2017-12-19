#include "ExpertSystem.h"

#include <algorithm>
#include <string>
#include <iostream>

ExpertSystem::ExpertSystem() : ExpertInterface{},
    Questions{{
        { "Oceñ jakoœæ powietrza.", false },
        { "Czy masz kolegów do gry?", true },
        { "Czy masz wiêcej ni¿ 3 kolegów do gry?", true },
        { "Na ile lubisz gry kontaktowe?", false },
        { "Na ile lubisz p³ywaæ?", false },
        { "Na ile blisko jest basen?", false },
        { "Na ile blisko jest si³ownia?", false },
        { "Na ile mocno pada?", false },
        { "Oceñ temperaturê (0 - b. zimno, 10 - b. ciep³o).", false },
        { "Oceñ na ile jest s³onecznie.", false },
        { "Na ile blisko jest pla¿a?", false },
        { "Jak mocno wieje?", false },
        { "Oceñ wilgotnoœæ powietrza.", false },
        { "Na ile lubisz biegaæ?", false },
        { "Na ile lubisz jazdê rowerem?", false },
        { "Na ile lubisz jazdê na rolkach?", false },
        { "Na ile jest œlisko?", false },
        { "Na ile blisko jest najbli¿szy stok?", false }
    }},
    Answers{
        "Dom",
        "Spacer",
        "£y¿wy",
        "Narty",
        "Rolki",
        "Rower",
        "Bieganie",
        "Pla¿a",
        "Si³ownia",
        "Basen",
        "Tenis sto³owy",
        "Koszykówka",
        "Siatkówka",
    },
    QIndex{
        { "AirQuality", 0 },
        { "Friends", 1 },
        { "ManyFriends", 2 },
        { "Contact", 3 },
        { "Swimming", 4 },
        { "SwimmingPool", 5 },
        { "Gym", 6 },
        { "Rain", 7 },
        { "Hot", 8 },
        { "Sunny", 9 },
        { "Beach", 10 },
        { "Wind", 11 },
        { "Wind2", 11 },
        { "Humidity", 12 },
        { "Running", 13 },
        { "Biking", 14 },
        { "Skating", 15 },
        { "Slippery", 16 },
        { "Slope", 17 }
    },
    AIndex{
        { "Dom", 0 },
        { "Spacer", 1 },
        { "£y¿wy", 2 },
        { "Narty", 3 },
        { "Rolki", 4 },
        { "Rower", 5 },
        { "Bieganie", 6 },
        { "Pla¿a", 7 },
        { "Si³ownia", 8 },
        { "Basen", 9 },
        { "Tenis sto³owy", 10 },
        { "Koszykówka", 11 },
        { "Siatkówka", 12 }
    },
    Tree{{
        { "AirQuality", "Friends", "Rain" },
        { "Friends", "Swimming", "ManyFriends" },
        { "Swimming", "Gym", "SwimmingPool" },
        { "Gym", "Dom", "Si³ownia" },
        { "SwimmingPool", "Gym", "Basen" },
        { "ManyFriends", "Tenis sto³owy", "Contact" },
        { "Contact", "Siatkówka", "Koszykówka" },
        { "Rain", "Hot", "Dom" },
        { "Hot", "Wind", "Sunny", "Wind2" },
        { "Wind", "Slippery", "Humidity" },
        { "Slippery", "Spacer", "Slope" },
        { "Slope", "£y¿wy", "Narty" },
        { "Humidity", "Spacer", "Dom" },
        { "Sunny", "Dom", "Beach" },
        { "Beach", "Dom", "Pla¿a" },
        { "Wind2", "Running", "Humidity" },
        { "Running", "Biking", "Bieganie" },
        { "Biking", "Skating", "Rower" },
        { "Skating", "Spacer", "Rolki" }
        }}
{}

void ExpertSystem::Run()
{
    Greet();
    //AskQuestions();
    CalculateRecommendations();
    ShowRecommendations();
}

void ExpertSystem::Greet() {
    std::cout << "Witamy w systemie doradczym wyboru aktywnoœci." << std::endl;
    Setup();
    std::cout << "Na pytania zaczynaj¹ce siê od \"czy\" odpowiedz 0 (nie) lub 1 (tak)." << std::endl;
    std::cout << "W przypadku pozosta³ych pytañ dokonaj oceny w skali 0-10." << std::endl;
}

void ExpertSystem::Setup()
{
    std::cout << "Wybierz sposób rozmycia logiki 3-wartoœciowej (A lub B, ró¿nice opisane w instrukcji)." << std::endl;
    std::string temp;
    unsigned char fuzz = 0;
    while (true) {
        std::cin >> temp;
        if (temp.size() != 1) {
            std::cout << "Niezrozumia³a odpowiedŸ, spróbuj jeszcze raz." << std::endl;
            continue;
        }
        fuzz = temp[0];
        if ((fuzz != 'a') && (fuzz != 'A') && (fuzz != 'b') && (fuzz != 'B')) {
            std::cout << "Niezrozumia³a odpowiedŸ, spróbuj jeszcze raz." << std::endl;
            continue;
        }
        break;
    }
    switch (fuzz) {
        case 'a':
        case 'A':
            TripleLogic = [this](int v) { return this->TripleLogicA(v); };
            break;
        case 'b':
        case 'B':
            TripleLogic = [this](int v) { return this->TripleLogicB(v); };
            break;
    }
	std::cout << "Wybierz sposób dzia³ania systemu (0 - mno¿enie, 1 - minimum)." << std::endl;
	while (true) {
		std::cin >> temp;
		if (temp.size() != 1) {
			std::cout << "Niezrozumia³a odpowiedŸ, spróbuj jeszcze raz." << std::endl;
			continue;
		}
		fuzz = temp[0];
		if ((fuzz != '0') && (fuzz != '1')) {
			std::cout << "Niezrozumia³a odpowiedŸ, spróbuj jeszcze raz." << std::endl;
			continue;
		}
		break;
	}
	switch (fuzz) {
	case '0':
		Choice = [this](double a, double b) { return a * b; };
		break;
	case '1':
		Choice = [this](double a, double b) { return a < b ? a : b; };
		break;
	}
}

void ExpertSystem::AskQuestions() {
    for (auto& question : Questions) {
        question.Ask();
    }
}

void ExpertSystem::CalculateRecommendations() {
    auto& node = Tree[0];
    DFS(Tree[0]);
}

void ExpertSystem::DFS(Node & node)
{
    QuestionNode& qn = Questions[QIndex[node.name]];
	if (!qn.asked)
		qn.Ask();
    int left, mid, right = qn.GetAnswer();
    if (qn.isYesNo) {
        left = right ? 0 : 1;
        SetChildrenWeights(node.left, Choice(node.GetWeight(), left));
        auto it = std::find_if(Tree.begin(), Tree.end(), [&node](auto n) {return n.name == node.left;});
        if (it != Tree.end() && left)
            DFS(*it);
        SetChildrenWeights(node.right, Choice(node.GetWeight(), right));
        it = std::find_if(Tree.begin(), Tree.end(), [&node](auto n) {return n.name == node.right;});
        if (it != Tree.end() && right)
            DFS(*it);
    }
    else if (node.mid == "") {
        left = 10 - right;
        SetChildrenWeights(node.left, Choice(node.GetWeight(), left * 1.0 / 10));
        auto it = std::find_if(Tree.begin(), Tree.end(), [&node](auto n) {return n.name == node.left;});
        if (it != Tree.end() && left != 0)
            DFS(*it);
        SetChildrenWeights(node.right, Choice(node.GetWeight(), right * 1.0 / 10));
        it = std::find_if(Tree.begin(), Tree.end(), [&node](auto n) {return n.name == node.right;});
        if (it != Tree.end() && right != 0)
            DFS(*it);
    }
    else {
        left = 10 - right;
        left = TripleLogic(left);
        right = TripleLogic(right);
        mid = 10 - right - left;

        SetChildrenWeights(node.left, Choice(node.GetWeight(), left * 1.0 / 10));
        auto it = std::find_if(Tree.begin(), Tree.end(), [&node](auto n) {return n.name == node.left;});
        if (it != Tree.end() && left != 0)
            DFS(*it);
        SetChildrenWeights(node.right, Choice(node.GetWeight(), right * 1.0 / 10));
        it = std::find_if(Tree.begin(), Tree.end(), [&node](auto n) {return n.name == node.right;});
        if (it != Tree.end() && right != 0)
            DFS(*it);
        SetChildrenWeights(node.mid, Choice(node.GetWeight(), mid * 1.0 / 10));
        it = std::find_if(Tree.begin(), Tree.end(), [&node](auto n) {return n.name == node.mid;});
        if (it != Tree.end() && mid != 0)
            DFS(*it);
    }
}

void ExpertSystem::ShowRecommendations() {
    std::sort(Answers.begin(), Answers.end(), [](auto a, auto b) {
        return b.GetValue() < a.GetValue();
    });
	double norm = 0;
	for (auto n : Answers)
		norm += n.GetValue();
    for (auto& answer : Answers)
        answer.Show(norm);
}

void ExpertSystem::SetChildrenWeights(const std::string& which, double val) {
    auto it = std::find_if(Tree.begin(), Tree.end(), [&which](auto& n) {return n.name == which;});
    if (it != Tree.end())
        it->SetWeight(val);
    else
        Answers[AIndex[which]].IncValueBy(val);
}

int ExpertSystem::TripleLogicA(int in)
{
    if (in <= 5) return 0;
    switch (in) {
        case 6:
            return 2;
        case 7:
            return 6;
        default:
            return in;
    }
}

int ExpertSystem::TripleLogicB(int in)
{
    switch (in) {
    case 0:
    case 1:
        return 0;
    case 2:
    case 3:
        return 1;
    case 4:
    case 5:
        return 2;
    case 6:
        return 3;
    case 7:
        return 5;
    case 8:
        return 7;
    default:
        return in;
    }
}