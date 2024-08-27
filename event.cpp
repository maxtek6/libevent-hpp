#include "internal.hpp"

namespace evhpp
{

#if (EVENT__VERSION_MINOR > 1)
#define DISALLOW_TIMERFD EVENT_BASE_FLAG_EPOLL_DISALLOW_TIMERFD
#else
#define DISALLOW_TIMERFD 0
#endif

    BEGIN_MAP_FLAGS_FUNCTION(map_event_flags, event_flag, short){
        {event_flag::timeout, EV_TIMEOUT},
        {event_flag::read, EV_READ},
        {event_flag::write, EV_WRITE},
        {event_flag::signal, EV_SIGNAL},
        {event_flag::persist, EV_PERSIST},
        {event_flag::edge_trigger, EV_ET},
        {event_flag::finalize, EV_FINALIZE},
        {event_flag::closed, EV_CLOSED},
    };
    END_MAP_FLAGS_FUNCTION(map_event_flags)

    BEGIN_MAP_FLAGS_FUNCTION(map_count_flags, count_flag, unsigned int){
        {count_flag::active_events, EVENT_BASE_COUNT_ACTIVE},
        {count_flag::virtual_events, EVENT_BASE_COUNT_VIRTUAL},
        {count_flag::added_events, EVENT_BASE_COUNT_ADDED},
    };
    END_MAP_FLAGS_FUNCTION(map_count_flags)

    BEGIN_MAP_FLAGS_FUNCTION(map_feature_flags, feature_flag, int){
        {feature_flag::require_et, EV_FEATURE_ET},
        {feature_flag::require_o1, EV_FEATURE_O1},
        {feature_flag::file_descriptors, EV_FEATURE_FDS},
        {feature_flag::early_close, EV_FEATURE_EARLY_CLOSE},
    };
    END_MAP_FLAGS_FUNCTION(map_feature_flags)

    BEGIN_MAP_FLAGS_FUNCTION(map_config_flags, config_flag, int){
        {config_flag::no_lock, EVENT_BASE_FLAG_NOLOCK},
        {config_flag::ignore_env, EVENT_BASE_FLAG_IGNORE_ENV},
        {config_flag::startup_iocp, EVENT_BASE_FLAG_STARTUP_IOCP},
        {config_flag::no_cache_time, EVENT_BASE_FLAG_NO_CACHE_TIME},
        {config_flag::epoll_use_changelist, EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST},
        {config_flag::precise_timer, EVENT_BASE_FLAG_PRECISE_TIMER},
        {config_flag::epoll_disallow_timerfd, DISALLOW_TIMERFD},
    };
    END_MAP_FLAGS_FUNCTION(map_config_flags)

    BEGIN_MAP_FLAGS_FUNCTION(map_loop_flags, loop_flag, unsigned int){
        {loop_flag::once, EVLOOP_ONCE},
        {loop_flag::nonblock, EVLOOP_NONBLOCK},
        {loop_flag::no_exit_on_empty, EVLOOP_NO_EXIT_ON_EMPTY},
    };
    END_MAP_FLAGS_FUNCTION(map_loop_flags)

    event_mask::event_mask(short event) : _event(event) {}

    bool event_mask::has_flag(event_flag flag) const
    {
        const static short flags[] = {
            EV_TIMEOUT,
            EV_READ,
            EV_WRITE,
            EV_SIGNAL,
            EV_PERSIST,
            EV_ET,
            EV_FINALIZE,
            EV_CLOSED,
        };
        return (_event & flags[flag]) > 0;
    }

    event::event(base &base, socket_type fd, const std::initializer_list<event_flag> &event_flags, const event_callback &callback)
    {
        evhpp::accessor<internal::base> base_accessor(&base);
        internal::event *handle;
        handle = ::event_new(base_accessor.data(), fd, map_event_flags(event_flags), nullptr, nullptr);
        set_handle<internal::event>(
            handle, [](internal::event *data)
            { event_del(data); });
    }

    void event::add()
    {
        event_add(get_handle<internal::event>(), nullptr);
    }

    void event::add(const struct timeval *timeout)
    {
        event_add(get_handle<internal::event>(), timeout);
    }

    void event::del()
    {
        event_del(get_handle<internal::event>());
    }

    void event::del(bool block)
    {
        if (block)
        {
            event_del_block(get_handle<internal::event>());
        }
        else
        {
            event_del_noblock(get_handle<internal::event>());
        }
    }

    base::base()
    {
        construct(nullptr);
    }

    base::base(const config &config)
    {
        construct(&config);
    }

    void base::reinit()
    {
        event_reinit(get_handle<internal::base>());
    }

    void base::dispatch()
    {
        event_base_dispatch(get_handle<internal::base>());
    }

    void base::loop(const std::initializer_list<loop_flag> &loop_flags)
    {
        event_base_loop(get_handle<internal::base>(), map_loop_flags(loop_flags));
    }

    void base::loopbreak()
    {
        event_base_loopbreak(get_handle<internal::base>());
    }

    std::string base::get_method() const
    {
        return event_base_get_method(get_handle<internal::base>());
    }

    int base::get_num_events(const std::initializer_list<count_flag> &count_flags)
    {
        return event_base_get_num_events(get_handle<internal::base>(), map_count_flags(count_flags));
    }

    int base::get_max_events(const std::initializer_list<count_flag> &count_flags, bool clear)
    {
        return event_base_get_max_events(get_handle<internal::base>(), map_count_flags(count_flags), clear ? 1 : 0);
    }

    void base::loopexit(const struct timeval *timeout)
    {
        event_base_loopexit(get_handle<internal::base>(), timeout);
    }

    void base::construct(const config *config)
    {
        const std::function<void(internal::base *)> deleter = [](internal::base *data)
        {
            event_base_free(data);
        };
        internal::base *handle;
        if (config)
        {
            const const_accessor<internal::config> config_accessor(config);
            handle = event_base_new_with_config(config_accessor.data());
        }
        else
        {
            handle = event_base_new();
        }
        set_handle<internal::base>(handle, deleter);
    }

    config::config()
    {
        const std::function<void(internal::config *)> deleter = [](internal::config *data)
        {
            event_config_free(data);
        };
        set_handle<internal::config>(event_config_new(), deleter);
    }

    void config::require_features(const std::initializer_list<feature_flag> &feature_flags)
    {
        event_config_require_features(get_handle<internal::config>(), map_feature_flags(feature_flags));
    }

    void config::set_flag(const std::initializer_list<config_flag> &config_flags)
    {
        event_config_set_flag(get_handle<internal::config>(), map_config_flags(config_flags));
    }

    void config::set_num_cpus_hint(int cpus)
    {
        event_config_set_num_cpus_hint(get_handle<internal::config>(), cpus);
    }

    void config::avoid_method(const std::string &method)
    {
        event_config_avoid_method(get_handle<internal::config>(), method.c_str());
    }

    void config::set_max_dispatch_interval(const struct timeval *max_interval, int max_callbacks,
                                           int min_priority)
    {
        event_config_set_max_dispatch_interval(get_handle<internal::config>(), max_interval, max_callbacks, min_priority);
    }

}