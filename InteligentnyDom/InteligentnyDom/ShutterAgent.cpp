#include "ShutterAgent.h"

ShutterAgent::ShutterAgent(Controller* controller)
	:Agent(controller)
{
	state = 1;
	gamma[1][EventType::DayStarted] = 3;
	gamma[2][EventType::NightStarted] = 1;
	gamma[2][EventType::SunStarted] = 4;
	gamma[3][EventType::NightStarted] = 1;
	gamma[3][EventType::SunStarted] = 5;
	gamma[4][EventType::NightStarted] = 1;
	gamma[4][EventType::SunEnded] = 2;
	gamma[4][EventType::TooHot] = 5;
	gamma[5][EventType::NightStarted] = 1;
	gamma[5][EventType::SunEnded] = 3;
	gamma[5][EventType::TooCold] = 4;

	for (auto& entry1 : gamma)
	{
		int s1 = entry1.first;
		for (auto& entry2 : entry1.second)
		{
			int s2 = entry2.second;
			bool shutter_up1 = s1 % 2 == 0;
			bool shutter_up2 = s2 % 2 == 0;
			if (shutter_up1 != shutter_up2)
			{
				if (shutter_up1)
					generated_events[s1][s2].push_back(EventType::CloseShutters);
				else
					generated_events[s1][s2].push_back(EventType::OpenShutters);
			}
		}
	}
}
