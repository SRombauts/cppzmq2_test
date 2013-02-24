#include <iostream>
#include <zmq.hpp>

/**
 * @brief Poin d'entr�e de l'application
*/
int main (int argc,char* argv)
{
   int major;
   int minor;
   int patch;
   zmq::version (&major, &minor, &patch);   
   
   std::cout << "libzmq version " << major << "." << minor << "." << patch << std::endl;
   
   return 0;
}
