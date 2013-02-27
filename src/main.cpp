#include <iostream>
#include "../include/zmq.hpp"
#include "client.h"
#include "server.h"
#include "stdlib.h"


void usage (void)
{
   std::cout << "usage :\n";
   std::cout << " cppzmq_test c\n";
   std::cout << " cppzmq_test s\n";
}


/**
 * @brief Point d'entrée de l'application
*/
int main (int argc,char* argv[])
{
   int ret = EXIT_SUCCESS;
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
         {
            client_t client;
            ret = client.loop ();
         }
         break;
      case 's':
         {
            server_t server;
            ret = server.loop ();
         }
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

   return ret;
}
