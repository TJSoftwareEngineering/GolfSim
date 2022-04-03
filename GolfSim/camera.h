#pragma once

class camera {
public:
	float x = 0.0, y = 0.0, z = 0.0;
	float angleX = 0.0, angleY = 0.0, angleZ = 0.0;
	camera(float xIn, float yIn, float zIn, float angleXIn) {
		x = xIn;
		y = yIn;
		z = zIn;
		angleX = angleXIn;
	}
};