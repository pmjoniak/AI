#pragma once
#include <vector>
#include <map>
#include <memory>
#include "Events.h"

class Controller;

class Agent
{
protected:
	std::map<int, std::map<EventType, int>> gamma;
	std::map<int, std::map<int, std::vector<EventType>>> generated_events;
	int state;
	Controller* controller;

public:
	class Agent(Controller* controller);

	void receiveEvent(EventType event_name);

};
