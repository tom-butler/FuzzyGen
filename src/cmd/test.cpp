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
#include "..\shared\gen.h"

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
//@TODO: Test the set coverage and overlap
//this test requires init system to be run before
TEST_CASE("Test the shared set functions", "[Controller]") {
	kNumPop = 100;
	InitControllers();
	//during init controllers the function ForceVarBounds is called,
	//this test checks the validity of the function
	for(int c = 0; c < kNumPop; c++){
		for(int i = 0; i < kNumInput; i++) {
			for(int s = 0; s < cont[c].input[i].num_sets; s++) {
				//check set is in bounds
				CHECK(cont[c].input[i].sets[s].centre_x >= cont[c].input[i].low);
				CHECK(cont[c].input[i].sets[s].centre_x <= cont[c].input[i].high);

				CHECK((cont[c].input[i].sets[s].centre_x - cont[c].input[i].sets[s].left_base) >= cont[c].input[i].low);
				CHECK((cont[c].input[i].sets[s].centre_x - cont[c].input[i].sets[s].left_top) >= cont[c].input[i].low);
			
				CHECK((cont[c].input[i].sets[s].centre_x + cont[c].input[i].sets[s].right_base) <= cont[c].input[i].high);
				CHECK((cont[c].input[i].sets[s].centre_x + cont[c].input[i].sets[s].right_top) <= cont[c].input[i].high);

				//check set looks like a set
				CHECK(cont[c].input[i].sets[s].left_base  >= 0);
				CHECK(cont[c].input[i].sets[s].left_top   >= 0);
				CHECK(cont[c].input[i].sets[s].right_base >= 0);
				CHECK(cont[c].input[i].sets[s].right_top  >= 0);

				CHECK(cont[c].input[i].sets[s].left_base  >= cont[c].input[i].sets[s].left_top);
				CHECK(cont[c].input[i].sets[s].right_base  >= cont[c].input[i].sets[s].right_top);
			}
		}
	}
}
//this test requires init controllers to be run before
TEST_CASE("Test the shared controller functions", "[Controller]") {
	SECTION( "ResetAccumulator") {
		int controller = GetRandInt(0, kNumPop - 1);
		float value_collection = 0;
		float scale_collection = 0;
		InitSimulation(controller);
		RunSim(controller);

		//confirm that pre reset the accumulator has data
		for(int r = 0; r < cont[controller].output[0].num_rules; r++){

			value_collection += cont[controller].output[0].value[r];
			scale_collection += cont[controller].output[0].scale[r];
		}
		CHECK_FALSE(value_collection == 0);
		CHECK_FALSE(scale_collection == 0);
		CHECK_FALSE(cont[controller].output[0].num_active == 0);
		CHECK_FALSE(cont[controller].output[0].output == 0);


		ResetAccumulator(controller, 0);

		value_collection = 0;
		scale_collection = 0;
		//confirm that the accumulator is empty
		for(int r = 0; r < cont[controller].output[0].num_rules; r++){

			value_collection += cont[controller].output[0].value[r];
			scale_collection += cont[controller].output[0].scale[r];
		}
		CHECK(value_collection == 0);
		CHECK(scale_collection == 0);
		CHECK(cont[controller].output[0].num_active == 0);
		CHECK(cont[controller].output[0].output == 0);

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
