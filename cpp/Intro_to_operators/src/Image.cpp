//
// Created by abrsas002 on 2018/04/17.
//

#include "Image.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

abrsas::Image::Image()
{
    image;
    h = 0;
    w = 0;
}

abrsas::Image::~Image()
{
    h = 0;
    w = 0;
    image.reset();
}

// TODO
// THIS DUN BROKE!
abrsas::Image::Image(abrsas::Image &&rhs)
{
    h = rhs.h;
    w = rhs.w;

    image = unique_ptr<unsigned char[]>(new unsigned char[w * h]);
    auto j = rhs.begin();
    for (auto i = begin(); i != end(); ++i)
    {
        *i = *j;
        *j = 0;
        ++j;
    }

    rhs.image.reset();
}

abrsas::Image::Image(const abrsas::Image &rhs)
{
    w = rhs.w;
    h = rhs.h;

    image = unique_ptr<unsigned char[]>(new unsigned char[w * h]);
    for (int i = 0; i < w * h; ++i)
    {
        image.get()[i] = rhs.image.get()[i];
    }
}

abrsas::Image &abrsas::Image::operator=(const abrsas::Image &rhs)
{
    w = rhs.w;
    h = rhs.h;

    image = unique_ptr<unsigned char[]>(new unsigned char[w * h]);
    for (int i = 0; i < w * h; ++i)
    {
        image.get()[i] = rhs.image.get()[i];
    }

    return *this;
}

void abrsas::Image::readHeader(std::string file)
{
    ifstream in(file, ios::binary);

    if (!in)
    {
        cout << "Could not read file" << endl;
        return;
    }
    string line;

    // skipping top two lines
    getline(in, line);
    getline(in, line);

    // using this line
    getline(in, line);

    // split
    string split;

    stringstream splitter(line);
    getline(splitter, split, ' ');

    h = stoi(split);

    getline(splitter, split, ' ');
    w = stoi(split);

    image = unique_ptr<unsigned char[]>(new unsigned char[w * h]);

    in.close();
}

void abrsas::Image::load(std::string file)
{
    readHeader(file);

    ifstream in(file, /*std::ios::in |*/ ios::binary);
    if (!in)
    {
        cout << "cannot open file" << endl;
        return;
    }

    string line;
    for (int j = 0; j < 4; ++j)
    {
        // ignore the lines
        getline(in, line);
    }

    for (auto i = begin(); i != end(); ++i)
    {
        *i = in.get();
    }

    in.close();
}

void abrsas::Image::save(std::string file)
{
    ofstream out(file, ios::binary);

    out << "P5" << endl;
    out << "#CREATOR: Sasha" << endl;
    out << h << " " << w << endl;
    out << "255" << endl;

    for (auto i = begin(); i != end(); ++i)
    {
        out.put(*i);
    }

    out.close();
}

void abrsas::Image::operator>>(const std::string &s)
{
    save(s);
}

void abrsas::Image::operator<<(const std::string &s)
{
    load(s);
}

abrsas::Image abrsas::Image::operator+(const abrsas::Image &rhs)
{
    if ((*this).w != rhs.w || (*this).h != rhs.h) throw invalid_argument("Dimensions unequal");

    Image result(*this);

    auto i = begin();
    auto j = rhs.begin();
    auto res = result.begin();

    while (i != end())
    {
        int sum = (int) *j + (int) *i;
        *res = clamp(sum);

        ++i;
        ++j;
        ++res;
    }

    return result;
}

abrsas::Image abrsas::Image::operator-(const abrsas::Image &rhs)
{
    if ((*this).w != rhs.w || (*this).h != rhs.h) throw invalid_argument("Dimensions unequal");

    Image result(*this);

    auto res = result.begin();
    auto i = begin();
    auto j = rhs.begin();

    while (i != end())
    {
        int sum = (int) *i - (int) *j;
        *res = clamp(sum);

        ++i;
        ++j;
        ++res;
    }

    return result;
}

abrsas::Image &abrsas::Image::operator!(void)
{
    for (auto i = begin(); i != end(); ++i)
    {
        int sum = 255 - (int) *i;
        *i = clamp(sum);
    }

    return *this;
}


abrsas::Image &abrsas::Image::operator*(const int &thresh)
{
    for (auto i = begin(); i != end(); ++i)
    {
        if (*i < thresh)
        {
            *i = 0;
        } else
        {
            *i = 255;
        }
    }

    return *this;
}

unsigned char abrsas::Image::clamp(const int &x)
{
    return (unsigned char) max(0, min(255, x));
}

abrsas::Image abrsas::Image::operator/(const abrsas::Image &rhs)
{
    Image result(*this);

    auto res = result.begin();
    auto rit = rhs.begin();

    while (rit != rhs.end())
    {
        if (*rit != 255)
        {
            *res = 0;
        }

        ++res;
        ++rit;
    }

    return result;
}

unsigned char
abrsas::Image::filter(const std::vector<std::vector<double>> &filter, const std::vector<std::vector<u_char>> &data)
{
    u_char tot = 0;
    for (int i = 0; i < filter.size(); ++i)
    {
        vector<unsigned char> row;
        for (int j = 0; j < filter[0].size(); ++j)
        {
            int add = (int) (tot + filter[i][j] * data[i][j]);
            tot = clamp(add);
        }
    }

    return tot;
}

abrsas::Image abrsas::Image::operator%(const vector<vector<double>> &filter)
{
    int height = filter.size();
    int width = filter[0].size();

    vector<vector<u_char >> unfiltered;
    Image toFilter(*this);

    for (int k = 0; k < w * h; ++k)
    {
        // getting the corresponding x and y values from 1D array
        int y = (int) floor(k / h);
        int x = 0;
        if (k != 0) x = w % k;

        // finds the y values to filter
        int y_min = y - (int) floor(height / 2);
        int y_max = y + (int) floor(height / 2) + 1;


        if (y_min < 0)
        {
            y_max -= y_min;
            y_min = 0;
        }

        if (y_max > h)
        {
            y_min -= (y_max - h);
            y_max = h;
        }

        // finds the x values to filter
        int x_min = x - (int) floor(width / 2);
        int x_max = x + (int) floor(width / 2) + 1;

        if (x_min < 0)
        {
            x_max -= x_min;
            x_min = 0;
        }

        if (x_max > w)
        {
            x_min -= (x_max - w);
            x_max = w;
        }

        for (int currentHeight = y_min; currentHeight < y_max; ++currentHeight)
        {
            vector<u_char> row;

            for (int currentWidth = x_min; currentWidth < x_max; ++currentWidth)
            {
                // adding each value to be filtered to a row
                row.push_back(image[currentHeight * w + currentWidth]);
            }
            // adding each row to an unfiltered
            unfiltered.push_back(row);
        }

        toFilter.image[k] = this->filter(filter, unfiltered);
        cout << (int)toFilter.image[k] << endl;
    }

    return toFilter;
}


