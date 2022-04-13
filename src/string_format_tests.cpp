#if defined(_MSC_VER)
#    define _CRT_SECURE_NO_WARNINGS
#endif

#include "test_suite.h"
#include "util/format.h"

#if !defined(_MSC_VER) || _MSC_VER >= 1920
#    include "fmt/format.h"
#endif

#include <cstdio>
#include <iomanip>
#include <sstream>

namespace {

int test_string_format_0() {
    VERIFY(util::format("{:.10f}:{:04}:{:+}:{}:{}:{}:%\n", 1.234, 42, 3.13, "str", (void*)1000, 'X') ==
           "1.2340000000:0042:+3.13:str:0x3e8:X:%\n");
    return 0;
}

int test_string_format_1() {
    VERIFY(util::format("abcdefghi") == "abcdefghi");
    VERIFY(util::format("{}abcdefghi", "A") == "Aabcdefghi");
    VERIFY(util::format("{}abcdefghi{}", "A", "B") == "AabcdefghiB");
    VERIFY(util::format("{}{}abcdefghi{}", "A", "C", "B") == "ACabcdefghiB");

    VERIFY(util::format("{}{}{}{}", "1", "2", "3", "4") == "1234");
    VERIFY(util::format("{:*>8}", 1) == "*******1");
    VERIFY(util::format("{:*>8.2f}", 2.34) == "****2.34");
    VERIFY(util::format("{:.2f}", 2.34) == "2.34");

    VERIFY(util::format("{3}{2}{1}{0}", "1", "2", "3", "4") == "4321");
    VERIFY(util::format("{2}{1}{1}{0}", "1", "2", "3") == "3221");
    VERIFY(util::format("{2:=^{1}.{0}f}", 3, 7, 1.23) == "=1.230=");

    const char* s1 = "world";
    char* ncs1 = const_cast<char*>("world");
    std::string_view s2 = "world";
    std::string s3 = "world";
    VERIFY(util::format("Hello, {}!", "world") == "Hello, world!");
    VERIFY(util::format("Hello, {}!", s1) == "Hello, world!");
    VERIFY(util::format("Hello, {}!", ncs1) == "Hello, world!");
    VERIFY(util::format("Hello, {}!", s2) == "Hello, world!");
    VERIFY(util::format("Hello, {}!", s3) == "Hello, world!");
    VERIFY(util::format("Hello, {}!", s3 + "123") == "Hello, world123!");
    VERIFY(util::format("Hello, {}!", (void*)1000) == "Hello, 0x3e8!");
    VERIFY(util::format("Hello, {}!", (const void*)1000) == "Hello, 0x3e8!");
    VERIFY(util::format("Hello, {}!", (const int*)1000) == "Hello, 0x3e8!");

    VERIFY(util::format("{:.^15}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82.....");
    VERIFY(util::format("{:.^15.6}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82.....");
    VERIFY(util::format("{:.^15.8}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82.....");
    VERIFY(util::format("{:.^15.4}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           ".....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2......");
    VERIFY(util::format("{:.^15.0}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") == "...............");
    VERIFY(util::format("{:.6}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82");
    VERIFY(util::format("{:.4}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2");
    VERIFY(util::format("{:.0}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") == "");
    VERIFY(util::format("{:2.4}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2");

    VERIFY(util::format("{:.^15}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0....");
    VERIFY(util::format("{:.^15.6}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82.....");
    VERIFY(util::format("{:.^15.7}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0....");
    VERIFY(util::format("{:.^15.8}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0....");

    return 0;
}

int perf(int iter_count) {
    int result = 0;

    auto start = std::clock();
    for (int iter = 0; iter < iter_count; ++iter) {
        std::string s = util::format("{:.10f}:{:04}:{:+}:{}:{}:{}:%\n", 1.234, 42, 3.13, "str", (void*)1000, 'X');
        result += static_cast<int>(s.size());
    }

    return result != 0 ? static_cast<int>(std::clock() - start) : 0;
}

int perf_libc(int iter_count) {
    int result = 0;
    char buf[128];

    auto start = std::clock();
    for (int iter = 0; iter < iter_count; ++iter) {
        result += std::sprintf(buf, "%0.10f:%04d:%+g:%s:%p:%c:%%\n", 1.234, 42, 3.13, "str", (void*)1000, (int)'X');
    }

    return result != 0 ? static_cast<int>(std::clock() - start) : 0;
}

int perf_std(int iter_count) {
    int result = 0;

    auto start = std::clock();
    for (int iter = 0; iter < iter_count; ++iter) {
        std::ostringstream ss;
        ss << std::setprecision(10) << std::fixed << 1.234 << ':' << std::resetiosflags(std::ios::floatfield)
           << std::setw(4) << std::setfill('0') << 42 << std::setfill(' ') << ':' << std::setiosflags(std::ios::showpos)
           << 3.13 << std::resetiosflags(std::ios::showpos) << ':' << "str" << ':' << (void*)1000 << ':' << 'X'
           << ":%\n"
           << std::flush;
        std::string s = std::move(ss).str();
        result += static_cast<int>(s.size());
    }

    return result != 0 ? static_cast<int>(std::clock() - start) : 0;
}

#if !defined(_MSC_VER) || _MSC_VER >= 1920
int perf_fmt(int iter_count) {
    int result = 0;

    auto start = std::clock();
    for (int iter = 0; iter < iter_count; ++iter) {
        std::string s = fmt::format("{:.10f}:{:04}:{:+}:{}:{}:{}:%\n", 1.234, 42, 3.13, "str", (void*)1000, 'X');
        result += static_cast<int>(s.size());
    }

    return result != 0 ? static_cast<int>(std::clock() - start) : 0;
}
#endif

const int perf_N = 2000000;
int test_perf() { return perf(perf_N); }
int test_perf_libc() { return perf_libc(perf_N); }
int test_perf_std() { return perf_std(perf_N); }
#if !defined(_MSC_VER) || _MSC_VER >= 1920
int test_perf_fmt() { return perf_fmt(perf_N); }
#endif

}  // namespace

ADD_TEST_CASE("", "string format", test_string_format_0);
ADD_TEST_CASE("", "string format", test_string_format_1);

ADD_TEST_CASE("2-perf", "string format", test_perf);
ADD_TEST_CASE("2-perf", "<libc> string format", test_perf_libc);
ADD_TEST_CASE("2-perf", "<std> string format", test_perf_std);
#if !defined(_MSC_VER) || _MSC_VER >= 1920
ADD_TEST_CASE("2-perf", "<fmt> string format", test_perf_fmt);
#endif
