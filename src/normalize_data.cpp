#include "loader2/glyphs_loader.hpp"
#include "utils/image_compare.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>

#include <cstring>
#include <cerrno>

using namespace ppocr;

int main(int ac, char **av)
{
    if (ac < 2) {
        std::cerr << av[0] << "datas\n";
        return 1;
    }

    std::ifstream file(av[1]);
    if (!file) {
        std::cerr << strerror(errno) << '\n';
        return 2;
    }

    using Glyphs = loader2::Glyphs;
    using Glyph = loader2::Glyph;
    using Views = loader2::Views;

    Glyphs glyps;
    file >> glyps;

    if (!file.eof()) {
        std::cerr << "read error\n";
        return 5;
    }

    auto glyph_img_cmp = [](Glyph const & g1, Glyph const & g2) { return image_compare(g1.img, g2.img) < 0; };
    auto glyph_img_equal = [](Glyph const & g1, Glyph const & g2) { return g1.img == g2.img; };

    auto last = glyps.end();

    std::sort(glyps.begin(), last, glyph_img_cmp);

    auto first = std::adjacent_find(glyps.begin(), last, glyph_img_equal);

    auto first2 = first;
    while (first != last) {
        auto pos = std::upper_bound(first+1, last, *first, glyph_img_cmp);
        first2->img = std::move(first->img);
        //NOTE The implementation may assume that this parameter is a unique reference to this argument.
        //NOTE Self-move-assignment is not possible
        first2->views = Views(std::move(first->views));
        Views & views = first2->views;
        while (++first < pos) {
            views.insert(
                views.end(),
                std::make_move_iterator(first->views.begin()),
                std::make_move_iterator(first->views.end())
            );
        }
        std::sort(views.begin(), views.end());
        views.erase(std::unique(views.begin(), views.end()), views.end());
        ++first2;
    }
    glyps.erase(first2, last);

    std::cout << glyps;
}
