#include <gtest/gtest.h>

#include "arena/arena.hpp"

TEST(ArenaTest, ConstructorDestructorWorking) {
    EXPECT_NO_THROW({ mia::arena a; });
}

TEST(ArenaTest, ConstructorSizeDefault) {
    mia::arena a;
    EXPECT_EQ(a.capacity, 4096);
}

TEST(ArenaTest, ConstructorSizeWithValidInputCapacity) {
    mia::arena a(50);
    EXPECT_EQ(a.capacity, 50);
}

/*TEST(ArenaTest, ConstructorSizeWithInvalidInputCapacity) {*/
/*    mia::arena a(-1);*/
/*    EXPECT_DEATH({ mia::arena a(-1); }, "capacity must be positive");*/
/*}*/

TEST(ArenaTest, AllocateMemoryWithArgs) {
    struct TestStruct {
        int x;
        double y;

        TestStruct(int a, double b)
            : x(a)
            , y(b) {}
    };

    mia::arena a;

    TestStruct* obj = a.alloc<TestStruct>(42, 3.14);

    ASSERT_NE(obj, nullptr);
    EXPECT_EQ(obj->x, 42);
    EXPECT_DOUBLE_EQ(obj->y, 3.14);
}
