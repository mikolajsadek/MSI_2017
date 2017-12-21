#include "RoughSetExpert.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>

using node = std::tuple<std::string, int, char>;

std::set<int> FindReducts(std::vector<std::set<int>>& cellsToConsider);

RoughSetExpert::RoughSetExpert() : ExpertInterface{},
Input{}, Questions{}, Decisions{}, Table{}, IndistinguishableMatrix{},
Inconsistencies{}, Rules{}, Answers{}
{
	Setup();
	if (!ReadTable()) {
		throw std::exception("Invalid table");
	}
	GetMaxVals();
	FillDecisions();
	if (verbose) {
		std::cout << "Wczytywanie danych systemu eksperckiego zakoñczone.\n";
		std::cout << "System wczyta³ " << Nq << " pytañ, " << Nd << " odpowiedzi i " << Na << " konfiguracji pytañ i odpowiedzi.\n";
	}
}

void RoughSetExpert::Prepare()
{
	FindInconsistencies();
	if (Inconsistencies.size() != 0) {
		if (verbose)
			std::cout << "Wykryto niespójnoœci w danych, trwa usuwanie...\n";
		GetRidOfInconsistency();
	}
	RemoveDuplicates();
	if (verbose) {
		std::cout << "Usuniêto duplikaty danych, jeœli takie istnia³y.\n";
		std::cout << "Pozosta³o " << Na << " konfiguracji pytañ i odpowiedzi.\n";
	}
	BuildIndistinguishableMatrix();
	FindRules();
}

void RoughSetExpert::Run()
{
	Greet();
	AskAndShowAnswer();
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
		Input.emplace_back(std::make_tuple(val, a++, 0));
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
	for (auto& a : Input) {
		Decisions.insert(std::get<std::string>(a));
	}
	Nd = Decisions.size();
}

void RoughSetExpert::Setup()
{
	std::cout << "Wpisz 'v' aby program wyœwietla³ szczegó³owe informacje lub 'q', aby uzyskaæ jedynie wynik.\n";
	std::string v;
	std::cin >> v;
	while (v != "v" && v != "V" && v != "q" && v != "Q") {
		std::cout << "Niezrozumia³a odpowiedŸ, spróbuj jeszcze raz." << std::endl;
		std::cin >> v;
	}
	verbose = (v == "v" || v == "V");
	v = "";
	std::cout << "Wybierz metodê usuniêcia ewentualnych niespójnoœci z danych: 1 - metoda jakoœciowa, 2 - metoda uogólnionego argumentu\n";
	std::cin >> v;
	while (v != "1" && v != "2") {
		std::cout << "Niezrozumia³a odpowiedŸ, spróbuj jeszcze raz." << std::endl;
		std::cin >> v;
	}
	if (v == "1")
		GetRidOfInconsistency = [this] { this->DeleteLowerScore(); };
	else
		GetRidOfInconsistency = [this] { this->Generalize(); };
}

void RoughSetExpert::FindInconsistencies()
{
	for (int i = 0; i < Na; ++i) {
		std::vector<int> sameRows{ i };
		std::vector<int> duplicates{};
		if (std::find_if(Inconsistencies.begin(), Inconsistencies.end(), [i](auto& v) { return std::find(v.begin(), v.end(), i) != v.end(); }) != Inconsistencies.end())
			continue;
		for (int j = i + 1; j < Na; ++j) {
			bool same = true;
			for (int q = 0; q < Nq; ++q) {
				if (Table[q][i] != Table[q][j]) {
					same = false;
					break;
				}
			}
			if (same) {
				if (std::get<std::string>(Input[i]) != std::get<std::string>(Input[j]))
					sameRows.push_back(j);
				else
					duplicates.push_back(j);
			}
		}
		if (sameRows.size() > 1) {
			if (duplicates.size() > 0)
				sameRows.insert(sameRows.end(), duplicates.begin(), duplicates.end());
			Inconsistencies.emplace(std::move(sameRows));
		}
	}
}

void RoughSetExpert::Generalize()
{
	for (auto& vec : Inconsistencies) {
		std::string general;
		for (int i : vec) {
			if (general.size())
				general += " lub ";
			general += std::get<std::string>(Input[i]);
		}
		for (int i : vec) {
			std::get<std::string>(Input[i]) = general;
		}
	}
	Decisions.clear();
	FillDecisions();
}

