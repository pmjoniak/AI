#include "Agent.h"
#include "Controller.h"

Agent::Agent(Controller* controller)
	: controller(controller)
{

}

void Agent::receiveEvent(EventType event_name)
{
	if (gamma.count(state) && gamma[state].count(event_name))
	{
		int new_state = gamma[state][event_name];
		if (generated_events.count(state) && generated_events[state].count(new_state))
			for (auto e : generated_events[state][new_state])
				controller->receiveEvent(e);
		state = new_state;
	}
}