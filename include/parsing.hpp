#include <string>
#include <utility>
// #include "mutable_view.hpp" // add once implemented. Use pair of iterators for now

// This will need to be expanded into a proper class with mutex locking when multithreading support is added
struct mut_view
{
    std::string data;
    std::string::iterator begin, end;
};

struct field
{
    std::string_view key;
    mut_view val;
};

// determine the json is valid. For now, can't diagnose error
bool verify_json();

// returns a copy of the string with all non-string whitespace removed
// necessary for all data parsing including verification
std::string remove_whitespace(std::string& s) noexcept;

// return field starting at start, search until end
// ASSUMES JSON HAS BEEN VERIFIED
// ASSUMES WHITE SPACE HAS BEEN REMOVED
field get_field(mut_view data) noexcept;

// extract value from data string

std::string         get_string(mut_view data);
long double         get_ldouble(mut_view data);
double              get_double(mut_view data);
float               get_float(mut_view data);
long long           get_llint(mut_view data);
long                get_lint(mut_view data);
int                 get_int (mut_view data);
unsigned long long  get_ullint (mut_view data);
unsigned long       get_ulint (mut_view data);
unsigned int        get_uint (mut_view data);

bool                get_bool (mut_view data);

// expose internal functions for tests
#ifdef TESTING

std::string::iterator skip_quotes(std::string::iterator start, std::string::iterator end) noexcept;
std::string::iterator skip_brackets(std::string::iterator start, std::string::iterator end) noexcept;
std::string::iterator find_applicable(std::string::iterator start, std::string::iterator end, char c) noexcept;

#endif //TESTING
