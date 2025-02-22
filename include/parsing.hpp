#ifndef __PARSING_HPP__
#define __PARSING_HPP__

#include <string>
#include <string_view>

// --------------------------------------------------------------------------------
//  For now I'm just making it work trying to keep the API clean.
//  Anything else like extracting repeat code and adding iterators can come later.
// --------------------------------------------------------------------------------

struct mut_view
{
    mut_view(std::string& line, std::string::iterator begin, std::string::iterator end)
        : data(line), begin(begin), end(end) {  }

    std::size_t size();

    std::string& data;
    std::string::iterator begin, end;
};

// remove non-string whitespace
std::string remove_whitespace(const std::string& s) noexcept;

// find next k-v pair, assumes start is character before field in verified data string
std::string_view next_field(std::string_view line, std::string_view::iterator start) noexcept;

std::string_view get_key(std::string_view field);
mut_view         get_key(mut_view field);

std::string_view get_val(std::string_view field);
mut_view         get_val(mut_view field);

void delete_field(mut_view field);
void change_field(mut_view field);


#endif//__PARSING_HPP__:w

