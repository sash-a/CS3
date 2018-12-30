//
// Created by abrsas002 on 2018/05/10.
//

#ifndef MUSICEDITOR_SOUND_H
#define MUSICEDITOR_SOUND_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cmath>
#include <fstream>

namespace abrsas
{
    template<typename T>
    struct square
    {
        T operator()(const T &lhs, const T &rhs) const
        {
            return (lhs + rhs * rhs);
        }
    };

    template<typename T>
    struct square<std::pair<T, T>>
    {
        std::pair<T, T> operator()(const std::pair<T, T> &lhs, const std::pair<T, T> &rhs) const
        {
            return (std::make_pair(lhs.first + rhs.first * rhs.first, lhs.second + rhs.second * rhs.second));
        }
    };

    // for non-pair
    template<typename bit>
    class Sound
    {
    public:
        std::vector<bit> bits;
        int rate;

        Sound() : rate(44100)
        { bits; }

        Sound(const std::vector<bit> &bits, const int &r) : bits(bits), rate(r)
        {}

        virtual ~Sound()
        {}

        Sound(const Sound<bit> &sound) : bits(sound.bits), rate(sound.rate)
        {}

        Sound(Sound<bit> &&sound) : bits(sound.bits), rate(sound.rate)
        {}

        Sound operator=(const Sound<bit> &&s)
        {
            Sound<bit> snd(s);
            return s;
        };

        Sound operator=(const Sound<bit> &s)
        {
            Sound<bit> snd(s);
            return s;
        };

        void read(const std::string &file)
        {
            std::ifstream in(file, std::ios::binary);

            int bits_to_read = sizeof(bit);

            if (!in) std::cout << "couldn't open\n";

            // finding file length
            // this is coppied from source in notes
            in.seekg(0, in.end);
            int length = in.tellg();
            in.seekg(0, in.beg);


            bits.resize(length / sizeof(bit));
            int i = 0;

            while (in.tellg() != length)
            {
                in.read((char *) &bits[i], sizeof(bit));
                ++i;
            }

            in.close();
        }

        void write(const std::string &file = "../out.raw")
        {
            std::ofstream out(file, std::ios::binary);
            for (int i = 0; i < bits.size(); ++i)
            {
                out.write((char *) &bits[i], sizeof(bit));
            }
            out.close();
        }

        Sound<bit> operator|(const Sound<bit> &sound)
        {
            Sound<bit> s(*this);

            for (auto &bt : sound.bits)
            {
                s.bits.push_back(bt);
            }

            return s;
        };


        Sound<bit> operator+(const Sound<bit> &sound)
        {
            Sound<bit> s(*this);

            for (int i = 0; i < s.bits.size(); ++i)
            {
                s.bits[i] += sound.bits[i];
            }
            return s;
        };

        Sound<bit> operator*(const std::pair<float, float> &volume)
        {
            Sound<bit> s(*this);

            for (auto &i : s.bits)
            {
                i *= volume.first;
            }
            return s;
        };

        Sound<bit> operator^(std::pair<int, int> const &slice)
        {
            Sound<bit> s;

            for (int i = slice.first; i < slice.second; ++i)
            {
                s.bits.push_back(bits[i]);
            }

            return s;
        };

        Sound<bit> cut(const std::pair<int, int> &range)
        {
            auto firstseg = (*this) ^std::make_pair(0, range.first);
            auto result = firstseg | ((*this) ^ std::make_pair(range.second, (int) bits.size()));
            return result;
        };


        Sound<bit> radd(Sound<bit> &other, std::pair<int, int> &range)
        {
            std::vector<bit> result(bits);
            auto added = ((*this) ^ range) + (other ^ range);

            std::copy(added.bits.begin(), added.bits.end(), result.begin() + range.first);

            Sound s(result, rate);
            return s;
        };

        Sound<bit> rev()
        {
            Sound<bit> cpy(*this);

            std::reverse(cpy.bits.begin(), cpy.bits.end());

            return cpy;
        };

        double rms()
        {
            double rms = std::accumulate(this->bits.begin(), this->bits.end(), 0.0, square<double>());
            rms = rms / (double) this->bits.size();
            rms = std::sqrt(rms);

            return rms;
        };

        // This is only here as arg parsing was giving issues
        Sound<bit> norm(const double &rms1, const double &rms2)
        {};


        Sound<bit> fadein(const float &time)
        {
            int i = 0;
            int ramp = (int) time * rate;

            std::vector<bit> result;

            std::transform(bits.begin(), bits.end(), std::back_inserter(result),
                           [&i, &ramp](bit &val) -> bit
                           {
                               if (ramp > i)
                               {
                                   auto res = ((i / (float) ramp) * val);
                                   ++i;
                                   return res;
                               }
                               return val;
                           });

            return *this;
        };

