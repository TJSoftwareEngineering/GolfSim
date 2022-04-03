#pragma once
class ball {
public:
	float radM = .020574;
	float radCM = 2.0574;
	float massKG = .0456;
	float massG = 45.6;
	float area = 3.14159265 * (radM * radM);
	float coLift = .54;
	float coDrag = .48;
	float coRestY = .34;
	float coRestX = .87;
	float spin = 0.0;
	float calcSpin(swing swingIn, club clubIn) {
		//placeholder, removed
	}
};