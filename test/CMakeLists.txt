if(BUILD_TESTING)
    add_executable(mia_test
        arena/arena-test.cpp
        utilities/utilities-test.cpp
    )
    target_include_directories(mia_test PRIVATE 
        ../include
        ../dependencies/googletest/googletest/include
    )

    target_link_libraries(mia_test PRIVATE GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(mia_test)
endif()