        Sound<bit> fadeout(const float &time)
        {
            int i = 0;
            int ramp = (int) time * rate;

            std::vector<bit> result;

            std::transform(bits.begin(), bits.end(), std::back_inserter(result),
                           [&i, &ramp, this](bit &val) -> bit
                           {
                               if (this->bits.size() - ramp <= i)
                               {
                                   // Avoids negative volume
                                   float modifier = (1 - i / (float) ramp);
                                   modifier = modifier > 0 ? modifier : 0;

                                   auto res = modifier * val;
                                   ++i;
                                   return res;
                               }
                               ++i;
                               return val;
                           });
            Sound<bit> faded(result, rate);
            return faded;
        };

        void printRMS()
        {
            std::cout << "RMS: " << this->rms() << std::endl;
        };


        void print()
        {
            for (auto &bt : bits)
            {
                std::cout << (int) bt << std::endl;
            }
        };


    private:
//        bool &isCompatible(const Sound<bit> &sound);
    };


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    template<typename bit>
    class Sound<std::pair<bit, bit>>
    {
    public:
        std::vector<std::pair<bit, bit>> bits;
        int rate;

        Sound() : rate(44100)
        {}

        Sound(const std::vector<std::pair<bit, bit>> &bits, const int &r) : bits(bits), rate(r)
        {}

        virtual ~Sound()
        {}

        Sound(const Sound<std::pair<bit, bit>> &sound) : bits(sound.bits), rate(sound.rate)
        {}

        Sound(Sound<std::pair<bit, bit>> &&sound) : bits(sound.bits), rate(sound.rate)
        {}

        Sound operator=(const Sound<std::pair<bit, bit>> &s)
        {
            Sound<std::pair<bit, bit>> snd(s);
            return s;
        };

        Sound operator=(const Sound<std::pair<bit, bit>> &&s)
        {
            Sound<std::pair<bit, bit>> snd(s);
            return s;
        };

        void read(const std::string &file)
        {
            std::ifstream in(file, std::ios::binary);

            int bits_to_read = sizeof(bit);

            if (!in) std::cout << "couldn't open\n";

            // finding file length
            // this is coppied from source in notes
            in.seekg(0, in.end);
            int length = in.tellg();
            in.seekg(0, in.beg);


            bits.resize(length / sizeof(bit));
            int i = 0;

            std::cout << "here\n";
            while (in.tellg() != length)
            {
                bit first;
                bit second;
                in.read((char *) &first, sizeof(bit));
                in.read((char *) &second, sizeof(bit));
                bits[i] = std::make_pair(first, second);
                ++i;
            }

            in.close();
        }

        void write(const std::string &file = "../out.raw")
        {
            std::ofstream out(file, std::ios::binary);
            for (int i = 0; i < bits.size(); ++i)
            {
                out.write((char *) &bits[i].first, sizeof(bit));
                out.write((char *) &bits[i].second, sizeof(bit));
            }
            out.close();
        }

        Sound<std::pair<bit, bit>> operator|(const Sound<std::pair<bit, bit>> &sound)
        {
            Sound<std::pair<bit, bit>> s(*this);

            for (auto &bt : sound.bits)
            {
                s.bits.push_back(bt);
            }

            return s;
        };

        Sound<std::pair<bit, bit>> operator+(const Sound<std::pair<bit, bit>> &sound)
        {
            Sound<std::pair<bit, bit>> s(*this);

            for (int i = 0; i < s.bits.size(); ++i)
            {
                // checking overflow
                int check = s.bits[i].first + sound.bits[i].first;
                if (check > std::numeric_limits<bit>::max())
                {
                    check = std::numeric_limits<bit>::max();
                }
                s.bits[i].first = check;

                // checking overflow
                check = s.bits[i].second + sound.bits[i].second;
                if (check > std::numeric_limits<bit>::max())
                {
                    check = std::numeric_limits<bit>::max();
                }
                s.bits[i].second = check;
            }

            return s;
        };

        Sound<std::pair<bit, bit>> operator*(std::pair<float, float> const &volume)
        {
            Sound<std::pair<bit, bit>> s(*this);

            for (std::pair<bit, bit> &i : s.bits)
            {
                i.first *= volume.first;
                i.second *= volume.first;
            }

            return s;
        };

