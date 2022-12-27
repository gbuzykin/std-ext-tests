#if defined(_MSC_VER)
#    define _CRT_SECURE_NO_WARNINGS
#endif

#include "uxs/format.h"

#include "test_suite.h"

#include "uxs/guid.h"

#if !defined(_MSC_VER) || _MSC_VER > 1800
#    include "fmt/format.h"
#endif

#if defined(_MSC_VER) && __cplusplus >= 201703L
#    include <charconv>
#endif

#define MUST_THROW(x) \
    try { \
        x; \
        VERIFY(false); \
    } catch (const uxs::format_error&) {}

#include <cmath>
#include <cstdio>
#include <iomanip>
#include <locale>
#include <sstream>

using namespace uxs_test_suite;

static_assert(uxs::sfmt::type_id_t<unsigned char>::value == uxs::sfmt::arg_type_id::kUnsignedChar, "");
static_assert(uxs::sfmt::type_id_t<unsigned short>::value == uxs::sfmt::arg_type_id::kUnsignedShort, "");
static_assert(uxs::sfmt::type_id_t<unsigned>::value == uxs::sfmt::arg_type_id::kUnsigned, "");
static_assert(uxs::sfmt::type_id_t<unsigned long>::value == uxs::sfmt::arg_type_id::kUnsignedLong, "");
static_assert(uxs::sfmt::type_id_t<unsigned long long>::value == uxs::sfmt::arg_type_id::kUnsignedLongLong, "");
static_assert(uxs::sfmt::type_id_t<signed char>::value == uxs::sfmt::arg_type_id::kSignedChar, "");
static_assert(uxs::sfmt::type_id_t<signed short>::value == uxs::sfmt::arg_type_id::kSignedShort, "");
static_assert(uxs::sfmt::type_id_t<signed>::value == uxs::sfmt::arg_type_id::kSigned, "");
static_assert(uxs::sfmt::type_id_t<signed long>::value == uxs::sfmt::arg_type_id::kSignedLong, "");
static_assert(uxs::sfmt::type_id_t<signed long long>::value == uxs::sfmt::arg_type_id::kSignedLongLong, "");
static_assert(uxs::sfmt::type_id_t<char>::value == uxs::sfmt::arg_type_id::kChar, "");
static_assert(uxs::sfmt::type_id_t<wchar_t>::value == uxs::sfmt::arg_type_id::kWChar, "");
static_assert(uxs::sfmt::type_id_t<bool>::value == uxs::sfmt::arg_type_id::kBool, "");
static_assert(uxs::sfmt::type_id_t<float>::value == uxs::sfmt::arg_type_id::kFloat, "");
static_assert(uxs::sfmt::type_id_t<double>::value == uxs::sfmt::arg_type_id::kDouble, "");
static_assert(uxs::sfmt::type_id_t<long double>::value == uxs::sfmt::arg_type_id::kLongDouble, "");
#if defined(_MSC_VER)
static_assert(uxs::sfmt::type_id_t<unsigned __int64>::value == uxs::sfmt::arg_type_id::kUnsignedLongLong, "");
static_assert(uxs::sfmt::type_id_t<signed __int64>::value == uxs::sfmt::arg_type_id::kSignedLongLong, "");
#endif  // defined(_MSC_VER)

