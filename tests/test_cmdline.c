#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <kernaux/cmdline.h>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static const unsigned int ARGV_COUNT_MAX = 100;
static const unsigned int ARG_SIZE_MAX = 4096;

static void test(
    const char *cmdline,
    unsigned int argv_count_max,
    unsigned int arg_size_max,

    bool expected_result,
    const char *expected_error_msg,
    unsigned int expected_argc,
    const char *const *const expected_argv
);

static const char *const argv0[] = {};

static const char *const argv_foo[] = {"foo"};

static const char *const argv_foo_bar[] = {"foo", "bar"};

static const char *const argv_foo_bar_car[] = {"foo", "bar", "car"};

static const char *const argv_space[] = {" "};

static const char *const argv_backslash[] = {"\\"};

static const char *const argv_foospace[] = {"foo "};

static const char *const argv_foobackslash[] = {"foo\\"};

static const char *const argv_spacefoo[] = {" foo"};

static const char *const argv_backslashfoo[] = {"\\foo"};

static const char *const argv_spacefoospace[] = {" foo "};

static const char *const argv_backslashfoobackslash[] = {"\\foo\\"};

static const char *const argv_foospacebar[] = {"foo bar"};

static const char *const argv_foobackslashbar[] = {"foo\\bar"};

static const char *const argv_spaceX3_X3[] = {"   ", "   ", "   "};

static const char *const argv_backslashX3_X3[] = {"\\\\\\", "\\\\\\", "\\\\\\"};

static const char *const argv_spacefoo_bar[] = {" foo", "bar"};

static const char *const argv_backslashfoo_bar[] = {"\\foo", "bar"};

static const char *const argv_foospace_bar[] = {"foo ", "bar"};

static const char *const argv_foobackslash_bar[] = {"foo\\", "bar"};

static const char *const argv_spacefoospace_bar[] = {" foo ", "bar"};

static const char *const argv_backslashfoobackslash_bar[] = {"\\foo\\", "bar"};

static const char *const argv_foo_spacebar[] = {"foo", " bar"};

static const char *const argv_foo_backslashbar[] = {"foo", "\\bar"};

static const char *const argv_foo_barspace[] = {"foo", "bar "};

static const char *const argv_foo_barbackslash[] = {"foo", "bar\\"};

static const char *const argv_foo_spacebarspace[] = {"foo", " bar "};

static const char *const argv_foo_backslashbarbackslash[] = {"foo", "\\bar\\"};

