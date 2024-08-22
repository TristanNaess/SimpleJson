#ifndef __ACCESSOR_HPP__
#define __ACCESSOR_HPP__

// ---------------------------------------------------------------------------------
//  This file may need to be appended to json.hpp for easier access between classes
//  or the accessors should be made a member of the json object
//  that is returned by reference from the get<T>() functions
//  this may be a problem when accessing sub-members through,
//  unless the object and array accessors have internal accessors as well,
//  but that could lead to indefinite recursion... problems for later me
// ---------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------
//  All the accessors may have to be templated to get the correct version
//  from get<type>()
//  run tests to see
// ---------------------------------------------------------------------------------

#include <string>
#include <vector>
#include <string_view>
#include <concepts>

namespace json
{
    class json;

    enum class Type;

    template <typename T>
    class const_accessor
    {
        public:
            operator T() const;

        private:
            const_accessor(json& parent) : parent(parent) {  }

            const_accessor(const const_accessor&) = delete;
            const_accessor& operator=(const const_accessor&) = delete;

            const_accessor(const_accessor&&) = delete;
            const_accessor& operator=(const_accessor&&) = delete;

            json& parent;
            friend class json;
    };

    template <typename T, typename Return_T> // may have to add Return_T to get correct return
    class accessor
    {
        public:
            accessor& operator=(const T& data); // this can't be T, may have to add second template call to dispatch to correct

        private:
            

    };

    class string_accessor
    {
        public:
            string_accessor& operator=(const std::string& line);
            string_accessor& operator=(const char* line);
            string_accessor& operator=(std::string_view line);

            operator std::string() const;
            // no char* or string_view since a new copy is made, and I don't want to allocate new data

        private:

            string_accessor(json& parent) : parent(parent) {  }

            string_accessor(const string_accessor&) = delete;
            string_accessor& operator=(const string_accessor&) = delete;
            
            string_accessor(string_accessor&&) = delete;
            string_accessor& operator=(string_accessor&&) = delete;

            json& parent;
            friend class json;
    };

    class number_accessor
    {
        public:
            template <std::integral T>
            number_accessor& operator=(T numeric);

            template <std::floating_point T>
            number_accessor& operator=(T numeric);

        private:

            number_accessor(json& parent) : parent(parent) {  }

            number_accessor(const number_accessor&) = delete;
            number_accessor& operator=(const number_accessor&) = delete;
            
            number_accessor(number_accessor&&) = delete;
            number_accessor& operator=(number_accessor&&) = delete;

            json& parent;
            friend class json;
    };

    class bool_accessor
    {
        public:
            bool_accessor& operator=(bool b);

        private:

            bool_accessor(json& parent) : parent(parent) {  }

            bool_accessor(const bool_accessor&) = delete;
            bool_accessor& operator=(const bool_accessor&) = delete;
            
            bool_accessor(bool_accessor&&) = delete;
            bool_accessor& operator=(bool_accessor&&) = delete;

            json& parent;
            friend class json;
    };

    class object_accessor
    {
        public:
            template <typename T>


            std::vector<std::string> keys() const;
            bool contains(const std::string& key) const;

             Type type() const;
             Type type(const std::string& key) const;

            // iterator begin();
            // iterator end();

        private:

            object_accessor(json& parent) : parent(parent) {  }

            object_accessor(const object_accessor&) = delete;
            object_accessor& operator=(const object_accessor&) = delete;
            
            object_accessor(object_accessor&&) = delete;
            object_accessor& operator=(object_accessor&&) = delete;

            json& parent;
            friend class json;
    };

    class const_object_accessor
    {
        public:
            // add templated get function

            std::vector<std::string> keys() const;
            bool contains(const std::string& key) const;

             Type type() const;
             Type type(const std::string& key) const;

            // iterator begin();
            // iterator end();

        private:

            const_object_accessor(json& parent) : parent(parent) {  }
            
            const_object_accessor(const const_object_accessor&) = delete;
            const_object_accessor& operator=(const const_object_accessor&) = delete;

            const_object_accessor(const_object_accessor&&) = delete;
            const_object_accessor& operator=(const_object_accessor&&) = delete;

            const json& parent;
            friend class json;
    };

    class array_accessor
    {
        public:
            // add templated operator= for write access

            std::vector<std::string> keys() const;
            bool contains(const std::string& key) const;

             Type type() const;
             Type type(const std::string& key) const;

            // iterator begin();
            // iterator end();

        private:

            array_accessor(json& parent) : parent(parent) {  }

            array_accessor(const string_accessor&) = delete;
            array_accessor& operator=(const string_accessor&) = delete;
            
            array_accessor(string_accessor&&) = delete;
            array_accessor& operator=(string_accessor&&) = delete;

            json& parent;
            friend class json;
    };

    class const_array_accessor
    {
        public:
            // add const templated cast operator for read access

            std::size_t size() const;

             Type type() const;
             Type type(std::size_t index) const;

            // iterator begin();
            // iterator end();

        private:

            const_array_accessor(json& parent) : parent(parent) {  }
            
            const_array_accessor(const const_array_accessor&) = delete;
            const_array_accessor& operator=(const const_array_accessor&) = delete;

            const_array_accessor(const_array_accessor&&) = delete;
            const_array_accessor& operator=(const_array_accessor&&) = delete;

            const json& parent;
            friend class json;
    };
}

#endif//__ACCESSOR_HPP__
