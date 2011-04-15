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
