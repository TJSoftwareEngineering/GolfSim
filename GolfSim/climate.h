#pragma once
class climate {
public:
	float temperature = 100.0; // deg F
	float gravity = 9.8;
	float altitude = 0.0;
	float airDensity = 1.2;
	float windSpeed = 0.0;// m/s
	float windDirection = 0.0;// degrees 
	climate(float tempIn, float altitudeIn, float windSpeedIn, float windDirIn) {
		temperature = tempIn;
		altitude = altitudeIn;
		windSpeed = windSpeedIn;
		windDirection = windDirIn;
	}

};