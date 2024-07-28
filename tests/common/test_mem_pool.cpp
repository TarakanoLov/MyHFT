#include <gtest/gtest.h>

#include <common/mem_pool.h>

TEST(MemPool, Construct) {
    common::MemPool<std::string> pool(2);
    const auto [vec, next_free_index] = pool._only_for_tests_get_internals();
    EXPECT_EQ(next_free_index, 0);
    ASSERT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0].object_.empty(), true);
    EXPECT_EQ(vec[0].is_free_, true);
    EXPECT_EQ(vec[1].object_.empty(), true);
    EXPECT_EQ(vec[1].is_free_, true);
}

TEST(MemPool, Allocate) {
    common::MemPool<int> pool(2);
    const int* el1 = pool.allocate(42);
    EXPECT_EQ(*el1, 42);

    const auto [vec1, next_free_index1] = pool._only_for_tests_get_internals();
    EXPECT_EQ(next_free_index1, 0);
    ASSERT_EQ(vec1.size(), 2);
    EXPECT_EQ(vec1[0].object_, 42);
    EXPECT_EQ(vec1[0].is_free_, false);
    EXPECT_EQ(vec1[1].object_, 0);
    EXPECT_EQ(vec1[1].is_free_, true);

    const int* el2 = pool.allocate(24);
    EXPECT_EQ(*el2, 24);
    const auto [vec2, next_free_index2] = pool._only_for_tests_get_internals();
    EXPECT_EQ(next_free_index2, 1);
    ASSERT_EQ(vec2.size(), 2);
    EXPECT_EQ(vec2[0].object_, 42);
    EXPECT_EQ(vec2[0].is_free_, false);
    EXPECT_EQ(vec2[1].object_, 24);
    EXPECT_EQ(vec2[1].is_free_, false);
}

TEST(MemPool, Deallocate) {
    common::MemPool<std::string> pool(2);
    const auto* el1 = pool.allocate("some str");
    EXPECT_EQ(*el1, "some str");
    const auto* el2 = pool.allocate(5, '-');
    EXPECT_EQ(*el2, "-----");
    pool.deallocate(el1);
    
    const auto [vec, next_free_index] = pool._only_for_tests_get_internals();
    EXPECT_EQ(next_free_index, 1);
    ASSERT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0].object_, "some str");
    EXPECT_EQ(vec[0].is_free_, true);
    EXPECT_EQ(vec[1].object_, "-----");
    EXPECT_EQ(vec[1].is_free_, false);

    const auto* el3 = pool.allocate("new str");
    EXPECT_EQ(*el3, "new str");
    EXPECT_EQ(el1, el3);
    const auto [vec2, next_free_index2] = pool._only_for_tests_get_internals();
    EXPECT_EQ(next_free_index2, 0);
    ASSERT_EQ(vec2.size(), 2);
    EXPECT_EQ(vec2[0].object_, "new str");
    EXPECT_EQ(vec2[0].is_free_, false);
    EXPECT_EQ(vec2[1].object_, "-----");
    EXPECT_EQ(vec2[1].is_free_, false);
}