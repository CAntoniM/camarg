#include "camarg.h"

int main(int argc, const char** argv)
{
    cam::arg::Parser parser("test", "test command");

    try {
        parser.flag("--test", "test", false);
        return 1;
    } catch (cam::arg::InvalidName& e) {
    }

    try {
        parser.flag("", "test", false);
        return 1;
    } catch (cam::arg::InvalidName& e) {
    }

    parser.flag("test", "test", false);
    try {
        parser.flag("test", "test", false);
        return 1;
    } catch (cam::arg::AlreadyExsists& e) {
    }

    parser.parse(argc, argv);

    try {
        parser.flag("foo", "bar", false);
        return 1;
    } catch (cam::arg::AlreadyParsed e) {
    }

    return 0;
}
