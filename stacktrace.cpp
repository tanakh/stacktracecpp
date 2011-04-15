#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <typeinfo>
#include <cstring>
#include <vector>
using namespace std;

#define UNW_LOCAL_ONLY
#include <libunwind.h>

#include <cxxabi.h>

const char *self_name()
{
  pid_t pid=getpid();
  static char buf[256];
  sprintf(buf, "/proc/%d/environ", pid);
  FILE *f=fopen(buf, "rb");
  
  for (;;){
    int i=0;
    for (int c; ; i++){
      c=fgetc(f);
      if (c==EOF || c=='\0') break;
      buf[i]=c;
    }
    buf[i]='\0';
    if (i==0) break;
    if (strncmp(buf, "_=", 2)==0){
      fclose(f);
      return buf+2;
    }
  }
  fclose(f);
  return buf;
}

vector<pair<string, string> > get_backtrace()
{
  vector<pair<string, string> > ret;
  
  unw_cursor_t cursor; unw_context_t uc;
  unw_word_t ip, sp;
  
  ostringstream oss;
  oss<<"echo '";
  
  unw_getcontext(&uc);
  unw_init_local(&cursor, &uc);
  while (unw_step(&cursor) > 0) {
    unw_get_reg(&cursor, UNW_REG_IP, &ip);
    unw_get_reg(&cursor, UNW_REG_SP, &sp);
    oss<<hex<<ip<<endl;
  }
  oss<<"' | addr2line -f -C -e "<<self_name();
  
  FILE *f=popen(oss.str().c_str(), "r");
  for (;;){
    char func[256];
    char file[256];
    if (!(fgets(func, 256, f) &&
	  fgets(file, 256, f)))
      break;
    func[strlen(func)-1]='\0';
    file[strlen(file)-1]='\0';
    ret.push_back(make_pair(func, file));
  }
  fclose(f);

  return ret;
}

void show_backtrace()
{
  vector<pair<string, string> > bt=get_backtrace();
  for (size_t i=2; i<bt.size(); i++){
    if (bt[i].first=="??") continue;
    cout<<bt[i].first<<" at "<<bt[i].second<<endl;
  }
}

string demangle(const string &id)
{
  int status;
  return abi::__cxa_demangle(id.c_str(), 0, 0, &status);
}

void backtrace_terminate()
{
  try{
    __throw_exception_again;
  }
  catch(const exception &e){
    cout<<"Uncaught exception \""<<demangle(typeid(e).name())<<"\": "<<e.what()<<endl;
  }
  catch(...){
    cout<<"Uncaught exception unknown-type:"<<endl;
  }

  vector<pair<string, string> > bt=get_backtrace();
  for (size_t i=1; i<bt.size(); i++){
    if (bt[i].first=="??") continue;
    cout<<"\t"<<bt[i].first<<"\t("<<bt[i].second<<")"<<endl;
  }

  abort();
}

class hook{
public:
  hook(){
    set_terminate(backtrace_terminate);
  }
} h;
