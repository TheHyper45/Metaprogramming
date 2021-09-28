#include <tuple>
#include <iostream>

template<typename>
struct function_traits;

template<typename type,typename... types>
struct function_traits<type(types...)>
{
    using return_type = type;

    static inline constexpr std::size_t argument_count = sizeof...(types);
    
    template<std::size_t index>
    using nth_argument_type = std::tuple_element_t<index,std::tuple<types...>>;
};

long function(int,float);

int main()
{
    using traits = function_traits<decltype(function)>;

    std::cout << traits::argument_count << std::endl;

    traits::return_type value0;
    traits::nth_argument_type<0> value1;
    traits::nth_argument_type<1> value2;

    std::cin.get();
    return 0;
}