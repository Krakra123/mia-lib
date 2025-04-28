if(BUILD_TESTING)
    set(TEST_NAME ${PROJECT_NAME}_test)
 
    # FIXME:
    add_executable(${TEST_NAME}
        # utilities/utilities-test.cpp
        ./math/vector-test.cpp
    )
    
    target_include_directories(${TEST_NAME} PRIVATE 
        ../include
    )
    
    target_link_libraries(${TEST_NAME} PRIVATE
        gtest
        gtest_main
    )
    
    # Add test to CTest
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endif()
