cmake_minimum_required(VERSION 3.12)

#nama projectnya
project(Perpus_Rouglike)

#versi c++ standar
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#donload raylib otomatis
include(FetchContent)
FetchContent_Declare(
	raylib
	GIT_REPOSITORY https://github.com/raysan5/raylib.git
	GIT_TAG		5.5
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)

#file program(file cpp dan headernya)
file(GLOB_RECURSE RESOURCE "RougeLike_Perpus/Source_Code/*.cpp" "RougeLike_Perpus/Source_Code/*.h")

#ambil main program di file utamana
list(APPEND RESOURCE "RougeLike_Perpus/main.cpp")

#Executable
add_executable(${PROJECT_NAME} ${RESOURCE})

#letak headerfilenya
target_include_directories(${PROJECT_NAME} PRIVATE "RougeLike_Perpus/Source_Code")

#link ke raylib
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

#Menghubungkan library system yang dibutuhkan raylib
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE winmm)
endif()

#asset
file(COPY "${CMAKE_SOURCE_DIR}/RougeLike_Perpus/Asset" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/RougeLike_Perpus/Shader" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/RougeLike_Perpus/Font" DESTINATION "${CMAKE_BINARY_DIR}")

#test
message(STATUS "test ")

