void RoughSetExpert::DeleteLowerScore()
{
	for (auto& vec : Inconsistencies) {
		std::set<std::string> Different{};
		for (int i = 0; i < vec.size(); ++i)
			Different.insert(std::get<std::string>(Input[vec[i]]));
		int maxL = 0;
		int maxU = 0;
		std::string L, U, W;
		for (auto& x : Different) {
			int lower = LowerBound(x);
			int upper = UpperBound(x);
			if (lower > maxL) {
				maxL = lower;
				L = x;
			}
			if (upper > maxU) {
				maxU = upper;
				U = x;
			}
		}
		if (L == U)
			W = U;
		else
			W = maxL > maxU ? L : U;
		// remove from input and table those rows that are in this vec, but are not decided to be W
		// we will mark rows to delete by setting decision as empty string, and actually remove later
		for (int i = 0; i < vec.size(); ++i) {
			if (W == std::get<std::string>(Input[vec[i]]))
				continue;
			std::get<std::string>(Input[vec[i]]) = "";
		}
	}

	// now remove rows with empty strings
	for (int i = Na - 1; i >= 0; --i) {
		if (std::get<std::string>(Input[i]) != "")
			continue;
		Input.erase(Input.begin() + i);
		for (auto& col : Table)
			col.erase(col.begin() + i);
		--Na;
	}

	// now update indices
	for (int i = 0; i < Na; ++i)
		std::get<int>(Input[i]) = i;
}

void RoughSetExpert::RemoveDuplicates()
{
	for (int i = 0; i < Na; ++i) {
		std::vector<int> duplicates{};
		for (int j = i + 1; j < Na; ++j) {
			bool same = true;
			for (int q = 0; q < Nq; ++q) {
				if (Table[q][i] != Table[q][j]) {
					same = false;
					break;
				}
			}
			if (same) {
				if (std::get<std::string>(Input[i]) == std::get<std::string>(Input[j]))
					duplicates.push_back(j);
			}
		}
		for (int d = duplicates.size() - 1; d >= 0; --d) {
			Input.erase(Input.begin() + duplicates[d]);
			for (auto& col : Table)
				col.erase(col.begin() + duplicates[d]);
		}
		Na -= duplicates.size();
	}
	// now update indices
	for (int i = 0; i < Na; ++i)
		std::get<int>(Input[i]) = i;
}

void RoughSetExpert::BuildIndistinguishableMatrix()
{
	for (int i = 0; i < Na; ++i) {
		IndistinguishableMatrix.emplace_back(std::vector<std::set<int>>());
		for (int j = 0; j < i; ++j) {
			IndistinguishableMatrix[i].emplace_back(std::set<int>());
			for (int q = 0; q < Nq; ++q) {
				if (Table[q][i] != Table[q][j] && Table[q][i] >= 0 && Table[q][j] >= 0) {
					IndistinguishableMatrix[i][j].insert(q);
				}
			}
		}
	}
	if (verbose) {
		std::cout << "Macierz rozró¿nialnoœci (zawiera indeksy pytañ):" << std::endl;
		for (int i = 0; i < Na; ++i) {
			for (int j = 0; j < i; ++j) {
				//std::cout << "\t" << 
				std::cout << "[ ";
				for (auto const& q : IndistinguishableMatrix[i][j]) {
					std::cout << q << " ";
				}
				std::cout << "] ";
			}
			std::cout << std::endl;
		}
	}
}

void RoughSetExpert::FindRules()
{
	for (int i = 0; i < Na; ++i) {
		std::vector<std::set<int>> cells;
		for (int j = 0; j < i; ++j) {
			cells.push_back(IndistinguishableMatrix[i][j]);
		}
		for (int j = i + 1; j < Na; ++j) {
			cells.push_back(IndistinguishableMatrix[j][i]);
		}
		std::set<int> reducts;
		while (true) {
			auto& singleAttr = std::find_if(cells.begin(), cells.end(), [](auto& c) { return c.size() == 1; });
			if (singleAttr == cells.end())
				break;
			int attr = *singleAttr->begin();
			reducts.insert(attr);
			while (true) {
				auto& it = std::find_if(cells.begin(), cells.end(), [attr](auto& c) { return c.find(attr) != c.end(); });
				if (it == cells.end())
					break;
				cells.erase(it);
			}
		}
		auto& otherReducts = FindReducts(cells);
		reducts.insert(otherReducts.begin(), otherReducts.end());
		std::string conclusion = std::get<0>(Input[i]);
		std::unordered_map<int, char> conditions;
		for (auto& a : reducts) {
			conditions.insert({ a, Table[a][i] });
		}
		Rules.emplace_back(std::make_tuple(conditions, conclusion));

		if (verbose) {
			std::cout << "Znaleziono najmniejszy redukt dla rekordu " << i << ": [ " << std::endl;
			for (auto& a : reducts) {
				std::cout << "\"" << std::get<0>(Questions[a]) << "\" ";
			}
			std::cout << "]" << std::endl;
			std::cout << "Wygenerowana regu³a: " << std::endl;
			std::cout << "IF" << std::endl;
			for (auto& c : conditions) {
				std::cout << "\t\"" << std::get<0>(Questions[c.first]) << "\" = " << (int)c.second << std::endl;
			}
			std::cout << "THEN" << std::endl;
			std::cout << "\tAktywnoœæ = \"" << conclusion << "\"" << std::endl << std::endl;
		}
	}
}

