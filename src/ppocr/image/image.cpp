#include "ppocr/image/image.hpp"
#include <ostream>

#include <cassert>

namespace ppocr {

namespace {
    using cP = Pixel const *;
    using P = Pixel *;
}

Image::Image(Bounds const& bounds, PtrImageData data)
: data_(std::move(data))
, bounds_(bounds)
{}

void section(Image const & from, Pixel * data, Index const & idx, Bounds const & bnd)
{
    cP d = from.data(idx);
    for (size_t y = 0; y != bnd.h(); ++y) {
        data = std::copy(d, d+bnd.w(), data);
        d += from.width();
    }
}

Image Image::section(Index const& section_idx, Bounds const& section_bnd) const
{
    assert(bounds_.contains(section_idx));
    assert(section_bnd.w() + section_idx.x() <= width() && section_bnd.h() + section_idx.y() <= height());
    PtrImageData data(new Pixel[section_bnd.area()]);
    ::ppocr::section(*this, data.get(), section_idx, section_bnd);
    return {{section_bnd.w(), section_bnd.h()}, std::move(data)};
}

void rotate90(Image const & from, Pixel * data)
{
    for (size_t x = from.width(); x; ) {
        --x;
        for (cP d = from.data() + x, e = d + from.area(); d != e; d += from.width()) {
            *data++ = *d;
        }
    }
}

Image Image::rotate90() const
{
    PtrImageData data(new Pixel[this->area()]);
    ::ppocr::rotate90(*this, data.get());
    return {{this->height(), this->width()}, std::move(data)};
}

Image Image::clone() const
{
    return this->section({}, this->bounds());
}

bool operator==(const Image& a, const Image& b)
{
    return a.width() == b.width()
        && a.height() == b.height()
        && std::equal(a.data(), a.data_end(), b.data());
}

std::ostream & operator<<(std::ostream & os, Image const & image)
{
    os.fill(':');
    os.width(image.width()+3);
    os << ":\n";
    cP p = image.data_.get();
    for (size_t h = 0; h != image.height(); ++h) {
        os << ':';
        os.write(p, image.width());
        os << ":\n";
        p += image.width();
    }
    os.width(image.width()+3);
    os << ":\n";
    return os;
}

}