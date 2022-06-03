#include "test_suite.h"

#include "uxs/format.h"
#include "uxs/io/filebuf.h"
#include "uxs/io/iobuf_iterator.h"
#include "uxs/io/istringbuf.h"
#include "uxs/io/ostringbuf.h"

#include <random>
#include <sstream>

extern std::string g_testdata_path;

namespace {

class sdev : public uxs::iodevice {
 public:
    sdev() = default;
    explicit sdev(std::string str) : str_(str) {}
    const std::string& str() const { return str_; }

 protected:
    int read(void* data, size_type sz, size_type& n_read) override {
        n_read = std::min(sz, str_.size() - pos_);
        std::copy_n(&str_[pos_], n_read, reinterpret_cast<char*>(data));
        pos_ += n_read;
        return 0;
    }

    int write(const void* data, size_type sz, size_type& n_written) override {
        const char* p = reinterpret_cast<const char*>(data);
        size_type sz1 = std::min(str_.size() - pos_, sz);
        std::copy_n(p, sz1, &str_[pos_]);
        p += sz1;
        str_.append(p, sz - sz1);
        n_written = sz;
        pos_ += n_written;
        return 0;
    }

    int64_t seek(int64_t off, uxs::seekdir dir) override {
        switch (dir) {
            case uxs::seekdir::kBeg: {
                VERIFY(off >= 0);
                pos_ = off;
            } break;
            case uxs::seekdir::kCurr: {
                VERIFY(off >= 0 || static_cast<size_type>(-off) <= pos_);
                pos_ += off;
            } break;
            case uxs::seekdir::kEnd: {
                VERIFY(off >= 0 || static_cast<size_type>(-off) <= str_.size());
                pos_ = str_.size() + off;
            } break;
        }
        if (pos_ > str_.size()) { str_.resize(pos_); }
        return pos_;
    }

    int ctrlesc_color(uxs::span<uint8_t> v) override { return -1; }
    int flush() override { return 0; }

 private:
    std::string str_;
    size_type pos_ = 0;
};

const int brute_N = 10000000;

int test_iobuf_crlf() {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 127);

    uxs::stdbuf::out.write("      \b\b\b\b\b\b").flush();

    uxs::ostringbuf ss;
    std::ostringstream ss_ref;
    while (ss_ref.tellp() < brute_N) {
        VERIFY(ss.tell() == ss_ref.tellp());
        for (unsigned n = distribution(generator); n > 0; --n) {
            char ch = ' ' + static_cast<char>(distribution(generator) % ('z' - ' '));
            ss.put(ch);
            ss_ref.put(ch);
        }
        ss.put('\n');
        ss_ref.put('\n');
    }

    std::string str = ss.str();
    VERIFY(str == ss_ref.str());

    sdev middev;

    {
        uxs::istringbuf ifile(str);
        uxs::devbuf middle(middev, uxs::iomode::kOut | uxs::iomode::kCrLf);

        uxs::ibuf_iterator in(ifile), in_end{};
        uxs::obuf_iterator out(middle);
        std::copy(in, in_end, out);
    }

    VERIFY(middev.str().size() > str.size());
    for (unsigned n = 1; n < middev.str().size(); ++n) {
        VERIFY(middev.str()[n] != '\n' || middev.str()[n - 1] == '\r');
    }

    uxs::ostringbuf ss2;

    {
        sdev idev(middev.str());
        uxs::devbuf ifile(idev, uxs::iomode::kIn | uxs::iomode::kCrLf);

        uxs::ibuf_iterator in(ifile), in_end{};
        uxs::obuf_iterator out(ss2);
        std::copy(in, in_end, out);
    }

    VERIFY(str == ss2.str());
    return 0;
}