std::set<int> FindReducts(std::vector<std::set<int>>& cellsToConsider) {
	if (cellsToConsider.empty())
		return std::set<int>();

	std::set<int> attrToConsider;
	for (const auto& c : cellsToConsider) {
		attrToConsider.insert(c.begin(), c.end());
	}

	std::set<int> smallestReduct;
	int attribute = -1;
	for (auto const& a : attrToConsider) {
		std::vector<std::set<int>> cells;
		for (auto const& c : cellsToConsider) {
			if (std::find(c.begin(), c.end(), a) == c.end()) {
				cells.emplace_back(c);
			}
		}
		auto reducts = FindReducts(cells);
		if (smallestReduct.empty() || reducts.size() < smallestReduct.size()) {
			smallestReduct = reducts;
			attribute = a;
			if (reducts.empty())
				break;
		}
	}
	if (attribute >= 0)
		smallestReduct.insert(attribute);
	return smallestReduct;
}

void RoughSetExpert::Greet()
{
	std::cout << "Witamy w systemie doradczym wyboru aktywnoœci." << std::endl;
	std::cout << "Odpowiedz na pytania zgodnie ze wskazówkami przy pytaniach." << std::endl;
}

void RoughSetExpert::AskAndShowAnswer()
{
	std::string decision;
	for (auto& rule : Rules) {
		bool inconsistentAnswer = false;
		for (auto& cond : std::get<0>(rule)) {
			int q = std::get<0>(cond);
			char a = std::get<1>(cond);
			char answer;
			auto answerFound = Answers.find(q);
			if (answerFound != Answers.end()) {
				answer = answerFound->second;
			}
			else {
				std::cout << std::get<0>(Questions[q]);
				if (std::get<2>(Questions[q]) == 1) {
					std::cout << " (0 - nie; 1 - tak)" << std::endl;
				}
				bool read = false;
				int v;
				while (!read) {
					try {
						std::string val;
						std::cin >> val;
						v = std::stoi(val);
						read = true;
					}
					catch (...) {
						read = false;
					}
					if (!read || v < 0 || v > std::get<2>(Questions[q])) {
						std::cout << "Niezrozumia³a odpowiedŸ, spróbuj jeszcze raz." << std::endl;
						read = false;
					}
				}
				answer = (char)v;
				Answers.insert({ q, answer });
			}
			if (answer != a) {
				inconsistentAnswer = true;
				break;
			}
		}
		if (!inconsistentAnswer) {
			decision = std::get<1>(rule);
			break;
		}
	}
	std::cout << "Podjêta decyzja: " << decision << std::endl;
}

int RoughSetExpert::LowerBound(const std::string& x)
{
	int count = 0;
	for (auto& row : Input) {
		if (std::get<std::string>(row) != x)
			continue;
		int i = std::get<int>(row);
		if (std::find_if(Inconsistencies.begin(), Inconsistencies.end(), [i](auto& v) { return std::find(v.begin(), v.end(), i) != v.end(); }) != Inconsistencies.end())
			continue;
		++count;
	}
	return count;
}

int RoughSetExpert::UpperBound(const std::string& x)
{
	int count = 0;
	for (auto& row : Input) {
		if (std::get<std::string>(row) != x)
			continue;
		int i = std::get<int>(row);
		auto vi = std::find_if(Inconsistencies.begin(), Inconsistencies.end(), [i](auto& v) { return std::find(v.begin(), v.end(), i) != v.end(); });
		if (vi != Inconsistencies.end())
			count += vi->size();
		else;
		++count;
	}
	return count;
}
