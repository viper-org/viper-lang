cmake_minimum_required(VERSION 3.8)

set(SOURCES
    "source/test.cpp"
)

set(HEADERS
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(viper-framework-viper-framework ${SOURCES} ${HEADERS})
add_library(viper::framework ALIAS viper-framework-viper-framework)

target_include_directories(viper-framework-viper-framework PUBLIC include)
target_compile_features(viper-framework-viper-framework PUBLIC cxx_std_20)