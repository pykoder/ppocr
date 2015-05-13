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

#include "container/trie.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <cstring>
#include <cerrno>

int main(int ac, char ** av) {
    if (ac < 2) {
        std::cerr << av[0] << " words_file...\n";
        return 1;
    }

    std::ifstream file;
    std::vector<std::string> words;

    for (int i = 1; i < ac; ++i) {
        file.open(av[i]);
        if (!file) {
            std::cerr << av[i] << ": " << strerror(errno) << std::endl;
            continue;
        }

        std::string s;
        while (std::getline(file, s)) {
            words.push_back(s);
        }
    }

    std::sort(words.begin(), words.end());
    words.erase(std::unique(words.begin(), words.end()), words.end());

    container::trie<char> trie(words.begin(), words.end());
    for (auto node : container::flat_trie<char>(trie).all()) {
        std::cout
          << unsigned(node.get()) << " "
          << node.relative_pos() << " "
          << node.size() << " "
          << node.is_terminal()
          << "\n";
    }
}
