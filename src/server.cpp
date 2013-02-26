#include "dirent.h"

#include "server.h"

server_t::server_t (void)
{
}

server_t::~server_t (void)
{
}

int server_t::loop (void)
{
   int            ret = EXIT_SUCCESS;
   DIR*           dir;
   struct dirent* ent;

   std::cout << "server_t::loop\n";

   if ((dir = opendir ("src")) != NULL)
   {
      // print all the files and directories within directory
      while ((ent = readdir (dir)) != NULL)
      {
         // filter les répertoires "." et ".."
         if ('.' != ent->d_name[0])
         {
            printf ("%s\n", ent->d_name);
         }
      }
      closedir (dir);
   }
   else
   {
      // could not open directory
      perror ("");
      ret = EXIT_FAILURE;
   }

   return ret;
}
