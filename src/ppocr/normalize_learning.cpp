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

#include "ppocr/factory/data_loader.hpp"
#include "ppocr/factory/registry.hpp"
#include "ppocr/utils/unique_sort_definition.hpp"

#include <iostream>
#include <fstream>

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

    DataLoader loader;
    all_registy(loader);

    std::vector<Definition> definitions = read_definitions(file, loader);

    if (!file.eof()) {
        std::cerr << "read error\n";
        return 5;
    }

    unique_sort_definitions(definitions);

    for (auto & def : definitions) {
        write_definition(std::cout, def, loader);
    }
}
