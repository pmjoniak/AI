#include "ACStoveAgent.h"

ACStoveAgent::ACStoveAgent(Controller* controller)
	:Agent(controller)
{
	state = 11;
	gamma[1][EventType::TooHot] = 3;
	gamma[1][EventType::SummerStarted] = 2;
	gamma[1][EventType::WindowOpened] = 6;

	gamma[2][EventType::TooHot] = 4;
	gamma[2][EventType::WinterStarted] = 1;
	gamma[2][EventType::WindowOpened] = 5;

	gamma[3][EventType::TooCold] = 1;
	gamma[3][EventType::SummerStarted] = 4;
	gamma[3][EventType::WindowOpened] = 6;

	gamma[4][EventType::TooCold] = 2;
	gamma[4][EventType::WinterStarted] = 3;
	gamma[4][EventType::WindowOpened] = 5;

	gamma[5][EventType::TooHot] = 10;
	gamma[5][EventType::WinterStarted] = 6;
	gamma[5][EventType::WindowClosed] = 7;

	gamma[6][EventType::TooHot] = 9;
	gamma[6][EventType::TooCold] = 8;
	gamma[6][EventType::WindowClosed] = 11;

	gamma[7][EventType::TooHot] = 4;
	gamma[7][EventType::TooCold] = 2;
	gamma[7][EventType::WinterStarted] = 11;	
	gamma[7][EventType::WindowOpened] = 5;
	
	gamma[8][EventType::TooHot] = 6;
	gamma[8][EventType::SummerStarted] = 12;
	gamma[8][EventType::WindowClosed] = 1;
	
	gamma[9][EventType::TooCold] = 1;
	gamma[9][EventType::SummerStarted] = 10;
	gamma[9][EventType::WindowClosed] = 3;

	gamma[10][EventType::TooCold] = 5;
	gamma[10][EventType::WinterStarted] = 9;
	gamma[10][EventType::WindowClosed] = 4;

	gamma[11][EventType::TooHot] = 3;
	gamma[11][EventType::TooCold] = 1;
	gamma[11][EventType::SummerStarted] = 7;
	gamma[11][EventType::WindowOpened] = 6;

	gamma[12][EventType::TooHot] = 5;
	gamma[12][EventType::WinterStarted] = 8;
	gamma[12][EventType::WindowClosed] = 2;

	int stove_on[] = { 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1 };
	int ac_on[] =    { 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0 };

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
