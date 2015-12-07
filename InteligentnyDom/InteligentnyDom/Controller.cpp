#include "Controller.h"
#include "WindowAgent.h"
#include "ShutterAgent.h"
#include "ACStoveAgent.h"
#include <iostream>

Controller::~Controller()
{
}

Controller::Controller(Symulation& symulation)
	:symulation(symulation)
{
	agents.push_back(std::make_unique<WindowAgent>(this));
	agents.push_back(std::make_unique<ShutterAgent>(this));
	agents.push_back(std::make_unique<ACStoveAgent>(this));

	last_window_state = symulation.isWindowOpen();
	last_rain_state = symulation.isRaining();
	last_summer_state = symulation.getOutsideTemerarure() > symulation.getInsideTemerature();
	last_day_state = symulation.isDay();
	last_sun_state = symulation.isSun();
}



void Controller::update(double delta_time)
{
	symulation.update(delta_time* time_speed);

	if (symulation.isWindowOpen() != last_window_state)
		if (symulation.isWindowOpen())
			sendEvent(EventType::WindowOpened);
		else
			sendEvent(EventType::WindowClosed);

	if (symulation.isRaining() != last_rain_state)
		if (symulation.isRaining())
			sendEvent(EventType::RainStarted);
		else
			sendEvent(EventType::RainEnded);

	if (symulation.isDay() != last_day_state)
		if (symulation.isDay())
			sendEvent(EventType::DayStarted);
		else
			sendEvent(EventType::NightStarted);

	if (symulation.isSun() != last_sun_state)
		if (symulation.isSun())
			sendEvent(EventType::SunStarted);
		else
			sendEvent(EventType::SunEnded);

	bool new_summer_state = symulation.getOutsideTemerarure() > symulation.getInsideTemerature();
	if (last_summer_state != new_summer_state)
		if (new_summer_state)
			sendEvent(EventType::SummerStarted);
		else
			sendEvent(EventType::WinterStarted);


	if (symulation.getInsideTemerature() > td + t_hist)
		sendEvent(EventType::TooHot);
	if (symulation.getInsideTemerature() < td - t_hist)
		sendEvent(EventType::TooCold);


	last_window_state = symulation.isWindowOpen();
	last_rain_state = symulation.isRaining();
	last_summer_state = symulation.getOutsideTemerarure() > symulation.getInsideTemerature();
	last_day_state = symulation.isDay();
	last_sun_state = symulation.isSun();

	if (!symulation.isCooling() && !symulation.isHeating())
	{
		std::cout << "cookie!";
	}
}

void Controller::setTimeSpeed(float speed)
{
	time_speed = speed;
}

float Controller::getTimeSpeed()
{
	return time_speed;
}

void Controller::forceRain(bool state)
{
	symulation.forceRain(state);
}

void Controller::forceSun(bool state)
{
	symulation.forceSun(state);
}

void Controller::sendEvent(EventType event_name)
{
	for (auto& agent : agents)
		agent->receiveEvent(event_name);
}

void Controller::receiveEvent(EventType event_name)
{
	if (event_name == EventType::CloseWindow)
		symulation.closeWindow();
	if (event_name == EventType::OpenWindow)
		symulation.openWindow();
	if (event_name == EventType::CloseShutters)
		symulation.closeShutters();
	if (event_name == EventType::OpenShutters)
		symulation.openShutters();
	if (event_name == EventType::ACOn)
		symulation.ACOn();
	if (event_name == EventType::ACOff)
		symulation.ACOff();
	if (event_name == EventType::StoveOn)
		symulation.stoveOn();
	if (event_name == EventType::StoveOff)
		symulation.stoveOff();
}

void Controller::setTd(float td)
{
	this->td = td;
}

float Controller::getTd()
{
	return td;
}

void Controller::setTempOut(float out)
{
	return symulation.forceTempOut(out);
}

float Controller::getTempOut()
{
	return symulation.getOutsideTemerarure();
}
