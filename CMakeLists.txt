cmake_minimum_required(VERSION 3.10)
project(waykey)

# Set C standard
set(CMAKE_C_STANDARD 99)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

# Add executable
add_executable(waykey src/main.c)

# Link GTK libraries
target_link_libraries(waykey ${GTK_LIBRARIES})
target_include_directories(waykey PRIVATE ${GTK_INCLUDE_DIRS})
target_compile_options(waykey PRIVATE ${GTK_CFLAGS_OTHER})

# Install target
install(TARGETS waykey DESTINATION bin)

# Enable testing
enable_testing()

# Add a test that checks if the executable builds correctly
add_test(NAME build_test
         COMMAND bash -c "test -x ./waykey"
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR})