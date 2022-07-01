#include "../rbtree_bruteforce.h"

#include "uxs/pool_allocator.h"

using namespace uxs_test_suite;

namespace {

#if defined(NDEBUG)
static const int brute_N = 50000;
#else   // defined(NDEBUG)
static const int brute_N = 500;
#endif  // defined(NDEBUG)

int test_bruteforce() {
    rbtree_bruteforce_test<uxs::set<T, std::less<T>, uxs::global_pool_allocator<T>>>(brute_N);
    return 0;
}

}  // namespace

ADD_TEST_CASE("1-bruteforce", "set", test_bruteforce);
