set(REELAY_CATCH2_VERSION "3.11.0" CACHE STRING "Version of Catch2 to fetch for tests")

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v${REELAY_CATCH2_VERSION} # change version as needed
    FIND_PACKAGE_ARGS ${REELAY_CATCH2_VERSION_MIN}
)
FetchContent_MakeAvailable(Catch2)

include(Catch)
include(CTest)

add_executable(
  reelay_tests
  src/dense_timed.test.cpp
  src/dense_timed_data.test.cpp
  src/dense_timed_robustness_0.test.cpp
  src/discrete_timed.test.cpp
  src/discrete_timed_data.test.cpp
  src/discrete_timed_robustness.test.cpp)

target_link_libraries(reelay_tests PRIVATE reelay::reelay)
target_link_libraries(reelay_tests PRIVATE Catch2::Catch2WithMain)

catch_discover_tests(reelay_tests)
