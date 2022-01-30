#include "pch.h"
#include "CppUnitTest.h"
#include "../GolfSim/main.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		//test 1: simple projectile test against third party simulator results
		// 50m/s @ 14 deg should be 119.7m
		// input is adjusted for mph, rounded for comparison
		TEST_METHOD(TestMethod1)
		{
			shot shot1;
			climate climate1 = climate(75.0);
			swing swing1(74.56454, 2.0, 0.0, 1.5);
			club club1(12.0);

			shot1 = simulate(shot1, swing1, club1, climate1);
			int testDist = shot1.distance;
			Assert::AreEqual(testDist, 119);
		}
	};
}
