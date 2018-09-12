cmake_minimum_required(VERSION 3.0)
project(Viking)

# Try and find the vulkan and OpenGL librarys to decide what API's can be developed for
# E.g Both Vulkan and OpenGL Runtimes/Drivers should already be present on user machines
find_package(Vulkan)
set(hasVulkan ${Vulkan_FOUND})

find_package(OpenGL)
set(hasOpenGL ${OpenGL_OpenGL_FOUND})

# If we cannot find Vulkan or OpenGL then panic (I mean what else are we going to do, it is a rendering API after all)
#if(NOT ${hasVulkan} AND NOT ${hasOpenGL})
#    message(FATAL "Cannot find any suitable rendering backend (e.g cannot find Vulkan or OpenGL)")
#endif()

# Any source (e.g .cpp) files that are common to all rendering API's (e.g not API specific)
set(common_source
	source/src/IRenderer.cpp
    source/src/IWindow.cpp
    source/src/IBuffer.cpp
    source/src/IUniformBuffer.cpp
    source/src/ITextureBuffer.cpp
    source/src/IDescriptor.cpp
    source/src/IComputePipeline.cpp
    source/src/IComputeProgram.cpp
    source/src/IGraphicsPipeline.cpp
    source/src/IModelPool.cpp
    source/src/IModel.cpp
    source/src/SDLWindow.cpp
)

# Any header (e.g .hpp) files that are common to all rendering API's (e.g not API specific)
set(common_headers
	source/include/viking/API.hpp
	source/include/viking/IRenderer.hpp
    source/include/viking/WindowDescriptor.hpp
    source/include/viking/IWindow.hpp
    source/include/viking/IBuffer.hpp
    source/include/viking/IUniformBuffer.hpp
    source/include/viking/ITextureBuffer.hpp
    source/include/viking/IDescriptor.hpp
    source/include/viking/IComputePipeline.hpp
    source/include/viking/IComputeProgram.hpp
    source/include/viking/SDLWindow.hpp
    source/include/viking/VertexBufferBase.hpp
    source/include/viking/IGraphicsPipeline.hpp
    source/include/viking/IModelPool.hpp
    source/include/viking/IModel.hpp
    source/include/viking/VertexBinding.hpp
    source/include/viking/ShaderStage.hpp
)

# Lists of all source and header files to be compiled (e.g common and API specific)
set(source ${common_source})
set(headers ${common_headers})

# If we support the OpenGL backend then we need to setup the OpenGL backend.
if( ${hasOpenGL} )
    # Add preprocessor definitions to tell the compiler (and preprocessor) that we support GL11 and GL3
	add_definitions(-DVIKING_SUPPORTS_OPENGL)
    
    # OpenGL specific code files
    set(opengl_source
        source/src/opengl/OpenGLSDLWindow.cpp
        source/src/opengl/OpenGLRenderer.cpp
        source/src/opengl/OpenGLGraphicsPipeline.cpp
        source/src/opengl/OpenGLModelPool.cpp
        source/src/opengl/OpenGLBuffer.cpp
        source/src/opengl/OpenGLModel.cpp
        source/src/opengl/OpenGLTextureBuffer.cpp
		source/src/opengl/OpenGLUniformBuffer.cpp
        source/src/opengl/glad.c
    )

    set(opengl_headers
        source/include/viking/opengl/OpenGLSDLWindow.hpp
        source/include/viking/opengl/OpenGLRenderer.hpp
        source/include/viking/opengl/OpenGLGraphicsPipeline.hpp
        source/include/viking/opengl/OpenGLModelPool.hpp
        source/include/viking/opengl/OpenGLBuffer.hpp
        source/include/viking/opengl/OpenGLModel.hpp
        source/include/viking/opengl/OpenGLUniformBuffer.hpp
        source/include/viking/opengl/OpenGLTextureBuffer.hpp
        source/include/viking/opengl/glad.h
    )

    # Add the OpenGL specific code files to the list of files to be compiled.
    list(APPEND source ${opengl_source})
    list(APPEND headers ${opengl_headers})
    
    # Mostly to sort the OpenGL specific files into a nice VS filter.
    source_group("Source Files\\OpenGL Backend" FILES ${opengl_source})
    source_group("Header Files\\OpenGL Backend" FILES ${opengl_headers})
endif()

