#include "vector_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_copy_empty_to_empty() {
    test_allocator<void> al, al2;

    {
        uxs::vector<Ty, test_allocator<Ty>> v(al), v_from(al2);
        v = v_from;
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(v.get_allocator() == al);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
        VERIFY(al2.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_copy_not_empty_to_empty() {
    test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> tst = {1, 2, 3, 4, 5};
        uxs::vector<Ty, test_allocator<Ty>> v(al), v_from(tst, al2);
        v = v_from;
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al);
        VERIFY(T::instance_count == 15);
        VERIFY(T::not_empty_count == 15);
        VERIFY(al.get_alloc_detected() == v.capacity());
        VERIFY(al2.get_alloc_detected() == v_from.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_copy_more_no_realloc() {
    test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> tst = {11, 12, 13, 14, 15, 16, 17};
        uxs::vector<Ty, test_allocator<Ty>> v(tst_prev, al), v_from(tst, al2);
        v.reserve(10);
        al.reset_alloc_detected();
        v = v_from;
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al);
        VERIFY(T::instance_count == 26);
        VERIFY(T::not_empty_count == 26);
        VERIFY(al.get_alloc_detected() == 0);
        VERIFY(al2.get_alloc_detected() == v_from.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty>
int test_copy_more_needs_realloc() {
    test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> tst = {11, 12, 13, 14, 15, 16, 17};
        uxs::vector<Ty, test_allocator<Ty>> v(tst_prev, al), v_from(tst, al2);
        v.shrink_to_fit();
        al.reset_alloc_detected();
        v = v_from;
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al);
        VERIFY(T::instance_count == 26);
        VERIFY(T::not_empty_count == 26);
        VERIFY(al.get_alloc_detected() == v.capacity());
        VERIFY(al2.get_alloc_detected() == v_from.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_copy_less() {
    test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5, 6, 7};
        std::initializer_list<Ty> tst = {11, 12, 13, 14, 15};
        uxs::vector<Ty, test_allocator<Ty>> v(tst_prev, al), v_from(tst, al2);
        v = v_from;
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al);
        VERIFY(T::instance_count == 22);
        VERIFY(T::not_empty_count == 22);
        VERIFY(al.get_alloc_detected() == v.capacity());
        VERIFY(al2.get_alloc_detected() == v_from.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_copy_same_amount() {
    test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        std::initializer_list<Ty> tst = {11, 12, 13, 14, 15};
        uxs::vector<Ty, test_allocator<Ty>> v(tst_prev, al), v_from(tst, al2);
        v = v_from;
        CHECK(v, tst.size(), tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al);
        VERIFY(T::instance_count == 20);
        VERIFY(T::not_empty_count == 20);
        VERIFY(al.get_alloc_detected() == v.capacity());
        VERIFY(al2.get_alloc_detected() == v_from.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_copy_empty_to_not_empty() {
    test_allocator<void> al, al2;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        uxs::vector<Ty, test_allocator<Ty>> v(tst_prev, al), v_from(al2);
        v = v_from;
        CHECK_EMPTY(v);
        VERIFY(v.get_allocator() == al);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == v.capacity());
        VERIFY(al2.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    VERIFY(al2.get_alloc_count() == 0);
    return 0;
}

int test_copy_more_needs_realloc_nothrow_move() { return test_copy_more_needs_realloc<T>(); }
int test_copy_more_needs_realloc_throwing_move() { return test_copy_more_needs_realloc<T_ThrowingMove>(); }

}  // namespace

ADD_TEST_CASE("", "vector", test_copy_empty_to_empty);
ADD_TEST_CASE("", "vector", test_copy_not_empty_to_empty);
ADD_TEST_CASE("", "vector", test_copy_more_no_realloc);
ADD_TEST_CASE("", "vector", test_copy_more_needs_realloc_nothrow_move);
ADD_TEST_CASE("", "vector", test_copy_more_needs_realloc_throwing_move);
ADD_TEST_CASE("", "vector", test_copy_less);
ADD_TEST_CASE("", "vector", test_copy_same_amount);
ADD_TEST_CASE("", "vector", test_copy_empty_to_not_empty);
