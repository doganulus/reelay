find_package(SIMDJSON REQUIRED)

add_executable(ryjson)

target_sources(ryjson PRIVATE "main.cpp")
target_link_libraries(ryjson PRIVATE reelay::reelay simdjson::simdjson)

install(TARGETS ryjson)
