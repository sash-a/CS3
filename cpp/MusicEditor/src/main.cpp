#include <iostream>
#include <vector>
#include <cstring>
#include "Sound.h"

using namespace std;


void parse(abrsas::Sound<int8_t>, int argc, char *argv[], bool is_mono, int rate);

void parse(abrsas::Sound<int16_t>, int argc, char *argv[], bool is_mono, int rate);

void parse(abrsas::Sound<pair<int8_t, int8_t>> s, int argc, char *argv[], bool is_mono, int rate);

void parse(abrsas::Sound<pair<int16_t, int16_t>> s, int argc, char *argv[], bool is_mono, int rate);

int main(int argc, char *argv[])
{
    // TODO arg parse, testing, member functions
    int rate;
    bool is_mono;

    rate = atoi(argv[2]);
    int bitCount = atoi(argv[4]);
    is_mono = atoi(argv[6]) == 1;

    if (is_mono)
    {
        if (bitCount == 8)
        {
            abrsas::Sound<int8_t> s;
            s.rate = rate;

            parse(s, argc, argv, is_mono, rate);
        } else
        {
            abrsas::Sound<int16_t> s;
            s.rate = rate;

            int16_t i = 1;
            parse(s, argc, argv, is_mono, rate);
        }
    } else
    {
        if (bitCount == 8)
        {
            abrsas::Sound<pair<int8_t, int8_t>> s;
            s.rate = rate;

            pair<int8_t, int8_t> i;
            parse(s, argc, argv, is_mono, rate);
        } else
        {
            abrsas::Sound<pair<int16_t, int16_t>> s;
            s.rate = rate;

            pair<int16_t, int16_t> i;
            parse(s, argc, argv, is_mono, rate);
        }
    }
}


void parse(abrsas::Sound<int8_t> s, int argc, char *argv[], bool is_mono, int rate)
{
    for (int i = 7; i < argc; i += 2)
    {
        string outfile = "out.raw";
        if (strcmp(argv[i], "-o") == 0)
        {
            outfile.assign(argv[i + 1]);
        } else if (strcmp(argv[i], "-add") == 0)
        {
            abrsas::Sound<int8_t> snd(s);
            s.read(argv[i + 1]);
            snd.read(argv[i + 2]);

            abrsas::Sound<int8_t> result = s + snd;
            result.write(outfile);

        } else if (strcmp(argv[i], "-radd") == 0)
        {
            cout << "ranged add\n";

            abrsas::Sound<int8_t> snd(s);
            s.read(argv[i + 3]);
            snd.read(argv[i + 4]);

            pair<int, int> range = make_pair(atoi(argv[i + 1]), atoi(argv[i + 2]));

            abrsas::Sound<int8_t> result = s.radd(snd, range);
            result.write(outfile);

        } else if (strcmp(argv[i], "-v") == 0)
        {
            s.read(argv[i + 3]);

            pair<int, int> range = make_pair(atoi(argv[i + 1]), atoi(argv[i + 2]));

            abrsas::Sound<int8_t> result = s * range;
            result.write(outfile);

        } else if (strcmp(argv[i], "-rev") == 0)
        {
            s.read(argv[i + 1]);
            abrsas::Sound<int8_t> result = s.rev();
            s.write(outfile);

        } else if (strcmp(argv[i], "-rms") == 0)
        {
            s.read(argv[i + 1]);

            s.printRMS();

        } else if (strcmp(argv[i], "-norm") == 0)
        {
            s.read(argv[i + 3]);

            string str;
            if (!is_mono)
            {
                try
                {
                    abrsas::Sound<int8_t> result = s.norm(stod(str.assign(argv[i + 1])), stod(str.assign(argv[i + 2])));
                    result.write(outfile);
                }
                catch (const std::exception &e)
                {

                }
            }
        } else if (strcmp(argv[i], "-fadein") == 0)
        {
            s.read(argv[i + 2]);

            string str(argv[i + 1]);
            float time = stof(str);

            abrsas::Sound<int8_t> result = s.fadein(time);
            result.write(outfile);

        } else if (strcmp(argv[i], "-fadeout") == 0)
        {
            s.read(argv[i + 2]);

            string str(argv[i + 1]);
            float time = stof(str);

            abrsas::Sound<int8_t> result = s.fadeout(time);
            result.write(outfile);
        }

    }

}


