//
// Created by abrsas002 on 2018/04/19.
//

#define CATCH_CONFIG_MAIN //So that catch will define a main method

#include "catch.hpp"
#include "Image.h"

using namespace std;

TEST_CASE("Image", "Testing constructors")
{
    abrsas::Image img;

    // would expect size to be 0
    int count = 0;
    for (auto i = img.begin(); i != img.end(); ++i)
    {
        count++;
    }

    REQUIRE(count == 0);

    // USING test.pgm IN TEST FOLDER
    // width is 1207 and height is 1280
    int w = 1207, h = 1280;
    img << "../test/test.pgm";

    abrsas::Image copy(img);
    abrsas::Image copy2;
    copy2 = img;

    count = 0;

    auto copyitr = copy.begin();
    auto copy2itr = copy2.begin();
    for (auto i = img.begin(); i != img.end(); ++i)
    {
        REQUIRE(*copyitr == *copy2itr);
        REQUIRE(*i == *copyitr);

        ++copyitr;
        ++copy2itr;
        ++count;
    }
    REQUIRE(count == w * h);


    // TODO test move
//    count = 0;
//    abrsas::Image mv(std::move(img));
//    auto mvitr = mv.begin();
//    copyitr = copy.begin();
//
//    for (auto i = mv.begin(); i != mv.end(); ++i)
//    {
//        REQUIRE(*mvitr == *copyitr);
//
//        ++copyitr;
//        count++;
//    }
//    REQUIRE(count == w * h);

    //for(auto i = )
}

TEST_CASE("Image1", "Testing +")
{
    abrsas::Image img;
    abrsas::Image img2;
    abrsas::Image sum;

    img << "../test/test.pgm";
    img2 << "../test/test.pgm";

    sum = img + img2;
    auto sumit = sum.begin();
    auto imgit = img.begin();
    auto img2it = img2.begin();

    while (sumit != sum.end())
    {
        REQUIRE((int) *sumit <= 255);

//        REQUIRE(*sumit <= (*imgit + *img2it)); // because clamp method is private...
        ++sumit;
        ++imgit;
        ++img2it;
    }
}

TEST_CASE("Image2", "Testing -")
{
    abrsas::Image img;
    abrsas::Image img2;
    abrsas::Image sum;

    img << "../test/test.pgm";
    img2 << "../test/test.pgm";

    sum = img + img2;
    auto sumit = sum.begin();
    auto imgit = img.begin();
    auto img2it = img2.begin();

    while (sumit != sum.end())
    {
        REQUIRE((int) *sumit >= 0);

        ++sumit;
        ++imgit;
        ++img2it;
    }
}

TEST_CASE("Image3", "Testing *")
{
    abrsas::Image img;
    img << "../test/test.pgm";

    abrsas::Image thresh;
    thresh = img * 256;

    for (auto it = thresh.begin(); it != thresh.end(); ++it)
    {
        REQUIRE(*it == 0);
    }

    thresh = img * -1;
    for (auto it = thresh.begin(); it != thresh.end(); ++it)
    {
        REQUIRE(*it == 255);
    }
}

TEST_CASE("Image4", "Testing !")
{
    abrsas::Image img;
    img << "../test/test.pgm";

    abrsas::Image inv(img);
    !inv;

    auto imgit = img.begin();
    auto invit = inv.begin();

    while (imgit != img.end())
    {
        REQUIRE((255 - *imgit) == *invit);

        ++imgit;
        ++invit;
    }
}

TEST_CASE("Image 5", "Testing /")
{
    abrsas::Image img;
    img << "../test/test.pgm";

    abrsas::Image mask;
    img << "../test/test_mask.pgm";

    abrsas::Image masked;
    masked = img / mask;

    auto maskedit = masked.begin();
    auto maskit = mask.begin();

    while (maskit != mask.end())
    {
        if (*maskit != 255)
        {
            REQUIRE(*maskedit == 0);
        } else
        {
            REQUIRE(*maskedit != 0);
        }

        ++maskedit;
        ++maskit;
    }
}

