#ifndef __JSON_HPP__
#define __JSON_HPP__

#include <string>
#include <vector>
#include <iterator>

// include other library headers
#include "error.hpp"
//#include "mutable_view.hpp"
#include "parsing.hpp"
#include "verification.hpp"
#include "accessor.hpp"


namespace json
{
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
    
    class json // TODO change std::string& to std::string_view where possible
    {
        public:
            json();
            json(const std::string& line);
            json(const char* line);


            // object specific
            std::vector<std::string> keys() const;
            bool contains(const std::string& key) const;

            // array specific
            index size() const;

            Type type() const;
            Type type(const std::string& key) const;
            Type type(std::size_t index) const;

            /*
            iterator begin();
            iterator end();
            */

            // TODO
            // make later change to templated version so:
            // Number can convert to any numeric, flooring if necessary
            // Bool, Number, Null can all convert to string if necessary

            // read operations return copy
            // throw json::wrong_type for accessor not matching data type
            // throw json::out_of_range for index > size
            json::const_object_accessor get_object(const std::string& key) const;
            json::object_accessor get_object(const std::string& key);
            
            json::const_object_accessor get_object(std::size_t index) const;
            json::object_accessor get_object(std::size_t index);


            json::const_array_accessor get_array(const std::string& key) const;
            json::array_accessor get_array(const std::string& key);

            json::const_array_accessor get_array(std::size_t index) const;
            json::array_accessor(std::size_t index);


            std::string get_string(const std::string& key) const;
            json::string_accessor get_string(const std::string& key);

            std::string get_string(std::size_t index) const;
            json::string_accessor get_string(std::size_t index);

            // when templating the get function replace with std::integral and std::floating_point to cover all types
            double get_number(const std::string& key) const;
            json::number_accessor get_number(const std::string& key);

            double get_number(std::size_t index) const;
            json::number_accessor get_number(std::size_t index);


            bool get_bool(const std::string& key) const;
            json::bool_accessor get_bool(const std::string& key);

            bool get_bool(std::size_t index) const;
            json::bool_accessor get_bool(std::size_t index);


            bool is_null(const std::string& key) const;
            json::null_accessor is_null(const std::string& key);

            bool is_null(std::size_t index) const;
            json::null_accessor is_null(std::size_t index);

            // if types can be changed, use a overwrite type function
            // something like
            // template <typename T> change_type(T data)


        private:
            std::string m_contents;
            Type m_type;


        public: // move this up later when needed
            /* IMPLEMENT AFTER GETTING BASIC ACCESS WORKING MANUALLY

            // This may be fully implemented as a STL like iterator later, but I don't know if the non-standard dereferences will make that possible.
            //      May have to add a generic wrapper type to implement dereferencing
            class Iterator
            {
                public:
                    // will parse the string to find the remaining locations
                    Iterator(char* start);
                    
                    std::string key() const;
                    Type type() const;
                    Type type(const std::string& key) const;
                    Type type(std::size_t index) const;

                    // Read and write accessors
                    // To be templated later
                    json get_object(const std::string& key) const;
                    json get_object(std::size_t index) const;

                    json get_array(const std::string& key) const;
                    json get_array(std::size_t index) const;

                    std::string get_string(const std::string& key) const;
                    std::string get_string(std::size_t index) const;
                    
                    double get_double(const std::string& key) const;
                    double get_double(std::size_t index) const;
                    
                    int get_int(const std::string& key) const;
                    int get_int(std::size_t index) const;
                    
                    bool get_bool(const std::string& key) const;
                    bool get_bool(std::size_t index) const;
                    
                    bool is_null(const std::string& key) const;
                    bool is_null(std::size_t index) const;
                    
                    // operators


                private:
                    char* key_start;
                    char* key_end;
                    char* val_start;
                    char* val_end;
            };
            */
    };
}

#endif//__JSON_HPP__
