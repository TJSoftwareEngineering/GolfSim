#pragma once
class shot {
public:
	vector<point> path3D;
	float ballSpeed = 0.0;
	float maxHeight = 0.0;
	float distance = 0.0;
	float curveInAir = 0.0;
	float curveTotal = 0.0;
	float timeInAir = 0.0;
	float timeTotal = 0.0;
	float carryDist = 0.0;
	float totalDist = 0.0;
};