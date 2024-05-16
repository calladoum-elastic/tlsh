#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include <tlsh.h>
#include <tlsh_util.h>

#include <array>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <tuple>
#include <vector>

#define NS "TLSH"

using namespace std::literals::string_view_literals;


TEST_CASE("Evaluation", "[" NS "]")
{
    // SECTION("Basic evalution from randomly generated input")
    // {
    //     std::array<std::tuple<std::string_view, std::string_view>, 10> expected_results{{
    //         // clang-format off
    //         //{$input, $known_result}
    //         {"0684076d82f90066d54f1ccaa555f093a7627286555b0e5cc2e36da3ab41732e79636276530a861db7732eeb420bc2261af2a956f9bb2cf20876ca8819b4a43e"sv,
    //         "8EA0022016221F9C51EE2F20BDAE164C82075B9DD563196665C4652425557542459C81"sv},
    //         {"72a2f15b5617989c615111d7cf1ea40e3f142a0665b9739ef5cc667bcfd947bea5fdf6f5b966800d5a7bbbc29d3b893a5ec9cbd94ec93131fdcf639763b44c79"sv,
    //         "57A022AA3E32038FC2CECC3C23002C803000820820C22B308BBBE038232C000C020000"sv},
    //         {"9a3e32e383495c89e8a4b99724aeba4067b746bfda3f4f1a443e688c32221bf96151864c27f85b30b0c29545f5806b757effe910fbd41722d9921028ae74226e"sv,
    //         "25A022302802030230302802032808E8308B03AFACB0E208202082BBBE33FEC80020CE"sv},
    //         {"03905da51374f29a46a907c601e11cf72f4a35d55a6464378189878d64bf4e39f58f69f2172e0d33eff38de892df6be647fd2fedf90dba8baa9c5b3c99469c75"sv,
    //         "A2A0228E0222AC3000E23C02B82300C3A00E0AB280828082000C0C220800020CF8EAB0"sv},
    //         {"16685eb854ec42dd803a6e270f73d6c7e4f88fc684ec33a6537e7d02fd445c6f777bb7a86bddfeffd2fa8e84c8b8a3cf86a5ba4064bf62d9f2d583a60d974170"sv,
    //         "9FA00204186B373A980CDB5C1034343A451803D01B0688158E9C983B55675D0445D17D"sv},
    //         {"bd15122089da73462d2200fad79dbb62bb1abdd14cfa21e5f179e7f4da870acd0f0a6e65ee993621bec367fe4661706997da496e387506f7a158ed72fffbb5d8"sv,
    //         "0FA022B22E08C8FC0EEBB00202CC0A3808220000B880202882280A0000C0C023300220"sv},
    //         {"5ea7d24ba475b20022d688c14c4537e5a2045915a5d93595ea78f3b0aad1fc688e15d2190557e50edec18ec38480c6c8b7196b85b07a7d90d99357320449050c"sv,
    //         "F4A022282A0C000A00328000E00A08020EE8C2082A3A8CAB82820283200CC02382008F"sv},
    //         {"76d7faa4625d8a4e9586c9b6b492e9d996df854801167081665ac1202ab5a0dbe9b91e07437e5ec41716477b9cf3b63ad084e2296871ca9b0001ef1ad3bcab5c"sv,
    //         "E0A022E220E00B3E28A8002A00BCC30CB2A303030A228BCF02003C3320008B00C2A08E"sv},
    //         {"32481ccd4feeca3ab2b5bf05f2f454e67fdced37810fa562d9c35b4385b9e8b1057ad3300e8b161552a44f581eaebf4b18b6dec7a70fb8dea694bf85f26a08bc"sv,
    //         "A0A0220C00000A00A02EEB32C888B0030000CA8002083832AF2C2028E2E0CC28C000F0"sv},
    //         {"239fd4c14fa272e409b6ccccee4bff3c01d76ff2c0fa702776f38c89207f8221217908b193d18755633aa3c996dfa244647d344e5264770dfcf094aa93ee5d3f"sv,
    //         "82A002A9F55005B1D51EE7614440224E628FA4775D6E9B1CC0815B473647E4A5D00D9E"sv},
    //         // clang-format on
    //     }};

    //     auto t = Tlsh();
    //     for (auto const [instr, res] : expected_results)
    //     {
    //         t.reset();
    //         REQUIRE_FALSE(t.isValid());
    //         std::vector<u8> inhex, outhex;
    //         from_hex(std::vector<u8>(instr.cbegin(), instr.cend()), inhex);
    //         t.update(inhex);
    //         t.final();
    //         REQUIRE(t.isValid());
    //         CHECK(t.getHashString() == res);
    //     }
    // }
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
