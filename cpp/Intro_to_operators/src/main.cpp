#include <iostream>
#include <cstring>
#include "Image.h"


int main(int argc, char *argv[])
{
    using namespace std;
    abrsas::Image img;

    img << "../test/test.pgm";
    vector<double> r1 = {0.09, 0.35, 0.3};
    vector<double> r2 = {0.25, 0.18, 0.14};
    vector<double> r3 = {0.29, 0.08, 0.18};

    vector<vector<double>> vec;
    vec.push_back(r1);
    vec.push_back(r2);
    vec.push_back(r3);

    abrsas::Image imgn;
    imgn = img % vec;
    imgn >> "../output/filtered.pgm";


    if (argc > 2)
    {
        img << argv[2];
    } else
    {
        return 0;
    }

    if (strcmp(argv[1], "-a") == 0)
    {
        abrsas::Image other;
        other << argv[3];

        (img + other) >> "../output/Adding.pgm";
    }
    if (strcmp(argv[1], "-s") == 0)
    {

        abrsas::Image other;
        other << argv[3];

        (img - other) >> "../output/Subtracting.pgm";
    }
    if (strcmp(argv[1], "-i") == 0)
    {
        (!img) >> "../output/inverse.pgm";
    }
    if (strcmp(argv[1], "-l") == 0)
    {
        abrsas::Image other;
        other << argv[3];

        (img / other) >> "../output/mask.pgm";
    }
    if (strcmp(argv[1], "-t") == 0)
    {
        int thresh = stoi(argv[3]);

        (img * thresh) >> "../output/thresh.pgm";
    }
}
