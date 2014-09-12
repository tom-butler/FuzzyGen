/**
 * Unit Tests for FuzzyGen
 *
 * This file uses catch (a c++ unit testing framework) 
 * to test the system for un-expected behaviour
 */

#define CATCH_CONFIG_MAIN
#include "..\..\libs\catch.hpp"
#include "..\shared\shared.h"

TEST_CASE("Init System", "[Shared]") {
	//we need to initialise the system to seed random vars
	InitSystem();
}

TEST_CASE("Get Random Int", "[Shared]") {
	CHECK( GetRandInt(-1, 0) >= -1 );
	CHECK( GetRandInt( 0, 1) <= 1 );
}

TEST_CASE("Get Random Float", "[Shared]") {
	CHECK( GetRandFloat(-0.2, -0.1) >= -0.2 );
	CHECK( GetRandFloat(-0.2, -0.1) <= -0.1 );
	CHECK( GetRandFloat(-1  , 0   ) >= -1.0 );
	CHECK( GetRandFloat(-1  , 0   ) <=  0.0 );
}
TEST_CASE("Lerp", "[Shared]") {
	CHECK( Intersect(0, 1, 100, 0, 50   ) >   0);
	CHECK( Intersect(0, 1, 100, 0, 50   ) <   1);
	CHECK( Intersect(0, 1, 100, 0, 50   ) == 0.5);
	CHECK( Intersect(0,	1,   0, 1,  0   ) ==  1);
	CHECK( Intersect(0, 0,   1, 0,  0.5f) ==  0);
	CHECK( Intersect(0, 1,  10, 0, 10   ) ==  0);

}
