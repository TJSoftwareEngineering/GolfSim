//-----------------------------------------------------------------------------
// Pennstate Capstone Project - Golf Simulator - Tim Myotte - 2022
//-----------------------------------------------------------------------------
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
using std::cos;
using std::sin;

// classes --------------------------------------------------------------------
// primatives -------
class vector {
public:
	float x, y, z;
	vector(float xIn, float yIn, float zIn) {
		x = xIn;
		y = yIn;
		z = zIn;
	}
	vector() {
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}
};

// simulator classes ----
class climate {
public:
	float temperature;
	float gravity = 9.8;
	climate(float temp){
		temperature = temp;
	}
};

class club {
public:
	float angle;
	club(float angleIn) {
		angle = angleIn;
	}
};

class swing {
public:
	float speed;
	float angle;
	float path;
	float smash;
	swing(float speedIn, float angleIn, float pathIn, float smashIn) {
		speed = speedIn;
		angle = angleIn;
		path = pathIn;
		smash = smashIn;
	}
};

class shot {
public:
	vector path3D[100];
	float maxHeight = 0;
	float distance = 0;
	float curve = 0;
	float time = 0;
};

// functions ---------------------------------------------------------------------------
shot simulate(shot shotIn, swing swingIn, club clubIn, climate climateIn) {
	float degToRad = 3.1415926 / 180;
	float velocityInit = swingIn.speed*swingIn.smash*.44704;
	float angleInit = (clubIn.angle+swingIn.angle)*degToRad;
	cout << "Initial Velocity: " << velocityInit << " m/s" << endl;
	cout << "Initial angle: " << clubIn.angle+swingIn.angle << " deg (" << angleInit << " rad)" << endl;
	cout << "Position" << endl;
	cout << "  x        y" << endl;
	cout << "-----------------------" << endl;
	//basic projectile motion - step by 1/10 of second until hits ground
	float tempX = 0.0, tempY = 0.0, time = 0.0, heightMax = 0.0;
	int count = 0;
	while(tempY >= 0.0){
		tempX = velocityInit * time * cos(angleInit);
		tempY = velocityInit * time * sin(angleInit) - .5*climateIn.gravity*(time*time);
		shotIn.path3D[count] = vector(0.0, tempY, tempX);
		time += .1;
		count++;
		if (tempY > heightMax) {
			heightMax = tempY;
		}
		cout << tempX << "   " << tempY<<endl;
	}

	//create and return stats
	float strikeDist = (velocityInit * velocityInit) / climateIn.gravity * sin(2 * angleInit);
	cout << "strike distance: " << strikeDist << endl;
	shotIn.maxHeight = heightMax;
	shotIn.time = time;
	shotIn.distance = strikeDist;
	
	return shotIn;
}

// main --------------------------------------------------------------------------------

int main() {
	shot shot1;
	climate climate1 = climate(75.0);
	swing swing1(90.0, 2.0, 0.0, 1.5);
	club club1(12.0);

	shot1 = simulate(shot1, swing1, club1, climate1);

	//temporary stat display
	cout << "-----------------------" << endl;
	cout << "Flight time: " << shot1.time << " seconds" << endl;
	cout << "Distance: " << shot1.distance << " m" << endl;
	cout << "Max Height: " << shot1.maxHeight << " m" << endl;
	system("pause>0");
}

