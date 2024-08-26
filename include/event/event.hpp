#ifndef EVENT_EVENT_HPP
#define EVENT_EVENT_HPP

#include "types.hpp"

namespace evhpp
{
    enum event_flag
    {
        timeout,
        read,
        write,
        signal,
        persist,
        edge_trigger,
        finalize,
        closed,
    };

    enum count_flag
    {
        active_events,
        virtual_events,
        added_events,
    };

    enum feature_flag
    {
        require_et,
        require_o1,
        file_descriptors,
        early_close,
    };

    enum config_flag
    {
        no_lock,
        ignore_env,
        startup_iocp,
        no_cache_time,
        epoll_use_changelist,
        precise_timer,
        epoll_disallow_timerfd,
        use_signalfd,
    };

    enum loop_flag
    {
        once,
        nonblock,
        no_exit_on_empty,
    };

    class event : protected basic_type
    {
    public:
        event(base &base, socket_type fd, const std::initializer_list<event_flag> &event_flags, const event_callback &callback);

        void add();

        template <class Rep, class Period = std::ratio<1>>
        void add(const std::chrono::duration<Rep, Period> &timeout)
        {
            struct timeval timeout_tv;
            convert_to_timeval(timeout, &timeout_tv);
            add(&timeout_tv);
        }

        void del();

        void del(bool block);

    private:
        void add(const struct timeval *timeout);
    };

    class base : protected basic_type
    {
    public:
        base();

        base(const config &config);

        void reinit();

        void dispatch();

        void loop(const std::initializer_list<loop_flag> &loop_flags = {});

        void loop_break();

        template <class Rep, class Period = std::ratio<1>>
        void loop_exit(const std::chrono::duration<Rep, Period> &timeout)
        {
            struct timeval timeout_tv;
            convert_to_timeval(timeout, &timeout_tv);
            loop_exit(&timeout_tv);
        }

        std::string get_method() const;

        int get_num_events(const std::initializer_list<count_flag>& count_flags);

        int get_max_events(const std::initializer_list<count_flag>& count_flags, bool clear);
        
    private:
        void construct(const config *config);
        void loop_exit(const struct timeval *timeout);
    };

    class config : protected basic_type
    {
    public:
        config();

        void require_features(const std::initializer_list<feature_flag> &feature_flags);

        void set_flag(const std::initializer_list<config_flag> &config_flags);

        void set_num_cpus_hint(int cpus);

        template <class Rep, class Period = std::ratio<1>>
        void set_max_dispatch_interval(const std::chrono::duration<Rep, Period> &max_interval, int max_callbacks,
                                       int min_priority)
        {
            struct timeval max_interval_tv;
            convert_to_timeval(max_interval, &max_interval_tv);
            set_max_dispatch_interval(&max_interval_tv, max_callbacks, min_priority);
        }

        void avoid_method(const std::string& method);

    private:
        void set_max_dispatch_interval(const struct timeval *max_interval, int max_callbacks,
                                       int min_priority);
    };
}

#endif