void parse(abrsas::Sound<int16_t> s, int argc, char *argv[], bool is_mono, int rate)
{
    for (int i = 7; i < argc; i += 2)
    {
        string outfile = "out.raw";
        if (strcmp(argv[i], "-o") == 0)
        {
            outfile.assign(argv[i + 1]);
        } else if (strcmp(argv[i], "-add") == 0)
        {
            abrsas::Sound<int16_t> snd(s);
            s.read(argv[i + 1]);
            snd.read(argv[i + 2]);

            abrsas::Sound<int16_t> result = s + snd;
            result.write(outfile);

        } else if (strcmp(argv[i], "-radd") == 0)
        {
            abrsas::Sound<int16_t> snd(s);
            s.read(argv[i + 3]);
            snd.read(argv[i + 4]);

            pair<int, int> range = make_pair(atoi(argv[i + 1]), atoi(argv[i + 2]));

            abrsas::Sound<int16_t> result = s.radd(snd, range);
            result.write(outfile);

        } else if (strcmp(argv[i], "-v") == 0)
        {
            s.read(argv[i + 3]);

            pair<int, int> range = make_pair(atoi(argv[i + 1]), atoi(argv[i + 2]));

            abrsas::Sound<int16_t> result = s * range;
            result.write(outfile);

        } else if (strcmp(argv[i], "-rev") == 0)
        {
            s.read(argv[i + 1]);
            abrsas::Sound<int16_t> result = s.rev();
            s.write(outfile);

        } else if (strcmp(argv[i], "-rms") == 0)
        {
            s.read(argv[i + 1]);

            s.printRMS();

        } else if (strcmp(argv[i], "-norm") == 0)
        {
            s.read(argv[i + 3]);

            string str;
            if (!is_mono)
            {
                try
                {
                    abrsas::Sound<int16_t> result = s.norm(stod(str.assign(argv[i + 1])),
                                                           stod(str.assign(argv[i + 2])));
                    result.write(outfile);
                }
                catch (const std::exception &e)
                {

                }
            }
        } else if (strcmp(argv[i], "-fadein") == 0)
        {
            s.read(argv[i + 2]);

            string str(argv[i + 1]);
            float time = stof(str);

            abrsas::Sound<int16_t> result = s.fadein(time);
            result.write(outfile);

        } else if (strcmp(argv[i], "-fadeout") == 0)
        {
            s.read(argv[i + 2]);

            string str(argv[i + 1]);
            float time = stof(str);

            abrsas::Sound<int16_t> result = s.fadeout(time);
            result.write(outfile);
        }

    }

}





///////////////////////////////////////////////////////


void parse(abrsas::Sound<pair<int8_t, int8_t>> s, int argc, char *argv[], bool is_mono, int rate)
{
    for (int i = 7; i < argc; i += 2)
    {
        string outfile = "out.raw";
        if (strcmp(argv[i], "-o") == 0)
        {
            outfile.assign(argv[i + 1]);
        } else if (strcmp(argv[i], "-add") == 0)
        {
            abrsas::Sound<pair<int8_t, int8_t>> snd(s);
            s.read(argv[i + 1]);
            snd.read(argv[i + 2]);

            abrsas::Sound<pair<int8_t, int8_t>> result = s + snd;
            result.write(outfile);

        } else if (strcmp(argv[i], "-radd") == 0)
        {
            abrsas::Sound<pair<int8_t, int8_t>> snd(s);
            s.read(argv[i + 3]);
            snd.read(argv[i + 4]);

            pair<int, int> range = make_pair(atoi(argv[i + 1]), atoi(argv[i + 2]));

            abrsas::Sound<pair<int8_t, int8_t>> result = s.radd(snd, range);
            result.write(outfile);

        } else if (strcmp(argv[i], "-v") == 0)
        {
            s.read(argv[i + 3]);

            pair<int, int> range = make_pair(atoi(argv[i + 1]), atoi(argv[i + 2]));

            abrsas::Sound<pair<int8_t, int8_t>> result = s * range;
            result.write(outfile);

        } else if (strcmp(argv[i], "-rev") == 0)
        {
            s.read(argv[i + 1]);
            abrsas::Sound<pair<int8_t, int8_t>> result = s.rev();
            s.write(outfile);

        } else if (strcmp(argv[i], "-rms") == 0)
        {
            s.read(argv[i + 1]);

            s.printRMS();

        } else if (strcmp(argv[i], "-norm") == 0)
        {
            s.read(argv[i + 3]);

            string str;
            if (!is_mono)
            {
                try
                {
                    abrsas::Sound<pair<int8_t, int8_t>> result = s.norm(stod(str.assign(argv[i + 1])),
                                                                        stod(str.assign(argv[i + 2])));
                    result.write(outfile);
                }
                catch (const std::exception &e)
                {

                }
            }
        } else if (strcmp(argv[i], "-fadein") == 0)
        {
            s.read(argv[i + 2]);

            string str(argv[i + 1]);
            float time = stof(str);

            abrsas::Sound<pair<int8_t, int8_t>> result = s.fadein(time);
            result.write(outfile);

        } else if (strcmp(argv[i], "-fadeout") == 0)
        {
            s.read(argv[i + 2]);

            string str(argv[i + 1]);
            float time = stof(str);

            abrsas::Sound<pair<int8_t, int8_t>> result = s.fadeout(time);
            result.write(outfile);
        }

    }
}


