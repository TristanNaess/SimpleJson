#ifndef __ERROR_HPP__
#define __ERROR_HPP__

#include <stdexcept>

namespace Json
{
    class parsing_error : public std::runtime_error
    {
        public:
            explicit parsing_error(const std::string& message = "") : std::runtime_error(message) {  }
            explicit parsing_error(const char* message = "") : std::runtime_error(message) {  }
    };
}

#endif//__ERROR_HPP__
