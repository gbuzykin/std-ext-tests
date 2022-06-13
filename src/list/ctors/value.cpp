#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_value_empty() {
    test_allocator<void> al;

    {
        uxs::list<Ty, test_allocator<Ty>> l(0, 10, al);
        CHECK_EMPTY(l);
        VERIFY(l.get_allocator() == al);
        VERIFY(T::instance_count == 0);
        VERIFY(al.get_alloc_detected() == 0);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_detected() == 0);
    return 0;
}

template<typename Ty = T>
int test_value_not_empty() {
    test_allocator<void> al;

    {
        int tst[] = {10, 10, 10, 10, 10};
        uxs::list<Ty, test_allocator<Ty>> l(5, 10, al);
        CHECK(l, 5, tst);
        VERIFY(l.get_allocator() == al);
        VERIFY(T::instance_count == 5);
        VERIFY(T::not_empty_count == 5);
        VERIFY(al.get_alloc_detected() == l.size());
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_value_empty);
ADD_TEST_CASE("", "list", test_value_not_empty);
