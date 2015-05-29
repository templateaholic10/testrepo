char *gets(char *str);

#include <iostream>
#include <boost/type_traits.hpp>

template <typename _type, typename _from, typename _to>
class replace_type {
public:
    static (boost::is_const<_type>::value ? (boost::is_array<_type>::value ? (boost::is_pointer<_type>::value ? (boost::is_reference<_type>::value ? _type : _type) : _type) : _type) : _type) type;
};
