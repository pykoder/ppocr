/*
* Copyright (C) 2016 Wallix
* 
* This library is free software; you can redistribute it and/or modify it under
* the terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; either version 2.1 of the License, or (at your option)
* any later version.
* 
* This library is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
* details.
* 
* You should have received a copy of the GNU Lesser General Public License along
* with this library; if not, write to the Free Software Foundation, Inc., 59
* Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <stdexcept>
#include <cstring>
#include <cerrno>

#include "ppocr/image/image_from_file.hpp"

#include "mln/image/image2d.hh"
#include "mln/io/ppm/load.hh"
#include "mln/io/pbm/load.hh"

#include <mln/trait_value_.hh>

namespace ppocr { namespace detail_  {
    class mln_pixel_image
    {
    public:
        typedef unsigned char value_type;

        constexpr value_type red()   const noexcept { return this->components[0]; }
        constexpr value_type green() const noexcept { return this->components[1]; }
        constexpr value_type blue()  const noexcept { return this->components[2]; }

    private:
        value_type components[3];
    };
} }


namespace mln
{
    namespace trait
    {
        template <>
        struct value_< ::ppocr::detail_::mln_pixel_image>
        {
            static ::ppocr::detail_::mln_pixel_image::value_type max() { return 255u; }
            static void read_value(std::istream& is, ::ppocr::detail_::mln_pixel_image & v)
            { is.read(reinterpret_cast<char*>(&v), sizeof(::ppocr::detail_::mln_pixel_image)); }
        };
    }
} // mln

namespace ppocr {

template<class T, class ToPix>
Image image2d_to_Image(mln::image2d<T> const & img, ToPix to_pix)
{
    PtrImageData vimg(new Pixel[img.ncols() * img.nrows()]);
    auto it = vimg.get();
    for (unsigned y = 0; y < img.nrows(); y++) {
        for (unsigned x = 0; x < img.ncols(); x++) {
            *it++ = to_pix(img.at(y, x));
        }
    }
    return Image(Bounds(img.ncols(), img.nrows()), std::move(vimg));
}


Image image_from_file(const char * filename, unsigned luminance)
{
    mln::image2d<detail_::mln_pixel_image> img;

    if (!mln::io::ppm::load(img, filename)) {
        mln::image2d<bool> img;
        if (!mln::io::pbm::load(img, filename)) {
            std::string desc = "image_from_file: ";
            desc += filename;
            if (errno) {
                desc += ": ";
                desc += strerror(errno);
            }
            throw std::runtime_error(std::move(desc));
        }

        return image2d_to_Image(img, [&](bool x){ return x ? 'x' : '-'; });
    }

    return image2d_to_Image(img, [&](const detail_::mln_pixel_image & rgb){
        unsigned char c
        = ((511/*PPM_RED_WEIGHT*/   * rgb.red()   + 511)>>10)
        + ((396/*PPM_GREEN_WEIGHT*/ * rgb.green() + 511)>>10)
        + ((117/*PPM_BLUE_WEIGHT*/  * rgb.blue()  + 511)>>10);
        return (c < luminance) ? '-' : 'x';
//         *it++ = !(rgb.red() == 60 && rgb.green() == 64 && rgb.blue() == 72) ? cc :ncc;
//         *it++ = (rgb.red() == 255 && rgb.green() == 255 && rgb.blue() == 255) ? ncc : cc;
    });
}

}
