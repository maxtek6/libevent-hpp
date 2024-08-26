#ifndef INTERNAL_HPP
#define INTERNAL_HPP

#include "event.hpp"

#include <event.h>

namespace evhpp
{
    // internal typedefs
    namespace internal
    {
        using event = ::event;
        using base = ::event_base;
        using config = ::event_config;
    }

    template <class Type>
    class accessor
    {
    public:
        template <class PrivateType>
        accessor(PrivateType *private_data)
        {
            static_assert(std::is_base_of_v<basic_type, PrivateType>);
            _data = reinterpret_cast<basic_type *>(private_data);
        }

        Type *data() const
        {
            return _data->get_handle<Type>();
        }

    private:
        basic_type *_data;
    };

    template <class Type>
    class const_accessor
    {
    public:
        template <class PrivateType>
        const_accessor(const PrivateType *private_data)
        {
            static_assert(std::is_base_of_v<basic_type, PrivateType>);
            _data = reinterpret_cast<const basic_type *>(private_data);
        }

        const Type *data() const
        {
            return _data->get_handle<Type>();
        }

    private:
        const basic_type *_data;
    };

#define BEGIN_MAP_FLAGS_FUNCTION(FUNCTION_NAME, FLAG_TYPE, RETURN_TYPE)             \
    static RETURN_TYPE FUNCTION_NAME(const std::initializer_list<FLAG_TYPE> &flags) \
    {                                                                               \
        RETURN_TYPE result(0);                                                      \
        const static std::unordered_map<FLAG_TYPE, RETURN_TYPE> mapped_flags =

#define END_MAP_FLAGS_FUNCTION(FUNCTION_NAME)                          \
    for (const auto &flag : flags)                                     \
    {                                                                  \
        const auto flag_iterator = mapped_flags.find(flag);            \
        if (flag_iterator == mapped_flags.end())                       \
        {                                                              \
            throw std::runtime_error(#FUNCTION_NAME ": invalid flag"); \
        }                                                              \
        result |= flag_iterator->second;                               \
    }                                                                  \
    return result;                                                     \
    }

}

#endif