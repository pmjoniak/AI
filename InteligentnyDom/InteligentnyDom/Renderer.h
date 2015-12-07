#pragma once
#include <memory>
#include <chrono>
#include <thread>
#include <GLFW/glfw3.h>
#include "Symulation.h"
#include "Controller.h"

class Renderer
{
private:
	using clock_source = std::chrono::steady_clock;
	using frame_duration = clock_source::duration;

private:
	clock_source::time_point next_frame = clock_source::now();
	void wait(int frame_count);

private:
	GLFWwindow *window;
	int width;
	int height;
	int cx, cy;

	
public:
	Renderer(Symulation& symulation, int width, int height);


	void setController(std::shared_ptr<Controller> controller);
	void run();

private:
	std::shared_ptr<Controller> controller;
	Symulation& symulation;

	void drawBoard();

private:
	void handleCharCallback(unsigned int codepoint);
	void handleDropCallback(int count, const char** names);
	void handleKeyCallback(int key, int scancode, int action, int mods);
	void handleMouseButtonCallback(int button, int action, int mods);
	void handleCursorPosCallback(double xpos, double ypos);

private:
	static void _handleErrorCallback(int error, const char* description);
	static void _handleCharCallback(GLFWwindow* window, unsigned int codepoint);
	static void _handleDropCallback(GLFWwindow* window, int count, const char** names);
	static void _handleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void _handleMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void _handleCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
};