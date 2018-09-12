#include <viking/IRenderer.hpp>

#ifdef VIKING_SUPPORTS_VULKAN
	#include <viking/vulkan/VulkanRenderer.hpp>
#endif

#ifdef VIKING_SUPPORTS_OPENGL
	#include <viking/opengl/OpenGLRenderer.hpp>
#endif

using namespace viking;

IRenderer* IRenderer::createRenderer(const RenderingAPI& api)
{
	switch(api)
	{
		#ifdef VIKING_SUPPORTS_OPENGL
			case RenderingAPI::GL3:
			case RenderingAPI::GL11:
				return new opengl::OpenGLRenderer();
		#endif

		#ifdef VIKING_SUPPORTS_VULKAN
			case RenderingAPI::Vulkan:
				return new vulkan::VulkanRenderer();
		#endif
	}

	return nullptr;
}

void IRenderer::addWindow(IWindow* window)
{
	m_window = window;
}
