#include "list_tests.h"

using namespace uxs_test_suite;

namespace {

template<typename Ty = T>
int test_swap() {
    test_allocator<void> al1, al2;
    uxs::list<Ty, test_allocator<Ty>> v1(al1), l2(al2);

    v1.swap(l2);
    CHECK_EMPTY(v1);
    VERIFY(v1.get_allocator() == al2);
    CHECK_EMPTY(l2);
    VERIFY(l2.get_allocator() == al1);

    std::initializer_list<Ty> tst1 = {1, 2, 3, 4, 5};
    v1.assign(tst1);
    v1.swap(l2);
    CHECK_EMPTY(v1);
    VERIFY(v1.get_allocator() == al1);
    CHECK(l2, tst1.size(), tst1.begin());
    VERIFY(l2.get_allocator() == al2);

    v1.swap(l2);
    CHECK(v1, tst1.size(), tst1.begin());
    VERIFY(v1.get_allocator() == al2);
    CHECK_EMPTY(l2);
    VERIFY(l2.get_allocator() == al1);

    std::initializer_list<Ty> tst2 = {10, 11, 12, 13, 14, 15, 16, 17};
    l2.assign(tst2);
    v1.swap(l2);
    CHECK(v1, tst2.size(), tst2.begin());
    VERIFY(v1.get_allocator() == al1);
    CHECK(l2, tst1.size(), tst1.begin());
    VERIFY(l2.get_allocator() == al2);
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "list", test_swap);
