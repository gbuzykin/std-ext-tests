#include "vector_tests.h"

#include "uxs/list.h"
#include "uxs/vector.h"

using namespace uxs_test_suite;

namespace {

template<typename Src>
int test_range_empty() {
    test_allocator<void> al;

    {
        Src tst;
        uxs::vector<T, test_allocator<T>> v(tst.begin(), tst.end(), al);
        CHECK_EMPTY(v);
        VERIFY(v.capacity() == 0);
        VERIFY(v.get_allocator() == al);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_detected() == 0);
    return 0;
}

template<typename Src>
int test_range_not_empty() {
    test_allocator<void> al;

    {
        Src tst = {1, 2, 3, 4, 5};
        uxs::vector<T, test_allocator<T>> v(tst.begin(), tst.end(), al);
        CHECK(v, 5, tst.begin());
        VERIFY(v.capacity() >= v.size());
        VERIFY(v.get_allocator() == al);
        VERIFY(T::instance_count == (std::is_same<typename Src::value_type, T>::value ? 10 : 5));
        VERIFY(T::not_empty_count == (std::is_same<typename Src::value_type, T>::value ? 10 : 5));
        VERIFY(al.get_alloc_detected() == v.capacity());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

int test_range_empty_random_access_range() { return test_range_empty<uxs::vector<T>>(); }
int test_range_not_empty_random_access_range() { return test_range_not_empty<uxs::vector<T>>(); }
int test_range_empty_generic_input_range() { return test_range_empty<uxs::list<T>>(); }
int test_range_not_empty_generic_input_range() { return test_range_not_empty<uxs::list<T>>(); }
int test_range_empty_random_access_range_int() { return test_range_empty<uxs::vector<int>>(); }
int test_range_not_empty_random_access_range_int() { return test_range_not_empty<uxs::vector<int>>(); }
int test_range_empty_generic_input_range_int() { return test_range_empty<uxs::list<int>>(); }
int test_range_not_empty_generic_input_range_int() { return test_range_not_empty<uxs::list<int>>(); }

}  // namespace

ADD_TEST_CASE("", "vector", test_range_empty_random_access_range);
ADD_TEST_CASE("", "vector", test_range_not_empty_random_access_range);
ADD_TEST_CASE("", "vector", test_range_empty_generic_input_range);
ADD_TEST_CASE("", "vector", test_range_not_empty_generic_input_range);
ADD_TEST_CASE("", "vector", test_range_empty_random_access_range_int);
ADD_TEST_CASE("", "vector", test_range_not_empty_random_access_range_int);
ADD_TEST_CASE("", "vector", test_range_empty_generic_input_range_int);
ADD_TEST_CASE("", "vector", test_range_not_empty_generic_input_range_int);
