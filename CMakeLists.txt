cmake_minimum_required(VERSION 3.6)
project(discordpp-plugin-responder)

set(CMAKE_CXX_STANDARD 14)

###################

add_library(${PROJECT_NAME} INTERFACE)

INCLUDE_DIRECTORIES(${discordpp_SOURCE_DIR})
TARGET_LINK_LIBRARIES(
        ${PROJECT_NAME} INTERFACE
        nlohmann_json::nlohmann_json
)
