cmake_minimum_required(VERSION 3.19)
project(Simple-Perceptron VERSION 0.0 LANGUAGES CXX)

# настраиваем стандарт языка для компиляции
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# включение заголовочных файлов
include_directories(include)

add_executable(Program main.cpp)