# If we've found the Vulkan SDK/Development Librarys then setup the Vulkan rendering backend
if(${hasVulkan})
    # Add preprocessor definitions to tell the compiler (and preprocessor) that we support Vulkan
	add_definitions(-DVIKING_SUPPORTS_VULKAN)
	
	# This is needed for vulkan platform specific code
	if(WIN32)
		add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)
	elseif(APPLE)
		message(FATAL_ERROR "We don't know how to make Vulkan work on Apple products yet :-(")
	elseif(UNIX)
		add_definitions(-DVK_USE_PLATFORM_XCB_KHR)
	endif()

	# Vulkan API specific code files
    set(vulkan_source
       source/src/vulkan/IVulkanPipeline.cpp
       source/src/vulkan/VulkanComputePipeline.cpp
       source/src/vulkan/VulkanInitializers.cpp
       source/src/vulkan/VulkanInstance.cpp
       source/src/vulkan/VulkanPhysicalDevice.cpp
       source/src/vulkan/VulkanDevice.cpp
       source/src/vulkan/VulkanRenderer.cpp
       source/src/vulkan/VulkanWindow.cpp
       source/src/vulkan/VulkanSDLWindow.cpp
       source/src/vulkan/IVulkanSurface.cpp
       source/src/vulkan/VulkanSDLSurface.cpp
       source/src/vulkan/VulkanSwapchain.cpp
       source/src/vulkan/VulkanCommon.cpp
       source/src/vulkan/VulkanBuffer.cpp
       source/src/vulkan/VulkanUniformBuffer.cpp
       source/src/vulkan/VulkanDescriptor.cpp
       source/src/vulkan/VulkanComputeProgram.cpp
    )
    
    set(vulkan_headers
        source/include/viking/vulkan/IVulkanPipeline.hpp
        source/include/viking/vulkan/VulkanComputePipeline.hpp
        source/include/viking/vulkan/VulkanInitializers.hpp
        source/include/viking/vulkan/VulkanInstance.hpp
        source/include/viking/vulkan/VulkanPhysicalDevice.hpp
        source/include/viking/vulkan/VulkanDevice.hpp
        source/include/viking/vulkan/VulkanRenderer.hpp
        source/include/viking/vulkan/VulkanWindow.hpp
        source/include/viking/vulkan/VulkanSDLWindow.hpp
        source/include/viking/vulkan/IVulkanSurface.hpp
        source/include/viking/vulkan/VulkanSDLSurface.hpp
        source/include/viking/vulkan/VulkanSwapchain.hpp
        source/include/viking/vulkan/VulkanQueueFamilyIndices.hpp
        source/include/viking/vulkan/VulkanSwapChainConfiguration.hpp
        source/include/viking/vulkan/VulkanCommon.hpp
        source/include/viking/vulkan/VulkanBuffer.hpp
        source/include/viking/vulkan/VulkanUniformBuffer.hpp
        source/include/viking/vulkan/VulkanDescriptor.hpp
        source/include/viking/vulkan/VulkanComputeProgram.hpp
    )

	# Add the include directory for Vulkan (from the Vulkan SDK)
	# include_directories(${Vulkan_INCLUDE_DIRS})
    
    # Add the Vulkan specific code files to the list of files to be compiled.
    list(APPEND source ${vulkan_source})
    list(APPEND headers ${vulkan_headers})
    
    # Mostly to sort the Vulkan specific files into a nice VS filter.
    source_group("Source Files\\Vulkan Backend" FILES ${vulkan_source})
    source_group("Header Files\\Vulkan Backend" FILES ${vulkan_headers})

	if(WIN32)
		include_directories(${Vulkan_INCLUDE_DIRS})
	elseif (UNIX)
		include_directories($ENV{VULKAN_SDK}/include)
	endif()

endif()

#SDL2 needs these libs if on Windows
if(WIN32)
	set(library_dirs 
		${library_dirs}
		imm32.lib
		ole32.lib
		oleaut32.lib
		opengl32.lib
		version.lib
		winmm.lib
		SDL2-static
		SDL2main
        #MSVCRT.LIB
        )
endif()# WIN32

# Packages for Unix.
if(UNIX)
	find_package(X11 REQUIRED)
	find_package(Threads)
	set(library_dirs
		${library_dirs}
		${CMAKE_DL_LIBS}
		${CMAKE_THREAD_LIBS_INIT}
		${X11_LIBRARIES}
		${X11_Xxf86vm_LIB}
		SDL2-static
		SDL2main
	)
	if(${hasVulkan})
		set(library_dirs 
			${library_dirs}
			${Vulkan_LIBRARIES}
		)
	endif()
endif()

# Add the `viking-renderer/include` directory to the list of include directories
# now we reference header files as
#     #include <viking/xxx.hpp>
#     #include <viking/vulkan/xxx.hpp>
# etc...

include_directories(source/include ../libdeps/SDL2/include)

set(LIBRARY_OUTPUT_PATH ../../COMPILE/lib)

# Create viking as a static library (compiled from the master list of source and header files)
add_library(${PROJECT_NAME} STATIC ${source} ${headers})

# Now link in any rendering API specific librarys
target_link_libraries(${PROJECT_NAME} ${library_dirs})

# Now link in any rendering API specific librarys
if(hasVulkan)
    target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
endif()

if(hasOpenGL)
    target_link_libraries(${PROJECT_NAME} OpenGL::GL)
endif()
