cmake_minimum_required (VERSION 3.26)

project (viper)

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
llvm_map_components_to_libnames(llvm_libs -15)
link_libraries(${llvm_libs})

message(STATUS ${llvm_libs})

include_directories(framework/include)
set(CMAKE_BUILD_TYPE Debug)

file(GLOB viper_framework_sources
    framework/src/lexer/Lexer.cpp
    framework/src/lexer/Token.cpp
)
add_library(viper_framework SHARED ${viper_framework_sources})

file(GLOB viper_compiler_sources compiler/src/main.cpp)
add_executable(viper ${viper_compiler_sources})

file(GLOB viper_tests_sources tests/src/main.cpp tests/src/Test.cpp
    tests/src/lexer/LexerTests.cpp
)
add_executable(viper_tests ${viper_tests_sources})

target_compile_features(viper_framework PUBLIC cxx_std_20)
target_link_libraries(viper_framework ${LLVM_LIBRARIES})

target_link_libraries(viper LINK_PUBLIC viper_framework)
target_compile_features(viper PUBLIC cxx_std_20)
target_include_directories(viper PUBLIC compiler/include)

target_link_libraries(viper_tests LINK_PUBLIC viper_framework)
target_compile_features(viper_tests PUBLIC cxx_std_20)
target_include_directories(viper_tests PUBLIC tests/include)