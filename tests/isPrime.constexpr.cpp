#include <cstdint>
#include <utility>

#include "imath.h"
#include "catch2/catch_test_macros.hpp"

uint32_t constexpr operator"" _u32(unsigned long long n) {
    return static_cast<uint32_t>(n);
}
uint64_t constexpr operator"" _u64(unsigned long long n) {
    return static_cast<uint64_t>(n);
}

constexpr std::pair<uint32_t, bool> small_is_prime_table[]{
    {0, false},
    {1, false},
    {2, true},
    {3, true},
    {4, false},
    {5, true},
    {6, false},
    {7, true},
    {8, false},
    {9, false},
    {10, false},
    {11, true},
    {12, false},
    {13, true},
    {14, false},
    {15, false},
    {16, false},
    {17, true},
    {18, false},
    {19, true},
    {20, false},
    {21, false},
    {22, false},
    {23, true},
    {24, false},
    {25, false},
    {26, false},
    {27, false},
    {28, false},
    {29, true},
    {30, false},
    {31, true}};

// An arbitrary set of Fermat Pseudoprimes to base 2 fitting in 32bits,
// that have the lowest divisor > 128
constexpr uint32_t pspsu32[] = {
    1185605209u, 1186325981u, 1187235193u, 1191216133u,
    1192314817u, 1192412033u, 1192903531u, 1195524181u,
    1196364727u, 1196852273u, 1196969707u, 1198880261u,
    1202050873u, 1202142061u, 1203090379u, 1204176751u,
    1204205449u, 1204218709u, 1205606533u, 1205772499u,
    1207193149u, 1210562701u, 1210653541u, 1214770577u,
    1217181061u, 1217823517u, 1217924159u, 1219816261u,
    1219858921u, 1220491691u, 1221044653u, 1221127013u,
    2304120001u, 2304344813u, 2304710123u, 2306909593u,
    2308966661u, 2309241601u, 2309405617u, 2311558021u,
    2315137261u, 2320527613u, 2323147201u, 2324867399u,
    2329584217u, 2330569541u, 2331181621u, 2335341601u,
    2335379707u, 2338157597u, 2339464681u, 2340460487u,
    2341131967u, 2345907961u, 2346273961u, 2347597981u,
    2348226091u, 2352371251u, 2356272367u, 2362421953u,
    2366169241u, 2366765851u, 2370771181u, 2370928337u,
    3881445751u, 3883439647u, 3886515361u, 3886643801u,
    3887423437u, 3887635753u, 3888441001u, 3890243701u,
    3891892421u, 3891919417u, 3892244491u, 3893979941u,
    3894053311u, 3896079281u, 3897241129u, 3897869201u,
    3898857757u, 3900327241u, 3901632313u, 3903711841u,
    3905533721u, 3905876501u, 3907577521u, 3907752241u,
    3914923211u, 3915604421u, 3915921241u, 3916342789u,
    3917319841u, 3922321561u, 3926200321u, 3926912669u,
    4172804707u, 4173838327u, 4176142891u, 4176385921u,
    4176538241u, 4181350177u, 4183664101u, 4185636781u,
    4186561633u, 4187360341u, 4189357987u, 4191864013u,
    4192060699u, 4195843037u, 4196323561u, 4197177629u,
    4204344601u, 4206006229u, 4206295433u, 4210095667u,
    4211747581u, 4212105409u, 4212665491u, 4215885697u,
    4217502449u, 4218773491u, 4218900001u, 4231512709u,
    4232966251u, 4234009397u, 4234224601u, 4235818861u,
};

// An arbitrary set of Strong Pseudoprimes to base 2 fitting in 32bits,
// that have the lowest divisor > 128
constexpr uint32_t strpspsu32[] = {
    271951u, 280601u, 357761u, 390937u,
    458989u, 486737u, 489997u, 514447u,
    580337u, 741751u, 838861u, 873181u,
    877099u, 916327u, 976873u, 983401u,
    1016801u, 1082401u, 1194649u, 1207361u,
    1251949u, 1252697u, 1302451u, 1325843u,
    1357441u, 1373653u, 1441091u, 1493857u,
    1507963u, 1509709u, 1530787u, 1678541u,
    89308771u, 91433281u, 93431521u, 93541537u,
    94316401u, 94502701u, 95451361u, 95452781u,
    96618397u, 96904081u, 96925921u, 97496449u,
    97796953u, 97863529u, 97924217u, 99115297u,
    100463443u, 100618933u, 100943201u, 101270251u,
    101276579u, 102004421u, 102678031u, 102690677u,
    104078857u, 105305443u, 105919633u, 106485121u,
    106743073u, 107543333u, 108596953u, 109437751u,
    967287751u, 968283247u, 970586713u, 974113601u,
    974471243u, 977483449u, 979363153u, 980056507u,
    981484561u, 994133479u, 995586373u, 998489017u,
    998590601u, 998596741u, 998724481u, 999828727u,
    1005833971u, 1006800829u, 1008839999u, 1009025263u,
    1009140161u, 1011319501u, 1011333061u, 1011570457u,
    1011909271u, 1012438391u, 1013833153u, 1020515761u,
    1022336611u, 1027744453u, 1028494429u, 1034958601u,
    2115769633u, 2116483027u, 2116541221u, 2117031263u,
    2117555641u, 2118621097u, 2120096161u, 2123601751u,
    2124691213u, 2127197489u, 2129304997u, 2131811501u,
    2140771609u, 2141340833u, 2144961253u, 2147418113u,
    2152627801u, 2155416251u, 2172155819u, 2173540951u,
    2173579801u, 2175126601u, 2175406201u, 2177645557u,
    2185362233u, 2187717761u, 2200115713u, 2201924341u,
    2202205897u, 2203649197u, 2206095589u, 2213431729u,
};

