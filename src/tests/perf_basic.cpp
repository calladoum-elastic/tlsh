#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#define let const auto

#include <stdio.h>
#include <tlsh.h>
#include <tlsh_util.h>

#include <array>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <memory>
#include <string_view>
#include <tuple>
#include <vector>

#define NS "TLSH"

template<typename T, auto Deleter>
using GenericHandle = std::unique_ptr<
    T,
    decltype(
        [](T* h)
        {
            if ( h )
            {
                Deleter(h);
                h = nullptr;
            }
        })>;

using UniqueHandle = GenericHandle<FILE, ::fclose>;

using namespace std::literals::string_view_literals;

const std::filesystem::path THIS_DIR{"."};
const std::filesystem::path DATASET_ROOT       = THIS_DIR / "./tests/datasets";
const std::filesystem::path LARGE_DATASET_ROOT = DATASET_ROOT / "large";

const std::filesystem::path SAMPLE_SMALL  = LARGE_DATASET_ROOT / "d3d12.dll";
const std::filesystem::path SAMPLE_MEDIUM = LARGE_DATASET_ROOT / "ntoskrnl.exe";
const std::filesystem::path SAMPLE_LARGE  = LARGE_DATASET_ROOT / "msedge.dll";

// Use
TEST_CASE("Hash Perf - reuse", "[" NS "]")
{
    SECTION("Sanity")
    {
        BENCHMARK("Check")
        {
            ::sleep(1);
        };
    }

    SECTION("TLSH tiny buffer hash (128B)")
    {
        let instr =
            "0684076d82f90066d54f1ccaa555f093a7627286555b0e5cc2e36da3ab41732e79636276530a861db7732eeb420bc2261af2a956f9bb2cf20876ca8819b4a43e"sv;
        let res = "8EA0022016221F9C51EE2F20BDAE164C82075B9DD563196665C4652425557542459C81"sv;

        std::vector<u8> inhex, outhex;
        from_hex(std::vector<u8>(instr.cbegin(), instr.cend()), inhex);
        auto t = Tlsh();
        BENCHMARK("TLSH (tiny buffer)")
        {
            t.reset();
            REQUIRE_FALSE(t.isValid());
            t.update(inhex);
            t.final();
            REQUIRE(t.isValid());
            CHECK(t.getHashString() == res);
        };
    }

    SECTION("TLSH small buffer hash (100KB)")
    {
        const auto sz = usize(158440);
        const auto fd = UniqueHandle{::fopen(SAMPLE_SMALL.c_str(), "r")};
        REQUIRE(fd != nullptr);

        std::vector<u8> in(sz);
        const auto cnt = ::fread(in.data(), sizeof(u8), sz, fd.get());
        REQUIRE(cnt == sz);

        auto t = Tlsh();
        BENCHMARK("TLSH (small buffer)")
        {
            t.reset();
            REQUIRE_FALSE(t.isValid());
            t.update(in);
            t.final();
            REQUIRE(t.isValid());
        };
    }

    SECTION("TLSH medium buffer hash (10MB)")
    {
        const auto sz = usize(12068320);
        const auto fd = UniqueHandle{::fopen(SAMPLE_MEDIUM.c_str(), "r")};
        REQUIRE(fd != nullptr);

        auto t = Tlsh();
        std::vector<u8> in(sz);
        const auto cnt = ::fread(in.data(), sizeof(u8), sz, fd.get());
        REQUIRE(cnt == sz);

        BENCHMARK("TLSH (medium buffer)")
        {
            t.reset();
            REQUIRE_FALSE(t.isValid());
            t.update(in);
            t.final();
        };
        REQUIRE(t.isValid());
    }

    SECTION("TLSH large buffer hash (100MB)")
    {
        const auto sz = usize(269893568);
        const auto fd = UniqueHandle{::fopen(SAMPLE_LARGE.c_str(), "r")};
        REQUIRE(fd != nullptr);

        auto t = Tlsh();
        std::vector<u8> in(sz);
        const auto cnt = ::fread(in.data(), sizeof(u8), sz, fd.get());
        REQUIRE(cnt == sz);

        BENCHMARK("TLSH (large buffer)")
        {
            t.reset();
            REQUIRE_FALSE(t.isValid());
            t.update(in);
            t.final();
        };
        REQUIRE(t.isValid());
    }
}

