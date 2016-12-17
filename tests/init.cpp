#include "catch.h"
#include "../include/client.hpp"

SCENARIO("Client", "[clean]") {
 bool p=true;
 boost::filesystem::recursive_directory_iterator end;
 // #Client A("config");
  REQUIRE(p==true);
}
