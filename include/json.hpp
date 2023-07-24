#ifndef __JSON_HPP__
#define __JSON_HPP__

#include <string>

namespace Json
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
            


        private:
            std::string m_contents;
            Type m_type;
    };
}

#endif//__JSON_HPP__