void parse(abrsas::Sound<pair<int16_t, int16_t>> s, int argc, char *argv[], bool is_mono, int rate)
{
    for (int i = 7; i < argc; i += 2)
    {
        string outfile = "out.raw";
        if (strcmp(argv[i], "-o") == 0)
        {
            outfile.assign(argv[i + 1]);
        } else if (strcmp(argv[i], "-add") == 0)
        {
            abrsas::Sound<pair<int16_t, int16_t>> snd(s);
            s.read(argv[i + 1]);
            snd.read(argv[i + 2]);

            abrsas::Sound<pair<int16_t, int16_t>> result = s + snd;
            result.write(outfile);

        } else if (strcmp(argv[i], "-radd") == 0)
        {
            abrsas::Sound<pair<int16_t, int16_t>> snd(s);
            s.read(argv[i + 3]);
            snd.read(argv[i + 4]);

            pair<int, int> range = make_pair(atoi(argv[i + 1]), atoi(argv[i + 2]));

            abrsas::Sound<pair<int16_t, int16_t>> result = s.radd(snd, range);
            result.write(outfile);

        } else if (strcmp(argv[i], "-v") == 0)
        {
            s.read(argv[i + 3]);

            pair<int, int> range = make_pair(atoi(argv[i + 1]), atoi(argv[i + 2]));

            abrsas::Sound<pair<int16_t, int16_t>> result = s * range;
            result.write(outfile);

        } else if (strcmp(argv[i], "-rev") == 0)
        {
            s.read(argv[i + 1]);
            abrsas::Sound<pair<int16_t, int16_t>> result = s.rev();
            s.write(outfile);

        } else if (strcmp(argv[i], "-rms") == 0)
        {
            s.read(argv[i + 1]);

            s.printRMS();

        } else if (strcmp(argv[i], "-norm") == 0)
        {
            s.read(argv[i + 3]);

            string str;
            if (!is_mono)
            {
                try
                {
                    abrsas::Sound<pair<int16_t, int16_t>> result = s.norm(stod(str.assign(argv[i + 1])),
                                                                          stod(str.assign(argv[i + 2])));
                    result.write(outfile);
                }
                catch (const std::exception &e)
                {

                }
            }
        } else if (strcmp(argv[i], "-fadein") == 0)
        {
            s.read(argv[i + 2]);

            string str(argv[i + 1]);
            float time = stof(str);

            abrsas::Sound<pair<int16_t, int16_t>> result = s.fadein(time);
            result.write(outfile);

        } else if (strcmp(argv[i], "-fadeout") == 0)
        {
            s.read(argv[i + 2]);

            string str(argv[i + 1]);
            float time = stof(str);

            abrsas::Sound<pair<int16_t, int16_t>> result = s.fadeout(time);
            result.write(outfile);
        }

    }
}
