cmake_minimum_required(VERSION 3.19)
project(Simple-Perceptron VERSION 0.0 LANGUAGES CXX)

# настраиваем стандарт языка для компиляции
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# для работы Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# смотрим, чтобы были установлены необходимые пакеты
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)

# включение заголовочных файлов
include_directories(include)
file(GLOB tools_src_files "tools/*.cpp")
file(GLOB gui_files "gui/*.cpp" "include/gui/*.h")
file(GLOB matrix_perceptron_src_files "perceptron/matrix/*.cpp")
file(GLOB graph_perceptron_src_files "perceptron/graph/*.cpp")
file(GLOB controller_src_files "controller/*.cpp" )
file(GLOB gtest_src_files "tests/*.cpp")

# Статическая библиотека внутренней логики
add_library(controller STATIC
        ${controller_src_files}
        ${matrix_perceptron_src_files}
        ${graph_perceptron_src_files}
        ${tools_src_files}
)

add_executable(Program main.cpp ${gui_files})
target_link_libraries(Program PRIVATE
        controller
        Qt6::Widgets
        Qt6::Charts
)

# ============================
# === Тестирование с GTest ===
# ============================

find_package(GTest REQUIRED)
include(GoogleTest)

enable_testing() # чтобы можно было запускать через ctest

add_executable(TestPerceptron ${gtest_src_files})

target_include_directories(TestPerceptron PUBLIC)
target_link_libraries(TestPerceptron
        controller
        GTest::gtest_main
)

gtest_discover_tests(TestPerceptron)