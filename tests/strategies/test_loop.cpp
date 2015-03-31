/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

  Author(s): Jonathan Poelen
*/

#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestLoop
#include <boost/test/auto_unit_test.hpp>

#undef SHARE_PATH
#define SHARE_PATH FIXTURES_PATH

#define LOGNULL
//#define LOGPRINT

#include "strategies/loop.hpp"
#include "image/image_from_string.hpp"
#include "image/image.hpp"
#include <sstream>

#define IMAGE_PATH "./images/"

using L = strategies::loop;

static L mk_loop(Bounds bnd, const char * data_text)
{
    Image img = image_from_string(bnd, data_text);
    return strategies::loop(img, img.rotate90());
}

BOOST_AUTO_TEST_CASE(TestLoop)
{
    L loop;

    using A = L::value_type;

    loop = mk_loop({3, 3},
        "xxx"
        "---"
        "-x-"
    );
    BOOST_CHECK((loop.datas() == A{1, 0, 0, 1, 1, 1, 0}));

    loop = mk_loop({3, 5},
        "--x"
        "---"
        "---"
        "xxx"
        "--x"
    );
    BOOST_CHECK((loop.datas() == A{0, 1, 1, 1, 1, 2, 0}));

    loop = mk_loop({3, 4},
        "-x-"
        "x-x"
        "xxx"
        "x-x"
    );
    BOOST_CHECK((loop.datas() == A{0, 1, 2, 1, 1, 1, 1}));

    loop = mk_loop({5, 6},
        "--x--"
        "-x-x-"
        "x---x"
        "xxxxx"
        "x---x"
        "x---x"
    );
    BOOST_CHECK((loop.datas() == A{0, 1, 2, 1, 1, 1, 1}));
}
