#ifndef EVENT_TYPES_HPP
#define EVENT_TYPES_HPP

#include <chrono>
#include <functional>
#include <initializer_list>
#include <memory>
#include <list>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>

namespace evhpp
{
#ifdef _WIN32
    using socket_type = intptr_t;
#else
    using socket_type = int;
#endif

    class event_mask;
    class event;
    class base;
    class config;

    using event_callback = std::function<void(socket_type, event_mask)>;

    class basic_type
    {
    public:
        basic_type() = default;
        ~basic_type() = default;

        template <class Type>
        Type *get_handle() const
        {
            return reinterpret_cast<Type *>(_handle.get());
        }

        template <class Type>
        void set_handle(Type *handle, const std::function<void(Type *)> &deleter)
        {
            const std::function<void(void *)> opaque_deleter = [&](void *data)
            {
                deleter(reinterpret_cast<Type *>(data));
            };
            _handle = std::unique_ptr<void, std::function<void(void *)>>(handle, opaque_deleter);
        }

    public:
        std::unique_ptr<void, std::function<void(void *)>> _handle;
    };

    template <class Rep, class Period = std::ratio<1>>
    void convert_to_timeval(const std::chrono::duration<Rep, Period> &time, timeval *tv)
    {
        const auto micros = std::chrono::duration_cast<std::chrono::microseconds>(time).count();
        tv->tv_sec = micros / 1000000;
        tv->tv_usec = micros % 1000000;
    }

}

#endif