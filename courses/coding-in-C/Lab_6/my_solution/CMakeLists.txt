cmake_minimum_required(VERSION 3.10)
project(Lab6_Solutions C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Section I: File I/O demonstrations
add_executable(section1_file_io section1_file_io.c)

# Section II: Car refuel
add_executable(section2_car_refuel section2_car_refuel.c)

# Section III: Sensor fusion (main task)
add_executable(section3_sensor_fusion section3_sensor_fusion.c)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../sensor1.txt ${CMAKE_BINARY_DIR}/sensor1.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../sensor2.txt ${CMAKE_BINARY_DIR}/sensor2.txt COPYONLY)
