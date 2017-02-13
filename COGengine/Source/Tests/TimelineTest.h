#pragma once

#include "ofMain.h"
#include "ofxCogEngine.h"


#include "catch.hpp"
using namespace Cog;
#include "TimeLine.h"













TEST_CASE("TimeLine")
{

	SECTION("Proper ordering")
	{
		auto id1 = StrId(1);

		auto kfA = KeyFrame(id1, 10.0f);
		auto kfB = KeyFrame(id1, 4.0f);
		auto kfC = KeyFrame(id1, 2.0f);

		TimeLine timeLine = TimeLine(10000); // ~166 BLOCKS PER SECOND
		timeLine.SetLength(18); // 18ms length
		timeLine.SetSpeed(1);
		timeLine.InsertKeyFrame(kfA);
		timeLine.InsertKeyFrame(kfB);
		timeLine.InsertKeyFrame(kfC);

		// check that equality works properly
		REQUIRE(kfA != kfB);
		REQUIRE(kfB != kfC);
		REQUIRE(kfA != kfC);
		REQUIRE(kfA == kfA);
		REQUIRE(kfB == kfB);
		REQUIRE(kfC == kfC);

		REQUIRE(timeLine.GetFirstKeyFrame(id1) == kfC); // first
		REQUIRE(timeLine.GetLastKeyFrame(id1) == kfA); // last
		REQUIRE(timeLine.GetNextKeyFrame(timeLine.GetFirstKeyFrame(id1)) == kfB); // second
		REQUIRE(timeLine.GetNextKeyFrame(timeLine.GetNextKeyFrame(timeLine.GetFirstKeyFrame(id1))) == kfA); // last

		REQUIRE(timeLine.GetPreviousKeyFrame(timeLine.GetPreviousKeyFrame(timeLine.GetLastKeyFrame(id1))) == kfC); // first
		REQUIRE(timeLine.GetPreviousKeyFrame(timeLine.GetLastKeyFrame(id1)) == kfB); // second

	}

	SECTION("Proper finding")
	{
		auto id1 = StrId(1);

		auto kfA = KeyFrame(id1, 10);
		auto kfB = KeyFrame(id1, 4);
		auto kfC = KeyFrame(id1, 2);

		TimeLine timeLine = TimeLine(3000); // ~166 BLOCKS PER SECOND
		timeLine.SetLength(18); // 18ms length
		timeLine.SetSpeed(1);
		timeLine.InsertKeyFrame(kfA);
		timeLine.InsertKeyFrame(kfB);
		timeLine.InsertKeyFrame(kfC);

		auto foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheLeft(0, foundFrames);

		// no frame found
		REQUIRE(foundFrames.size() == 0);

		foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheLeft(2, foundFrames);
		// first frame found
		REQUIRE(foundFrames.size() == 1);
		REQUIRE(foundFrames.at(0) == kfC);

		foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheRight(3, foundFrames);
		// second frame found
		REQUIRE(foundFrames.size() == 1);
		REQUIRE(foundFrames.at(0) == kfB);

		foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheLeft(5, foundFrames);
		// second frame found
		REQUIRE(foundFrames.size() == 1);
		REQUIRE(foundFrames.at(0) == kfB);

		foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheRight(5, foundFrames);
		// third frame found
		REQUIRE(foundFrames.size() == 1);
		REQUIRE(foundFrames.at(0) == kfA);

		foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheLeft(14, foundFrames);
		// third frame found
		REQUIRE(foundFrames.size() == 1);
		REQUIRE(foundFrames.at(0) == kfA);
	}

	SECTION("Proper finding 2")
	{
		// do the same with higher granularity (more blocks per second)
		auto id1 = StrId(1);

		auto kfA = KeyFrame(id1, 10);
		auto kfB = KeyFrame(id1, 4);
		auto kfC = KeyFrame(id1, 2);

		TimeLine timeLine = TimeLine(300000); 
		timeLine.SetLength(18); // 18ms length
		timeLine.SetSpeed(1);
		timeLine.InsertKeyFrame(kfA);
		timeLine.InsertKeyFrame(kfB);
		timeLine.InsertKeyFrame(kfC);

		auto foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheLeft(0, foundFrames);

		// no frame found
		REQUIRE(foundFrames.size() == 0);

		foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheLeft(2, foundFrames);
		// first frame found
		REQUIRE(foundFrames.size() == 1);
		REQUIRE(foundFrames.at(0) == kfC);

		foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheRight(3, foundFrames);
		// second frame found
		REQUIRE(foundFrames.size() == 1);
		REQUIRE(foundFrames.at(0) == kfB);

		foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheLeft(5, foundFrames);
		// second frame found
		REQUIRE(foundFrames.size() == 1);
		REQUIRE(foundFrames.at(0) == kfB);

		foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheRight(5, foundFrames);
		// third frame found
		REQUIRE(foundFrames.size() == 1);
		REQUIRE(foundFrames.at(0) == kfA);

		foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheLeft(14, foundFrames);
		// third frame found
		REQUIRE(foundFrames.size() == 1);
		REQUIRE(foundFrames.at(0) == kfA);
	}

	SECTION("Proper finding 2")
	{
		// do the same with higher granularity (more blocks per second)
		auto id1 = StrId(1);

		auto kfA = KeyFrame(id1, 10);
		auto kfB = KeyFrame(id1, 4);
		auto kfC = KeyFrame(id1, 2);

		TimeLine timeLine = TimeLine(300000);
		timeLine.SetLength(18); // 18ms length
		timeLine.SetSpeed(1);
		timeLine.InsertKeyFrame(kfA);
		timeLine.InsertKeyFrame(kfB);
		timeLine.InsertKeyFrame(kfC);

		auto foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheLeft(0, foundFrames);

		// no frame found
		REQUIRE(foundFrames.size() == 0);

		foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheLeft(2, foundFrames);
		// first frame found
		REQUIRE(foundFrames.size() == 1);
		REQUIRE(foundFrames.at(0) == kfC);

		foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheRight(3, foundFrames);
		// second frame found
		REQUIRE(foundFrames.size() == 1);
		REQUIRE(foundFrames.at(0) == kfB);

		foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheLeft(5, foundFrames);
		// second frame found
		REQUIRE(foundFrames.size() == 1);
		REQUIRE(foundFrames.at(0) == kfB);

		foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheRight(5, foundFrames);
		// third frame found
		REQUIRE(foundFrames.size() == 1);
		REQUIRE(foundFrames.at(0) == kfA);

		foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheLeft(14, foundFrames);
		// third frame found
		REQUIRE(foundFrames.size() == 1);
		REQUIRE(foundFrames.at(0) == kfA);
	}

	SECTION("More types")
	{
		auto id1 = StrId(1);
		auto id2 = StrId(2);
		auto id3 = StrId(3);

		auto kf1A = KeyFrame(id1, 0);
		auto kf1B = KeyFrame(id1, 4);
		auto kf1C = KeyFrame(id1, 10);
		auto kf1D = KeyFrame(id1, 12);

		auto kf2A = KeyFrame(id2, 1);
		auto kf2B = KeyFrame(id2, 2);
		auto kf2C = KeyFrame(id2, 3);
		auto kf2D = KeyFrame(id2, 4);

		auto kf3A = KeyFrame(id3, 2);
		auto kf3B = KeyFrame(id3, 4);
		auto kf3C = KeyFrame(id3, 20);
		auto kf3D = KeyFrame(id3, 30);

		TimeLine timeLine = TimeLine(300000);
		timeLine.SetLength(18); // 18ms length
		timeLine.SetSpeed(1);

		timeLine.InsertKeyFrame(kf1A);
		timeLine.InsertKeyFrame(kf1B);
		timeLine.InsertKeyFrame(kf1C);
		timeLine.InsertKeyFrame(kf1D);
		timeLine.InsertKeyFrame(kf2A);
		timeLine.InsertKeyFrame(kf2B);
		timeLine.InsertKeyFrame(kf2C);
		timeLine.InsertKeyFrame(kf2D);
		timeLine.InsertKeyFrame(kf3A);
		timeLine.InsertKeyFrame(kf3B);
		timeLine.InsertKeyFrame(kf3C);
		timeLine.InsertKeyFrame(kf3D);

		auto foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheLeft(0, foundFrames);
		REQUIRE(foundFrames.size() == 1);
		REQUIRE(foundFrames.at(0) == kf1A);

		// also from the opposite side
		foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheRight(0, foundFrames);
		REQUIRE(foundFrames.size() == 3);
		REQUIRE(foundFrames.at(0) == kf1A);	

		foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheRight(3, foundFrames);
		REQUIRE(foundFrames.size() == 3);

		// if the time is out of range of the time line, don't return anything
		foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheLeft(35, foundFrames);
		REQUIRE(foundFrames.size() == 0);

		foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheLeft(20, foundFrames);
		REQUIRE(foundFrames.size() == 0);

		foundFrames = vector<KeyFrame>();
		timeLine.GetNearestKeyFramesToTheLeft(18, foundFrames);
		REQUIRE(foundFrames.size() == 3);
	}
}
