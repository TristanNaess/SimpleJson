#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "verification.hpp"
#include "error.hpp"
#include "parsing.hpp"
#include "json.hpp"

int main(int argc, char** argv)
{
    std::cout << "Dummy body" << std::endl;
    /*
    std::ifstream file{ "resource/data.json" };

    std::string buffer;
    std::getline(file, buffer);
   
    file.close();

    json::Json data{buffer};

    auto keys = data.keys();

    for (const auto& k : keys)
    {
        std::cout << k << '\n';
    }
    */
}
