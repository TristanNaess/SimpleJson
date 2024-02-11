#ifndef __JSON_HPP__
#define __JSON_HPP__

#include <string>
#include <string_view> // c++17
#include <vector>
#include <iterator>

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
        Null,
        Undefined
    };
    
    class Json // TODO Make sure all std::string_view args have matching const std::string&
    {
        public:
            Json();
            Json(const std::string& line);
            Json(std::string_view line);

            /* IMPLEMENT AFTER GETTING BASIC ACCESS WORKING MANUALLY

            // This may be fully implemented as a STL like iterator later, but I don't know if the non-standard dereferences will make that possible.
            //      May have to add a generic wrapper type to implement dereferencing
            // Actual implementations of functions will go here, while the versions in the Json class will use iterators to find the data
            class iterator
            {
                public:
                    // will parse the string to find the remaining locations
                    iterator(char* start);
                    
                    std::string key() const;
                    Type type() const;
                    Type type(std::string_view key) const;
                    Type type(index index) const;

                    // Read and write accessors
                    // To be templated later
                    Json get_object(std::string_view key) const;
                    Json get_object(index index) const;

                    Json get_array(std::string_view key) const;
                    Json get_array(index index) const;

                    std::string get_string(std::string_view key) const;
                    std::string get_string(index index) const;
                    
                    double get_double(std::string_view key) const;
                    double get_double(index index) const;
                    
                    int get_int(std::string_view key) const;
                    int get_int(index index) const;
                    
                    bool get_bool(std::string_view key) const;
                    bool get_bool(index index) const;
                    
                    bool is_null(std::string_view key) const;
                    bool is_null(index index) const;
                    
                    // operators


                private:
                    char* key_start;
                    char* key_end;
                    char* val_start;
                    char* val_end;
            };
            */

            // object specific
            std::vector<std::string> keys() const;
            bool contains(std::string_view key) const;
            bool contains(const std::string& key) const;

            // array specific
            index size() const;

            // returns type, either json::Object or json::Array
            Type type() const;
            // deduces and returns type of data at key/index
            Type type(std::string_view key) const;
            Type type(index index) const;

            iterator begin();
            iterator end();

            // TODO
            // make later change to templated version so:
            // Number can convert to any numeric, flooring if necessary
            // Bool, Number, Null can all convert to string if necessary

            // read operations return copy
            // throw json::wrong_type for accessor not matching data type
            // throw json::out_of_range for index > size
            Json get_object(std::string_view key) const;
            Json get_object(index index) const;

            Json get_array(std::string_view key) const;
            Json get_array(index index) const;

            std::string get_string(std::string_view key) const;
            std::string get_string(index index) const;

            double get_double(std::string_view key) const;
            double get_double(index index) const;

            int get_int(std::string_view key) const;
            int get_int(index index) const;

            bool get_bool(std::string_view key) const;
            bool get_bool(index index) const;

            bool is_null(std::string_view key) const;
            bool is_null(index index) const;


        private:
            std::string m_contents;
            Type m_type;
    };
}

#endif//__JSON_HPP__
