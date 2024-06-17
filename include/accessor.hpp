#ifndef __ACCESSOR_HPP__
#define __ACCESSOR_HPP__

#include <string>
#include <string_view>
#include <concept>

class json::json;

namespace json
{
    class string_accessor
    {
        public:
            string_accessor& operator=(const std::string& line);
            string_accessor& operator=(const char* line);
            string_accessor& operator=(std::string_view line);

            operator std::string() const;
            // no char* or string_view since a new copy is made, and I don't want to allocate new data

        private:

            string_accessor(json::json* parent) : parent(parent) {  }

            string_accessor(const string_accessor&) = delete;
            string_accessor& operator=(const string_accessor&) = delete;
            
            string_accessor(string_accessor&&) = delete;
            string_accessor& operator=(string_accessor&&) = delete;

            json::json& parent; // r
            friend class json::json;
    };

    class number_accessor
    {
        public:
            template <Numeric T>
            number_accessor& operator=(T numeric);

        private:

            number_accessor(json::json* parent) : parent(parent) {  }

            number_accessor(const number_accessor&) = delete;
            number_accessor& operator=(const number_accessor&) = delete;
            
            number_accessor(number_accessor&&) = delete;
            number_accessor& operator=(number_accessor&&) = delete;

            json::json& parent;
            friend class json::json;
    };

    class bool_accessor
    {
        public:
            bool_accessor& operator=(bool b);

        private:

            bool_accessor(json::json* parent) : parent(parent) {  }

            bool_accessor(const string_accessor&) = delete;
            bool_accessor& operator=(const string_accessor&) = delete;
            
            bool_accessor(string_accessor&&) = delete;
            bool_accessor& operator=(string_accessor&&) = delete;

            json::json& parent;
            friend class json::json;
    };

    class object_accessor
    {
        public:
            // add templated gets() like in json::json

            std::vector<std::string> keys() const;
            bool contains(const std::string& key) const;

            json::Type type() const;
            json::Type type(const std::string& key) const;

            // iterator begin();
            // iterator end();

        private:

            object_accessor(json::json* parent) : parent(parent) {  }

            object_accessor(const string_accessor&) = delete;
            object_accessor& operator=(const string_accessor&) = delete;
            
            object_accessor(string_accessor&&) = delete;
            object_accessor& operator=(string_accessor&&) = delete;

            json::json& parent;
            friend class json::json;
    };

    class array_accessor
    {
        public:
            // add templated gets() like in json::json

            std::vector<std::string> keys() const;
            bool contains(const std::string& key) const;

            json::Type type() const;
            json::Type type(const std::string& key) const;

            // iterator begin();
            // iterator end();

        private:

            array_accessor(json::json* parent) : parent(parent) {  }

            array_accessor(const string_accessor&) = delete;
            array_accessor& operator=(const string_accessor&) = delete;
            
            array_accessor(string_accessor&&) = delete;
            array_accessor& operator=(string_accessor&&) = delete;

            json::json& parent;
            friend class json::json;
    };
}

#endif//__ACCESSOR_HPP__
