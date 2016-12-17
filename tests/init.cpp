#include "catch.h"
//#include "client.cpp"
//#include "server.hpp"

SCENARIO("Client", "[clean]") {
 bool p=true;
  Client A("config");
  REQUIRE(p==true);
}
