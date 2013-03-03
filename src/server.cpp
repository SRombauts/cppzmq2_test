#ifdef _WIN32
#include "../include/win32/dirent.h"
#include "winsock2.h"
#else
#include <dirent.h>
#include <net/hton.h>
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
                std::cout << "request(" << request.size () << ")=" << request.string ().c_str () << std::endl;

                std::cout << "filename=" << ent->d_name << std::endl;

                // reply with the name of the file found
                zmq::message_t reply(ent->d_name, strlen(ent->d_name));
                std::cout << "reply(" << reply.size () << ")=" << reply.string ().c_str () << std::endl;
                receiver.send (reply, zmq::SNDMORE);

                // add the size of the file found
                std::string filename = _dir_name;
                filename += ent->d_name;
                struct stat st;
                int ok = stat(filename.c_str (), &st);
                if (0 == ok)
                {
                    u_long val = htonl(st.st_size);
                    zmq::message_t reply_part2((void*)&val, sizeof(st.st_size));
                    std::cout << "reply_part2(" << reply_part2.size () << ")=" << st.st_size << std::endl;
                    receiver.send (reply_part2, zmq::SNDMORE);
                }
                else
                {
                    abort();
                }

                // add the content of the file found
                FILE* fp = fopen(filename.c_str (), "rb");
                if (NULL != fp)
                {
                    zmq::message_t reply_part3(st.st_size);
                    fread(reply_part3.data (), 1, st.st_size, fp);
                    fclose (fp);
                    std::cout << "reply_part3(" << reply_part3.size () << ")" << std::endl;
                    receiver.send (reply_part3);
                }
                else
                {
                    abort();
                }
            }
        }
        closedir (dir);

        // wait for a last request message
        std::cout << "waiting for a last get message...\n";
        zmq::message_t request;
        receiver.recv (request);
        std::cout << "request(" << request.size () << ")=" << request.string ().c_str () << std::endl;

        // Then, tell the client to quit
        zmq::message_t reply(0);
        std::cout << "end reply(" << reply.size () << ")=" << reply.string ().c_str () << std::endl;
        receiver.send (reply);
    }
    else
    {
        // could not open directory
        perror ("could not open directory");
        ret = EXIT_FAILURE;
    }
    std::cout << "quitting\n";

    return ret;
}
