#include <iostream>
#include <cstdlib>
#include <cstring>

#include "lilc_compiler.hpp"

int 
main( const int argc, const char **argv )
{
   if (argc != 3){
	std::cout << "Usage: P3 <infile> <outfile>" << std::endl;
	return 1;
   }

   LILC::LilC_Compiler compiler;
   compiler.parse( argv[1], argv[2] );
   return 0;
}
