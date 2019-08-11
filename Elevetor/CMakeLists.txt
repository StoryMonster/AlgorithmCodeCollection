cmake_minimum_required(VERSION 3.0)


project(ElevatorSystem VERSION 1.0  LANGUAGES CXX)

SET(SOURCES src/Elevator.cpp
	    src/Elevators.cpp
	    src/Humans.cpp
	    src/main.cpp
	    src/Threads.cpp
	    src/Utils.cpp)
SET(HEADERS src/Elevator.h
	    src/Elevators.h
	    src/Humans.h
	    src/MessageBus.h
	    src/Messages.h
	    src/Threads.h
	    src/Utils.h)

add_executable(ElevatorSystem ${HEADERS} ${SOURCES})
target_link_libraries(ElevatorSystem pthread)
