#include "pch.h"
#include "CppUnitTest.h"
#include "../GolfSim/main.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		climate testClimate = climate(100.0, 1.2, 0, 0);
		ball testBall = ball();
		club testClub = club();
		shot testShot = shot();

		//tests amout of curve for 2 degree axis tilt
		TEST_METHOD(spinTest2deg)
		{
			float expectedCurve = 3.0;
			swing testSwing = swing(96.0, 1.43, 11.0, -3.4, 2.0, 4836.0);
			testShot = simulate(testShot, testSwing, testClub, testBall, testClimate);
			Assert::IsTrue(testShot.curveInAir >= (expectedCurve-1.0) && testShot.curveInAir <= (expectedCurve+1.0));
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
			climate testClimate2 = climate(90.0, 1.2, 0, 0);
			shot testShot2 = shot();
			swing wood3 = swing(107.0, 1.48, 9.2, -2.9, 0.0, 3655.0);
			testShot = simulate(testShot, wood3, testClub, testBall, testClimate);
			testShot2 = simulate(testShot, wood3, testClub, testBall, testClimate2);
			int testValue1 = testShot.carryDist * .99;
			int testValue2 = testShot2.carryDist;
			Assert::AreEqual(testValue1, testValue2);
		}
	};


	//Test trackman data input against distance output
	//Criteria: must be within 5% range of error
	//output of simulation is converted from meters to yards for comparison
	TEST_CLASS(acceptanceTest1) {


		climate testClimate = climate(100.0, 1.2, 0, 0);
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
}
