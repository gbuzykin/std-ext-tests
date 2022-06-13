#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_emplace_back_to_empty() {
    test_allocator<void> al;

    {
        int tst[] = {10};
        uxs::list<Ty, test_allocator<Ty>> l(al);
        auto* p = &l.emplace_back(10);
        VERIFY(p == &l.back());
        CHECK(l, 1, tst);
        VERIFY(T::instance_count == 1);
        VERIFY(al.get_alloc_detected() == 1);
        VERIFY(al.get_alloc_count() == 1);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

template<typename Ty = T>
int test_emplace_back() {
    test_allocator<void> al;

    {
        std::initializer_list<Ty> tst_prev = {1, 2, 3, 4, 5};
        int tst[] = {1, 2, 3, 4, 5, 6};
        uxs::list<Ty, test_allocator<Ty>> l(tst_prev, al);
        auto* p = &l.emplace_back(6);
        VERIFY(p == &l.back());
        CHECK(l, 6, tst);
        VERIFY(T::instance_count == 11);
        VERIFY(al.get_alloc_detected() == 6);
        VERIFY(al.get_alloc_count() == 6);
    }

    VERIFY(T::instance_count == 0);
    VERIFY(al.get_alloc_count() == 0);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_emplace_back_to_empty);
ADD_TEST_CASE("", "list", test_emplace_back);
