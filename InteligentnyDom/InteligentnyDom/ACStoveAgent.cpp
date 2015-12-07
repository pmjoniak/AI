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

	for (auto& entry1 : gamma)
	{
		int s1 = entry1.first;
		for (auto& entry2 : entry1.second)
		{
			int s2 = entry2.second;
			bool shutter_up1 = s1 & 2 == 0;
			bool shutter_up2 = s1 & 2 == 0;
			if (shutter_up1 != shutter_up2)
			{
				if (shutter_up1)
					generated_events[s1][s2] = EventType::CloseShutters;
				else
					generated_events[s1][s2] = EventType::OpenShutters;
			}
		}
	}
}
