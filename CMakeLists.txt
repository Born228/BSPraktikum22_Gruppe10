cmake_minimum_required(VERSION 3.7.2)
project(BetriebssystemePraktikum C)

set(CMAKE_C_STANDARD 99)

add_executable(BetriebssystemePraktikum main.c keyValueStorage.c keyValueStorage.h main.h subRoutines.c subRoutines.h)