// An arbitrary set of Strong Pseudoprimes to base 2 fitting in 64bits,
// that have the lowest divisor > 128
constexpr uint64_t strpspsu64[] {
    9883178023ull, 9886640653ull, 9888495469ull, 9901416673ull,
    9906899941ull, 9920101297ull, 9923904181ull, 9956839471ull,
    9974083381ull, 9974580661ull, 9975599993ull, 9977722421ull,
    9979343501ull, 9983920777ull, 9983968633ull, 9998721001ull,
    10000321321ull, 10000400749ull, 10019370133ull, 10033865431ull,
    10039090753ull, 10039353721ull, 10052858269ull, 10070775901ull,
    10074400337ull, 10076435503ull, 10076533741ull, 10081229201ull,
    10087437403ull, 10103806351ull, 10114384697ull, 10122850541ull,
    100004790097ull, 100009005079ull, 100034689201ull, 100046540833ull,
    100085989921ull, 100230611713ull, 100238884327ull, 100270188481ull,
    100270642393ull, 100286762357ull, 100302838021ull, 100345107637ull,
    100347437287ull, 100349182561ull, 100462162457ull, 100489577917ull,
    1000002977551ull, 1000036877971ull, 1000112847949ull, 1000114503277ull,
    1000245491101ull, 1000254486421ull, 1000372975477ull, 1000373131301ull,
    1000592696197ull, 1000645804033ull, 1000651749589ull, 1000661873221ull,
    1000762627303ull, 1001186962693ull, 1001362964101ull, 1001370510901ull,
    10000130243671ull, 10000425128321ull, 10000608524749ull, 10000655170351ull,
    10001089506241ull, 10001616943753ull, 10002267497461ull, 10002633950779ull,
    10002807196981ull, 10003008846997ull, 10003323884417ull, 10004062414081ull,
    10004303867137ull, 10004317235641ull, 10004591629441ull, 10005417703657ull,
    100002236680837ull, 100003882537681ull,
    100006001449933ull, 100006157068241ull,
    100006425103201ull, 100010025251251ull,
    100015751428517ull, 100018603780001ull,
    1000003918690669ull, 1000004994934549ull,
    1000012716506497ull, 1000030137901579ull,
    1000036059414841ull, 1000041946839253ull,
    1000043653945373ull, 1000048636028341ull,
    10000008250001701ull, 10000008663854653ull,
    10000019260488421ull, 10000029105812149ull,
    10000065660250093ull, 10000110825307051ull,
    10000126584210673ull, 10000138979476801ull,
    100000150553089531ull, 100000157387600201ull,
    100000174032555661ull, 100000209045665633ull,
    100000282080193093ull, 100000614807854191ull,
    100000679036502911ull, 100000715576502913ull,
    1000000274500018837ull, 1000000613130121513ull,
    1000000881716046341ull, 1000001048150670617ull,
    1000001659617979201ull, 1000002013944168709ull,
    1000002683432632489ull, 1000002867088603897ull,
    10000003599249373469ull, 10000004725548637213ull,
    10000007383908867697ull, 10000008408611691937ull,
    10000009701595859431ull, 10000011346918773161ull,
    10000014397616837441ull, 10000015890910031741ull
};

