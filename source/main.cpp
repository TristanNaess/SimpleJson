//
// For now, this is a just a test of functionality, to see if the accessors and casting work as I hope they do
//

#include <iostream>
#include <string>
#include "parsing.hpp"

class Data
{
    public:
        Data(const std::string& line) : m_data(line) {  }

        // add accessors
        

        // debug print, to dump m_data
        void debug_print()
        {
            std::cout << "Contents: " << m_data << std::endl;
        }
    private:
        std::string m_data;
};


int main()
{
    // construct data
    Data d{"\"String\":\"Hello World\",\"Number\":3.14159"};
    d.debug_print();

    // read string field
    std::cout << d.get<std::string>("String") << std::endl;

    // read number field
    std::cout << d.get<double>("Number") << std::endl;

    // change string field
    d.get<std::string>("String") = "This is a longer string and will mangle the number if not properly inserted";
    d.debug_print();

    // change number field
    d.get<double>("Number") = 42;
    d.debug_print();
}
