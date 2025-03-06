#include "json.hpp"

namespace json
{

    // -----------------------------------
    //  json::json
    // -----------------------------------

    json::json(Type type)
    {
        switch (type)
        {
            case Type::Object:
                m_data = "{}";
                break;
            case Type::Array:
                m_data = "[]";
                break;
            default:
                throw wrong_type{"Top level json object must be array or object data"};
        }
        m_type = type;
    }

    json::json(std::string_view data)
    {
        m_data = remove_whitespace(data);
        //if (!verify_json(data)) throw parsing_error("Error verifying json data"); TODO: not yet added
        
        if (m_data[0] == '{') m_type = Type::Object;
        else m_type = Type::Array;
    }

    std::vector<std::string> json::keys() // json::wrong_type
    {
        //throw todo{"TODO: json::keys()"};
        if (m_type != Type::Object) throw wrong_type("Cannot get keys from array type json");
        std::vector<std::string> keys;
        mut_view data(m_data, m_data.begin(), m_data.end());
        mut_view field(m_data, m_data.begin(), m_data.begin());
        while ((field = next_field(data, field.end)) != mut_view(m_data, m_data.end(), m_data.end()))
        {
            keys.push_back(to_string(get_key(field)));
        }

        return keys;
    }

    bool json::contains(std::string_view key) // json::wrong_type
    {
        throw todo{"TODO: json::contains(std::string_view)"};
    }

    std::size_t json::size()
    {
        throw todo{"TODO: json::size()"};
    }

    Type json::type() noexcept
    {
        return m_type;
    }

    accessor json::operator[](std::string_view key) // json::wrong_type, json::out_of_range
    {
        throw todo{"TODO: json::operator[](std::string_view)"};
    }

    accessor json::operator[](const char* key) // json::wrong_type, json::out_of_range
    {
        return (*this)[std::string_view(key)];
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

    accessor::accessor(mut_view data) : m_data(data)
    {
        throw todo{"TODO: accessor(mut_view data)"};
    }

    std::vector<std::string> accessor::keys()       // json::wrong_type
    {
        throw todo{"TODO: accessor::keys()"};
    }

    bool accessor::contains(std::string_view key) // json::wrong_type
    {
        throw todo{"TODO: accessor::contains(std::string_view)"};
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
    accessor& accessor::operator=(std::string_view str)
    {
        throw todo{"TODO: accessor::operator=(std::string_view)"};
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
    accessor::operator std::string() const
    {
        throw todo{"TODO: accessor::operator std::string()"};
    }

    accessor::operator long double() const
    {
        throw todo{"TODO: accessor::operator long double()"};
    }

    accessor::operator double() const
    {
        throw todo{"TODO: accessor::operator double()"};
    }

    accessor::operator float() const
    {
        throw todo{"TODO: accessor::operator float()"};
    }

    accessor::operator unsigned long long int() const
    {
        throw todo{"TODO: accessor::operator unsigned long long int()"};
    }

    accessor::operator unsigned long int() const
    {
        throw todo{"TODO: accessor::operator unsigned long int()"};
    }

    accessor::operator unsigned int() const
    {
        throw todo{"TODO: accessor::operator unsigned int()"};
    }

    accessor::operator unsigned char() const
    {
        throw todo{"TODO: accessor::operator unsigned char()"};
    }

    accessor::operator long long int() const
    {
        throw todo{"TODO: accessor::operator long long int()"};
    }

    accessor::operator long int() const
    {
        throw todo{"TODO: accessor::operator long int()"};
    }

    accessor::operator int() const
    {
        throw todo{"TODO: accessor::operator int()"};
    }

    accessor::operator char() const
    {
        throw todo{"TODO: accessor::operator char()"};
    }

    accessor::operator bool() const
    {
        throw todo{"TODO: accessor::operator bool()"};
    }


    // subscript operators; throw json::out_of_range, json::wrong_type
    accessor accessor::operator[](std::string_view key)
    {
        throw todo{"TODO: accessor::operator[](std::string_view)"};
    }

    accessor accessor::operator[](const char* key)
    {
        return (*this)[std::string_view(key)];
    }

    accessor accessor::operator[](std::size_t index)
    {
        throw todo{"TODO: accessor::operator[](std::size_t)"};
    }

    // throw json::wrong_type
    bool operator==(const accessor& a, std::string_view str)
    {
        throw todo{"TODO: operator==(const accessor& a, std::string_view str)"};
    }

    bool operator==(const accessor& a, const long double num)
    {
        throw todo{"TODO: operator==(const accessor& a, const long double num)"};
    }

    bool operator==(const accessor& a, const double num)
    {
        throw todo{"TODO: operator==(const accessor& a, const double num)"};
    }

    bool operator==(const accessor& a, const float num)
    {
        throw todo{"TODO: operator==(const accessor& a, const float num)"};
    }

    bool operator==(const accessor& a, const unsigned long long int num)
    {
        throw todo{"TODO: operator==(const accessor& a, const unsigned long long int num)"};
    }

    bool operator==(const accessor& a, const unsigned long int num)
    {
        throw todo{"TODO: operator==(const accessor& a, const unsigned long int num)"};
    }

    bool operator==(const accessor& a, const unsigned int num)
    {
        throw todo{"TODO: operator==(const accessor& a, const unsigned int num)"};
    }

    bool operator==(const accessor& a, const long long int num)
    {
        throw todo{"TODO: operator==(const accessor& a, const long long int num)"};
    }

    bool operator==(const accessor& a, const long int num)
    {
        throw todo{"TODO: operator==(const accessor& a, const long int num)"};
    }

    bool operator==(const accessor& a, const int num)
    {
        throw todo{"TODO: operator==(const accessor& a, const int num)"};
    }

    bool operator==(const accessor& a, const char num)
    {
        throw todo{"TODO: operator==(const accessor& a, const char num)"};
    }

    bool operator==(const accessor& a, const bool boolean)
    {
        throw todo{"TODO: operator==(const accessor& a, const bool boolean)"};
    }

}