int test_iobuf_dev_sequential() {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 127);

    uxs::stdbuf::out.write("      \b\b\b\b\b\b").flush();

    sdev dev;
    std::ostringstream ss_ref;

    {
        uxs::devbuf ofile(dev, uxs::iomode::kOut);
        uxs::obuf_iterator out(ofile);
        std::ostreambuf_iterator<char> out_ref(ss_ref);

        while (ss_ref.tellp() < brute_N) {
            VERIFY(ofile.tell() == ss_ref.tellp());
            char ch = ' ' + static_cast<char>(distribution(generator) % ('z' - ' '));
            *out = ch;
            *out_ref = ch;
        }
        ss_ref.flush();
    }

    std::string str = dev.str();
    VERIFY(str.size() == brute_N);
    VERIFY(str == ss_ref.str());

    {
        uxs::devbuf ifile(dev, uxs::iomode::kIn);
        std::istringstream in_ss_ref(str);
        ifile.seek(0);

        uxs::ibuf_iterator in(ifile), in_end{};
        std::istreambuf_iterator<char> in_ref(in_ss_ref);
        VERIFY(in != in_end);
        do {
            VERIFY(ifile.tell() == in_ss_ref.tellg());
            VERIFY(*in == *in_ref);
            ++in, ++in_ref;
        } while (in != in_end);
        VERIFY(ifile.peek() == std::char_traits<char>::eof());
        VERIFY(in_ss_ref.peek() == std::char_traits<char>::eof());
    }

    return 0;
}

int test_iobuf_dev_sequential_str() {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 127);

    uxs::stdbuf::out.write("      \b\b\b\b\b\b").flush();

    uxs::ostringbuf ofile;
    std::ostringstream ss_ref;

    {
        uxs::obuf_iterator out(ofile);
        std::ostreambuf_iterator<char> out_ref(ss_ref);

        while (ss_ref.tellp() < brute_N) {
            VERIFY(ofile.tell() == ss_ref.tellp());
            char ch = ' ' + static_cast<char>(distribution(generator) % ('z' - ' '));
            *out = ch;
            *out_ref = ch;
        }
        ofile.flush();
        ss_ref.flush();
    }

    std::string str = ofile.str();
    VERIFY(str.size() == brute_N);
    VERIFY(str == ss_ref.str());

    {
        uxs::istringbuf ifile(str);
        std::istringstream in_ss_ref(str);

        uxs::ibuf_iterator in(ifile), in_end{};
        std::istreambuf_iterator<char> in_ref(in_ss_ref);
        VERIFY(in != in_end);
        do {
            VERIFY(ifile.tell() == in_ss_ref.tellg());
            VERIFY(*in == *in_ref);
            ++in, ++in_ref;
        } while (in != in_end);
        VERIFY(ifile.peek() == std::char_traits<char>::eof());
        VERIFY(in_ss_ref.peek() == std::char_traits<char>::eof());
    }

    return 0;
}

int test_iobuf_dev_sequential_block() {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 127);

    uxs::stdbuf::out.write("      \b\b\b\b\b\b").flush();

    sdev dev;
    std::ostringstream ss_ref;

    {
        uxs::devbuf ofile(dev, uxs::iomode::kOut);

        while (ss_ref.tellp() < brute_N) {
            VERIFY(ofile.tell() == ss_ref.tellp());
            unsigned sz = distribution(generator);
            char buf[256];
            for (unsigned n = 0; n < sz; ++n) {
                buf[n] = ' ' + static_cast<char>(distribution(generator) % ('z' - ' '));
            }
            ofile.write(uxs::as_span(buf, sz));
            ss_ref.write(buf, sz);
        }
        ss_ref.flush();
    }

    std::string str = dev.str();
    VERIFY(str.size() >= brute_N);
    VERIFY(str == ss_ref.str());

    {
        uxs::devbuf ifile(dev, uxs::iomode::kIn);
        std::istringstream in_ss_ref(str);
        ifile.seek(0);

        while (ifile) {
            VERIFY(ifile.tell() == in_ss_ref.tellg());
            unsigned sz = distribution(generator);
            char buf1[256], buf2[256];
            size_t n_read = ifile.read(uxs::as_span(buf1, sz));
            in_ss_ref.read(buf2, sz);
            VERIFY(static_cast<std::streamsize>(n_read) == in_ss_ref.gcount());
            VERIFY(std::equal(buf1, buf1 + n_read, buf2));
        }
        VERIFY(ifile.peek() == std::char_traits<char>::eof());
        VERIFY(in_ss_ref.peek() == std::char_traits<char>::eof());
    }

    return 0;
}

