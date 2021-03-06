#pragma once
#include "Events.h"
#include "Symulation.h"
#include <vector>
#include <memory>

class Agent;

class Controller
{
private:
	std::vector<std::unique_ptr<Agent>> agents;
	Symulation& symulation;

	float td = 20;
	float td_hist = 2;
	float tconf_hist = 4;
	float time_speed = 3600;


	bool last_rain_state;
	bool last_window_state;
	bool last_summer_state;
	bool last_day_state;
	bool last_sun_state;

	void sendEvent(EventType event_name);

public:
	Controller(Symulation& symulation);
	~Controller();

	void update(double delta_time);

	void receiveEvent(EventType event_name);

public:
	void forceRain(bool state);
	void forceSun(bool state);

	void setTimeSpeed(float speed);
	float getTimeSpeed();
	void setTd(float td);
	float getTd();
	void setTempOut(float out);
	float getTempOut();
};