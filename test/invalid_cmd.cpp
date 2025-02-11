#include "camarg.h"
#include <memory>

int main(int argc, const char* argv[])
{

    using namespace cam::arg;

    int arg_c = 2;
    const char* arg_v[2] = { "test", "test" };

    Parser parser("test", "Test");
    try {
        std::unique_ptr<Parser> sub_command(new Parser("--test", "help"));
        parser.cmd(std::move(sub_command));
        return 1;
    } catch (InvalidName e) {
    }

    try {
        std::unique_ptr<Parser> sub_command(new Parser("", "help"));
        parser.cmd(std::move(sub_command));
        return 1;
    } catch (InvalidName e) {
    }

    try {
        std::unique_ptr<Parser> sub_command(new Parser("test", "help"));
        parser.cmd(std::move(sub_command));
        std::unique_ptr<Parser> sub_command2(new Parser("test", "help"));
        parser.cmd(std::move(sub_command2));
        return 1;
    } catch (AlreadyExsists e) {
    }

    try {
        parser.parse(arg_c, arg_v);
        return 1;
    } catch (InvalidParserConfig config) {
    }
    try {
        std::unique_ptr<Parser> sub_command(new Parser("foo", "help"));
        parser.cmd(std::move(sub_command));
        return 1;
    } catch (AlreadyParsed e) {
    }

    return 0;
}
