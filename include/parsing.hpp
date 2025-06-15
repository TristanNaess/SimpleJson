#ifndef __PARSING_HPP__
#define __PARSING_HPP__

#include <string>
#include <string_view>

// --------------------------------------------------------------------------------
//  For now I'm just making it work trying to keep the API clean.
//  Anything else like extracting repeat code and adding iterators can come later.
// --------------------------------------------------------------------------------

// This may need to be changed/flushed out. I can't get it to work in my head
struct mut_view
{
    mut_view(std::string& line, std::string::iterator begin, std::string::iterator end);

    mut_view& operator=(const mut_view& other);

    std::size_t size() const;

    operator std::string() const;
    operator std::string_view() const;

    std::string& data;
    std::string::iterator begin, end;
};

bool operator==(const mut_view& lhs, const mut_view& rhs);
bool operator==(const mut_view& lhs, const char* rhs);

// remove non-string whitespace
std::string remove_whitespace(std::string_view s) noexcept;

// find next k-v pair, assumes start is character before field in verified data string
mut_view next_field(mut_view line, std::string::iterator start) noexcept;

mut_view get_key(mut_view field);

mut_view get_val(mut_view field);

void delete_field(mut_view field);
void change_field(mut_view field);

// need conversion to datatype functions



#endif//__PARSING_HPP__:w

