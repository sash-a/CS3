//
// Created by abrsas002 on 2018/04/17.
//

#ifndef IMAGEOPS_IMAGE_H
#define IMAGEOPS_IMAGE_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <vector>

namespace abrsas
{
    class Image
    {
    private:
        std::unique_ptr<unsigned char[]> image;

        unsigned char clamp(const int &);

        unsigned char filter(const std::vector<std::vector<double>> &, const std::vector<std::vector<u_char>> &filter);

        int w;
        int h;

        void readHeader(std::string);

    public:
        Image();

        //Image(const int &, const int &);
        ~Image();

        Image(Image &&);

        Image(const Image &);

        Image &operator=(const Image &);

        void load(std::string s);

        void save(std::string s);

        Image operator+(const Image &);

        Image operator-(const Image &);

        Image operator/(const Image &);

        Image operator%(const std::vector<std::vector<double>> &filter);

        // TODO is this right with Image &
        Image &operator!(void); // invert
        Image &operator*(const int &); // threshold

        void operator<<(const std::string &s);

        void operator>>(const std::string &s);

        /*
         * Iterator
         */
        class iterator
        {
        private:
            friend class Image;

            unsigned char *ptr;

            // construct only via Image class (begin/end)
            iterator(u_char *p) : ptr(p)
            {}

        public://copy construct is public
            iterator(const iterator &rhs) : ptr(rhs.ptr)
            {}

            // define overloaded ops: *, ++, --, =
            iterator &operator=(const iterator &rhs)
            {
                this->ptr = rhs.ptr;
            }

            bool operator!=(const iterator &rhs)
            {
                return ptr != rhs.ptr;
            }

            abrsas::Image::iterator &operator++()
            {
                ptr += sizeof(unsigned char);
                return *this;
            }

            abrsas::Image::iterator &operator--()
            {
                ptr -= sizeof(unsigned char);
                return *this;
            }

            u_char &operator*(void)
            {
                return *ptr;
            }
        };

        abrsas::Image::iterator begin(void) const
        {
            return iterator(&image.get()[0]);
        }

        abrsas::Image::iterator end(void) const
        {
            return iterator(&image.get()[w * h]);
        }
    };
}

#endif //IMAGEOPS_IMAGE_H
