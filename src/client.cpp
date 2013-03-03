#include "client.h"

static const char* _dir_name = "out/";

client_t::client_t (void) :
   context(1)
{
}

client_t::~client_t (void)
{
}

int client_t::loop (void)
{
    int  ret     = EXIT_SUCCESS;
    bool running = true;

    std::cout << "client_t::loop\n";

    try
    {
        // socket to request files to the server
        std::cout << "binding...\n";
        zmq::socket_t requester(context, zmq::REQ);
        requester.connect ("tcp://localhost:5555");

        do
        {
            // send "get" request
            zmq::message_t request("get", strlen("get"));
            std::cout << "request(" << request.size () << ")=" << request.string ().c_str () << std::endl;
            requester.send (request);
      
            // wait for the filename reply message
            std::cout << "waiting for a filename reply\n";
            zmq::message_t reply;
            requester.recv (reply);
            std::cout << "reply(" << reply.size() << ")=" << reply.string ().c_str () << std::endl;
            if (0 < reply.size())
            {
                std::string filename = _dir_name;
                filename += reply.string ().c_str ();
                // then, its size,
                zmq::message_t reply_part2;
                requester.recv (reply_part2);
                std::cout << "reply_part2(" << reply_part2.size () << ")=" << ntohl(*(u_long*)reply_part2.data ()) << std::endl;
                // and finaly its content
                zmq::message_t reply_part3;
                requester.recv (reply_part3);
                std::cout << "reply_part3(" << reply_part3.size () << ")" << std::endl;
                // then fwrite the file to the out directory
                FILE* fp = fopen(filename.c_str (), "wb");
                if (NULL != fp)
                {
                    fwrite(reply_part3.data (), 1, reply_part3.size (), fp);
                    fclose (fp);
                }
                else
                {
                    abort();
                }
            }
            else
            {
                // quit the loop
                running = false;
            }
        } while (running);
    }
    catch (std::exception& e)
    {
        std::cout << e.what () << std::endl;
        ret = EXIT_FAILURE;
    }
    std::cout << "quitting\n";

    return ret;
}
