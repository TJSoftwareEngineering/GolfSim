#pragma once

class point {
public:
	float x, y, z;
	point(float xIn, float yIn, float zIn) {
		x = xIn;
		y = yIn;
		z = zIn;
	}
	point() {
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}
};