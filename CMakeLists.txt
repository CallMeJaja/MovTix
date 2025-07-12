# Set the minimum required version of CMake for this project.
cmake_minimum_required(VERSION 3.10)

# Define the project name.
project(MovtixApp)

# Set the C++ standard to C++11.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the 'include' directory so CMake can find your header files.
include_directories(include)

# Automatically find all .cpp files in the 'src' directory.
file(GLOB SOURCES "src/*.cpp")

# Define the name for your executable.
set(EXECUTABLE_NAME movtix)

# Create the executable target from your source files.
add_executable(${EXECUTABLE_NAME} ${SOURCES})

# Set the output directory for the final executable to be 'bin'.
set_target_properties(${EXECUTABLE_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# --- Custom 'run' Target ---
# This creates a convenient 'run' target that first builds the project
# and then executes the compiled application.
add_custom_target(run
    # The command to execute is your compiled program.
    COMMAND ${EXECUTABLE_NAME}
    # This ensures the executable is built before this target is run.
    DEPENDS ${EXECUTABLE_NAME}
    # Sets the working directory to where the executable is located.
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Display a message to the user after configuration.
message(STATUS "To build: 'cmake --build .'")
message(STATUS "To run:   'cmake --build . --target run'")