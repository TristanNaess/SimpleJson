#ifndef __JSON_HPP__
#define __JSON_HPP__

#include <string>
#include "error.hpp"


namepsace json
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

    class json; // forward declaration to resolve circular references

    class accessor
    {
        public:
            // constructors
            //accessor(json& parent, std::string::iterator front, std::string::iterator back);
            accessor(std::string& data, std::string::iterator front, std::string::iterator back);
            accessor(accessor&) = delete;
            accessor(accessor&&) = delete;

            // object data
            std::vector<std::string> keys();        // json::wrong_type
            bool contains(const std::string& key);  // json::wrong_type
            bool contains(const char* key);         // json::wrong_type

            // array data
            std::size_t size(); // json::wrong_type

            // general
            Type type() noexcept;

            // TODO: Need to add a set to null function

            // write operators; throw json::wrong_type
            accessor& operator=(const std::string& str);
            accessor& operator=(const char* str);
            accessor& operator=(long double num);
            accessor& operator=(double num);
            accessor& operator=(float num);
            accessor& operator=(unsigned long long int num);
            accessor& operator=(unsigned long int num);
            accessor& operator=(unsigned int num);
            accessor& operator=(unsigned char num);
            accessor& operator=(long long int num);
            accessor& operator=(long int num);
            accessor& operator=(int num);
            accessor& operator=(char num);
            accessor& operator=(bool boolean);

            // read operators; throw json::wrong_type
            operator std::string();
            operator long double();
            operator double();
            operator float();
            operator unsigned long long int();
            operator unsigned long int();
            operator unsigned int();
            operator unsigned char();
            operator long long int();
            operator long int();
            operator int();
            operator char();
            operator bool();

            // subscript operators; throw json::out_of_range, json::wrong_type
            accessor operator[](const std::string& key);
            accessor operator[](const char* key);
            accessor operator[](std::size_t index);

            // TODO: Need to add insert and delete operators (keep explicit; std::map::operator[] inserting is stupid)

        private:
            //json& m_parent_json;
            std::string& m_data; // see if this is enough, or if I need the json&
            std::string::iterator m_front;
            std::string::iterator m_back;
            Type m_type;
    };


    class json
    {
        public:
            // constructors
            json();
            json(const std::string& data);
            json(const char* data);

            // object data
            std::vector<std::string> keys();        // json::wrong_type
            bool contains(const std::string& key);  // json::wrong_type
            bool contains(const char* key);         // json::wrong_type

            // array data
            std::size_t size(); // json::wrong_type, could arguably return the number of fields in the object instead of throwing

            // general
            Type type() noexcept;

            // access
            accessor operator[](const std::string& key);    // json::wrong_type, json::out_of_range
            accessor operator[](const char* key);           // json::wrong_type, json::out_or_range
            accessor operator[](std::size_t index);         // json::wrong_type, json::out_of_range

            // once iterators are added
            // iterator begin();
            // iterator end();

        private:
            std::string m_data;
            Type m_type;

            friend class accessor;
    };
}

#endif//__JSON_HPP__