int test_iobuf_dev_sequential_block_str() {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 127);

    uxs::stdbuf::out.write("      \b\b\b\b\b\b").flush();

    uxs::ostringbuf ofile;
    std::ostringstream ss_ref;

    {
        while (ss_ref.tellp() < brute_N) {
            VERIFY(ofile.tell() == ss_ref.tellp());
            unsigned sz = distribution(generator);
            char buf[256];
            for (unsigned n = 0; n < sz; ++n) {
                buf[n] = ' ' + static_cast<char>(distribution(generator) % ('z' - ' '));
            }
            ofile.write(uxs::as_span(buf, sz));
            ss_ref.write(buf, sz);
        }
        ofile.flush();
        ss_ref.flush();
    }

    std::string str = ofile.str();
    VERIFY(str.size() >= brute_N);
    VERIFY(str == ss_ref.str());

    {
        uxs::istringbuf ifile(str);
        std::istringstream in_ss_ref(str);

        while (ifile) {
            VERIFY(ifile.tell() == in_ss_ref.tellg());
            unsigned sz = distribution(generator);
            char buf1[256], buf2[256];
            size_t n_read = ifile.read(uxs::as_span(buf1, sz));
            in_ss_ref.read(buf2, sz);
            VERIFY(static_cast<std::streamsize>(n_read) == in_ss_ref.gcount());
            VERIFY(std::equal(buf1, buf1 + n_read, buf2));
        }
        VERIFY(ifile.peek() == std::char_traits<char>::eof());
        VERIFY(in_ss_ref.peek() == std::char_traits<char>::eof());
    }

    return 0;
}

int test_iobuf_dev_random_block() {
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> distribution(0, 1000000000);

    sdev dev;
    uxs::ostringbuf ss_ref;

    int iter_count = brute_N;

    {
        uxs::devbuf ofile(dev, uxs::iomode::kOut);

        for (int i = 0, perc0 = -1; i < iter_count; ++i) {
            int perc = (500 * static_cast<int64_t>(i)) / iter_count;
            if (perc > perc0) {
                uxs::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
                perc0 = perc;
            }

            VERIFY(ofile.tell() == ss_ref.tell());
            unsigned sz = distribution(generator) % 128;
            unsigned tot_size = static_cast<unsigned>(ss_ref.seek(0, uxs::seekdir::kEnd));
            unsigned pos = distribution(generator) % (tot_size + std::max<unsigned>(tot_size / 1000, 16));
            char buf[256];
            for (unsigned n = 0; n < sz; ++n) {
                buf[n] = ' ' + static_cast<char>(distribution(generator) % ('z' - ' '));
            }

            ofile.seek(pos);
            ss_ref.seek(pos);
            VERIFY(ofile.tell() == ss_ref.tell());

            ofile.write(uxs::as_span(buf, sz));
            ss_ref.write(uxs::as_span(buf, sz));
        }
        ss_ref.flush();
    }

    std::string str = dev.str();
    VERIFY(str == ss_ref.str());

    {
        uxs::devbuf ifile(dev, uxs::iomode::kIn);
        uxs::istringbuf in_ss_ref(str);

        ifile.seek(0);
        for (int i = 0, perc0 = -1; i < iter_count; ++i) {
            int perc = 500 + (500 * static_cast<int64_t>(i)) / iter_count;
            if (perc > perc0) {
                uxs::print("{:3}.{}%\b\b\b\b\b\b", perc / 10, perc % 10).flush();
                perc0 = perc;
            }

            VERIFY(ifile.tell() == in_ss_ref.tell());
            unsigned sz = distribution(generator) % 128;
            unsigned pos = distribution(generator) % (str.size() > sz ? static_cast<unsigned>(str.size() - sz) : 0u);

            ifile.seek(pos);
            in_ss_ref.seek(pos);
            VERIFY(ifile.tell() == in_ss_ref.tell());

            char buf1[256], buf2[256];
            size_t n_read = ifile.read(uxs::as_span(buf1, sz));
            VERIFY(n_read == in_ss_ref.read(uxs::as_span(buf2, sz)));
            VERIFY(std::equal(buf1, buf1 + n_read, buf2));
            VERIFY(ifile.peek() == in_ss_ref.peek());
            ifile.clear();
            in_ss_ref.clear();
        }
    }

    return 0;
}

