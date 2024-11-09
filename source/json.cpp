#include "json.hpp"

namespace json
{

    // -----------------------------------
    //  json::json
    // -----------------------------------

    json::json()
    {
        m_type = Type::Null;
    }

    json::json(const std::string& key)
    {
        throw todo{"TODO: json::json(const std::string&)"};
    }

    json::json(const char* key)
    {
        throw todo{"TODO: json::json(const char*)"};
    }

    std::vector<std::string> json::keys() // json::wrong_type
    {
        throw todo{"TODO: json::keys()"};
    }

    bool json::contains(const std::string& key) // json::wrong_type
    {
        throw todo{"TODO: json::contains(const std::string&)"};
    }

    bool json::contains(const char* key) // json::wrong_type
    {
        throw todo{"TODO: json::contains(const char*)"};
    }

    std::size_t json::size() // json::wrong_type
    {
        throw todo{"TODO: json::size()"};
    }

    Type json::type() noexcept
    {
        return Type::Null; // would throw todo, but marked noexcept for later
    }

    accessor json::operator[](const std::string& key) // json::wrong_type, json::out_of_range
    {
        throw todo{"TODO: json::operator[](const std::string&)"};
    }

    accessor json::operator[](const char* key) // json::wrong_type, json::out_of_range
    {
        throw todo{"TODO: json::operator[](const char*)"};
    }

    accessor json::operator[](std::size_t index) // json::wrong_type, json::out_of_range
    {
        throw todo{"TODO: json::operator[](std::size_t)"};
    }

    // iterator json::begin()

    // iterator json::end()


    // -----------------------------------
    //  json::accessor
    // -----------------------------------

    accessor(std::string& data, std::string::iterator front, std::string::iterator back)
    {
        throw todo{"TODO: accessor(std::string&, std::string::iterator, std::string::iterator)"};
    }

    std::vector<std::string> accessor::keys()       // json::wrong_type
    {
        throw todo{"TODO: accessor::keys()"};
    }

    bool accessor::contains(const std::string& key) // json::wrong_type
    {
        throw todo{"TODO: accessor::contains(const std::string&)"};
    }

    bool accessor::contains(const char* key)        // json::wrong_type
    {
        throw todo{"TODO: accessor::contains(const char*)"};
    }

    // array data
    std::size_t accessor::size() // json::wrong_type
    {
        throw todo{"TODO: accessor::size()"};
    }

    // general
    Type accessor::type() noexcept
    {
        return Type::Null;
    }


    // write operators; throw json::wrong_type
    accessor& accessor::operator=(const std::string& str)
    {
        throw todo{"TODO: accessor::operator=(const std::string&)"};
    }

    accessor& accessor::operator=(const char* str)
    {
        throw todo{"TODO: accessor::operator=(const char*)"};
    }

    accessor& accessor::operator=(long double num)
    {
        throw todo{"TODO: accessor::operator=(long double)"};
    }

    accessor& accessor::operator=(double num)
    {
        throw todo{"TODO: accessor::operator=(double)"};
    }

    accessor& accessor::operator=(float num)
    {
        throw todo{"TODO: accessor::operator=(float)"};
    }

    accessor& accessor::operator=(unsigned long long int num)
    {
        throw todo{"TODO: accessor::operator=(unsigned long long int)"};
    }

    accessor& accessor::operator=(unsigned long int num)
    {
        throw todo{"TODO: accessor::operator=(unsigned long int)"};
    }

    accessor& accessor::operator=(unsigned int num)
    {
        throw todo{"TODO: accessor::operator=(unsigned int)"};
    }

    accessor& accessor::operator=(unsigned char num)
    {
        throw todo{"TODO: accessor::operator=(unsigned char)"};
    }

    accessor& accessor::operator=(long long int num)
    {
        throw todo{"TODO: accessor::operator=(long long int)"};
    }

    accessor& accessor::operator=(long int num)
    {
        throw todo{"TODO: accessor::operator=(long int)"};
    }

    accessor& accessor::operator=(int num)
    {
        throw todo{"TODO: accessor::operator=(int)"};
    }

    accessor& accessor::operator=(char num)
    {
        throw todo{"TODO: accessor::operator=(char)"};
    }

    accessor& accessor::operator=(bool boolean)
    {
        throw todo{"TODO: accessor::operator=(bool)"};
    }


    // read operators; throw json::wrong_type
    accessor::operator std::string()
    {
        throw todo{"TODO: accessor::operator std::string()"};
    }

    accessor::operator long double()
    {
        throw todo{"TODO: accessor::operator long double()"};
    }

    accessor::operator double()
    {
        throw todo{"TODO: accessor::operator double()"};
    }

    accessor::operator float()
    {
        throw todo{"TODO: accessor::operator float()"};
    }

    accessor::operator unsigned long long int()
    {
        throw todo{"TODO: accessor::operator unsigned long long int()"};
    }

    accessor::operator unsigned long int()
    {
        throw todo{"TODO: accessor::operator unsigned long int()"};
    }

    accessor::operator unsigned int()
    {
        throw todo{"TODO: accessor::operator unsigned int()"};
    }

    accessor::operator unsigned char()
    {
        throw todo{"TODO: accessor::operator unsigned char()"};
    }

    accessor::operator long long int()
    {
        throw todo{"TODO: accessor::operator long long int()"};
    }

    accessor::operator long int()
    {
        throw todo{"TODO: accessor::operator long int()"};
    }

    accessor::operator int()
    {
        throw todo{"TODO: accessor::operator int()"};
    }

    accessor::operator char()
    {
        throw todo{"TODO: accessor::operator char()"};
    }

    accessor::operator bool()
    {
        throw todo{"TODO: accessor::operator bool()"};
    }


    // subscript operators; throw json::out_of_range, json::wrong_type
    accessor accessor::operator[](const std::string& key)
    {
        throw todo{"TODO: accessor::operator[](const std::string&)"};
    }

    accessor accessor::operator[](const char* key)
    {
        throw todo{"TODO: accessor::operator[](const char*)"};
    }
    
    accessor accessor::operator[](std::size_t index)
    {
        throw todo{"TODO: accessor::operator[](std::size_t)"};
    }
}
