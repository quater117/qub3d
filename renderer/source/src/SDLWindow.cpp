#include <viking/SDLWindow.hpp>

#ifdef VIKING_SUPPORTS_VULKAN
	#include <viking/vulkan/VulkanSDLWindow.hpp>
	using namespace viking::vulkan;
#endif

#ifdef VIKING_SUPPORTS_OPENGL
	#include <viking/opengl/OpenGLSDLWindow.hpp>
#endif

using namespace viking;

viking::SDLWindow::~SDLWindow()
{
	SDL_DestroyWindow(m_window);
}

SDLWindow * viking::SDLWindow::createWindow(WindowDescriptor descriptor, RenderingAPI api)
{
	switch (api)
	{
	#ifdef VIKING_SUPPORTS_VULKAN
		case RenderingAPI::Vulkan:
			return new VulkanSDLWindow(descriptor);
			break;
	#endif

	#ifdef VIKING_SUPPORTS_OPENGL
		case RenderingAPI::GL3:
		case RenderingAPI::GL11:
			return new opengl::OpenGLSDLWindow(descriptor);
	#endif
	}

	return nullptr;
}

void SDLWindow::poll()
{
	SDL_Event event;
	while (SDL_PollEvent(&event) > 0)
	{
		if (event.type == SDL_QUIT)
			m_running = false;
	}
}
#include <iostream>
void SDLWindow::swapBuffers()
{
	float current_tick = SDL_GetTicks();
	std::cout << (int)(1000 / (current_tick - m_last_tick)) << " FPS"<<std::endl; 
	m_last_tick = current_tick;
	SDL_GL_SwapWindow(m_window);
}

bool SDLWindow::isRunning()
{
	return m_running;
}

void SDLWindow::GetSize(int & width, int & height)
{
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(m_window), &DM);
	width = DM.w;
	height = DM.h;
}

SDL_Window * viking::SDLWindow::GetSDLWindow()
{
	return m_window;
}

viking::SDLWindow::SDLWindow(WindowingAPI windowing_api) : IWindow(windowing_api) {}
