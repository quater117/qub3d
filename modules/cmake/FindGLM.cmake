INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(GLM REQUIRED glm)
if(NOT GLM_FOUND)
	message(STATUS "Using backup for GLM")
	set(GLM_TEST_ENABLE OFF)
	set(GLM_SOURCES_PATH "${CMAKE_CURRENT_LIST_DIR}/../../libdeps/glm")
	set(GLM_INCLUDE_DIRS ${GLM_SOURCES_PATH})
	add_subdirectory(${GLM_SOURCES_PATH})
endif()