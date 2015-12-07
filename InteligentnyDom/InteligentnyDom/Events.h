#pragma once


enum class Events
{
	WindowClosed,
	WindowOpened,
	TooHot,
	TooCold,
	RainStarted,
	RainEnded,
	SummerStarted,
	WinterStarted,
	DayStarted,
	NightStarted,
	SunStarted,
	SunEnded,


	CloseShutters,
	OpenShutters,
	CloseWindow,
	OpenWindow,
	StoveOn,
	StoveOff,
	ACOn,
	ACOff
};

using EventType = Events;