find_package(GTest REQUIRED)

add_executable(runUnitTests
    test_network.cpp
    ../NetworkManager.cpp
)

target_include_directories(runUnitTests PRIVATE ..)

target_link_libraries(runUnitTests
    GTest::gtest_main
    GTest::gmock_main
    Qt6::Core
)

include(GoogleTest)
gtest_discover_tests(runUnitTests)