TEST_CASE("Hash Perf - no reuse", "[" NS "]")
{
    SECTION("Sanity")
    {
        BENCHMARK("Check")
        {
            ::sleep(1);
        };
    }

    SECTION("TLSH tiny buffer hash (128B)")
    {
        let instr =
            "0684076d82f90066d54f1ccaa555f093a7627286555b0e5cc2e36da3ab41732e79636276530a861db7732eeb420bc2261af2a956f9bb2cf20876ca8819b4a43e"sv;
        let res = "8EA0022016221F9C51EE2F20BDAE164C82075B9DD563196665C4652425557542459C81"sv;

        std::vector<u8> inhex, outhex;
        from_hex(std::vector<u8>(instr.cbegin(), instr.cend()), inhex);

        BENCHMARK("TLSH (tiny buffer)")
        {
            auto t = Tlsh();
            REQUIRE_FALSE(t.isValid());
            t.update(inhex);
            t.final();
            REQUIRE(t.isValid());
            CHECK(t.getHashString() == res);
        };
    }

    SECTION("TLSH small buffer hash (100KB)")
    {
        const auto sz = usize(158440);
        const auto fd = UniqueHandle{::fopen(SAMPLE_SMALL.c_str(), "r")};
        REQUIRE(fd != nullptr);

        std::vector<u8> in(sz);
        const auto cnt = ::fread(in.data(), sizeof(u8), sz, fd.get());
        REQUIRE(cnt == sz);

        BENCHMARK("TLSH (small buffer)")
        {
            auto t = Tlsh();
            REQUIRE_FALSE(t.isValid());
            t.update(in);
            t.final();
            REQUIRE(t.isValid());
        };
    }

    SECTION("TLSH medium buffer hash (10MB)")
    {
        const auto sz = usize(12068320);
        const auto fd = UniqueHandle{::fopen(SAMPLE_MEDIUM.c_str(), "r")};
        REQUIRE(fd != nullptr);


        std::vector<u8> in(sz);
        const auto cnt = ::fread(in.data(), sizeof(u8), sz, fd.get());
        REQUIRE(cnt == sz);

        BENCHMARK("TLSH (medium buffer)")
        {
            auto t = Tlsh();
            REQUIRE_FALSE(t.isValid());
            t.update(in);
            t.final();
            REQUIRE(t.isValid());
        };
    }

    SECTION("TLSH large buffer hash (100MB)")
    {
        const auto sz = usize(269893568);
        const auto fd = UniqueHandle{::fopen(SAMPLE_LARGE.c_str(), "r")};
        REQUIRE(fd != nullptr);


        std::vector<u8> in(sz);
        const auto cnt = ::fread(in.data(), sizeof(u8), sz, fd.get());
        REQUIRE(cnt == sz);

        BENCHMARK("TLSH (large buffer)")
        {
            auto t = Tlsh();
            REQUIRE_FALSE(t.isValid());
            t.update(in);
            t.final();
            REQUIRE(t.isValid());
        };
    }
}


TEST_CASE("Helpers", "[" NS "]")
{
}

TEST_CASE("Conversion", "[" NS "]")
{
    SECTION("from_hex small buffer")
    {
        const auto instr1 = "0123456789ABCDEF"sv;
        const std::vector<u8> in(instr1.cbegin(), instr1.cend());
        std::vector<u8> out;
        const std::vector<u8> res{
            0x01,
            0x23,
            0x45,
            0x67,
            0x89,
            0xab,
            0xcd,
            0xef,
        };
        BENCHMARK("from_hex")
        {
            from_hex(in, out);
        };
    }

    SECTION("from_hex large buffer")
    {
        const auto instr1 =
            "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"sv;
        const std::vector<u8> in(instr1.cbegin(), instr1.cend());
        std::vector<u8> out;
        const std::vector<u8> res{
            // clang-format off
            0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
            0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
            0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
            0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
            0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
            0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
            0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
            0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
            0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
            0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
            // clang-format on
        };
        BENCHMARK("from_hex")
        {
            from_hex(in, out);
        };
        CHECK(out == res);
    }
}
