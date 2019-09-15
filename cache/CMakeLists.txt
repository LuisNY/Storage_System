include_directories(
        .
        ..
        src
)

add_library(
        cache src/cache.h)

target_link_libraries(cache pthread)
set_target_properties(cache PROPERTIES LINKER_LANGUAGE CXX)