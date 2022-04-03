#include "pch.h"
#include "CppUnitTest.h"
#include <time.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using std::cout;
using std::cin;
using std::endl;
using std::cos;
using std::sin;
using std::fstream;
using std::string;
using std::vector;

const int screenWidth = 1280, screenHeight = 768;
const int halfWidth = screenWidth / 2, halfHeight = screenHeight / 2;
const int mapSizeX = 50, mapSizeZ = 400, mapScale = 25;

#include "..\golfsim\point.h"
#include "..\golfsim\climate.h"
#include "..\golfsim\club.h"
#include "..\golfsim\swing.h"
#include "..\golfsim\ball.h"
#include "..\golfsim\shot.h"
#include "..\golfSim\utilityfunctions.h"
#include "..\golfsim\simulator.h"
#include "..\GolfSim\camera.h"
camera cam = camera(0.0f, 30.0f, -250.0f, 0.0f);
#include "..\GolfSim\polygon.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest01
{
	TEST_CLASS(UnitTest01)
	{
	public:

		climate testClimate = climate(100.0, 0, 0, 0);
		ball testBall = ball();
		club testClub = club();
		shot testShot = shot();

		//tests amout of curve for 2 degree axis tilt
		TEST_METHOD(spinTest2deg)
		{
			float expectedCurve = 3.0;
			swing testSwing = swing(96.0, 1.43, 11.0, -3.4, 2.0, 4836.0);
			testShot = simulate(testShot, testSwing, testClub, testBall, testClimate);
			Assert::IsTrue(testShot.curveInAir >= (expectedCurve - 1.0) && testShot.curveInAir <= (expectedCurve + 1.0));
		}

		//Test amount of curve for 10 degree axis tilt
		TEST_METHOD(spinTest10deg)
		{
			float expectedCurve = 15.0;
			swing testSwing = swing(96.0, 1.43, 11.0, -3.4, 10.0, 4836.0);
			testShot = simulate(testShot, testSwing, testClub, testBall, testClimate);
			Assert::IsTrue(testShot.curveInAir >= (expectedCurve - 1.0) && testShot.curveInAir <= (expectedCurve + 1.0));
		}

		//Tests temperature effect on distance
		//Change should be 1% per 10 degrees
		//results rounded for comparison
		TEST_METHOD(temperatureTest)
		{
			climate testClimate2 = climate(90.0, 0, 0, 0);
			shot testShot2 = shot();
			swing wood3 = swing(107.0, 1.48, 9.2, -2.9, 0.0, 3655.0);
			testShot = simulate(testShot, wood3, testClub, testBall, testClimate);
			testShot2 = simulate(testShot, wood3, testClub, testBall, testClimate2);
			int testValue1 = testShot.carryDist * .99;
			int testValue2 = testShot2.carryDist;
			Assert::AreEqual(testValue1, testValue2);
		}

		//unit conversion test
		TEST_METHOD(unitConvertTest)
		{
			float expectedYards = 5.0;
			float meters = 4.572;
			float yards = metersToYards(meters);
			Assert::AreEqual(yards, expectedYards, 0.001F);
		}

		//altitude test
		//tests 0 alt shot vs 5000ft alt shot for expected change
		//change is estimated as feet*.00116 = %
		TEST_METHOD(altitudeTest)
		{
			climate testClimate2 = climate(100.0, 5000.0, 0, 0);
			swing driver = swing(113.0, 1.48, 10.9, -1.3, 0.0, 2686.0); // normal distance 275m
			shot testShot = simulate(testShot, driver, testClub, testBall, testClimate2);
			shot testShot2 = simulate(testShot, driver, testClub, testBall, testClimate);
			float expectedMeters = testShot2.carryDist * (1.0 + 5000.0 * .0000116);
			Assert::AreEqual(testShot.carryDist, expectedMeters, .001F);
		}


		//test to make sure polygon with 0 or negetive depth values is not drawn
		//this is becuse we cannt divide by 0 and negitive depth values are behind us, and not drawn
		TEST_METHOD(polyTest1) {
			polygon testPoly(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -500.0f,0,0,0);
			testPoly.convert2D();
			bool test = false;
			if (testPoly.points2D.empty()) test = true;
			Assert::IsTrue(test);
		}

	};


	//Acceptance test for shot distances 
	//compared to trackman data with 5% error 
	TEST_CLASS(acceptanceTest1) {


		climate testClimate = climate(100.0, 0, 0, 0);
		ball testBall = ball();
		club testClub = club();
		shot testShot = shot();
		float expected, testHigh, testLow, testValue;
		float error = .05;//room for error

		TEST_METHOD(driver) {
			expected = 275.0;
			swing driver = swing(113.0, 1.48, 10.9, -1.3, 0.0, 2686.0);
			testShot = simulate(testShot, driver, testClub, testBall, testClimate);
			testHigh = expected + expected * error;
			testLow = expected - expected * error;
			testValue = testShot.carryDist * 1.0936;
			Assert::IsTrue(testValue > testLow && testValue < testHigh);
		}

		TEST_METHOD(wood3) {
			expected = 243.0;
			swing wood3 = swing(107.0, 1.48, 9.2, -2.9, 0.0, 3655.0);
			testShot = simulate(testShot, wood3, testClub, testBall, testClimate);
			testHigh = expected + expected * error;
			testLow = expected - expected * error;
			testValue = testShot.carryDist * 1.0936;
			Assert::IsTrue(testValue > testLow && testValue < testHigh);
		}

		TEST_METHOD(wood5) {
			expected = 230.0;
			swing wood5 = swing(103.0, 1.47, 9.4, -3.3, 0.0, 4350.0);
			testShot = simulate(testShot, wood5, testClub, testBall, testClimate);
			testHigh = expected + expected * error;
			testLow = expected - expected * error;
			testValue = testShot.carryDist * 1.0936;
			Assert::IsTrue(testValue > testLow && testValue < testHigh);
		}

		TEST_METHOD(wood7) {
			expected = 225.0;
			swing wood7 = swing(101.0, 1.46, 10.0, -3.4, 0.0, 4400.0);
			testShot = simulate(testShot, wood7, testClub, testBall, testClimate);
			testHigh = expected + expected * error;
			testLow = expected - expected * error;
			testValue = testShot.carryDist * 1.0936;
			Assert::IsTrue(testValue > testLow && testValue < testHigh);
		}

		TEST_METHOD(hybrid) {
			expected = 225.0;
			swing hybrid = swing(100.0, 1.46, 10.2, -3.5, 0.0, 4437.0);
			testShot = simulate(testShot, hybrid, testClub, testBall, testClimate);
			testHigh = expected + expected * error;
			testLow = expected - expected * error;
			testValue = testShot.carryDist * 1.0936;
			Assert::IsTrue(testValue > testLow && testValue < testHigh);
		}

		TEST_METHOD(iron3) {
			expected = 212.0;
			swing iron3 = swing(98.0, 1.45, 10.4, -3.1, 0.0, 4630.0);
			testShot = simulate(testShot, iron3, testClub, testBall, testClimate);
			testHigh = expected + expected * error;
			testLow = expected - expected * error;
			testValue = testShot.carryDist * 1.0936;
			Assert::IsTrue(testValue > testLow && testValue < testHigh);
		}

		TEST_METHOD(iron4) {
			expected = 203.0;
			swing iron4 = swing(96.0, 1.43, 11.0, -3.4, 0.0, 4836.0);
			testShot = simulate(testShot, iron4, testClub, testBall, testClimate);
			testHigh = expected + expected * error;
			testLow = expected - expected * error;
			testValue = testShot.carryDist * 1.0936;
			Assert::IsTrue(testValue > testLow && testValue < testHigh);
		}

		TEST_METHOD(iron5) {
			expected = 194.0;
			swing iron5 = swing(94.0, 1.41, 12.1, -3.7, 0.0, 5361.0);
			testShot = simulate(testShot, iron5, testClub, testBall, testClimate);
			testHigh = expected + expected * error;
			testLow = expected - expected * error;
			testValue = testShot.carryDist * 1.0936;
			Assert::IsTrue(testValue > testLow && testValue < testHigh);
		}

		TEST_METHOD(iron6) {
			expected = 183.0;
			swing iron6 = swing(92.0, 1.38, 14.1, -4.1, 0.0, 6231.0);
			testShot = simulate(testShot, iron6, testClub, testBall, testClimate);
			testHigh = expected + expected * error;
			testLow = expected - expected * error;
			testValue = testShot.carryDist * 1.0936;
			Assert::IsTrue(testValue > testLow && testValue < testHigh);
		}

		TEST_METHOD(iron7) {
			expected = 172.0;
			swing iron7 = swing(90.0, 1.33, 16.3, -4.3, 0.0, 7097.0);
			testShot = simulate(testShot, iron7, testClub, testBall, testClimate);
			testHigh = expected + expected * error;
			testLow = expected - expected * error;
			testValue = testShot.carryDist * 1.0936;
			Assert::IsTrue(testValue > testLow && testValue < testHigh);
		}

		TEST_METHOD(iron8) {
			expected = 160.0;
			swing iron8 = swing(87.0, 1.32, 18.1, -4.5, 0.0, 7998.0);
			testShot = simulate(testShot, iron8, testClub, testBall, testClimate);
			testHigh = expected + expected * error;
			testLow = expected - expected * error;
			testValue = testShot.carryDist * 1.0936;
			Assert::IsTrue(testValue > testLow && testValue < testHigh);
		}

		TEST_METHOD(iron9) {
			expected = 148.0;
			swing iron9 = swing(85.0, 1.28, 20.4, -4.7, 0.0, 8647.0);
			testShot = simulate(testShot, iron9, testClub, testBall, testClimate);
			testHigh = expected + expected * error;
			testLow = expected - expected * error;
			testValue = testShot.carryDist * 1.0936;
			Assert::IsTrue(testValue > testLow && testValue < testHigh);
		}

		TEST_METHOD(ironPW) {
			expected = 136.0;
			swing ironPW = swing(83.0, 1.23, 24.2, -5.0, 0.0, 9304.0);
			testShot = simulate(testShot, ironPW, testClub, testBall, testClimate);
			testHigh = expected + expected * error;
			testLow = expected - expected * error;
			testValue = testShot.carryDist * 1.0936;
			Assert::IsTrue(testValue > testLow && testValue < testHigh);
		}




	};


	//Acceptance test for input values and sanitization
	TEST_CLASS(acceptanceTest2) {
		climate testClimate = climate(100.0, 0, 0, 0);
		ball testBall = ball();
		club testClub = club();
		shot testShot = shot();

		TEST_METHOD(swingBoundaryLow) {

			swing testSwingBoundLow = swing(-0.1F, -.1F, -.1F, -.1F, -20.1F, -.1F);
			swing testSwingBoundLowExpected = swing(-0.0F, -0.0F, -0.0F, -0.0F, -20.0F, -0.0F);
			testSwingBoundLow = sanitizeSwing(testSwingBoundLow);
			bool passed = true;
			if (testSwingBoundLow.clubSpeed != testSwingBoundLowExpected.clubSpeed) passed = false;
			if (testSwingBoundLow.smash != testSwingBoundLowExpected.smash) passed = false;
			if (testSwingBoundLow.launchAngle != testSwingBoundLowExpected.launchAngle) passed = false;
			if (testSwingBoundLow.spin != testSwingBoundLowExpected.spin) passed = false;
			if (testSwingBoundLow.spinAxis != testSwingBoundLowExpected.spinAxis) passed = false;

			Assert::IsTrue(passed);
		}

		TEST_METHOD(swingBoundaryHigh) {

			swing testSwingBoundLow = swing(150.1F, 1.51F, 90.1F, 100.1F, 20.1F, 15000.1F);
			swing testSwingBoundLowExpected = swing(150.0F, 1.5F, 90.0F, 100.1F, 20.0F, 15000.0F);
			testSwingBoundLow = sanitizeSwing(testSwingBoundLow);
			bool passed = true;
			if (testSwingBoundLow.clubSpeed != testSwingBoundLowExpected.clubSpeed) passed = false;
			if (testSwingBoundLow.smash != testSwingBoundLowExpected.smash) passed = false;
			if (testSwingBoundLow.launchAngle != testSwingBoundLowExpected.launchAngle) passed = false;
			if (testSwingBoundLow.spin != testSwingBoundLowExpected.spin) passed = false;
			if (testSwingBoundLow.spinAxis != testSwingBoundLowExpected.spinAxis) passed = false;

			Assert::IsTrue(passed);
		}

		TEST_METHOD(climateBoundaryLow) {

			climate testClimateBoundLow = climate(-.1F, -.1F, -.1f, -.1f);
			climate testClimateBoundLowExpected = climate(-0.0F, -0.0F, -0.0f, -0.0f);
			testClimateBoundLow = sanitizeClimate(testClimateBoundLow);
			bool passed = true;
			if (testClimateBoundLow.temperature != testClimateBoundLowExpected.temperature) passed = false;
			if (testClimateBoundLow.altitude != testClimateBoundLowExpected.altitude) passed = false;
			if (testClimateBoundLow.windSpeed != testClimateBoundLowExpected.windSpeed) passed = false;
			if (testClimateBoundLow.windDirection != testClimateBoundLowExpected.windDirection) passed = false;

			Assert::IsTrue(passed);
		}

		TEST_METHOD(climateBoundaryHigh) {

			climate testClimateBoundLow = climate(120.1F, 15000.1F, 20.1f, 360.1f);
			climate testClimateBoundLowExpected = climate(120.0F, 15000.0F, 20.0f, 360.0f);
			testClimateBoundLow = sanitizeClimate(testClimateBoundLow);
			bool passed = true;
			if (testClimateBoundLow.temperature != testClimateBoundLowExpected.temperature) passed = false;
			if (testClimateBoundLow.altitude != testClimateBoundLowExpected.altitude) passed = false;
			if (testClimateBoundLow.windSpeed != testClimateBoundLowExpected.windSpeed) passed = false;
			if (testClimateBoundLow.windDirection != testClimateBoundLowExpected.windDirection) passed = false;

			Assert::IsTrue(passed);
		}


	};

	//Performance test
	TEST_CLASS(acceptanceTest3) {
		climate testClimate = climate(100.0, 0, 0, 0);
		swing testSwing = swing(83.0, 1.23, 24.2, -5.0, 0.0, 9304.0);
		ball testBall = ball();
		club testClub = club();
		shot testShot = shot();

		TEST_METHOD(performanceTest) {
			clock_t start, end;
			start = clock();
			simulate(testShot, testSwing, testClub, testBall, testClimate);
			end = clock();
			Assert::IsTrue(end - start < 500);
			string a = std::to_string(long (end - start));
			char* b = _strdup(a.c_str());
			Logger::WriteMessage(b);
		}

	};
}
