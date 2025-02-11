#include "camarg.h"

int main(int arc, const char** argv)
{
    cam::arg::Parser parser("test", "test");
    try {
        parser.arg("--test", "help");
        return 1;
    } catch (cam::arg::InvalidName e) {
    }

    try {
        parser.arg("", "help");
        return 1;
    } catch (cam::arg::InvalidName e) {
    }

    parser.arg("test", "help");
    try {
        parser.arg("test", "help");
        return 1;
    } catch (cam::arg::AlreadyExsists e) {
    }

    int arg_c = 2;
    const char* arg_v[2] = { "test", "test" };

    parser.parse(arg_c, arg_v);

    try {
        parser.arg("test", "test");
        return 1;
    } catch (cam::arg::AlreadyParsed arg) {
    }

    return 0;
}
