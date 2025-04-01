if(BUILD_TESTING)
    add_executable(mia-test
        arena/arena-test.cpp
        utilities/utilities-test.cpp
    )
    target_include_directories(mia-test PRIVATE 
        ../include
        ../dependencies/googletest/googletest/include
    )

    target_link_libraries(mia-test PRIVATE GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(mia-test)
endif()