void test_iobuf_file_mode(uxs::iomode mode, std::string_view what_to_write, bool can_create_new,
                          bool can_open_when_existing, std::string_view what_to_write_when_existing,
                          std::string_view what_to_read_when_existing) {
    std::string fname = g_testdata_path + "test_file.txt";

    if (!(mode & uxs::iomode::kOut)) {
        {
            uxs::filebuf ifile(fname.c_str(), mode);
            VERIFY(!ifile);
        }

        VERIFY(mode & uxs::iomode::kIn);
        uxs::filebuf ofile(fname.c_str(), "w");
        VERIFY(ofile);
        ofile.write(what_to_write);
        ofile.close();

        {
            uxs::filebuf ifile(fname.c_str(), mode);
            VERIFY(ifile);

            std::vector<char> data;
            data.resize(what_to_write.size());
            VERIFY(ifile.read(data) == what_to_write.size());
            VERIFY(std::equal(data.begin(), data.end(), what_to_write.data()));
        }

        uxs::sysfile::remove(fname.c_str());
        return;
    }

    {  // try create non-existing file
        uxs::filebuf ofile(fname.c_str(), mode);
        VERIFY(!!ofile == can_create_new);

        if (ofile) {
            ofile.write(what_to_write);
            ofile.close();

            uxs::filebuf ifile(fname.c_str(), "r");
            VERIFY(ifile);

            std::vector<char> data;
            data.resize(what_to_write.size());
            VERIFY(ifile.read(data) == what_to_write.size());
            VERIFY(std::equal(data.begin(), data.end(), what_to_write.data()));
        } else {
            VERIFY(ofile.open(fname.c_str(), "w"));
            ofile.write(what_to_write);
        }
    }

    {  // try to work with existing file
        uxs::filebuf ofile(fname.c_str(), mode);
        VERIFY(!!ofile == can_open_when_existing);

        if (ofile) {
            ofile.write(what_to_write_when_existing);
            ofile.close();

            uxs::filebuf ifile(fname.c_str(), "r");
            VERIFY(ifile);

            std::vector<char> data;
            data.resize(what_to_read_when_existing.size());
            VERIFY(ifile.read(data) == what_to_read_when_existing.size());
            VERIFY(std::equal(data.begin(), data.end(), what_to_read_when_existing.data()));
        }
    }

    uxs::sysfile::remove(fname.c_str());
}

void test_iobuf_file_mode(const char* mode, std::string_view what_to_write, bool can_create_new,
                          bool can_open_when_existing, std::string_view what_to_write_when_existing,
                          std::string_view what_to_read_when_existing) {
    test_iobuf_file_mode(uxs::detail::iomode_from_str(mode, uxs::iomode::kNone), what_to_write, can_create_new,
                         can_open_when_existing, what_to_write_when_existing, what_to_read_when_existing);
}

