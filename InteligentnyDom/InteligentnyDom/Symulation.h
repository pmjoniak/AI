#pragma once

class Symulation
{
private:
	float t_in = 20;
	float t_out = -10;
	bool heating = false;
	bool cooling = false;
	bool day = true;
	bool raining = false;
	bool window_opened = false;
	bool shutters_opened = false;
	bool sun = false;

	double time = 0;

	double since_raining_start = 0;
	double since_sun_start = 0;

public:
	float getInsideTemerature();
	float getOutsideTemerarure();
	bool isWindowOpen();
	bool isRaining();
	bool isDay();
	bool isHeating();
	bool isCooling();
	bool isShutterOpen();
	bool isSun();

public:
	void closeWindow();
	void openWindow();
	void stoveOn();
	void stoveOff();
	void ACOn();
	void ACOff();
	void closeShutters();
	void openShutters();

public:
	void forceRain(bool state);
	void forceSun(bool state);

	void update(double delta_time);
};