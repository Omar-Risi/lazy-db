cmake_minimum_required(VERSION 3.10)
project(lazydb)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Fetch FTXUI automatically
include(FetchContent)
FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
    GIT_TAG v5.0.0
)
FetchContent_MakeAvailable(ftxui)

# PostgreSQL
find_package(PostgreSQL REQUIRED)

add_executable(lazydb 
  src/main.cpp
  src/ui.cpp
)

target_include_directories(lazydb PRIVATE 
    include
    ${PostgreSQL_INCLUDE_DIRS}
)

target_link_libraries(lazydb PRIVATE
    ftxui::screen
    ftxui::dom
    ftxui::component
    ${PostgreSQL_LIBRARIES}
)
