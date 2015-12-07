#include "Symulation.h"
#include "Controller.h"
#include "Renderer.h"
#include <thread>

int main()
{
	Symulation sym;
	auto controller = std::make_shared<Controller>(sym);
	Renderer render(sym, 640, 480);
	render.setController(controller);

	render.run();

}