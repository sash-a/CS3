#include "reinforce.h"

using namespace std;

typedef reinforce::state state;

int main(int argc, char const *argv[])
{
	reinforce rf;
	rf.discount = 0.8f;

	pair<int, int> pos = make_pair<int, int> (0,0);

	// 0,0
	rf.states[pos] = state("s1", false, 0.0f, pos);

	pos.first += 1; // 1,0
	rf.states[pos] = state("s2", false, 0.0f, pos);

	pos.first += 1; // 2,0
    rf.states[pos] = state("s3", true, 0.0f, pos);

    pos.first = 0; pos.second = 1; // 0,1
  	rf.states[pos] = state("s4", false, 0.0f, pos);

  	pos.first += 1; // 1,1
	rf.states[pos] = state("s5", false, 0.0f, pos);

	pos.first += 1; // 1,2
	rf.states[pos] = state("s6", false, 0.0f, pos);


	// Adding reachable states
	for (int x = 0; x < 3; ++x)
	{
		for (int y = 0; y < 2; ++y)
		{
			vector<std::pair<int,int>> neighbours;
			if(rf.states[make_pair(x + 1, y)].name != "")
				neighbours.push_back(make_pair(x + 1, y));
			
			if(rf.states[make_pair(x - 1, y)].name != "")
				neighbours.push_back(make_pair(x - 1, y));
			
			if(rf.states[make_pair(x, y + 1)].name != "")
				neighbours.push_back(make_pair(x, y + 1));
			
			if(rf.states[make_pair(x, y - 1)].name != "")
				neighbours.push_back(make_pair(x, y - 1));

			rf.states[make_pair(x, y)].reachableStates = neighbours;
		}
	}

	rf.learn();
	

	return 0;
}