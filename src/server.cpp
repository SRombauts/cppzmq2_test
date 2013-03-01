#ifdef _WIN32
#include "../include/win32/dirent.h"
#else
#include <dirent.h>
#endif
#include <sys/stat.h>

#include "server.h"


static const char* _dir_name = "src/";


server_t::server_t (void) :
   context(1)
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

   if ((dir = opendir (_dir_name)) != NULL)
   {
      // socket to reply to client requests
      std::cout << "binding...\n";
      zmq::socket_t receiver(context, zmq::REP);
      receiver.bind ("tcp://*:5555");

      {
         // wait for the hello request message
         std::cout << "waiting for the hello message...\n";
         zmq::message_t request;
         receiver.recv (request);
         std::cout << "request(" << request.size () << ")=" << (const char*)request.data () << std::endl;

         // reply with the welcome message
         zmq::message_t reply("welcome", strlen("welcome")+1);
         std::cout << "reply(" << reply.size () << ")=" << (const char*)reply.data() << std::endl;
         receiver.send (reply);
      }

      // print all the files and directories within directory
      while ((ent = readdir (dir)) != NULL)
      {
         // filter les répertoires "." et ".."
         if ('.' != ent->d_name[0])
         {
            // wait for a request message
            std::cout << "waiting for a get message...\n";
            zmq::message_t request;
            receiver.recv (request);
            std::cout << "request(" << request.size () << ")=" << (const char*)request.data () << std::endl;

            std::cout << "filename=" << ent->d_name << std::endl;

            // reply with the name of the file found
            zmq::message_t reply(ent->d_name, strlen(ent->d_name)+1);
            std::cout << "reply(" << reply.size () << ")=" << (const char*)reply.data() << std::endl;
            receiver.send (reply);

            // TODO add the size of the file found
            std::string filename = _dir_name;
            filename += ent->d_name;
            struct stat st;
            int ok = stat(filename.c_str (), &st);
            if (0 == ok)
            {
                std::cout << "filesize=" << st.st_size << std::endl;
            }

            // TODO add the content of the file found
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
