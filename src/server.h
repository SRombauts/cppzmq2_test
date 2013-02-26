#include <iostream>
#include "../include/zmq.hpp"

class server_t
{
public:
   server_t (void);
   ~server_t (void);

   int loop (void);
};
