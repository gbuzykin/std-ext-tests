#include "rbtree_tests.h"

#include "uxs/set.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_initializer_empty_to_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init;
        uxs::set<Ty, std::less<Ty>, test_allocator<Ty>> s(al);
        s = init;
        CHECK_EMPTY(s);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_initializer_not_empty_to_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 3, 3, 4, 4, 5};
        int tst[] = {1, 2, 3, 4, 5};
        uxs::set<Ty, std::less<Ty>, test_allocator<Ty>> s(al);
        s = init;
        CHECK(s, 5, tst);
        VERIFY(T::instance_count == 13);
        VERIFY(T::not_empty_count == 13);
        VERIFY(al.get_alloc_detected() == 8);
        VERIFY(al.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_initializer_more() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> init2 = {11, 12, 12, 13, 14, 14, 14, 15, 15, 16, 16, 17};
        int tst[] = {11, 12, 13, 14, 15, 16, 17};
        uxs::set<Ty, std::less<Ty>, test_allocator<Ty>> s(init, al);
        s = init2;
        CHECK(s, 7, tst);
        VERIFY(T::instance_count == 24);
        VERIFY(T::not_empty_count == 24);
        VERIFY(al.get_alloc_detected() == 9);
        VERIFY(al.get_alloc_count() == 7);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_initializer_less() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5, 6, 7};
        std::initializer_list<Ty> init2 = {11, 12, 12, 13, 14, 14, 14, 15};
        int tst[] = {11, 12, 13, 14, 15};
        uxs::set<Ty, std::less<Ty>, test_allocator<Ty>> s(init, al);
        s = init2;
        CHECK(s, 5, tst);
        VERIFY(T::instance_count == 20);
        VERIFY(T::not_empty_count == 20);
        VERIFY(al.get_alloc_detected() == 7);
        VERIFY(al.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_initializer_same_amount() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> init2 = {11, 11, 12, 12, 13, 14, 15, 15, 15};
        int tst[] = {11, 12, 13, 14, 15};
        uxs::set<Ty, std::less<Ty>, test_allocator<Ty>> s(init, al);
        s = init2;
        CHECK(s, 5, tst);
        VERIFY(T::instance_count == 19);
        VERIFY(T::not_empty_count == 19);
        VERIFY(al.get_alloc_detected() == 7);
        VERIFY(al.get_alloc_count() == 5);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_initializer_empty_to_not_empty() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> init = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> init2;
        uxs::set<Ty, std::less<Ty>, test_allocator<Ty>> s(init, al);
        s = init2;
        CHECK_EMPTY(s);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == 5);
        VERIFY(al.get_alloc_count() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "set", test_initializer_empty_to_empty);
ADD_TEST_CASE("", "set", test_initializer_not_empty_to_empty);
ADD_TEST_CASE("", "set", test_initializer_more);
ADD_TEST_CASE("", "set", test_initializer_less);
ADD_TEST_CASE("", "set", test_initializer_same_amount);
ADD_TEST_CASE("", "set", test_initializer_empty_to_not_empty);
