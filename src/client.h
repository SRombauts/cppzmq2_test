#include <iostream>
#include "../include/zmq.hpp"

class client_t
{
public:
   client_t (void);
   ~client_t (void);

   int loop (void);
};
