cmake_minimum_required(VERSION 3.28)
project(TextEditor LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Turn JetBrainsMono-Regular.ttf into a compilable C++ source file
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/JetBrainsMono-Regular.ttf" FONT_HEX HEX)
string(REGEX REPLACE "(..)" "0x\\1," FONT_BYTES "${FONT_HEX}")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/generated/font_data.cpp"
"#include \"font_data.hpp\"\n"
"const unsigned char JETBRAINS_TTF[] = { ${FONT_BYTES} };\n"
"const unsigned int JETBRAINS_TTF_SIZE = sizeof(JETBRAINS_TTF);\n")

add_executable(main
    src/main.cpp
    src/gapbuffer.cpp
    src/screen.cpp
    src/caret.cpp
    src/selectionbox.cpp
    src/scroll_view.cpp
    src/scroll_bar.cpp
    src/logger.cpp
    src/filehandler.cpp
    src/helpers.cpp
    src/event_manager.cpp
    src/warning_popup.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/generated/font_data.cpp
    src/font_metrics.cpp
)
target_compile_features(main PRIVATE cxx_std_20)
target_link_libraries(main PRIVATE SFML::Graphics)
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)

enable_testing()

add_executable(
  buffer_test
  tests/buffer_test.cpp
  src/gapbuffer.cpp
)
target_link_libraries(
  buffer_test
  GTest::gtest_main
)
target_include_directories(buffer_test PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}/generated
)

include(GoogleTest)
gtest_discover_tests(buffer_test)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)