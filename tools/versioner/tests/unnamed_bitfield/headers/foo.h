// <sys/timex.h> was causing a segfault when compiled in C++ mode because
// versioner was trying to mangle the name of an unnamed bitfield.
struct foo {
  int : 32;
  int : 32;
  int : 32;
  int : 32;
};
