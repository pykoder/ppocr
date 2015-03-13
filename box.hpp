/*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
*   Product name: redemption, a FLOSS RDP proxy
*   Copyright (C) Wallix 2010-2015
*   Author(s): Jonathan Poelen
*/

#ifndef REDEMPTION_BOX_HPP
#define REDEMPTION_BOX_HPP

#include "coordinate.hpp"

#include <iosfwd>


struct Box
{
    Box(Index idx,  Bounds bnd)
    : idx_(idx)
    , bounds_(bnd)
    {}

    Box(Bounds bnd)
    : bounds_(bnd)
    {}

    size_t x() const noexcept { return idx_.x(); }
    size_t y() const noexcept { return idx_.y(); }

    size_t w() const noexcept { return bounds_.w(); }
    size_t h() const noexcept { return bounds_.h(); }

    Index const & index() const noexcept { return idx_; }
    Bounds const & bounds() const noexcept { return bounds_; }

    explicit operator bool() const noexcept { return w() && h(); }

private:
    Index idx_;
    Bounds bounds_;
};

std::ostream & operator<<(std::ostream & os, Box const & box);
std::istream & operator>>(std::istream & is, Box & box);

#endif