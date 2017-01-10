cmake_minimum_required (VERSION 2.8.11)
project (WhimsyAudioTest)

#set(DEBUG_MODE Yes)

#if(DEBUG_MODE)
#    set(CMAKE_BUILD_TYPE Debug)
#else()
#    set(CMAKE_BUILD_TYPE Release)
#endif()

# Change it to the path where QT is installed if this doesn't work for you.
set(QT_CMAKE_MODULE_PATH        "/usr/share/qt/5.7/gcc_64/lib/cmake")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR   ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC               ON)

# Append custom CMake modules: Search in Qt Cmake folder and in cmake-modules here.
set(CMAKE_MODULE_PATH           "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")
set(CMAKE_PREFIX_PATH           "${CMAKE_PREFIX_PATH};${QT_CMAKE_MODULE_PATH}")

# Include C++11 support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# Include PortAudio support. If you have installed it properly, it will find it with absolutely no problems.
find_package(PortAudio REQUIRED)

# Find the QtWidgets library -- Change the QT_CMAKE_MODULE_PATH above if you have problems with this step.
find_package(Qt5Widgets REQUIRED)

include_directories(${PORTAUDIO_INCLUDE_DIRS})

# Create our test programs
add_subdirectory(testA1)
add_subdirectory(testA2)
add_subdirectory(testA3)
