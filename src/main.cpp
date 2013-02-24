#include <iostream>
#include <zmq.hpp>

void usage (void)
{
   std::cout << "usage :\n";
   std::cout << " cppzmq_test c\n";
   std::cout << " cppzmq_test s\n";
}

void client (void)
{
   std::cout << "client\n";
}

void server (void)
{
   std::cout << "server\n";
}


/**
 * @brief Poin d'entrée de l'application
*/
int main (int argc,char* argv[])
{
   int major;
   int minor;
   int patch;
   zmq::version (&major, &minor, &patch);   
   
   std::cout << "libzmq version " << major << "." << minor << "." << patch << std::endl;
   
   if (2 <= argc)
   {
      switch (argv[1][0])
      {
      case 'c':
         client ();
         break;
      case 's':
         server ();
         break;
      default:
         usage ();
         break;
      }
   }
   else
   {
      usage ();
   }

   return 0;
}
