#include "ACStoveAgent.h"

ACStoveAgent::ACStoveAgent(Controller* controller)
	:Agent(controller)
{
	state = 1;

	gamma[1][EventType::TooCold] = 2;
	gamma[1][EventType::TooHot] = 3;
	gamma[1][EventType::Comfort] = 4;

	gamma[2][EventType::TooHot] = 1;
	gamma[2][EventType::Comfort] = 6;

	gamma[3][EventType::TooCold] = 1;
	gamma[3][EventType::Comfort] = 5;

	gamma[4][EventType::OutOfComfort] = 1;

	gamma[5][EventType::TooCold] = 4;
	gamma[6][EventType::TooHot] = 4;

	int stove_on[] = { 0, 1, 0, 0, 0, 1 };
	int ac_on[] = { 0, 0, 1, 0, 1, 0 };

	for (auto& entry1 : gamma)
	{
		int s1 = entry1.first;
		for (auto& entry2 : entry1.second)
		{
			int s2 = entry2.second;
			bool stove_on1 = stove_on[s1 - 1];
			bool stove_on2 = stove_on[s2 - 1];
			if (stove_on1 != stove_on2)
			{
				if (stove_on1)
					generated_events[s1][s2].push_back(EventType::StoveOff);
				else
					generated_events[s1][s2].push_back(EventType::StoveOn);
			}
			bool ac_on1 = ac_on[s1 - 1];
			bool ac_on2 = ac_on[s2 - 1];
			if (ac_on1 != ac_on2)
			{
				if (ac_on1)
					generated_events[s1][s2].push_back(EventType::ACOff);
				else
					generated_events[s1][s2].push_back(EventType::ACOn);
			}
		}
	}
}
