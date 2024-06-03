#define CATCH_CONFIG_MAIN

#include <stdio.h>
#include <tlsh.h>
#include <tlsh_util.h>

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
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

const std::filesystem::path THIS_DIR{"../../../.."};
const std::filesystem::path BASE_DATASET_ROOT = THIS_DIR / "tests/datasets/base";


TEST_CASE("Batch", "[" NS "]")
{
    SECTION("Batch")
    {
        const std::array<std::tuple<std::string_view, std::string_view>, 78> filenames = {{
            {
                "021106_yossivassa.txt"sv,
                "1FA1B357F78913B236924271569EA6D1FB2C451C33668484552C812D33138B8C73FFCE"sv,
            },
            {
                "0Alice.txt"sv,
                "45D18407A78523B35A030267671FA2C2F725402973629B25545EB43C3356679477F7FC"sv,
            },
            {
                "11-17-06_Academy.txt"sv,
                "C8E1E8277B98A6255FA70063B70FBAE1F6618229332C9438084FB331178ADEE53B41D5"sv,
            },
            {
                "1english-only.txt"sv,
                "E951784702042376169012B1BA5A76EAF36092FC3311A595B4856235278F9F973763EF"sv,
            },
            {
                "2005NISSE.txt"sv,
                "06D29517F780237185070293B60E36FAB735C0F833D66460688DA22D6756E751B7BAEB"sv,
            },
            {
                "2006-07_Resource_Brochure.txt"sv,
                "35214031F3C6AEA059EB1ABA870E8C4758D70024C63224B5D4C89354E46315D26BF736"sv,
            },
            {
                "2006_2007PhysicalEducationConceptMap.txt"sv,
                "C081B063DB6455D3411303561428A9FBE1369B047760286686FFA067B2DA70D823DEFF"sv,
            },
            {
                "2007ShowcaseFilm_Package.txt"sv,
                "E222B667EB006374230721F2620F5283EF3E817A6265462A745DF36933E7690037F7AA"sv,
            },
            {
                "22-ppa-3rd_e2snewsletter_jun06.txt"sv,
                "7E529567B7C413B61A93119266DD11E2E37A49BCB3718C61B04E8438235ECB7477B3EA"sv,
            },
            {
                "42nd_street.txt"sv,
                "4221519BD71126276E6282B9668A6A81FF08C5293735431AA0CE50002B17DAE67FA7D4"sv,
            },
            {
                "5._.txt"sv,
                "F502E727F36923366E020362E50EA1F1D76943BC935A4221454DE03C3276D6E5BBF6ED"sv,
            },
            {
                "7-prikaz-slozeno.txt"sv,
                "7D514252EBC42331ABD704331B5E5786D319172DD3064C22BD4D82A42B864A91B3B7EC"sv,
            },
            {
                "AccessToNationalSportsCentresPassportScheme.txt"sv,
                "E9E1B7BDEB0873EB275983DEB84150A09731C51453904A40391D7B3A135ABBA4FB7E79"sv,
            },
            {
                "Adult-Sports.txt"sv,
                "EF71831FBB7D13058AC3A3956318C2D9BBBCC25DF3B5499132DE6118377A12D433A654"sv,
            },
            {
                "AfghanistanLiteracyDiscussionPaper.txt"sv,
                "B6A2A377F38617A186C60116725A96DA9BBEC97C73114910E84C817EF30BA64433FBFA"sv,
            },
            {
                "BBVDlivepr.txt"sv,
                "1971DA5BD348133621131373654F39EBEB18933C63274561487ED169335ACAD17B7AD8"sv,
            },
            {
                "BBVDpr.txt"sv,
                "1751C967C354133467220270AD2F3CEDF714917DA3568B20582AF42D372681B43375E9"sv,
            },
            {
                "BENEFITS_OF_EXERCISE_for_the_manual.txt"sv,
                "29514417E3C8337006414292395E3AA5F627C4782368B938DD69C26FA325FA50376ADB"sv,
            },
            {
                "Call-for-abstracts.txt"sv,
                "A812B413A70917721ED31250734F17FAABBA876C32144A55982CC12C1646DB8D7BFBFA"sv,
            },
            {
                "ClaremontTrioBio0607.txt"sv,
                "3BC1985B330F037657B603A1424E43E173595E68733A9A54486B612B3387C5CE7BA3ED"sv,
            },
            {
                "CommandGde.txt"sv,
                "5C62C717E78923B100070266320F39D3A73EC678B22556147C9D919E7389E7D177BBE6"sv,
            },
            {
                "DOC-DHHS_Joint_Action_Plan_summary.txt"sv,
                "23E181177B1533304A4301C27D5A10E6E77AA578B35A1A01B0FEC11457EACAB9B3BBF9"sv,
            },
            {
                "DanceFestOMAIPressReleaseFeb15_07.txt"sv,
                "89A1B98BE70413734A820238F5AE5697FBA5846673139D94D4AC607D321287983FFFA9"sv,
            },
            {
                "ExercisePocketSliderText.txt"sv,
                "BDC1B61FC71C22350EB20A7B324C36D8AE7B88690245ED75CD1CD11D3365BEE427E6A5"sv,
            },
            {
                "FF06_press_9-7.txt"sv,
                "99F1EA07E70927351B2301E2AA0F61E3FB589458736AC9054C5EB12133C3DAD937EEE9"sv,
            },
            {
                "Fitness_Myths.txt"sv,
                "D7D1E727E748377142220064964DB1F5ABAD963833210DB5349D823E238EDBE837F7D9"sv,
            },
            {
                "Global_Warming_Resolution_April_2001.txt"sv,
                "1061964BF2C4223767037275A14B22CCAB7C5934E3245519D4ED51AC9312FA98B3BEE8"sv,
            },
            {
                "GovernorsTaskForceReport.txt"sv,
                "AD82B823A25013B51AD3017371DD2AD1F37AC1ACA3253C85648DC53C2B57AB9933A7EB"sv,
            },
            {
                "Groove_Yard.txt"sv,
                "3B71D867960813745AD201D290591CC2932EC22836E1C5A470CF9ACC1B2F8FDD7FBAE1"sv,
            },
            {
                "HSCPR10306.txt"sv,
                "A7B10837B36C23315F0302B53589A1E6FB78854A33BA8E58C41E926533428F447BA3CD"sv,
            },
            {
                "HandoutSportsInjuryPrevention.txt"sv,
                "C7D1B73FEB482735479100A57B4D3ED6E7258A3833367AB2AC5CD15C132B67906721F6"sv,
            },
            {
                "IntlChildrensFestRevisedRelease.txt"sv,
                "8292E81BFB4423351B1341F2111D79CBBB28436CB3624D34789D9269278296C97FEBEA"sv,
            },
            {
                "LOTRLeadRelease.txt"sv,
                "98028597735813360E564221A68D63D2F328C83A73BF8B2550BDA16C3687C6953FB5EC"sv,
            },
            {
                "Landau-Pickford-Press-Rel.txt"sv,
                "11A1A52753E603564767032A699EB2D3B325813C6150C964306C6BAD37829B483BF9EA"sv,
            },
            {
                "Lotuspike-VBTD-Jenkins.txt"sv,
                "6861C707E729133966738191C24D27DBB76E5028A3298E5414D75ADB2391C6C933FFEC"sv,
            },
            {
                "MEDT6491.txt"sv,
                "1582C4137B9833324B638255B10A1FD3A738D19433705CA4B8A9421867EFA7853F67EB"sv,
            },
            {
                "MRbio.txt"sv,
                "5181C72B571613399B63062B920DA5E17355C129333BCD0194FDE26C39178B9873DBEC"sv,
            },
            {
                "Meeting_the_Challenges.txt"sv,
                "F6626653A788337246920399AB1E57EAEB19D0BC23555934D8CC852C350AC6643FFFF5"sv,
            },
            {
                "Memo_Ehrlich_1103.txt"sv,
                "3722A417A7441F7212D32031764B2AEEE729C96CB3915C659889B12C131C775833BBEA"sv,
            },
            {
                "Midwestern_Rock_Garden.txt"sv,
                "DAD1D82BFB2013660E520228F54E22E5EF798A791362A74178D5812C3396CBD27FD5F8"sv,
            },
            {
                "Newsletter_20-1-3.txt"sv,
                "A271960B978527B58A6301F9166C25E5D399C92C332989208D19812D330AABD47BBFE6"sv,
            },
            {
                "Reviews-GrooveYard.txt"sv,
                "F202E867DB81033449E242A7B14F25EBF725C1393211897470BED2E82359D2993FBAE8"sv,
            },
            {
                "SAR_Dog_Avalanche_promo.txt"sv,
                "4FB1663FD308233447220276B64E61E5BB40D32DD3605572689E94BD33DAD7AD2772E8"sv,
            },
            {
                "SCE_3320.txt"sv,
                "0CB2F727B3D0272653E3016AD81516C3DA3B9149B3040975B47EC278278EB5B63BA7BF"sv,
            },
            {
                "Shopping.txt"sv,
                "A161866FF77463B91F828264B19C18E09F6D85247BF680E5B02E536D038585A07FA7B8"sv,
            },
            {
                "SoundResources.txt"sv,
                "B65144DF581881590D92CEB572FEBBC5AA3201E0C0C586D4FC9D8CB12715A23B914FE1"sv,
            },
            {
                "Summer_Math_Syllabus.txt"sv,
                "F3C1722B67C823B156430214A51A95DAEF38E9283320A539FCDE446C3B1D6561B3BBB6"sv,
            },
            {
                "TeachingMathCameroon.txt"sv,
                "DF12A57B778413B106B34504992D66EAE728C17923665C21F4CC826C370A9B647FEBF9"sv,
            },
            {
                "The-Global-Warming-Response-Act.txt"sv,
                "1BC150871A8433368ED351FCA23A18ACF3B1C5B9372A90554C4E4359B6999290B3F6F2"sv,
            },
            {
                "WPTC2006Season.txt"sv,
                "6DC1D787BB4413790F1102E1AA9E61E9F7500A75B3B24F24C4AED0642742C7E2BF97EC"sv,
            },
            {
                "Week3.txt"sv,
                "EA42C71FE64C233A09103236364F77DEAB27886CD3141A65AC1D925D3769F9E037A8E9"sv,
            },
            {
                "aReportOfTheSurgeonGeneral.txt"sv,
                "D252A527E74823210B530216761F76E6B73BC42D6315AE20E49DD26C2359E6807FBBDE"sv,
            },
            {
                "ashaam2.txt"sv,
                "BE22A522F35D13A58B518369911762D3BB71C249F3386A84C49FA2253747CE8827B7FA"sv,
            },
            {
                "billboard-021205.txt"sv,
                "F37197A7BB911376899301B1715E31F2FB68D17CA39991428C2DD1AC32638B9127FECD"sv,
            },
            {
                "bio.txt"sv,
                "F271B567761C0379579203A3519F22F5AB20822C73B68E80405D64A57383A7CA6FEAFD"sv,
            },
            {
                "cme2i05.txt"sv,
                "F522E2336FC023568A838147758957EEF22DD9A833145645DC0783A9260F6F9A3BFEC6"sv,
            },
            {
                "didges_brew_band_info.txt"sv,
                "FA120A3BE7450375C66B8281224E55D6B639815CF3768A111CAE9038378B467C77BBFC"sv,
            },
            {
                "exploring_dallas.txt"sv,
                "3A72A61BB35473B91BA38365225DAAF1D7364068B33A0964F46EC1202791C7693FE7EC"sv,
            },
            {
                "exxonmobil_climate_change_backgrounder.txt"sv,
                "D3C1A363B355173A82930175626B36E1DBFF8474B3948905684DA2AC834AC68D33F7EE"sv,
            },
            {
                "fitnessTerms.txt"sv,
                "CB42A653BB0C3337426362B59A5EB6D0F32A842CF3513A70845DD1A92385EB447B76DA"sv,
            },
            {
                "gogirls.2003-10.txt"sv,
                "E112C61FE35033A00B2F4166B74626DAEB26022C73970969742D823C73A56AF533B7F5"sv,
            },
            {
                "greenhouse.txt"sv,
                "5942D71FA381233B496102B5770EA2CBEB15C0BC5369A571A45EC12E3367D7C937B9E8"sv,
            },
            {
                "lac_rfp_inner-cities.txt"sv,
                "0292B51BF74413364D6731B1265FA5DFE72EC628F3650A71E8AC802C2258EA493363DD"sv,
            },
            {
                "ledger091505.txt"sv,
                "DEC1E60BB344033B4EE352D0318E05AEF7A9907957F2CA94D46CC0593A93C78617ABED"sv,
            },
            {
                "modelgeneralnotice.txt"sv,
                "61122097328C273521B3025B764FA4D6BB318465D30E4A0028ED99FC936BFA8D7372D9"sv,
            },
            {
                "parentarticle.txt"sv,
                "6B82C75FB74C13B229610595332FAAFAF723C1BC1316916C9C1D802C1365F6A927F6DA"sv,
            },
            {
                "record_voting.txt"sv,
                "0171863E9341233EAB334611369C1FE3761C913AE322885574DD35282F5A62F9ABD3D9"sv,
            },
            {
                "rkc_flyer.txt"sv,
                "E2F1E60B638A3335682A4157B35F11EAB366A1247265C1E0905D333F23DDEEA813B7D7"sv,
            },
            {
                "sisters_pressrelease.txt"sv,
                "8541414FF38513711827423599BD78D37A78D378B304494550B884EA02A392A47FFBDC"sv,
            },
            {
                "socy642_f02_milkie.txt"sv,
                "A592825B7B5952365703327AB1471ACE6625002DF212CC39A0A9AC383781D6DAFF278E"sv,
            },
            {
                "spanish_place_namesA.txt"sv,
                "E481852B33C423B545639375535F5AFBB74EC694421183F0A89EC43E735698C11B9AE8"sv,
            },
            {
                "spanish_place_namesB.txt"sv,
                "5C12E729F30903720143429821CF67F2B75691E8D2720365B86CCA3EF693EE951F4CEA"sv,
            },
            {
                "sports_massage_and_recovery.txt"sv,
                "1C52E827BB8813744A5303ABB60D39C1F76981BC7650A6A164DE925DB30EC740377BEA"sv,
            },
            {
                "srilanka.txt"sv,
                "6A82C773A39423714BA30263A56E43D7F77F846C22698960A44DC13D624ADF4437FBEA"sv,
            },
            {
                "survey_mathtch.txt"sv,
                "3602855BF340635353631252A22F8CDACB14F13DB3927931286CD13D2679E26477BBA6"sv,
            },
            {
                "syllabus.txt"sv,
                "98A2C77FA280037117E30126755D9BCAF72A906C337548B9F85E507C331BA28677BBA6"sv,
            },
            {
                "victim.txt"sv,
                "5C32E96FF70463252A410264660E22DFE3A54038F3AD8B605C3E901403D6F9B5ABB7FE"sv,
            },
            {
                "website_course_descriptors06-07.txt"sv,
                "5702A65FF780237092AA01D5670E21E2E36A942CB3265960945D922C3366FFD837B7FD"sv,
            },
        }};

        for (auto const &[fname, expected_hash_str] : filenames)
        {
            auto const fpath = BASE_DATASET_ROOT / fname;
            REQUIRE(std::filesystem::exists(fpath));

            const auto fd = UniqueHandle{::fopen(fpath.c_str(), "rb")};
            REQUIRE(fd != nullptr);

            std::vector<u8> expected_hash(TLSH_STRING_LEN / 2);
            from_hex(std::vector<u8>(expected_hash_str.cbegin(), expected_hash_str.cend()),
                expected_hash);

            const usize blocksz = 512;
            auto t              = Tlsh();
            REQUIRE_FALSE(t.isValid());
            t.reset();

            while (!::feof(fd.get()))
            {
                REQUIRE_FALSE(::ferror(fd.get()));
                std::vector<u8> in(blocksz);
                const auto cnt = ::fread(in.data(), sizeof(u8), blocksz, fd.get());
                if (cnt == 0)
                    break;
                if (cnt < blocksz)
                    in.resize(cnt);
                t.update(in);
            }

            t.final();
            INFO("file is " << fpath.c_str());
            REQUIRE(t.isValid());

            // test result as bytes
            auto const &res_bytes = t.getHashBytes(0);
            REQUIRE(res_bytes.size() == expected_hash.size());
            CHECK(res_bytes == expected_hash);

            // test result as string
            auto res_str = t.getHashString(0);
            REQUIRE(res_str.size() == expected_hash_str.size());
            CHECK(res_str == expected_hash_str);
        }
    }
}