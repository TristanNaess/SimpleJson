#ifndef __ERROR_HPP__
#define __ERROR_HPP__

#include <stdexcept>
#include <string>

namespace json
{
    class parsing_error : public std::runtime_error
    {
        public:
            explicit parsing_error(const std::string& message = "") : std::runtime_error(message) {  }
            explicit parsing_error(const char* message = "") : std::runtime_error(message) {  }
    };

    class out_of_range : public std::runtime_error
    {
        public:
            explicit out_of_range(const std::string& message = "") : std::runtime_error(message) {  }
            explicit out_of_range(const char* message = "") : std::runtime_error(message) {  }
    };

    class wrong_type : public std::runtime_error
    {
        public:
            explicit wrong_type(const std::string& message = "") : std::runtime_error(message) {  }
            explicit wrong_type(const char* message = "") : std::runtime_error(message) {  }
    };

    class Result
    {
        public:
            Result() : m_status(true) { } // good state
            Result(const std::string& error) : m_status(false), m_message(error) {  } // error state
            Result(const std::string& prefix, const Result& other) : m_status(false)
            {
                m_message = prefix + other.m_message;
            }

            operator bool() const { return m_status; }
            const std::string& message() const { return m_message; }

        private:
            bool m_status;
            std::string m_message = "";
    };
}

#endif//__ERROR_HPP__
