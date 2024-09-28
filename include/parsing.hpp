#include <string>
#include <utility>

// This will need to be expanded into a proper class with mutex locking when multithreading support is added
struct mut_view
{
    std::string& data;
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
std::string remove_whitespace(const std::string& s) noexcept;

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
int                 get_int(mut_view data);
signed char         get_char(mut_view data);
unsigned long long  get_ullint(mut_view data);
unsigned long       get_ulint(mut_view data);
unsigned int        get_uint(mut_view data);
unsigned char       get_uchar(mut_view data);
bool                get_bool(mut_view data);

// insert value into data string

mut_view insert_string(mut_view data, const std::string& val);
mut_view insert_ldouble(mut_view data, long double val);
mut_view insert_double(mut_view data, double val);
mut_view insert_float(mut_view data, float val);
mut_view insert_llint(mut_view data, long long val);
mut_view insert_lint(mut_view data, long val);
mut_view insert_int(mut_view data, int val);
mut_view insert_char(mut_view data, signed char val);
mut_view insert_ullint(mut_view data, unsigned long long val);
mut_view insert_ulint(mut_view data, unsigned long val);
mut_view insert_uint(mut_view data, unsigned int val);
mut_view insert_uchar(mut_view data, unsigned char val);
mut_view insert_bool(mut_view data, bool val);


// expose internal functions for tests
#ifdef TESTING

std::string::iterator skip_quotes(std::string::iterator start, std::string::iterator end) noexcept;
std::string::iterator skip_brackets(std::string::iterator start, std::string::iterator end) noexcept;
std::string::iterator find_applicable(std::string::iterator start, std::string::iterator end, char c) noexcept;

#endif //TESTING
