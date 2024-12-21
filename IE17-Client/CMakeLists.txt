cmake_minimum_required(VERSION 3.20)

# Project name and settings
project(IE17Client LANGUAGES CXX)

# Set the target platform version
set(CMAKE_SYSTEM_VERSION 10.0)

# Include sub-projects.
#add_subdirectory ("Deps/minhook")include(FetchContent)

include(FetchContent)

FetchContent_Declare(
  MinHook
  GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
)
FetchContent_MakeAvailable(MinHook)

# Statically link the GCC runtime
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

# Create a shared library (DynamicLibrary)
add_library(IE17Client SHARED
    dllmain.cpp
    main.cpp
    player.cpp
)

# Include directories
target_include_directories(IE17Client PRIVATE
    ${CMAKE_SOURCE_DIR}/lib
)

# Compiler definitions for different configurations
target_compile_definitions(IE17Client PRIVATE
    $<$<CONFIG:Debug>:WIN32;_DEBUG;IE17CLIENT_EXPORTS;_WINDOWS;_USRDLL>
    $<$<CONFIG:Release>:WIN32;NDEBUG;IE17CLIENT_EXPORTS;_WINDOWS;_USRDLL>
)

# Linker options and libraries
target_link_libraries(IE17Client PRIVATE
    kernel32
    user32
    gdi32
    winspool
    comdlg32
    advapi32
    shell32
    ole32
    oleaut32
    uuid
    odbc32
    odbccp32
    minhook
    #libMinHook-x64-v141-mt
)

# Additional library directories for x64 builds
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_link_directories(IE17Client PRIVATE
        ${CMAKE_SOURCE_DIR}/lib
    )
endif()

# Enable warnings and additional settings
target_compile_options(IE17Client PRIVATE
    $<$<CONFIG:Debug>:-g -Wall -Wextra>
    $<$<CONFIG:Release>:-O2 -Wall -Wextra>
)
