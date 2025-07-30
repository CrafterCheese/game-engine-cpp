# I Use Clion more than VScode so this is to make it work with CMake in Clion
cmake_minimum_required(VERSION 3.17)

# set vcpkg toolchain before project()
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

project(GameEngineCPP VERSION 0.1 LANGUAGES CXX)

# use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# get sources
file(GLOB_RECURSE ENGINE_SOURCES
  "${CMAKE_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_SOURCE_DIR}/src/*.cc"
  "${CMAKE_SOURCE_DIR}/src/*.c"
)
file(GLOB_RECURSE ENGINE_HEADERS
  "${CMAKE_SOURCE_DIR}/src/*.h"
  "${CMAKE_SOURCE_DIR}/src/*.hpp"
)

# executable
add_executable(${PROJECT_NAME}
  ${ENGINE_SOURCES}
  ${ENGINE_HEADERS}
)

# include dirs
target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_SOURCE_DIR}/src
)

# find and link dependencies via vcpkg
find_package(SDL2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)

# link libs
target_link_libraries(${PROJECT_NAME} PRIVATE
  SDL2::SDL2
  spdlog::spdlog
  glm::glm
  EnTT::EnTT
  fmt::fmt
  imgui::imgui
  GLEW::GLEW
)

# asset directories
set(ASSET_DIRS
  "${CMAKE_SOURCE_DIR}/game-jam"
  "${CMAKE_SOURCE_DIR}/gfx"
  "${CMAKE_SOURCE_DIR}/models"
  "${CMAKE_SOURCE_DIR}/sfx"
  "${CMAKE_SOURCE_DIR}/shaders"
)
foreach(dir IN LISTS ASSET_DIRS)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${dir}
      $<TARGET_FILE_DIR:${PROJECT_NAME}>/${dir}
  )
endforeach()

# allow compile commands (for CLion)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
