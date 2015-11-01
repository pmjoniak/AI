#include "Renderer.h"
#include <cmath>

Renderer::Renderer(Board& board, int width, int height) : width{ width }, height{ height }, board{ board }
{
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 16);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(width, height, "Connect4 v0.1.0", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);
	//glewInit();

	glfwSetErrorCallback(_handleErrorCallback);
	glfwSetCharCallback(window, _handleCharCallback);
	glfwSetDropCallback(window, _handleDropCallback);
	glfwSetKeyCallback(window, _handleKeyCallback);
	glfwSetMouseButtonCallback(window, _handleMouseButtonCallback);
	glfwSetCursorPosCallback(window, _handleCursorPosCallback);

}


void Renderer::handleCharCallback(unsigned int codepoint)
{
	(void)codepoint;
}

void Renderer::handleDropCallback(int count, const char** names)
{
	(void)count;
	(void)names;
}

void Renderer::handleKeyCallback(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, 1);
	(void)key;
	(void)scancode;
	(void)action;
	(void)mods;
}

void Renderer::handleMouseButtonCallback(int button, int action, int mods)
{
	(void)mods;
	if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (cy > 30 && cy < 450 && cx > 30 && cx < 450)
		{
			int x = ((cx - 30) / 60);
			to_drop = x;
			droping = 0;
			//controller->drop(x);
		}
	}
	else if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		//gc->clearSelected();
	}
}

void Renderer::handleCursorPosCallback(double xpos, double ypos)
{
	cx = (int)xpos;
	cy = (int)ypos;
}

void Renderer::run()
{
	glfwMakeContextCurrent(window);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		// glFrustum(fNear * (headX - fFov * ratio),
		// 		  fNear * (headX + fFov * ratio),
		// 		  fNear * (headY - fFov),
		// 		  fNear * (headY + fFov),
		// 		  fNear, fFar);
		glOrtho(0, width, height, 0, -10, 10);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		drawBoard();
		//drawGrid(4, 4);

		controller->process();

		glfwSwapBuffers(window);
		glfwPollEvents();
		// wait(10);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	glfwDestroyWindow(window);
}


void Renderer::drawBoard()
{
	for (int x = 0; x < 7; x++)
	{
		for (int y = 0; y < 6; y++)
		{
			glPushMatrix();

			glColor3f(0, 0, 1);
			glTranslatef(x * 60 + 30 + 30, 480 - (y * 60 + 30 + 30),0);
			glBegin(GL_QUADS);
				glVertex3f(-30, -30, -0.01f);
				glVertex3f(30, -30, - 0.01f);
				glVertex3f(30, 30, -0.01f);
				glVertex3f(-30, 30, -0.01f);
			glEnd();

			if (board.board[x][y] == NONE)
				glColor3f(0, 0, 0);
			else if (board.board[x][y] == YELLOW)
				glColor3f(1, 1, 0);
			else if (board.board[x][y] == RED)
				glColor3f(1, 0, 0);

			glBegin(GL_TRIANGLE_FAN);
				glVertex2f(0, 0);
				for (float a = 0; a <= 2 * 3.3; a += 0.1)
					glVertex2f(25 * std::cos(a), 25 * std::sin(a));
			glEnd();

			glPopMatrix();
		}
	}

	if (cy > 30 && cy < 450 && cx > 30 && cx < 450)
	{
		int x = ((cx - 30) / 60) * 60 + 60;
		glPushMatrix();
			glTranslatef(x , 60 , 0);
			if (board.current_color == YELLOW) glColor3f(1, 1, 0);	else glColor3f(1, 0, 0);
			glBegin(GL_TRIANGLE_FAN);
				glVertex2f(0, 0);
				for (float a = 0; a <= 2 * 3.3; a += 0.1)
					glVertex2f(25 * std::cos(a), 25 * std::sin(a));
			glEnd();
		glPopMatrix();
	}
	if (to_drop != -1)
	{
		droping++;
		int y = droping;
		if (y > 5 || board.board[to_drop][5 - y] != NONE)
		{
			controller->drop(to_drop);
			to_drop = -1;
			return;
		}

		glPushMatrix();
		glTranslatef(to_drop * 60 + 60, y * 60 + 120, 0.01);
		if (board.current_color == YELLOW) glColor3f(1, 1, 0);	else glColor3f(1, 0, 0);
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(0, 0);
		for (float a = 0; a <= 2 * 3.3; a += 0.1)
			glVertex2f(25 * std::cos(a), 25 * std::sin(a));
		glEnd();
		glPopMatrix();
	}
}


void Renderer::setController(std::shared_ptr<Controller> controller)
{
	this->controller = controller;
}

void Renderer::wait(int frame_count)
{
	next_frame += frame_count * frame_duration(1);
	std::this_thread::sleep_until(next_frame);
}

//----------------------------------------------------------------------------

void Renderer::_handleErrorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW error: %d '%s'\n", error, description);
}

void Renderer::_handleCharCallback(GLFWwindow* window, unsigned int codepoint)
{
	((Renderer*)glfwGetWindowUserPointer(window))->handleCharCallback(codepoint);
}

void Renderer::_handleDropCallback(GLFWwindow* window, int count, const char** names)
{
	((Renderer*)glfwGetWindowUserPointer(window))->handleDropCallback(count, names);
}

void Renderer::_handleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	((Renderer*)glfwGetWindowUserPointer(window))->handleKeyCallback(key, scancode, action, mods);
}

void Renderer::_handleMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	((Renderer*)glfwGetWindowUserPointer(window))->handleMouseButtonCallback(button, action, mods);
}

void Renderer::_handleCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	((Renderer*)glfwGetWindowUserPointer(window))->handleCursorPosCallback(xpos, ypos);
}
