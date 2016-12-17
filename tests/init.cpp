#include "catch.h"
#include "../include/client.hpp"
//#include "server.hpp"

SCENARIO("Client", "[clean]") {
 bool p=true;
  Client A("config");
  REQUIRE(p==true);
}