int test_iobuf_file_modes() {
    test_iobuf_file_mode(uxs::iomode::kIn, "Hello, world", false, false, "", "");

    test_iobuf_file_mode(uxs::iomode::kOut, "Hello, world", false, true, "HELLO", "HELLO, world");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kTruncate, "Hello, world", false, true, "HELLO", "HELLO");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kTruncate | uxs::iomode::kAppend, "Hello, world", false, true,
                         "HELLO", "HELLO");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kAppend, "Hello, world", false, true, "HELLO",
                         "Hello, worldHELLO");

    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kExcl, "Hello, world", false, true, "HELLO", "HELLO, world");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kExcl | uxs::iomode::kTruncate, "Hello, world", false, true,
                         "HELLO", "HELLO");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kExcl | uxs::iomode::kTruncate | uxs::iomode::kAppend,
                         "Hello, world", false, true, "HELLO", "HELLO");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kExcl | uxs::iomode::kAppend, "Hello, world", false, true,
                         "HELLO", "Hello, worldHELLO");

    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kCreate, "Hello, world", true, true, "HELLO", "HELLO, world");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kCreate | uxs::iomode::kTruncate, "Hello, world", true, true,
                         "HELLO", "HELLO");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kCreate | uxs::iomode::kTruncate | uxs::iomode::kAppend,
                         "Hello, world", true, true, "HELLO", "HELLO");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kCreate | uxs::iomode::kAppend, "Hello, world", true, true,
                         "HELLO", "Hello, worldHELLO");

    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kCreate | uxs::iomode::kExcl, "Hello, world", true, false, "",
                         "");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kCreate | uxs::iomode::kExcl | uxs::iomode::kTruncate,
                         "Hello, world", true, false, "", "");
    test_iobuf_file_mode(
        uxs::iomode::kOut | uxs::iomode::kCreate | uxs::iomode::kExcl | uxs::iomode::kTruncate | uxs::iomode::kAppend,
        "Hello, world", true, false, "", "");
    test_iobuf_file_mode(uxs::iomode::kOut | uxs::iomode::kCreate | uxs::iomode::kExcl | uxs::iomode::kAppend,
                         "Hello, world", true, false, "", "");

    test_iobuf_file_mode("r", "Hello, world", false, false, "", "");
    test_iobuf_file_mode("r+", "Hello, world", false, true, "HELLO", "HELLO, world");
    test_iobuf_file_mode("w", "Hello, world", true, true, "HELLO", "HELLO");
    test_iobuf_file_mode("w+", "Hello, world", true, true, "HELLO", "HELLO");
    test_iobuf_file_mode("a", "Hello, world", true, true, "HELLO", "Hello, worldHELLO");
    test_iobuf_file_mode("a+", "Hello, world", true, true, "HELLO", "Hello, worldHELLO");
    test_iobuf_file_mode("wx", "Hello, world", true, false, "", "");
    test_iobuf_file_mode("w+x", "Hello, world", true, false, "", "");
    test_iobuf_file_mode("ax", "Hello, world", true, false, "", "");
    test_iobuf_file_mode("a+x", "Hello, world", true, false, "", "");
    return 0;
}

int test_iobuf_file_text_mode() {
    std::string fname = g_testdata_path + "test_file.txt";
    std::string_view txt = "hello\n\nhello\n";
    std::string_view crlf_txt = "hello\r\n\r\nhello\r\n";

    uxs::filebuf ofile(fname.c_str(),
                       uxs::iomode::kOut | uxs::iomode::kCreate | uxs::iomode::kTruncate | uxs::iomode::kCrLf);
    VERIFY(ofile);
    ofile.write(txt);
    ofile.close();

    {
        uxs::filebuf ifile(fname.c_str(), uxs::iomode::kIn);
        VERIFY(ifile);

        std::vector<char> data;
        data.resize(crlf_txt.size());
        VERIFY(ifile.read(data) == crlf_txt.size());
        VERIFY(std::equal(data.begin(), data.end(), crlf_txt.data()));
    }

    {
        uxs::filebuf ifile(fname.c_str(), uxs::iomode::kIn | uxs::iomode::kCrLf);
        VERIFY(ifile);

        std::vector<char> data;
        data.resize(txt.size());
        VERIFY(ifile.read(data) == txt.size());
        VERIFY(std::equal(data.begin(), data.end(), txt.data()));
    }

    uxs::sysfile::remove(fname.c_str());
    return 0;
}

}  // namespace

ADD_TEST_CASE("", "iobuf", test_iobuf_file_modes);
ADD_TEST_CASE("", "iobuf", test_iobuf_file_text_mode);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_crlf);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_dev_sequential);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_dev_sequential_str);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_dev_sequential_block);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_dev_sequential_block_str);
ADD_TEST_CASE("1-bruteforce", "iobuf", test_iobuf_dev_random_block);
