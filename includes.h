#ifndef __includes__
#define __includes__

using namespace std;

#include <alloca.h>
#include <stddef.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <error.h>
#include <libgen.h>
#include <pwd.h>
#include <shadow.h>
#include <grp.h>
#include <limits.h>
#include <errno.h>
#include <dirent.h>
#include <getopt.h>
#include <stdio.h>
#include <malloc.h>
#include <regex.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <exception>
#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <fstream>
#include <vector>
#include <list>
#include <iterator>
#include <zbcl/plain_c/cfile-functions.h>
#include <zbcl/plain_c/carray-functions.h>
#include <zbcl/plain_c/cstring-functions.h>
#include <zbcl/plain_c/capache-functions-ext.h>
#include <zbcl/plain_c/user-functions.h>
#include <zbcl/plain_c/time-functions.h>
#include <zbcl/plain_cpp/free_functions/crypt-functions.hh>
#include <zbcl/plain_cpp/free_functions/conversion.hh>
#include <zbcl/plain_cpp/free_functions/exec-functions.hh>
#include <zbcl/plain_cpp/free_functions/filesystem-functions.hh>
#include <zbcl/plain_cpp/free_functions/file-functions.hh>
#include <zbcl/plain_cpp/free_functions/user-admin.hh>
#include <zbcl/plain_cpp/free_functions/apache-functions-ext.hh>
#include <soapH.h>  // include the generated source code headers
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options/config.hpp>
#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/option.hpp>

//#include <boost/process.hpp>
namespace pt = boost::property_tree;
namespace bs = boost::system;
//using namespace boost::system;
namespace filesys = boost::filesystem;
namespace ba = boost::algorithm;
namespace po = boost::program_options;

namespace pf = plain_cpp::free_functions;

using namespace pf;

#include <common_functions.h>
#include <misc_soap_functions.h>
#include <apache_functions.h>
#include <user_functions.h>
#include <soap_ssl_helpers.h>

const int REGEX_FLAGS = REG_EXTENDED|REG_ICASE; //|REG_NEWLINE;

#endif