namespace {

int test_string_format_0() {
    VERIFY(uxs::format("{:.10f}:{:04}:{:+}:{}:{}:{}:%\n", 1.234, 42, 3.13, "str", (void*)1000, 'X') ==
           "1.2340000000:0042:+3.13:str:0x3e8:X:%\n");
    return 0;
}

int test_string_format_1() {
    VERIFY(uxs::format("abcdefghi") == "abcdefghi");
    VERIFY(uxs::format("{}abcdefghi", "A") == "Aabcdefghi");
    VERIFY(uxs::format("{}abcdefghi{}", "A", "B") == "AabcdefghiB");
    VERIFY(uxs::format("{}{}abcdefghi{}", "A", "C", "B") == "ACabcdefghiB");

    VERIFY(uxs::format("{}{}{}{}", "1", "2", "3", "4") == "1234");
    VERIFY(uxs::format("{:*>8}", 1) == "*******1");
    VERIFY(uxs::format("{:*>8.2f}", 2.34) == "****2.34");
    VERIFY(uxs::format("{:.2f}", 2.34) == "2.34");

    VERIFY(uxs::format("{3}{2}{1}{0}", "1", "2", "3", "4") == "4321");
    VERIFY(uxs::format("{2}{1}{1}{0}", "1", "2", "3") == "3221");
    VERIFY(uxs::format("{2:=^{1}.{0}f}", 3, 7, 1.23) == "=1.230=");

    const char* s1 = "world";
    char* ncs1 = const_cast<char*>("world");
    std::string_view s2 = "world";
    std::string s3 = "world";
    VERIFY(uxs::format("Hello, {}!", "world") == "Hello, world!");
    VERIFY(uxs::format("Hello, {}!", s1) == "Hello, world!");
    VERIFY(uxs::format("Hello, {}!", ncs1) == "Hello, world!");
    VERIFY(uxs::format("Hello, {}!", s2) == "Hello, world!");
    VERIFY(uxs::format("Hello, {}!", s3) == "Hello, world!");
    VERIFY(uxs::format("Hello, {}!", s3 + "123") == "Hello, world123!");
    VERIFY(uxs::format("Hello, {}!", (void*)1000) == "Hello, 0x3e8!");
    VERIFY(uxs::format("Hello, {}!", (const void*)1000) == "Hello, 0x3e8!");
    VERIFY(uxs::format("Hello, {}!", (const int*)1000) == "Hello, 0x3e8!");

    VERIFY(uxs::format("{:.^15}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82.....");
    VERIFY(uxs::format("{:.^15.6}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82.....");
    VERIFY(uxs::format("{:.^15.8}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82.....");
    VERIFY(uxs::format("{:.^15.4}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           ".....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2......");
    VERIFY(uxs::format("{:.^15.0}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") == "...............");
    VERIFY(uxs::format("{:.6}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82");
    VERIFY(uxs::format("{:.4}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2");
    VERIFY(uxs::format("{:.0}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") == "");
    VERIFY(uxs::format("{:2.4}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82") ==
           "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2");

    VERIFY(uxs::format("{:.^15}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0....");
    VERIFY(uxs::format("{:.^15.6}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82.....");
    VERIFY(uxs::format("{:.^15.7}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0....");
    VERIFY(uxs::format("{:.^15.8}", "\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0") ==
           "....\xd0\x9f\xd1\x80\xd0\xb8\xd0\xb2\xd0\xb5\xd1\x82\xc0....");

    return 0;
}

int test_string_format_2() {
    MUST_THROW((void)uxs::format(uxs::runtime("{"), 123));

    VERIFY(uxs::format(uxs::runtime("{}"), 123) == "123");
    VERIFY(uxs::format(uxs::runtime("{:+}"), 123) == "+123");
    MUST_THROW((void)uxs::format(uxs::runtime("{:+}"), 123u));
    VERIFY(uxs::format(uxs::runtime("{}"), 'A') == "A");
    MUST_THROW((void)uxs::format(uxs::runtime("{:+}"), 'A'));
    MUST_THROW((void)uxs::format(uxs::runtime("{:0}"), 'A'));
    VERIFY(uxs::format(uxs::runtime("{}"), true) == "true");
    MUST_THROW((void)uxs::format(uxs::runtime("{:+}"), true));
    VERIFY(uxs::format(uxs::runtime("{}"), 123.) == "123");
    VERIFY(uxs::format(uxs::runtime("{:+}"), 123.) == "+123");
    VERIFY(uxs::format(uxs::runtime("{}"), 123.f) == "123");
    VERIFY(uxs::format(uxs::runtime("{:+}"), 123.f) == "+123");
    VERIFY(uxs::format(uxs::runtime("{}"), reinterpret_cast<void*>(0x123)) == "0x123");
    MUST_THROW((void)uxs::format(uxs::runtime("{:+}"), reinterpret_cast<void*>(0x123)));
    VERIFY(uxs::format(uxs::runtime("{}"), "hello") == "hello");
    MUST_THROW((void)uxs::format(uxs::runtime("{:+}"), "hello"));
    MUST_THROW((void)uxs::format(uxs::runtime("{:+}"), "hello"));

    VERIFY(uxs::format(uxs::runtime("{:d}"), 123) == "123");
    VERIFY(uxs::format(uxs::runtime("{:+d}"), 123) == "+123");
    MUST_THROW((void)uxs::format(uxs::runtime("{:+d}"), 123u));
    VERIFY(uxs::format(uxs::runtime("{:d}"), 'A') == "65");
    VERIFY(uxs::format(uxs::runtime("{:+d}"), 'A') == "+65");
    VERIFY(uxs::format(uxs::runtime("{:d}"), true) == "1");
    MUST_THROW((void)uxs::format(uxs::runtime("{:+d}"), true));
    MUST_THROW((void)uxs::format(uxs::runtime("{:d}"), 123.));
    MUST_THROW((void)uxs::format(uxs::runtime("{:d}"), 123.f));
    MUST_THROW((void)uxs::format(uxs::runtime("{:d}"), reinterpret_cast<void*>(0x123)));
    MUST_THROW((void)uxs::format(uxs::runtime("{:d}"), "hello"));

    MUST_THROW((void)uxs::format(uxs::runtime("{:f}"), 123));
    MUST_THROW((void)uxs::format(uxs::runtime("{:f}"), 'A'));
    MUST_THROW((void)uxs::format(uxs::runtime("{:f}"), true));
    VERIFY(uxs::format(uxs::runtime("{:f}"), 123.) == "123.000000");
    VERIFY(uxs::format(uxs::runtime("{:+f}"), 123.) == "+123.000000");
    VERIFY(uxs::format(uxs::runtime("{:f}"), 123.f) == "123.000000");
    VERIFY(uxs::format(uxs::runtime("{:+f}"), 123.f) == "+123.000000");
    MUST_THROW((void)uxs::format(uxs::runtime("{:f}"), reinterpret_cast<void*>(0x123)));
    MUST_THROW((void)uxs::format(uxs::runtime("{:f}"), "hello"));

    VERIFY(uxs::format(uxs::runtime("{:c}"), 123) == "{");
    MUST_THROW((void)uxs::format(uxs::runtime("{:+c}"), 123));
    MUST_THROW((void)uxs::format(uxs::runtime("{:0c}"), 123));
    MUST_THROW((void)uxs::format(uxs::runtime("{:c}"), 1230));
    VERIFY(uxs::format(uxs::runtime("{:c}"), 'A') == "A");
    MUST_THROW((void)uxs::format(uxs::runtime("{:+c}"), 'A'));
    MUST_THROW((void)uxs::format(uxs::runtime("{:0c}"), 'A'));
    MUST_THROW((void)uxs::format(uxs::runtime("{:c}"), true));
    MUST_THROW((void)uxs::format(uxs::runtime("{:c}"), 123.));
    MUST_THROW((void)uxs::format(uxs::runtime("{:c}"), 123.f));
    MUST_THROW((void)uxs::format(uxs::runtime("{:c}"), reinterpret_cast<void*>(0x123)));
    MUST_THROW((void)uxs::format(uxs::runtime("{:c}"), "hello"));

    MUST_THROW((void)uxs::format(uxs::runtime("{:p}"), 123));
    MUST_THROW((void)uxs::format(uxs::runtime("{:p}"), 'A'));
    MUST_THROW((void)uxs::format(uxs::runtime("{:p}"), true));
    MUST_THROW((void)uxs::format(uxs::runtime("{:p}"), 123.));
    MUST_THROW((void)uxs::format(uxs::runtime("{:p}"), 123.f));
    VERIFY(uxs::format(uxs::runtime("{:p}"), reinterpret_cast<void*>(0x123)) == "0x123");
    MUST_THROW((void)uxs::format(uxs::runtime("{:+p}"), reinterpret_cast<void*>(0x123)));
    MUST_THROW((void)uxs::format(uxs::runtime("{:p}"), "hello"));

    MUST_THROW((void)uxs::format(uxs::runtime("{:s}"), 123));
    MUST_THROW((void)uxs::format(uxs::runtime("{:s}"), 'A'));
    VERIFY(uxs::format(uxs::runtime("{:s}"), true) == "true");
    MUST_THROW((void)uxs::format(uxs::runtime("{:+s}"), true));
    MUST_THROW((void)uxs::format(uxs::runtime("{:s}"), 123.));
    MUST_THROW((void)uxs::format(uxs::runtime("{:s}"), 123.f));
    MUST_THROW((void)uxs::format(uxs::runtime("{:s}"), reinterpret_cast<void*>(0x123)));
    VERIFY(uxs::format(uxs::runtime("{:s}"), "hello") == "hello");
    MUST_THROW((void)uxs::format(uxs::runtime("{:+s}"), "hello"));

    VERIFY(uxs::format(uxs::runtime(L"{}"), 'A') == L"A");
    MUST_THROW((void)uxs::format(uxs::runtime(L"{:+}"), 'A'));
    MUST_THROW((void)uxs::format(uxs::runtime(L"{:0}"), 'A'));
    VERIFY(uxs::format(uxs::runtime(L"{}"), L'A') == L"A");
    MUST_THROW((void)uxs::format(uxs::runtime(L"{:+}"), L'A'));
    MUST_THROW((void)uxs::format(uxs::runtime(L"{:0}"), L'A'));
    VERIFY(uxs::format(uxs::runtime(L"{}"), L"hello") == L"hello");
    MUST_THROW((void)uxs::format(uxs::runtime(L"{:+}"), L"hello"));
    MUST_THROW((void)uxs::format(uxs::runtime(L"{:0}"), L"hello"));
    VERIFY(uxs::format(uxs::runtime(L"{:c}"), 123) == L"{");
    MUST_THROW((void)uxs::format(uxs::runtime(L"{:+c}"), 123));
    MUST_THROW((void)uxs::format(uxs::runtime(L"{:0c}"), 123));
#if defined(_MSC_VER)
    MUST_THROW((void)uxs::format(uxs::runtime(L"{:c}"), 123000));
#endif
    VERIFY(uxs::format(uxs::runtime(L"{:c}"), 'A') == L"A");
    MUST_THROW((void)uxs::format(uxs::runtime(L"{:+c}"), 'A'));
    MUST_THROW((void)uxs::format(uxs::runtime(L"{:0c}"), 'A'));
    VERIFY(uxs::format(uxs::runtime(L"{:c}"), L'A') == L"A");
    MUST_THROW((void)uxs::format(uxs::runtime(L"{:+c}"), L'A'));
    MUST_THROW((void)uxs::format(uxs::runtime(L"{:0c}"), L'A'));
    VERIFY(uxs::format(uxs::runtime(L"{:s}"), L"hello") == L"hello");
    MUST_THROW((void)uxs::format(uxs::runtime(L"{:+s}"), L"hello"));
    MUST_THROW((void)uxs::format(uxs::runtime(L"{:0s}"), L"hello"));

    VERIFY(uxs::to_wstring(123.4556) == L"123.4556");
    VERIFY(uxs::format(L"{} {} {}", 123.4556, L"aaa", 567) == L"123.4556 aaa 567");

    std::string s{"{"};
    MUST_THROW((void)uxs::format(uxs::runtime(s)));
    return 0;
}

int test_string_format_3() {
    uxs::guid id(0x17364152, 0x36b4, 0x4b3e, 0x81, 0xba, 0x5e, 0x79, 0xa6, 0x81, 0xba, 0xee);
    VERIFY(uxs::format("{}", id) == "{17364152-36b4-4b3e-81ba-5e79a681baee}");
    VERIFY(uxs::format("{:=^42}", id) == "=={17364152-36b4-4b3e-81ba-5e79a681baee}==");
    return 0;
}

int test_string_format_4() {
    {
        char c = 120;
        VERIFY(uxs::format("{:6}", 42) == "    42");
        VERIFY(uxs::format("{:6}", 'x') == "x     ");
        VERIFY(uxs::format("{:*<6}", 'x') == "x*****");
        VERIFY(uxs::format("{:*>6}", 'x') == "*****x");
        VERIFY(uxs::format("{:*^6}", 'x') == "**x***");
        VERIFY(uxs::format("{:6d}", c) == "   120");
        VERIFY(uxs::format("{:6}", true) == "true  ");
    }
    {
        double inf = std::numeric_limits<double>::infinity();
        double nan = std::numeric_limits<double>::quiet_NaN();
        VERIFY(uxs::format("{0:},{0:+},{0:-},{0: }", 1) == "1,+1,1, 1");
        VERIFY(uxs::format("{0:},{0:+},{0:-},{0: }", -1) == "-1,-1,-1,-1");
        VERIFY(uxs::format("{0:},{0:+},{0:-},{0: }", inf) == "inf,+inf,inf, inf");
        VERIFY(uxs::format("{0:},{0:+},{0:-},{0: }", nan) == "nan,+nan,nan, nan");
    }
    {
        char c = 120;
        VERIFY(uxs::format("{:+06d}", c) == "+00120");
        VERIFY(uxs::format("{:#06x}", 0xa) == "0x000a");
        VERIFY(uxs::format("{:<06}", -42) == "-42   ");
    }
    {
        float pi = 3.14f;
        VERIFY(uxs::format("{:10f}", pi) == "  3.140000");
        VERIFY(uxs::format("{:{}f}", pi, 10) == "  3.140000");
        VERIFY(uxs::format("{:.5f}", pi) == "3.14000");
        VERIFY(uxs::format("{:.{}f}", pi, 5) == "3.14000");
        VERIFY(uxs::format("{:10.5f}", pi) == "   3.14000");
        VERIFY(uxs::format("{:{}.{}f}", pi, 10, 5) == "   3.14000");
        MUST_THROW((void)uxs::format(uxs::runtime("{:{}f}"), pi, 10.0));
        MUST_THROW((void)uxs::format(uxs::runtime("{:{}f}"), pi, -10));
        MUST_THROW((void)uxs::format(uxs::runtime("{:.{}f}"), pi, 5.0));
    }

    VERIFY(uxs::format("{}", nullptr) == "0x0");
    VERIFY(uxs::format("{:6}", nullptr) == "   0x0");
    VERIFY(uxs::format("{:6}", reinterpret_cast<void*>(1)) == "   0x1");
    VERIFY(uxs::format("{:6}", std::numeric_limits<double>::quiet_NaN()) == "nan   ");
    VERIFY(uxs::format("{:6f}", std::numeric_limits<double>::quiet_NaN()) == "nan   ");
    VERIFY(uxs::format("{:06f}", std::numeric_limits<double>::quiet_NaN()) == "   nan");
    VERIFY(uxs::format("{:6}", std::numeric_limits<double>::infinity()) == "inf   ");
    VERIFY(uxs::format("{:6e}", std::numeric_limits<double>::infinity()) == "inf   ");
    VERIFY(uxs::format("{:06e}", std::numeric_limits<double>::infinity()) == "   inf");
    VERIFY(uxs::format("{:6}", true) == "true  ");
    VERIFY(uxs::format("{:10}", "hello") == "hello     ");

    return 0;
}

int test_string_format_5() {
    struct grouping : std::numpunct<char> {
        char_type do_decimal_point() const override { return ','; }
        char_type do_thousands_sep() const override { return '\''; }
        string_type do_grouping() const override { return "\1\2\3"; }
    };

    std::locale loc{std::locale::classic(), new grouping};

    VERIFY(uxs::format(loc, "{:#Lb}", -25510) == "-0b110'001'110'100'11'0");
    VERIFY(uxs::format(loc, "{:#Lo}", -1452346510) == "-012'644'206'21'6");
    VERIFY(uxs::format(loc, "{:#Lx}", -1452346510) == "-0x56'910'c8'e");
    VERIFY(uxs::format(loc, "{:#017Lx}", -1452346510) == "-0x00056'910'c8'e");
    VERIFY(uxs::format(loc, "{:L}", 1452346510u) == "1'452'346'51'0");
    VERIFY(uxs::format(loc, "{:L}", -1452346510) == "-1'452'346'51'0");
    VERIFY(uxs::format(loc, "{:020L}", -1452346510) == "-000001'452'346'51'0");
    VERIFY(uxs::format(loc, "{:Lf}", 1452346510.0) == "1'452'346'51'0,000000");

    return 0;
}

ADD_TEST_CASE("", "string format", test_string_format_0);
ADD_TEST_CASE("", "string format", test_string_format_1);
ADD_TEST_CASE("", "string format", test_string_format_2);
ADD_TEST_CASE("", "string format", test_string_format_3);
ADD_TEST_CASE("", "string format", test_string_format_4);
ADD_TEST_CASE("", "string format", test_string_format_5);

//-----------------------------------------------------------------------------
// Performance tests

const int perf_N_secs = 5;

template<typename Func>
int perf_format_to_string(const Func& fn, int n_secs) {
    std::array<char, 128> buf;

    size_t len = 0;
    const int estimate_loop_count = 10000;
    const auto start0 = curr_clock_t::now();
    for (int i = 0; i < estimate_loop_count; ++i) {
        len = std::max<size_t>(len, fn(buf.data(), buf.data() + buf.size(), i));
    }
    const auto start = curr_clock_t::now();
    const int loop_count = static_cast<int>(
        std::ceil((estimate_loop_count * n_secs * 1000000000.0) / as_ns_duration(start - start0)));
    for (int i = 0; i < loop_count; ++i) {
        len = std::max<size_t>(len, fn(buf.data(), buf.data() + buf.size(), i % estimate_loop_count));
    }
    return len ? static_cast<int>(1000 * as_ns_duration(curr_clock_t::now() - start) / loop_count) : 0;
}

ADD_TEST_CASE("2-perf", "format string", ([]() {
                  return perf_format_to_string(
                      [](char* first, char* last, int i) {
                          return static_cast<size_t>(uxs::format_to(first, "{:.10f}:{:04}:{:+}:{}:{}:{}:%\n", 1.234 * i,
                                                                    i, 3.13 * i, "str",
                                                                    reinterpret_cast<void*>(static_cast<uintptr_t>(i)),
                                                                    static_cast<char>('A' + i % 26)) -
                                                     first);
                      },
                      perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "<libc> format string", ([]() {
                  return perf_format_to_string(
                      [](char* first, char* last, int i) {
                          return std::sprintf(first, "%0.10f:%04d:%+g:%s:%p:%c:%%\n", 1.234 * i, i, 3.13 * i, "str",
                                              reinterpret_cast<void*>(static_cast<uintptr_t>(i)),
                                              static_cast<char>('A' + i % 26));
                      },
                      perf_N_secs);
              }));
ADD_TEST_CASE("2-perf", "<std::ostringstream> format string", ([]() {
                  return perf_format_to_string(
                      [](char* first, char* last, int i) {
                          std::ostringstream ss;
                          ss << std::setprecision(10) << std::fixed << 1.234 * i << ':'
                             << std::resetiosflags(std::ios::floatfield) << std::setw(4) << std::setfill('0') << i
                             << std::setfill(' ') << ':' << std::setiosflags(std::ios::showpos) << 3.13 * i
                             << std::resetiosflags(std::ios::showpos) << ':' << "str" << ':'
                             << reinterpret_cast<void*>(static_cast<uintptr_t>(i)) << ':'
                             << static_cast<char>('A' + i % 26) << ":%\n"
                             << std::flush;
                          std::string s = std::move(ss).str();
                          return s.size();
                      },
                      perf_N_secs);
              }));
#if defined(_MSC_VER) && __cplusplus > 201703L
ADD_TEST_CASE("2-perf", "<c++17/20> format string", ([]() {
                  return perf_format_to_string(
                      [](char* first, char* last, int i) {
                          return static_cast<size_t>(std::format_to(first, "{:.10f}:{:04}:{:+}:{}:{}:{}:%\n", 1.234 * i,
                                                                    i, 3.13 * i, "str",
                                                                    reinterpret_cast<void*>(static_cast<uintptr_t>(i)),
                                                                    static_cast<char>('A' + i % 26)) -
                                                     first);
                      },
                      perf_N_secs);
              }));
#endif
#if !defined(_MSC_VER) || _MSC_VER > 1800
ADD_TEST_CASE("2-perf", "<{fmt}> format string", ([]() {
                  return perf_format_to_string(
                      [](char* first, char* last, int i) {
                          return static_cast<size_t>(fmt::format_to(first, "{:.10f}:{:04}:{:+}:{}:{}:{}:%\n", 1.234 * i,
                                                                    i, 3.13 * i, "str",
                                                                    reinterpret_cast<void*>(static_cast<uintptr_t>(i)),
                                                                    static_cast<char>('A' + i % 26)) -
                                                     first);
                      },
                      perf_N_secs);
              }));
#endif

}  // namespace
