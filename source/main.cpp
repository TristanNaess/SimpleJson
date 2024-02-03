#include <iostream>
#include <string>

#include "verification.hpp"
#include "error.hpp"

int main(int argc, char** argv)
{
    //std::string data = R"({"Key 1":"Value 1","Key 2":{"Key 4.1":[1,2,3,4,5],"Key 4.2":null},"Key 3":true,"Key 4":12345})";
    std::string data = R"(["This","was","a","triumph",{"Key 1":"foobar","Key 2":3.14159,"Key 3":true},null,null,[1,2,3,4,5],false])";

    json::result res = verify_json(data);

    std::cout << (res ? "Verified" : res.message()) << '\n';
}