// An arbitrary set of (very) Strong Pseudoprimes to both base 2 and base 15,
// fitting in 64bits and greater than 55245642489451
// (up to that number all have a known deterministic 4 MR test)
constexpr uint64_t vstrpspsu64[] {
    100032767564329ull, 100095714178433ull,
    100103183370581ull, 100161085425481ull,
    100228475353597ull, 100260207880201ull,
    100382493196081ull, 100419473774957ull,
    100455743074501ull, 100496783063881ull,
    100585246856641ull, 100597715500201ull,
    100604214942307ull, 100698807024001ull,
    100772942411041ull, 100787890846147ull,
    10000339452880657ull, 10000666045451381ull,
    10002354134157001ull, 10002847652699287ull,
    10003760570147401ull, 10004839848020033ull,
    10004900831053441ull, 10005139614009601ull,
    10005525012933451ull, 10007952613350049ull,
    10008404267816261ull, 10008825396331747ull,
    10010098931593441ull, 10010383765970941ull,
    10011710625953041ull, 10012475479668481ull,
    100002417651328501ull, 100003937979496321ull,
    100004274960092887ull, 100005988703641921ull,
    100007171823290401ull, 100008476127176777ull,
    100009126048851001ull, 100010047338428761ull,
    100010210482435843ull, 100011718171839961ull,
    100013946413520833ull, 100018257367392421ull,
    100020425628098809ull, 100020955129375057ull,
    100026422722485451ull, 100026498627189751ull,
    1000010288472173917ull, 1000039655616840833ull,
    1000041442369507693ull, 1000048638528927553ull,
    1000079836093386847ull, 1000080100503830081ull,
    1000081344987517801ull, 1000086612375344677ull,
    1000097236863636997ull, 1000100293372942081ull,
    1000110973578612967ull, 1000112218940978477ull,
    1000135293727936843ull, 1000138833782223049ull,
    1000139502716622451ull, 1000142360197420061ull,
    10000053639141814951ull, 10000069871074328033ull,
    10000087639352657641ull, 10000115493554386201ull,
    10000116024820108027ull, 10000134909146176381ull,
    10000154171371182001ull, 10000163902964719901ull,
    10000180525292635681ull, 10000193394278472481ull,
    10000198696284800033ull, 10000248668669413129ull,
    10000268980865126881ull, 10000279148993899651ull,
    10000290090812909161ull, 10000290115155591343ull,
    10000295397455211157ull, 10000300263669201737ull,
    10000368098317736641ull, 10000492088596543957ull,
    10000501196182357477ull, 10000520771361258601ull,
    10000586402908307317ull, 10000651475030755501ull,
    10000653606828605089ull, 10000660954468507381ull,
    10000662627007156501ull, 10000678415448448087ull,
    10000698978699573841ull, 10000706117959577677ull,
    10000729537192871821ull, 10000748089234473451ull,
    10000830412895032327ull, 10000841344002952111ull,
    10000881021093835969ull, 10000921136288948251ull,
    10000924324011889927ull, 10000927290672122417ull,
    10000950928973986801ull, 10000962121336922851ull,
    10000967408920215997ull, 10000988685764725351ull,
    10001028467897272081ull, 10001050497188591521ull,
    10001059271415732451ull, 10001067594973076077ull,
    10001073458024428781ull, 10001157506173841281ull,
    10001157523845299197ull, 10001209608355536841ull,
    10001218994947214371ull, 10001253019196685761ull,
    10001279002279744009ull, 10001335944765635681ull,
    10001337082735286881ull, 10001390344212573967ull,
    10001399895992007361ull, 10001410839842193257ull,
    10001470952539429921ull, 10001474376711654961ull,
    10001489058686535643ull, 10001518473666250081ull,
    10001521160821730777ull, 10001538279258594301ull
};

constexpr uint32_t bigprimesu32 [] {
    2147483237u, 2147483249u, 2147483269u, 2147483323u,
    2147483353u, 2147483399u, 2147483423u, 2147483477u,
    2147483489u, 2147483497u, 2147483543u, 2147483549u,
    2147483563u, 2147483579u, 2147483587u, 2147483629u,
    2147483647u, 2147483659u, 2147483693u, 2147483713u,
    2147483743u, 2147483777u, 2147483783u, 2147483813u,
    2147483857u, 2147483867u, 2147483869u, 2147483887u,
    2147483893u, 2147483929u, 2147483951u, 2147483993u,
    4294965193u, 4294965203u, 4294965229u, 4294965251u,
    4294965263u, 4294965307u, 4294965313u, 4294965331u,
    4294965347u, 4294965361u, 4294965383u, 4294965413u,
    4294965457u, 4294965461u, 4294965487u, 4294965529u,
    4294965581u, 4294965601u, 4294965613u, 4294965617u,
    4294965641u, 4294965659u, 4294965671u, 4294965673u,
    4294965679u, 4294965683u, 4294965691u, 4294965721u,
    4294965733u, 4294965737u, 4294965757u, 4294965767u,
    4294965793u, 4294965821u, 4294965839u, 4294965841u,
    4294965847u, 4294965887u, 4294965911u, 4294965937u,
    4294965949u, 4294965967u, 4294965971u, 4294965977u,
    4294966001u, 4294966007u, 4294966043u, 4294966073u,
    4294966087u, 4294966099u, 4294966121u, 4294966129u,
    4294966153u, 4294966163u, 4294966177u, 4294966187u,
    4294966217u, 4294966231u, 4294966237u, 4294966243u,
    4294966297u, 4294966337u, 4294966367u, 4294966373u,
    4294966427u, 4294966441u, 4294966447u, 4294966477u,
    4294966553u, 4294966583u, 4294966591u, 4294966619u,
    4294966639u, 4294966651u, 4294966657u, 4294966661u,
    4294966667u, 4294966769u, 4294966813u, 4294966829u,
    4294966877u, 4294966909u, 4294966927u, 4294966943u,
    4294966981u, 4294966997u, 4294967029u, 4294967087u,
    4294967111u, 4294967143u, 4294967161u, 4294967189u,
    4294967197u, 4294967231u, 4294967279u, 4294967291u,
};

