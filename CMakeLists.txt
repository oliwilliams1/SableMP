cmake_minimum_required(VERSION 3.15)

project(SableMP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add SableUI
add_subdirectory(vendor/SableUI)

# Add Media Library
add_subdirectory(vendor/MediaLibrary)

add_executable(SableMP
	"src/main.cpp"
	"include/components/PlaylistView.h"
)

target_include_directories(SableMP PRIVATE "include")

target_link_libraries(SableMP PRIVATE SableUI MediaLibrary)
