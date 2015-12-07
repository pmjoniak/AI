#include "WindowAgent.h"

WindowAgent::WindowAgent(Controller* controller)
	:Agent(controller)
{
	state = 1;

	gamma[1][EventType::TooCold] = 1;
	gamma[1][EventType::TooHot] = 3;
	gamma[1][EventType::RainStarted] = 5;
	gamma[1][EventType::SummerStarted] = 2;
	gamma[1][EventType::WinterStarted] = 1;

	gamma[2][EventType::TooCold] = 4;
	gamma[2][EventType::TooHot] = 2;
	gamma[2][EventType::RainStarted] = 6;
	gamma[2][EventType::SummerStarted] = 2;
	gamma[2][EventType::WinterStarted] = 1;

	gamma[3][EventType::TooCold] = 1;
	gamma[3][EventType::TooHot] = 3;
	gamma[3][EventType::RainStarted] = 5;
	gamma[3][EventType::SummerStarted] = 4;
	gamma[3][EventType::WinterStarted] = 3;

	gamma[4][EventType::TooCold] = 4;
	gamma[4][EventType::TooHot] = 2;
	gamma[4][EventType::RainStarted] = 6;
	gamma[4][EventType::SummerStarted] = 4;
	gamma[4][EventType::WinterStarted] = 3;


	gamma[5][EventType::TooCold] = 5;
	gamma[5][EventType::TooHot] = 5;
	gamma[5][EventType::RainEnded] = 1;
	gamma[5][EventType::SummerStarted] = 6;
	gamma[5][EventType::WinterStarted] = 5;

	gamma[6][EventType::TooCold] = 6;
	gamma[6][EventType::TooHot] = 6;
	gamma[6][EventType::RainEnded] = 2;
	gamma[6][EventType::SummerStarted] = 6;
	gamma[6][EventType::WinterStarted] = 5;

	for (auto& entry1 : gamma)
	{
		int s1 = entry1.first;
		for (auto& entry2 : entry1.second)
		{
			int s2 = entry2.second;
			bool window_open1 = ((s1 - 1) & 2);
			bool window_open2 = ((s2 - 1) & 2);
			if (window_open1 != window_open2)
			{
				if (window_open1)
					generated_events[s1][s2].push_back(EventType::CloseWindow);
				else	
					generated_events[s1][s2].push_back(EventType::OpenWindow);
			}
		}
	}

}