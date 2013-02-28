#include <iostream>
#include "../cppzmq2/zmq.hpp"

class client_t
{
public:
   client_t (void);
   ~client_t (void);

   int loop (void);

private:
   zmq::context_t context;
};
