=================================
stacktrace++: Stacktracer for C++
=================================

About
-----

This library make your C++ program to show a stacktrace like Java
when your program crash on an uncaught exception.

Install
-------

To install, you need to install libunwind.
Then do following instructions:

::

  $ ./waf configure
  $ ./waf build
  $ sudo ./waf install

Usage
-----

You only need to link this library.

::

  $ g++ test.cpp -g -O0 -lstacktrace++

(-g and -O0 helps library to lookup names of functions and filenames)

Sample
------

Here is tiny C++ program.

::

  #include <iostream>
  #include <stdexcept>
  using namespace std;
  
  void foo()
  {
    throw runtime_error("hoge");
  }
  
  int main()
  {
    foo();
    return 0;
  }

Compile and run this:

::

  $ g++ test.cpp -g -O0 && ./a.out
  terminate called after throwing an instance of 'std::runtime_error'
    what():  hoge

It becomes to:

::

  $ g++ test.cpp -g -O0 -lstacktrace++ && ./a.out
  Uncaught exception "std::runtime_error": hoge
    foo()	(/home/tanakh/project/stacktracecpp/test.cpp:7)
    main	(/home/tanakh/project/stacktracecpp/test.cpp:13)
    _start	(??:0)
