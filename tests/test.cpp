#include "test.hpp"

void test::assert(
    bool condition,
    const std::string &file,
    const std::string &function,
    int line,
    const std::string &description)
{
    std::stringstream error_stream;
    if (!condition)
    {
        error_stream << file << ":" << function << ":" << line << ":"
                     << " failed to assert \"" << description << "\"";
        throw std::runtime_error(error_stream.str());
    }
}

int test::run(int argc, const char **argv)
{
    const std::string module_name(argv[1]);
    const std::string test_name(argv[2]);
    test::suite suite;
    int result(EXIT_SUCCESS);
    
    try
    {
        auto module = suite.get_module(module_name);
        module->run_test(test_name);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        result = EXIT_FAILURE;
    }

    return result;
}