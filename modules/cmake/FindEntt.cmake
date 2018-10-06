if(NOT ENTT_FOUND)
	set(ENTT_SOURCES_PATH "${CMAKE_CURRENT_LIST_DIR}/../../libdeps/entt")
	set(ENTT_INCLUDE_DIRS ${ENTT_SOURCES_PATH}/src/entt)
	#set(ENTT_LIBRARIES)
	add_subdirectory(${ENTT_SOURCES_PATH})
endif()

