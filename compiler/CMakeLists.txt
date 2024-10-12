cmake_minimum_required(VERSION 3.8)

set(SOURCES
    "source/main.cpp"
)

set(HEADERS
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_executable(viper ${SOURCES} ${HEADERS})
add_executable(viper::compiler ALIAS viper)

target_include_directories(viper PUBLIC include)
target_compile_features(viper PUBLIC cxx_std_20)
target_link_libraries(viper viper::framework)