cmake_minimum_required(VERSION 3.0)

# We are building a C Game
set(CMAKE_C_STANDARD 99)
project(thread_pool)

# Add source files
add_executable(${PROJECT_NAME} "src/main.c" "src/thread_pool/thread_pool.c" "src/data_structures/lists/linked_list.c")

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/src/thread_pool")
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/src/data_structures/linked_list")
#[[
# Conditional linking for platform-specific libraries
if(WIN32)
    # For Windows, use pthreads-w32
    target_include_directories(${PROJECT_NAME} PRIVATE "path/to/pthreads-w32/include")
    target_link_libraries(${PROJECT_NAME} PRIVATE "path/to/pthreads-w32/lib/pthreadVC2.lib")
elseif(UNIX AND NOT APPLE)
    # For Linux and other Unix-like systems (not macOS), use POSIX pthreads
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
elseif(APPLE)
    # For macOS, use POSIX pthreads
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()
]]

# Set runtime output directory for debug mode
set_target_properties(${PROJECT_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin
)


# Install the project
install(TARGETS ${PROJECT_NAME})