constexpr uint64_t bigprimesu64[] {
    static_cast<uint64_t>(-59),
    static_cast<uint64_t>(-83),
    static_cast<uint64_t>(-95),
    static_cast<uint64_t>(-179),
    static_cast<uint64_t>(-189),
    static_cast<uint64_t>(-257),
    static_cast<uint64_t>(-279),
    static_cast<uint64_t>(-323),
    static_cast<uint64_t>(-353),
    static_cast<uint64_t>(-363)
};

#if IMATHLIB_HAS_CONSTEXPR_INTR
TEST_CASE( "Correct constexpr prime test for u32", "[isPrime32constexpr]" ) {
    STATIC_REQUIRE(imath::isPrime(0_u32) == false);
    STATIC_REQUIRE(imath::isPrime(1_u32) == false);
    STATIC_REQUIRE(imath::isPrime(2_u32) == true);
    STATIC_REQUIRE(imath::isPrime(3_u32) == true);
    STATIC_REQUIRE(imath::isPrime(4_u32) == false);
    STATIC_REQUIRE(imath::isPrime(5_u32) == true);
    STATIC_REQUIRE(imath::isPrime(2213431729_u32) == false);
    STATIC_REQUIRE(imath::isPrime(4294967291_u32) == true);

    // set of xxhash primes
    STATIC_REQUIRE(imath::isPrime(2654435761_u32));
    STATIC_REQUIRE(imath::isPrime(2246822519_u32));
    STATIC_REQUIRE(imath::isPrime(3266489917_u32));
    STATIC_REQUIRE(imath::isPrime( 668265263_u32));
    STATIC_REQUIRE(imath::isPrime( 374761393_u32));
}
#endif

#if IMATHLIB_HAS_CONSTEXPR_X64
TEST_CASE( "Correct constexpr prime test for u64", "[isPrime64constexpr]" ) {

    STATIC_REQUIRE(imath::isPrime(10001538279258594301_u64) == false);

    // biggest 64 bit prime
    STATIC_REQUIRE(imath::isPrime(static_cast<uint64_t>(-59)) == true);

    // set of xxhash primes
    STATIC_REQUIRE(imath::isPrime(11400714785074694791_u64));
    STATIC_REQUIRE(imath::isPrime(14029467366897019727_u64));
    STATIC_REQUIRE(imath::isPrime( 1609587929392839161_u64));
    STATIC_REQUIRE(imath::isPrime( 9650029242287828579_u64));
    STATIC_REQUIRE(imath::isPrime( 2870177450012600261_u64));
}
#endif

#if __cpp_lib_ranges >= 201911L && IMATHLIB_HAS_CONSTEXPR20
#include <algorithm>

TEST_CASE( "Correct constexpr prime test for all problematic cases",
           "[isPrime64constexpr]" ) {
    constexpr auto is_prime =
        [](auto&& n) constexpr { return imath::isPrime(n); };
    STATIC_REQUIRE(std::ranges::all_of(
                    small_is_prime_table,
                    [](auto&& p) { return imath::isPrime(p.first) == p.second; } ));

    STATIC_REQUIRE(std::ranges::none_of(
                    pspsu32, is_prime));

    STATIC_REQUIRE(std::ranges::none_of(
                    strpspsu32, is_prime));

    STATIC_REQUIRE(std::ranges::none_of(
                    strpspsu64,
                    is_prime));

    STATIC_REQUIRE(std::ranges::none_of(
                    vstrpspsu64,
                    is_prime));

    STATIC_REQUIRE(std::ranges::all_of(
                    bigprimesu32,
                    is_prime));

    STATIC_REQUIRE(std::ranges::all_of(
                    bigprimesu64,
                    is_prime));
}

#endif
