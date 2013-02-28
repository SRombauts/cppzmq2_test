#include <iostream>
#include "../cppzmq2/zmq.hpp"

class server_t
{
public:
   server_t (void);
   ~server_t (void);

   int loop (void);

private:
   zmq::context_t context;
};
