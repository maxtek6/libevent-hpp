#ifndef TEST_HPP
#define TEST_HPP

#include <event.hpp>

#include <iostream>
#include <sstream>

namespace test
{
    class module
    {
    public:
        void run_test(const std::string &name);

    protected:
        void add_test(const std::string &name, const std::function<void()> &test_case);

    private:
        std::unordered_map<std::string, std::function<void()>> _test_cases;
    };

    class suite
    {
    public:
        std::shared_ptr<module> get_module(const std::string &name);

        template <class Module>
        void add_module(const std::string &name)
        {
            static_assert(std::is_base_of_v<module, Module>);
            _modules[name] = std::make_shared<Module>();
        }

    private:
        std::unordered_map<std::string, std::shared_ptr<module>> _modules;
    };

    void assert(
        bool condition,
        const std::string &file,
        const std::string &function,
        int line,
        const std::string &description);

    int run(int argc, const char **argv);
}

#ifndef __FUNCTION_NAME__
#ifdef WIN32 // WINDOWS
#define __FUNCTION_NAME__ __FUNCTION__
#else //*NIX
#define __FUNCTION_NAME__ __func__
#endif
#endif

#define TEST_ASSERT(COND) test::assert((COND), __FILE__, __FUNCTION_NAME__, __LINE__, #COND)

#endif