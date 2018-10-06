#
#	 Copyright (C) 2018 Qub³d Engine Group.
#	 All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification,
#  are permitted provided that the following conditions are met:
# 
#  1. Redistributions of source code must retain the above copyright notice, this
#  list of conditions and the following disclaimer.
#  
#  2. Redistributions in binary form must reproduce the above copyright notice,
#  this list of conditions and the following disclaimer in the documentation and/or
#  other materials provided with the distribution.
#  
#  3. Neither the name of the copyright holder nor the names of its contributors
#  may be used to endorse or promote products derived from this software without
#  specific prior written permission.
#  
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
#  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
#  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#

# Use this version because it adds some options that we use.
cmake_minimum_required(VERSION 3.0)

project(qub3d-game)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set include, library, and source directories.

if(UNIX)
	find_package(X11 REQUIRED)
endif()

set(OpenGL_GL_PREFERENCE "LEGACY")
find_package(OpenGL REQUIRED)
#find_package(SDL2 REQUIRED)
#find_package(GLM REQUIRED)
#find_package(GLEW REQUIRED)
#find_package(IMGUI REQUIRED)

#set(PLATFORM_LIBS -lpthread ${CMAKE_DL_LIBS})

set(source_dir ${PROJECT_SOURCE_DIR}/src)
set(headers_dir ${PROJECT_SOURCE_DIR}/include)

set(client_sources
	${source_dir}/main.cpp
	${source_dir}/window.cpp
	${source_dir}/io.cpp
	${source_dir}/imgui.cpp
	${source_dir}/chunk.cpp
	${source_dir}/fly_cam.cpp
	${source_dir}/shader_pipeline.cpp
	${headers_dir}/qub3d/opengl.hpp
	${headers_dir}/qub3d/shader_pipeline.hpp
	${headers_dir}/qub3d/io.hpp
	${headers_dir}/qub3d/imgui.hpp
	${headers_dir}/qub3d/fly_cam.hpp
	${headers_dir}/qub3d/chunk.hpp
	${source_dir}/texture.cpp
)

include_directories(
#	${X11_INCLUDE_DIR}
	${ENTT_INCLUDE_DIR}
	${IMGUI_INCLUDE_DIR}
	${GLEW_INCLUDE_DIRS}
	${GLM_INCLUDE_DIRS}
	${SDL2_INCLUDE_DIRS}
	${YAML_CPP_INCLUDE_DIRS}
	${headers_dir}
)

#if(APPLE)
#  message(STATUS "Including SDL2 dependency frameworks for macOS")
#  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework CoreAudio -framework AudioToolbox -framework CoreFoundation -framework CoreGraphics -framework CoreVideo -framework ForceFeedback -framework IOKit -framework Carbon -framework AppKit -framework OpenGL -liconv")
#endif()# APPLE

if(WIN32)
	list(APPEND library_dirs 
		${library_dirs}
		imm32.lib
		ole32.lib
		oleaut32.lib
		opengl32.lib
		version.lib
		winmm.lib
   )
endif()# WIN32
 
# Link the "SandBlox" binary.

add_executable(${PROJECT_NAME} ${client_sources})
set(qub3d_LIBS
	${PROJECT_NAME}
	${ENTT_LIBRARIES}
	${IMGUI_LIBRARIES}
	${GLEW_LIBRARIES}
	${GLM_LIBRARIES}
	${SDL2_LIBRARIES}
	${YAML_CPP_LIBRARIES}
	${X11_LIBRARIES}
	GL
)

target_link_libraries(${qub3d_LIBS})
