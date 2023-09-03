#ifndef __JSON_HPP__
#define __JSON_HPP__

#include <string>
#include <string_view> // c++17
#include <vector>

namespace json
{
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
            
            // for now use a separate getter for each type
            // all throw if wrong type, bad key or bad index
            // throw json::out_of_range for bad key or index
            // throw json::wrong_type if the type requested was incorrect

            // later change to templated version so:
            //    Number can convert to any numeric, flooring if necessary
            //    Bool, Number, Null can all convert to string if necessary

            // all come in matching object and array access versions
            // ModifyingAccessorType getObject(std::string_view key);
            // ModifyingAccessorType getObject(std::size_t index);

            // ModifyingAccessorType getArray(std::string_view key);
            // ModifyingAccessorType getArray(std::size_t index);

            // std::string_view getString(std::string_view key) const;
            // std::string_view getString(std::size_t index) const;
            // ModifyingAccessorType getString(std::string_view key);
            // ModifyingAccessorType getString(std::size_t index);

            // double getDouble(std::string_view key) const;
            // double getDouble(std::size_t index) const;
            // ModifyingAccessorType getDouble(std::string_view key);
            // ModifyingAccessorType getDouble(std::size_t index);

            // bool getBool(std::string_view key) const;
            // bool getBool(std::size_t index) const;
            // ModifyingAccessorType getBool(std::string_view key) const;
            // ModifyingAccessorType getBool(std::size_t index) const;

            // Type get_type(std::string_view key) const;
            // Type get_type(std::size_t index) const;

            bool is_null(std::string_view key) const;
            bool is_null(std::size_t index) const;

            bool contains_key(std::string_view key) const;
            std::vector<std::string> get_keys() const;
            std::size_t size() const;

        private:
            std::string m_contents;
            Type m_type;
    };
}

#endif//__JSON_HPP__
