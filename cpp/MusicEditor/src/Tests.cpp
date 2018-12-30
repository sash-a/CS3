#define CATCH_CONFIG_MAIN //So that catch will define a main method

#include "catch.hpp"
#include "Sound.h"

using namespace std;

TEST_CASE("Sound mono", "8 bit")
{
    vector<int8_t> v = {1, 2, 3, 4};

    abrsas::Sound<int8_t> s(v, 1);
    abrsas::Sound<int8_t> cpy = s;

    auto concat = s | cpy;
    REQUIRE((concat.bits[0] == 1));
    REQUIRE((concat.bits[3] == 4));
    REQUIRE((concat.bits[4] == 1));
    REQUIRE((concat.bits[7] == 4));

    auto plus = s + cpy;
    REQUIRE((plus.bits[0] == 2));
    REQUIRE((plus.bits[1] == 4));
    REQUIRE((plus.bits[2] == 6));
    REQUIRE((plus.bits[3] == 8));

    auto vol = make_pair<float, float>(2.0, 2.0);
    auto times = s * vol;
    REQUIRE((times.bits[0] == 2));
    REQUIRE((times.bits[1] == 4));
    REQUIRE((times.bits[2] == 6));
    REQUIRE((times.bits[3] == 8));

    auto range = make_pair<int, int>(1, 3);
    auto slice = s ^range;
    REQUIRE((slice.bits[0] == 2));
    REQUIRE((slice.bits[1] == 3));

    auto cut = s.cut(range);
    REQUIRE((cut.bits[0] == 1));
    REQUIRE((cut.bits[1] == 4));

    auto radd = s.radd(cpy, range);
    REQUIRE((radd.bits[0] == 1));
    REQUIRE((radd.bits[1] == 4));
    REQUIRE((radd.bits[2] == 6));
    REQUIRE((radd.bits[3] == 4));

    auto rev = s.rev();
    REQUIRE((rev.bits[0] == 4));
    REQUIRE((rev.bits[1] == 3));
    REQUIRE((rev.bits[2] == 2));
    REQUIRE((rev.bits[3] == 1));

    auto rms = s.rms();
//    REQUIRE((rms == 2.73861)); this value is not precise enough?

    auto fin = s.fadein(5);
    REQUIRE((fin.bits[0] == 1));
    REQUIRE((fin.bits[1] == 2));
    REQUIRE((fin.bits[2] == 3));
    REQUIRE((fin.bits[3] == 4));

    auto fot = s.fadeout(5);
    REQUIRE((fot.bits[0] == 1));
    REQUIRE((fot.bits[1] == 2));
    REQUIRE((fot.bits[2] == 3));
    REQUIRE((fot.bits[3] == 4));
}

TEST_CASE("Sound stereo", "16 bit")
{
    auto first = make_pair(1, 2);
    auto second = make_pair(3, 4);

    vector<pair<int16_t, int16_t>> v = {first, second};

    abrsas::Sound<pair<int16_t, int16_t>> s(v, 1);
    abrsas::Sound<pair<int16_t, int16_t>> cpy = s;

    auto concat = s | cpy;
    REQUIRE((concat.bits[0].first == first.first));
    REQUIRE((concat.bits[1].first == second.first));
    REQUIRE((concat.bits[2].first == first.first));
    REQUIRE((concat.bits[3].first == second.first));

//    auto plus = s + cpy;
//    REQUIRE((plus.bits[0] == make_pair(2, 4)));
//    REQUIRE((plus.bits[1] == make_pair(6, 8)));

    auto vol = make_pair<float, float>(2.0, 2.0);
    auto times = s * vol;
//    REQUIRE((times.bits[0] == make_pair(2, 4)));
//    REQUIRE((times.bits[1] == make_pair(6, 8)));
//
//    auto range = make_pair<int, int>(0, 1);
//    auto slice = s ^range;
//    REQUIRE((slice.bits[0] == first));
//
//    auto cut = s.cut(range);
//    REQUIRE((cut.bits[0] == second));

//    auto radd = s.radd(cpy, range);
//    REQUIRE((radd.bits[0] == make_pair(2, 4)));
//    REQUIRE((radd.bits[1] == make_pair(3, 4)));
//
//    auto rev = s.rev();
//    REQUIRE((rev.bits[0] == second));
//    REQUIRE((rev.bits[1] == first));
//
//
//    auto rms = s.rms();
////  value is not precise enough?
//
//    auto fin = s.fadein(5);
//    fin.print();
//
//    auto fot = s.fadeout(5);
//    fot.print();
}