#include "client.h"

client_t::client_t (void) :
   context(1)
{
}

client_t::~client_t (void)
{
}

int client_t::loop (void)
{
   int ret = EXIT_SUCCESS;

   std::cout << "client_t::loop\n";

   try
   {
      // socket to request files to the server
      std::cout << "binding...\n";
      zmq::socket_t requester(context, zmq::REQ);
      requester.connect ("tcp://localhost:5555");

      {
         // begin with a "hello" request
         zmq::message_t request("hello", strlen("hello"));
         std::cout << "request(" << request.size () << ")=" << request.string ().c_str () << std::endl;
         requester.send (request);
      
         // wait for the welcome reply message
         std::cout << "waiting for a welcome reply\n";
         zmq::message_t reply;
         requester.recv (reply);
         std::cout << "reply(" << reply.size () << ")=" << reply.string ().c_str () << std::endl;
      }

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
         std::cout << "reply(" << reply.size () << ")=" << reply.string ().c_str () << std::endl;
         // then, its size,
         zmq::message_t reply_part2;
         requester.recv (reply_part2);
         std::cout << "reply_part2(" << reply_part2.size () << ")=" << ntohl(*(u_long*)reply_part2.data ()) << std::endl;
         // and finaly ist content
         zmq::message_t reply_part3;
         requester.recv (reply_part3);
         std::cout << "reply_part3(" << reply_part3.size () << ")=" << reply_part3.string ().c_str () << std::endl;
         // TODO fopen and fwrite
      } while (true);
   }
   catch (std::exception& e)
   {
      std::cout << e.what () << std::endl;
      ret = EXIT_FAILURE;
   }

   return ret;
}
