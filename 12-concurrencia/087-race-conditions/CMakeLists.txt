cmake_minimum_required(VERSION 3.16)
project(race_conditions_main C)
include(../../cmake/Exercise.cmake)
c100_exercise(race_conditions_main src/main.c src/race_conditions.c)