        Sound<std::pair<bit, bit>> operator^(std::pair<int, int> const &slice)
        {
            Sound<std::pair<bit, bit>> s;

            for (int i = slice.first; i < slice.second; ++i)
            {
                s.bits.push_back(bits[i]);
            }

            return s;
        };

        Sound<std::pair<bit, bit>> cut(const std::pair<int, int> &range)
        {
            auto firstseg = (*this) ^std::make_pair(0, range.first);
            auto result = firstseg | ((*this) ^ std::make_pair(range.second, (int) bits.size()));
            return result;
        };

        Sound<std::pair<bit, bit>>
        radd(Sound<std::pair<bit, bit>> &other, std::pair<int, int> &range)
        {
            range.first *= rate;
            range.second *= rate;

            std::vector<std::pair<bit, bit>> result(bits);
            auto added = ((*this) ^ (range)) + (other ^ range);

            std::copy(added.bits.begin(), added.bits.end(), result.begin() + range.first);

            Sound<std::pair<bit, bit>> s(result, rate);
            return s;
        };

        Sound<std::pair<bit, bit>> rev()
        {
            Sound<std::pair<bit, bit>> cpy(*this);

            std::reverse(cpy.bits.begin(), cpy.bits.end());

            return cpy;
        };

        std::pair<double, double> rms()
        {
            std::pair<double, double> rms = std::accumulate(this->bits.begin(), this->bits.end(),
                                                            std::make_pair(0.0, 0.0),
                                                            square<std::pair<double, double>>());

            rms.first = rms.first / (double) this->bits.size();
            rms.second = rms.second / (double) this->bits.size();

            rms.first = std::sqrt(rms.first);
            rms.second = std::sqrt(rms.second);

            return rms;
        };

        Sound<std::pair<bit, bit>> norm(const double &rms1, const double &rms2)
        {
            std::vector<std::pair<bit, bit>> result;
            auto currentRMS = this->rms();

            std::transform(bits.begin(), bits.end(), std::back_inserter(result),
                           [rms1, rms2, currentRMS](const std::pair<bit, bit> &val) -> std::pair<bit, bit>
                           {
                               bit first = std::max(
                                       std::min((int) val.first * rms1 / currentRMS.first,
                                                (double) std::numeric_limits<bit>::max()),
                                       (double) std::numeric_limits<bit>::min());

                               bit second = std::max(
                                       std::min((int) val.second * rms2 / currentRMS.second,
                                                (double) std::numeric_limits<bit>::max()),
                                       (double) std::numeric_limits<bit>::min());

                               return std::make_pair(first, second);
                           });

            Sound<std::pair<bit, bit>> s(result, rate);
            return s;
        };

        Sound<std::pair<bit, bit>> fadein(const float &time)
        {
            int i = 0;
            int ramp = (int) time * rate;

            std::vector<std::pair<bit, bit>> result;

            std::transform(bits.begin(), bits.end(), std::back_inserter(result),
                           [&i, &ramp](std::pair<bit, bit> &val) -> std::pair<bit, bit>
                           {
                               if (ramp > i)
                               {
                                   auto res = std::make_pair((i / (float) ramp) * val.first,
                                                             (i / (float) ramp) * val.second);
                                   ++i;
                                   return res;
                               }
                               return val;
                           });

            return *this;
        };

        Sound<std::pair<bit, bit>> fadeout(const float &time)
        {
            int i = 0;
            int ramp = (int) time * rate;

            std::vector<std::pair<bit, bit>> result;

            std::transform(bits.begin(), bits.end(), std::back_inserter(result),
                           [&i, &ramp, this](std::pair<bit, bit> &val) -> std::pair<bit, bit>
                           {
                               if (this->bits.size() - ramp <= i)
                               {
                                   // Avoids negative volume
                                   float modifier = (1 - i / (float) ramp);
                                   modifier = modifier > 0 ? modifier : 0;

                                   auto res = std::make_pair(
                                           modifier * val.first,
                                           modifier * val.second);
                                   ++i;
                                   return res;
                               }
                               ++i;
                               return val;
                           });
            Sound<std::pair<bit, bit>> faded(result, rate);
            return faded;
        };

        void printRMS()
        {

            auto r = this->rms();
            std::cout << "RMS: " << r.first << "    " << r.second << std::endl;
        };

        void print()
        {
            for (auto &bt : bits)
            {
                std::cout << (int) bt.first << " " << (int) bt.second << std::endl;
            }
        };

    private:
//        bool &isCompatible(const Sound<std::pair<bit,bit>> &sound);
    };
}
#endif //MUSICEDITOR_SOUND_H
