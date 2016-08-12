cmake_minimum_required(VERSION 2.8)
project(portAudioPlugin)

if(APPLE)
	# This has to be before most other options so CMake properly handles the 
	# compiler variables, it MUST bebefore the project() definition

	option(BUILD_UNIVERSAL_BINARIES "Build universal binaries for all architectures supported" ON)
	if (NOT CMAKE_OSX_ARCHITECTURES AND BUILD_UNIVERSAL_BINARIES)
		if(IOS)
			# set the architecture for iOS 
			if (${IOS_PLATFORM} STREQUAL "OS")
				set (IOS_ARCH armv6 armv7 armv7s arm64)
				set (CMAKE_OSX_ARCHITECTURES ${IOS_ARCH} CACHE string  "Build architecture for iOS")
			else (${IOS_PLATFORM} STREQUAL "OS")
				set (IOS_ARCH i386 x86_64)
				set (CMAKE_OSX_ARCHITECTURES ${IOS_ARCH} CACHE string  "Build architecture for iOS Simulator")
			endif (${IOS_PLATFORM} STREQUAL "OS")

		else(IOS)
			# set the architectures for OS X
			set (OSXI_ARCH i386 x86_64)
			set (CMAKE_OSX_ARCHITECTURES ${OSXI_ARCH} CACHE string  "Build architecture for OS X universal binaries")		
		endif(IOS)
	endif (NOT CMAKE_OSX_ARCHITECTURES AND BUILD_UNIVERSAL_BINARIES)
endif(APPLE)


if(WIN32)
  add_definitions( -DWIN32 )
endif(WIN32)

if(UNIX)
  set(CMAKE_CXX_FLAGS "-std=c++0x -fPIC ${CMAKE_CXX_FLAGS}")
endif(UNIX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(LuaJIT REQUIRED)
find_package(Luabind REQUIRED)
find_package(SFML REQUIRED system audio)
find_package(Gsage REQUIRED)

add_subdirectory(Plugin)
