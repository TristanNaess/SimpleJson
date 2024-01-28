#include <iostream>
#include <string>

#include "error.hpp"

int main(int argc, char** argv)
{
    json::Result res("This is the initial message");
    json::Result res2("this is the prefix: ", res);

    std::cout << res2.message() << '\n';
}
