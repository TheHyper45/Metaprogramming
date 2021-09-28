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

template<typename A,typename B>
struct is_same : std::false_type 
{};

template<typename A>
struct is_same<A,A> : std::true_type 
{};

struct wildcard
{
    constexpr wildcard(std::size_t) noexcept;

    template<typename type>
    constexpr operator type() const noexcept;
};

template<typename aggregate_type,std::size_t count,typename = std::make_index_sequence<count>>
struct aggregate_initializable_with_n_types;

template<typename aggregate_type,std::size_t count,std::size_t... indices>
struct aggregate_initializable_with_n_types<aggregate_type,count,std::index_sequence<indices...>>
{
    static inline constexpr std::size_t value = std::is_constructible_v<aggregate_type,decltype(wildcard(indices))...>;
};

long function(int,float);

struct aggregate_type
{
    int a;
    long b;
    double c;
};

int main()
{
    using traits = function_traits<decltype(function)>;

    std::cout << traits::argument_count << std::endl;

    traits::return_type value0{};
    traits::nth_argument_type<0> value1{};
    traits::nth_argument_type<1> value2{};

    std::cout << "is_same<int,long>::value == " << is_same<int,long>::value << std::endl;
    std::cout << "is_same<int,int>::value == " << is_same<int,int>::value << std::endl;
    std::cout << "is_same<int,std::string>::value == " << is_same<int,std::string>::value << std::endl;

    std::cout << "\nstruct aggregate_type\n{\n    int a;\n    long b;\n    double c;\n};\n" << std::endl;
    std::cout << "aggregate_initializable_with_n_types<aggregate_type,0> == " << aggregate_initializable_with_n_types<aggregate_type,0>::value << std::endl;
    std::cout << "aggregate_initializable_with_n_types<aggregate_type,1> == "<< aggregate_initializable_with_n_types<aggregate_type,1>::value << std::endl;
    std::cout << "aggregate_initializable_with_n_types<aggregate_type,2> == "<< aggregate_initializable_with_n_types<aggregate_type,2>::value << std::endl;
    std::cout << "aggregate_initializable_with_n_types<aggregate_type,3> == "<< aggregate_initializable_with_n_types<aggregate_type,3>::value << std::endl;
    std::cout << "aggregate_initializable_with_n_types<aggregate_type,4> == "<< aggregate_initializable_with_n_types<aggregate_type,4>::value << std::endl;

    std::cin.get();
    return 0;
}