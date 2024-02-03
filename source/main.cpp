#include <iostream>
#include <string>

#include "verification.hpp"
#include "error.hpp"

int main(int argc, char** argv)
{
    std::string data = R"({"Key 1":"Value 1","Key 2":{"Key 4.1":[1,2,3,4,5],"Key 4.2":null},"Key 3":true,"Key 4":12345})";

    json::result res = verify_json(data);

    std::cout << (res ? "Verified" : res.message()) << '\n';
}
