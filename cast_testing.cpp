#include <iostream>
#include <string>
#include <sstream>
#include <concepts>

template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

class container
{
    public:
        container(const std::string& data) : m_data(data) {  }
        container(const char* data) : m_data{data} {  }
    
    private:
        class accessor
        {
            private:
                accessor(container* parent) : parent(parent) {  }
                accessor(const accessor&) = delete;
                accessor(accessor&&) = delete;
                accessor& operator=(accessor&&) = delete;

                container* parent;

                friend class container;

            public:
                
                template <Numeric T>
                accessor& operator=(T numeric)
                {
                    std::stringstream ss;
                    ss << numeric;
                    parent->m_data = ss.str();

                    return *this;
                }

                template<std::integral T>
                operator T() const
                {
                    return std::stoll(parent->m_data);
                }

                template <std::floating_point T>
                operator T() const
                {
                    return std::stod(parent->m_data);
                }

        };

        class const_accessor
        {
            private:
                const_accessor(const container* parent) : parent(parent) {  }
                const_accessor(const accessor&) = delete;
                const_accessor(accessor&&) = delete;
                const_accessor& operator=(const accessor&) = delete;
                const_accessor& operator=(accessor&&) = delete;
                
                const container* parent;

                friend class container;

            public:
                template<std::integral T>
                operator T() const
                {
                    return std::stoll(parent->m_data);
                }

                template <std::floating_point T>
                operator T() const
                {
                    return std::stod(parent->m_data);
                }
                
        };

    public:
        const_accessor get() const
        {
            return const_accessor(this);
        }

        accessor get()
        {
            return accessor(this);
        }
        
        friend std::ostream& operator<<(std::ostream&, const container&);

    private:
        std::string m_data;
        friend class accessor;
        friend class const_accessor;
};

std::ostream& operator<<(std::ostream& os, const container& c)
{
    os << c.m_data;
    return os;
}

int main(int argc, char** argv)
{

    const container number{ "123.456" };

    double c = number.get();
    c += 20;
    std::cout << c << '\n';

    //number.get() = 4.2;
    std::size_t i = number.get();
    std::cout << i << '\n';
}