int main()
{
    test("",             0, 0, true, "", 0, argv0);
    test("   ",          0, 0, true, "", 0, argv0);
    test("foo",          0, 0, true, "", 1, argv_foo);
    test("foo bar",      0, 0, true, "", 2, argv_foo_bar);
    test(" foo bar",     0, 0, true, "", 2, argv_foo_bar);
    test("foo bar ",     0, 0, true, "", 2, argv_foo_bar);
    test(" foo bar ",    0, 0, true, "", 2, argv_foo_bar);
    test("foo  bar",     0, 0, true, "", 2, argv_foo_bar);
    test("  foo  bar",   0, 0, true, "", 2, argv_foo_bar);
    test("foo  bar  ",   0, 0, true, "", 2, argv_foo_bar);
    test("  foo  bar  ", 0, 0, true, "", 2, argv_foo_bar);
    test("foo bar car",  0, 0, true, "", 3, argv_foo_bar_car);

    test("foo bar car", 3, 0, true, "", 3, argv_foo_bar_car);
    test("foo bar car", 0, 4, true, "", 3, argv_foo_bar_car);
    test("foo bar car", 3, 4, true, "", 3, argv_foo_bar_car);

    test("foo bar car", 2, 0, false, "too many args", 0, argv0);
    test("foo bar car", 0, 3, false, "arg too long",  0, argv0);
    test("foo bar car", 2, 3, false, "arg too long",  0, argv0);

    test("\\ ",             0, 0, true, "", 1, argv_space);
    test("\\\\",            0, 0, true, "", 1, argv_backslash);
    test("foo\\ ",          0, 0, true, "", 1, argv_foospace);
    test("foo\\\\",         0, 0, true, "", 1, argv_foobackslash);
    test("\\ foo",          0, 0, true, "", 1, argv_spacefoo);
    test("\\\\foo",         0, 0, true, "", 1, argv_backslashfoo);
    test("\\ foo\\ ",       0, 0, true, "", 1, argv_spacefoospace);
    test("\\\\foo\\\\",     0, 0, true, "", 1, argv_backslashfoobackslash);
    test("foo\\ bar",       0, 0, true, "", 1, argv_foospacebar);
    test("foo\\\\bar",      0, 0, true, "", 1, argv_foobackslashbar);
    test("\\ foo bar",      0, 0, true, "", 2, argv_spacefoo_bar);
    test("\\\\foo bar",     0, 0, true, "", 2, argv_backslashfoo_bar);
    test("foo\\  bar",      0, 0, true, "", 2, argv_foospace_bar);
    test("foo\\\\ bar",     0, 0, true, "", 2, argv_foobackslash_bar);
    test("\\ foo\\  bar",   0, 0, true, "", 2, argv_spacefoospace_bar);
    test("\\\\foo\\\\ bar", 0, 0, true, "", 2, argv_backslashfoobackslash_bar);
    test("foo \\ bar",      0, 0, true, "", 2, argv_foo_spacebar);
    test("foo \\\\bar",     0, 0, true, "", 2, argv_foo_backslashbar);
    test("foo bar\\ ",      0, 0, true, "", 2, argv_foo_barspace);
    test("foo bar\\\\",     0, 0, true, "", 2, argv_foo_barbackslash);
    test("foo \\ bar\\ ",   0, 0, true, "", 2, argv_foo_spacebarspace);
    test("foo \\\\bar\\\\", 0, 0, true, "", 2, argv_foo_backslashbarbackslash);

    test("\\ \\ \\  \\ \\ \\  \\ \\ \\ ",          3, 0, true, "", 3, argv_spaceX3_X3);
    test("\\\\\\\\\\\\ \\\\\\\\\\\\ \\\\\\\\\\\\", 3, 0, true, "", 3, argv_backslashX3_X3);
    test("\\ \\ \\  \\ \\ \\  \\ \\ \\ ",          0, 4, true, "", 3, argv_spaceX3_X3);
    test("\\\\\\\\\\\\ \\\\\\\\\\\\ \\\\\\\\\\\\", 0, 4, true, "", 3, argv_backslashX3_X3);
    test("\\ \\ \\  \\ \\ \\  \\ \\ \\ ",          3, 4, true, "", 3, argv_spaceX3_X3);
    test("\\\\\\\\\\\\ \\\\\\\\\\\\ \\\\\\\\\\\\", 3, 4, true, "", 3, argv_backslashX3_X3);

    test("\\ \\ \\  \\ \\ \\  \\ \\ \\ ",          2, 0, false, "too many args", 0, argv0);
    test("\\\\\\\\\\\\ \\\\\\\\\\\\ \\\\\\\\\\\\", 2, 0, false, "too many args", 0, argv0);
    test("\\ \\ \\  \\ \\ \\  \\ \\ \\ ",          0, 3, false, "arg too long",  0, argv0);
    test("\\\\\\\\\\\\ \\\\\\\\\\\\ \\\\\\\\\\\\", 0, 3, false, "arg too long",  0, argv0);
    test("\\ \\ \\  \\ \\ \\  \\ \\ \\ ",          2, 3, false, "arg too long",  0, argv0);
    test("\\\\\\\\\\\\ \\\\\\\\\\\\ \\\\\\\\\\\\", 2, 3, false, "arg too long",  0, argv0);

    test("\\",     0, 0, false, "EOL after backslash", 0, argv0);
    test(" \\",    0, 0, false, "EOL after backslash", 0, argv0);
    test("\\ \\",  0, 0, false, "EOL after backslash", 0, argv0);
    test("\\\\\\", 0, 0, false, "EOL after backslash", 0, argv0);
    test("foo\\",  0, 0, false, "EOL after backslash", 0, argv0);

    return 0;
}

void test(
    const char *const cmdline,
    unsigned int argv_count_max,
    unsigned int arg_size_max,

    const bool expected_result,
    const char *const expected_error_msg,
    unsigned int expected_argc,
    const char *const *const expected_argv
) {
    if (argv_count_max == 0) {
        argv_count_max = ARGV_COUNT_MAX;
    }

    if (arg_size_max == 0) {
        arg_size_max = ARG_SIZE_MAX;
    }

    char error_msg[KERNAUX_CMDLINE_ERROR_MSG_SIZE_MAX];
    unsigned int argc = 1234;
    char *argv[argv_count_max];
    char buffer[argv_count_max * arg_size_max];

    assert(
        kernaux_cmdline_parse(
            cmdline,
            error_msg,
            &argc,
            argv,
            buffer,
            argv_count_max,
            arg_size_max
        ) == !!expected_result
    );

    assert(strcmp(error_msg, expected_error_msg) == 0);
    assert(argc == expected_argc);

    for (unsigned int index = 0; index < argc; ++index) {
        assert(strcmp(argv[index], expected_argv[index]) == 0);
    }

    for (unsigned int index = argc; index < argv_count_max; ++index) {
        assert(argv[index] == NULL);
    }
}
