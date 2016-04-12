#include "ppocr/strategies/hbar.hpp"
#include "ppocr/image/image.hpp"

#include <algorithm>

namespace ppocr { namespace strategies {

hbar::value_type hbar::load(const Image& img, const Image& /*img90*/) const
{
    hbar::value_type n{};
    auto p = img.data();
    auto const e = img.data_end();
    bool previous_is_plain = false;
    for (; p != e; p += img.width()) {
        if (std::all_of(p, p+img.width(), is_pix_letter_fn())) {
            if (!previous_is_plain) {
                previous_is_plain = true;
                ++n;
            }
        }
        else {
            previous_is_plain = false;
        }
    }
    return n;
}

} }