#pragma once

#include <string>
#include <map>
#include <vector>
#include <cmath>
#include <iostream>

class reinforce
{
public:
	struct state
	{
		std::string name;
		bool isTerminal;
		float value;
		std::pair<int, int> pos;
		std::vector<std::pair<int,int>> reachableStates;

		state (const std::string &n, const bool &term, const float &v, const std::pair<int, int> &p) :
		name(n), isTerminal(term), value(v), pos(p) {}

		state (std::string n) : name(n) {}

		state () {}

		std::string tostr() { return name + ": " + std::to_string((int)value);}
	};

	struct action
	{
		std::string name;
	};

	std::map<std::pair<int, int>, state> states;
	std::map<std::pair<int, int>, state> prevStates;
	std::vector<action> actions;

	float discount;

	// std::vector<state> visitedStates;

	// calculates reward of action in current state
	static float reward(const state &s, const state &prev/*, const action &a*/)
	{
		if (s.name != "s3") return s.value;

		// std::cout << "s3\n";
		if (prev.name == "s2")
			return 50.0f;
		else
			return 100.0f;
	}

	state transition(const state &s, const action &a)
	{
		auto newPos = s.pos;

		if (a.name == "up")
		{
			newPos.second += 1;
		}
		else if (a.name == "down")
		{
			newPos.second -= 1;
		}
		else if (a.name == "left")
		{
			newPos.first -= 1;
		}
		else if (a.name == "right")
		{
			newPos.first += 1;
		}

		return states[newPos];
		// reward();
		// visitedStates.push_back(nextState);
		

		state wrongstate("wrong");
		return wrongstate;
	}

	void learn(state &s, const float &dc)
	{
		if (s.isTerminal) return;
		// std::cout << "Node: " <<name<<std::endl;
		for(auto &nei : s.reachableStates)
		{
			float transitionReward = reinforce::reward(states[nei], s) * dc;
			// cout << nei.name << " " << transitionReward << std::endl
			// std::cout << "Reward for " << name << " to " << nei.name << " " << transitionReward << std::endl;
			s.value = std::max(s.value, transitionReward);
		}
		std::cout << s.value << " ";
	}

	bool statesChanged()
	{
		float theta = 0.1f;
		for (int y = 0; y < 2; ++y)
		{
			for (int x = 0; x < 3; ++x)
			{
				if(states[std::make_pair(x, y)].value - prevStates[std::make_pair(x, y)].value > theta) return true;
			}
		}

		return false;
	}

	float learn()
	{
		prevStates = states;
		int itter = 0;
		while (itter == 0 || statesChanged())
		{
			std::cout << "Itteration " << itter << std::endl;
			for (int y = 0; y < 2; ++y)
			{
				for (int x = 0; x < 3; ++x)
				{
					prevStates[std::make_pair(x, y)] = states[std::make_pair(x, y)]; 
					learn(states[std::make_pair(x, y)], (itter == 0 ? 1 : discount));
				}
				std::cout << "\n";
			}
			++itter;
			// std::cout << prevStates[std::make_pair(0,1)].value << " " << states[std::make_pair(0,1)].value<<std::endl;
			std::cout << "\n\n";
		}
		
	}

};