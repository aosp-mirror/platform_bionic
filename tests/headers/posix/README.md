# POSIX header tests

These compile-time tests check that each POSIX header contains _at
least_ what POSIX says. Every POSIX header file gets a corresponding
`.c` file in this directory. Every constant, macro, type, struct field,
and function in the header gets a corresponding assertion in the file.

See `header_checks.h` for the implementation of the assertions.
