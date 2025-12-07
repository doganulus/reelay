add_executable(rybinx)

target_sources(rybinx PRIVATE "main.cpp")
target_link_libraries(rybinx PRIVATE reelay::reelay)

install(TARGETS rybinx)
