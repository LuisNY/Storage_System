include_directories(
        .# top level
        ..
        src
)

add_executable(storage_system src/main.cpp)

target_link_libraries(storage_system cache)