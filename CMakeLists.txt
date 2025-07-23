cmake_minimum_required(VERSION 4.0.3)
project(RaceControlSystem VERSION 0.0.1 LANGUAGES CXX)

find_package(Threads REQUIRED)

set(PAHO_DIR ~/opt/pahomqtt/)
find_package(PahoMqttCpp REQUIRED PATHS ~/opt/pahomqtt/lib/cmake)
# include_directories(${PAHO_DIR}/include)

# include_directories(${PAHO_DIR}/include)
# add_library(MQTT SHARED IMPORTED)
# # target_include_directories(MQTT  ${PAHO_DIR}/include)
# link_directories(${PAHO_DIR}/lib)

add_library(RCS_Lib src/Track.cpp src/RCS.cpp src/Server.cpp)
target_include_directories(RCS_Lib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

add_executable(${PROJECT_NAME}
  src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC RCS_Lib)
target_link_libraries(${PROJECT_NAME} PUBLIC PahoMqttCpp::paho-mqttpp3)

# install(TARGETS
#   RaceControlSystem
#   DESTINATION lib/${PROJECT_NAME}
# )
# install(DIRECTORY
#   include
#   DESTINATION include/${PROJECT_NAME}/
# )
# install(DIRECTORY
#   launch
#   DESTINATION share/${PROJECT_NAME}/
# )
# install(DIRECTORY
#   config
#   DESTINATION share/${PROJECT_NAME}/
# )
