cmake_minimum_required(VERSION 2.6)

project("M2 project: Intersection norms - Paul Cottalorda")


#public incude deirectory
include_directories(extlib/SFML-2.3.2/include)
include_directories(extlib/)



set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

#libraries: This should probably be in a CMakeCache
set(EXT_BASE extlib/SFML-2.3.2)
if(UNIX AND NOT APPLE)
	set(SFML_PATH ${EXT_BASE}/linux)
endif()
if(MSVC)
	set(SFML_PATH ${EXT_BASE}/windows)
endif()
if(APPLE)
	set(SFML_PATH ${EXT_BASE}/mac)
endif()
set(SFML_LIBS ${SFML_PATH}/*)


#Getting source files
file(

	GLOB_RECURSE
	
	source_files
	
	src/*
	
)

add_executable(
	main
	${source_files}
)

target_link_libraries (
	main
	${SFML_LIBS}
)

