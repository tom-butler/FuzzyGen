/**
 * Unit Tests for FuzzyGen
 *
 * This file uses catch (a c++ unit testing framework) 
 * to test the system for un-expected behaviour
 */

const float PI = 3.14159;

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
	//test middle range
	REQUIRE( Lerp(0.5f, 0, 100) == 50);
	REQUIRE( Lerp(0.5f, -50, 50) == 0);
	REQUIRE( Lerp(0.5f, -100, -50) == -75);
	//low range
	REQUIRE( Lerp(0.0f, 0, 100) == 0);
	REQUIRE( Lerp(0.0f, -50, 50) == -50);
	REQUIRE( Lerp(0.0f, -100, -50) == -100);
	//high range
	REQUIRE( Lerp(1.0f, 0, 100) == 100);
	REQUIRE( Lerp(1.0f, -50, 50) == 50);
	REQUIRE( Lerp(1.0f, -100, -50) == -50);
}

TEST_CASE("Intersect", "[Shared]") {
	CHECK( Intersect(0, 1, 100, 0, 50   ) >   0);
	CHECK( Intersect(0, 1, 100, 0, 50   ) <   1);
	REQUIRE( Intersect(0, 1, 100, 0, 50   ) == 0.5);
	REQUIRE( Intersect(0,	1,   0, 1,  0   ) ==  1);
	REQUIRE( Intersect(0, 0,   1, 0,  0.5f) ==  0);
	REQUIRE( Intersect(0, 1,  10, 0, 10   ) ==  0);
}

TEST_CASE("DegToRad", "[Shared]") {
	REQUIRE(DegToRad(0) == 0);
	REQUIRE(DegToRad(90) == 0.5 * PI);
	REQUIRE(DegToRad(180) == PI);
	REQUIRE(DegToRad(270) == 1.5 * PI);

	REQUIRE(DegToRad(0) == 0);
	REQUIRE(DegToRad(-90) == -0.5 * PI);
	REQUIRE(DegToRad(-180) == -PI);
	REQUIRE(DegToRad(-270) == -1.5 * PI);

}
TEST_CASE("RadToDeg", "[Shared]") {
	REQUIRE(RadToDeg(0) == 0);
	REQUIRE(RadToDeg(0.5 * PI) == 90);
	REQUIRE(RadToDeg(PI) == 180);
	REQUIRE(RadToDeg(1.5 * PI) == 270);
	REQUIRE(RadToDeg(2 * PI) == 360);

	REQUIRE(RadToDeg(0) == 0);
	REQUIRE(RadToDeg(-0.5 * PI) == -90);
	REQUIRE(RadToDeg(-PI) == -180);
	REQUIRE(RadToDeg(-1.5 * PI) == -270);
	REQUIRE(RadToDeg(-2 * PI) == -360);
}
TEST_CASE("Sqr","[Shared]") {
 	REQUIRE(sqr(0) == 0);
 	REQUIRE(sqr(1) == 1);
 	REQUIRE(sqr(10) == 100);
 	REQUIRE(sqr(-10) == 100);
 	REQUIRE(sqr(-1) == 1);
 	REQUIRE(sqr(0.5) == 0.25);
 	REQUIRE(sqr(-0.5) == 0.25);
}

TEST_CASE("Test the shared set functions", "[Controller]") {
	kNumPop = 1;
	kForceSetCoverage = true;

	SECTION( "CheckSet") {

	}
	SECTION( "ForceBounds") {

	}
}

TEST_CASE("Test the shared controller functions", "[Controller]") {
	kNumPop = 1;
	kForceSetCoverage = true;

	SECTION( "ResetAccumulator") {

	}
	SECTION( "CleanController") {

	}
	SECTION( "CleanAccumulators") {

	}
	SECTION( "CleanSets") {

	}
	SECTION( "CleanRules") {

	}
	SECTION( "CopyController") {

	}
}
