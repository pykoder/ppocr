#include "image/image.hpp"
#include "image/image_from_file.hpp"
#include "box_char/make_box_character.hpp"

#include "loader2/glyphs_loader.hpp"

#include <iostream>
#include <fstream>
#include <vector>

#include <cstring>
#include <cerrno>
#include <stdexcept>

using namespace ppocr;

static void load_file(
    loader2::Glyphs & glyps,
    const char * imagefilename,
    const char * textfilename,
    const char * fontname,
    unsigned luminance
) {
    std::ifstream file(textfilename);
    if (!file) {
        throw std::runtime_error(std::string(textfilename) + ": " + strerror(errno));
    }

    Image img = image_from_file(imagefilename, luminance);
    //std::cerr << img << '\n';

    size_t x = 0;
    Bounds const & bounds = img.bounds();

    std::string s;

    while (auto const cbox = make_box_character(img, {x, 0}, bounds)) {
        //std::cerr << "\nbox(" << cbox << ")\n";

        if (!(file >> s)) {
            throw std::runtime_error("definition not found");
        }

        auto newimg = img.section(cbox.index(), cbox.bounds());
        //std::cerr << newimg << s << '\n';

        glyps.push_back({
            std::move(newimg),
            {{s, fontname, ~0u}}
        });

        x = cbox.x() + cbox.w();
    }

    if (file >> s) {
        throw std::runtime_error("box character not found");
    }
}


int main(int ac, char **av)
{
    int i_ac = 0;
    unsigned luminance = 128;
    if (ac > 3) {
        if (av[1][0] == '-' && av[1][1] == 'l' && av[1][2] == '\0') {
            char * p;
            luminance = std::strtoul(av[2], &p, 10);
            if (p == av[2] || *p != '\0' || errno) {
                return 2;
            }
            i_ac += 2;
        }
    }
    if ((ac - i_ac) < 4 || (ac - i_ac - 1) % 3) {
        std::cerr << av[0] << " [-l luminance] image_file text_file font_name [image_file text_file font_name ...]\n";
        return 1;
    }

    loader2::Glyphs glyps;

    for (++i_ac; i_ac < ac; i_ac += 3) {
        load_file(glyps, av[i_ac], av[i_ac+1], av[i_ac+2], luminance);
    }

    std::cout << glyps;
}
