#find_package(SDL2 REQUIRED)
set(IMGUI_LIBRARIES ImGUI)
set(IMGUI_SOURCES ${CMAKE_CURRENT_LIST_DIR}/../../libdeps/ImGUI)
set(IMGUI_INCLUDE_DIR ${IMGUI_SOURCES})

# Not working due to the fact it wasn't design *this* style
#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../libdeps/ImGUI)

set(IMGUI_CORE_SRC
	${IMGUI_SOURCES}/imgui.cpp
	${IMGUI_SOURCES}/imgui_draw.cpp
	${IMGUI_SOURCES}/imgui_demo.cpp
	${IMGUI_SOURCES}/examples/imgui_impl_sdl.cpp
	${IMGUI_SOURCES}/examples/imgui_impl_opengl2.cpp
	${IMGUI_SOURCES}/examples/imgui_impl_opengl3.cpp
	#${IMGUI_SOURCES}/examples/imgui_impl_vulkan.cpp
)

set(IMGUI_LIB_HEADERS
	${IMGUI_SOURCES}/imgui.h
	${IMGUI_SOURCES}/imgui_internal.h
	${IMGUI_SOURCES}/imconfig.h
	${IMGUI_SOURCES}/stb_textedit.h
	${IMGUI_SOURCES}/stb_truetype.h
	${IMGUI_SOURCES}/stb_rect_pack.h
	${IMGUI_SOURCES}/examples/imgui_impl_opengl2.h
	${IMGUI_SOURCES}/examples/imgui_impl_opengl3.h
	#${IMGUI_SOURCES}/examples/imgui_impl_vulkan.h
)

include_directories(${IMGUI_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS})
add_library(ImGUI STATIC ${IMGUI_CORE_SRC})
target_link_libraries(ImGUI)
set(IMGUI_STATIC_LIB ImGUI)
set(IMGUI_LIBS ImGUI)
set(IMGUI_VERSION "1.63")

set_target_properties(${IMGUI_LIBS} PROPERTIES
	VERSION ${IMGUI_VERSION}
	CLEAN_DIRECT_OUTPUT 1
)

