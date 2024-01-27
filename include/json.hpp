#ifndef __JSON_HPP__
#define __JSON_HPP__

#include <string>
#include <string_view> // c++17
#include <vector>

namespace json
{
    using index = std::size_t;
    enum class Type
    {
        Object,
        Array,
        String,
        Number,
        Boolean,
        Null
    };

    class Json
    {
        public:
            Json();
            Json(const std::string& line);
            
            // object specific
            std::vector<std::string> keys() const;
            bool contains(std::string_view key) const;

            // array specific
            json::index size() const;

            // returns type, either json::Object or json::Array
            Type type() const;
            // deduces and returns type of data at key/index
            Type type(std::string_view key) const;
            Type type(json::index index) const;

            // TODO
            // make later change to templated version so:
            // Number can convert to any numeric, flooring if necessary
            // Bool, Number, Null can all convert to string if necessary

            // read operations return copy
            // throw json::wrong_type for accessor not matching data type
            // throw json::out_of_range for index > size
            Json get_object(std::string_view key) const;
            Json get_object(json::index index) const;

            Json get_array(std::string_view key) const;
            Json get_array(json::index index) const;

            std::string get_string(std::string_view key) const;
            std::string get_string(json::index index) const;

            double get_double(std::string_view key) const;
            double get_double(json::index index) const;

            int get_int(std::string_view key) const;
            int get_int(json::index index) const;

            bool get_bool(std::string_view key) const;
            bool get_bool(json::index index) const;

            bool is_null(std::string_view key) const;
            bool is_null(std::size_t index) const;

        private:
            std::string m_contents;
            Type m_type;
    };
}

#endif//__JSON_HPP__
