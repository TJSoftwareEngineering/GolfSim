#pragma once
class swing {
public:
	float clubSpeed;
	float ballSpeed;
	float attackAngle;
	float faceAngle;
	float spinAxis;
	float smash;
	float loftMod = 0.0;
	float launchAngle = 0.0;
	float spin = 0.0;

	swing(float speedIn, float smashIn, float launchIn, float angleIn, float spinAxisIn, float spinIn) {
		clubSpeed = speedIn;
		attackAngle = angleIn;
		launchAngle = launchIn;
		spinAxis = spinAxisIn;
		smash = smashIn;
		spin = spinIn;
	}

	void calcBallSpeed() {
		ballSpeed = clubSpeed * smash;
	}
};