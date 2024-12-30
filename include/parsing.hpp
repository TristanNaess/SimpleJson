#include <string>
#include <utility>

// These will need to be modified when adding multithreading
struct field
{
    std::string& data;
    // really a toss-up whether iterators or indices are better because the string library is inconsistent
    std::string::iterator key_start, key_end;
    std::string::iterator field_start, field_end;
};

struct mut_view
{
    std::string& data;
    std::string::iterator start, end;
};

// determine the json is valid. For now, can't diagnose error
bool verify_json(const std::string& str);

// TODO: verifies and fixes errors if possible
// bool validate_json(std::string& str);

// returns a copy of the string with all non-string whitespace removed
// necessary for all data parsing including verification
std::string remove_whitespace(const std::string& s) noexcept;

// MORE TO BE ADDED AS NEEDED, REFACTOR LATER

#ifdef DEBUG
std::string::const_iterator skip_quotes(std::string::const_iterator start, std::string::const_iterator end) noexcept;
std::string::const_iterator skip_brackets(std::string::const_iterator start, std::string::const_iterator end) noexcept;
#endif //DEBUG
