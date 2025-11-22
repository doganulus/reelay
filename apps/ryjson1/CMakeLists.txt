include(FetchContent)

fetchcontent_declare(
  simdjson
  GIT_REPOSITORY https://github.com/simdjson/simdjson.git
  GIT_TAG v3.6.0)
fetchcontent_makeavailable(simdjson)

add_executable(ryjson)

target_sources(ryjson PRIVATE "main.cpp")
target_link_libraries(ryjson PRIVATE reelay::reelay simdjson::simdjson)

install(TARGETS ryjson)
