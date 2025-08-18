cmake_minimum_required(VERSION 4.0.3)
project(RaceControlSystem VERSION 0.0.1 LANGUAGES CXX)

find_package(Threads REQUIRED)
find_package(PahoMqttCpp REQUIRED PATHS ${PAHO_DIR})

add_library(RCS_Lib
  ${SOURCES}/src/RCS.cpp
  ${SOURCES}/src/Track.cpp
  ${SOURCES}/src/MQTT.cpp
)
target_include_directories(RCS_Lib PUBLIC
  ${SOURCES}/include
)

add_executable(${PROJECT_NAME}
  ${SOURCES}/src/main.cpp
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
