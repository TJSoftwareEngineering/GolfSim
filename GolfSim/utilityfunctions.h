#pragma once
//utility functions

//Unit Conversion
float metersToYards(float m) {
	float y = m * 1.093613;
	return y;
}

//Input Sanitization
swing sanitizeSwing(swing swingIn) {
	//club speed
	if (swingIn.clubSpeed < 0.0F) swingIn.clubSpeed = 0.0f;
	if (swingIn.clubSpeed > 150.0F) swingIn.clubSpeed = 150.0f;
	//smash factor
	if (swingIn.smash < 0.0F) swingIn.smash = 0.0f;
	if (swingIn.smash > 1.5F) swingIn.smash = 1.5f;
	//launch angle
	if (swingIn.launchAngle < 0.0F) swingIn.launchAngle = 0.0f;
	if (swingIn.launchAngle > 90.0F) swingIn.launchAngle = 90.0f;
	//spin
	if (swingIn.spin < 0.0F) swingIn.spin = 0.0f;
	if (swingIn.spin > 15000.0F) swingIn.spin = 15000.0f;
	//spin axis
	if (swingIn.spinAxis < -20.0F) swingIn.spinAxis = -20.0f;
	if (swingIn.spinAxis > 20.0F) swingIn.spinAxis = 20.0f;
	return swingIn;
}

climate sanitizeClimate(climate climateIn) {
	//temperature
	if (climateIn.temperature < 0.0F) climateIn.temperature = 0.0f;
	if (climateIn.temperature > 120.0F) climateIn.temperature = 120.0f;
	//altitude
	if (climateIn.altitude < 0.0F) climateIn.altitude = 0.0f;
	if (climateIn.altitude > 15000.0F) climateIn.altitude = 15000.0f;
	//wind speed
	if (climateIn.windSpeed < 0.0F) climateIn.windSpeed = 0.0f;
	if (climateIn.windSpeed > 20.0F) climateIn.windSpeed = 20.0f;
	//wind dir
	if (climateIn.windDirection < 0.0F) climateIn.windDirection = 0.0f;
	if (climateIn.windDirection > 360.0F) climateIn.windDirection = 360.0f;
	return climateIn;
}