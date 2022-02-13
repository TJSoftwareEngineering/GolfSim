//-----------------------------------------------------------------------------
// Pennstate Capstone Project - Golf Simulator - Tim Myotte - 2022
//-----------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using std::cout;
using std::cin;
using std::endl;
using std::cos;
using std::sin;
using std::fstream;
using std::string;
using std::vector;

// classes --------------------------------------------------------------------
// primatives -------
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

// simulator classes ----
class climate {
public:
	float temperature; // deg F
	float gravity = 9.8;
	float airDensity = 1.2;
	float windSpeed = 0.0;// m/s
	float windDirection = 0.0;// degrees 
	climate(float tempIn, float densityIn, float windSpeedIn, float windDirIn){
		temperature = tempIn;
		airDensity = densityIn;
		windSpeed = windSpeedIn;
		windDirection = windDirIn;
	}
};

class club {
public:
	string name;
	float loft = 0.0;
	float smash = 0.0;
	float spinMod = 0.0;
	float attack;
	club(string nameIn, float loftIn, float smashIn, float spinIn, float attackIn) {
		name = nameIn;
		loft = loftIn;
		smash = smashIn;
		spinMod = spinIn;
		attack = attackIn;
	}
	club() {

	}
};
//array to store clubs
vector<club> clubs;

class swing {
public:
	float speed;
	float attackAngle;
	float faceAngle;
	float path;
	float smash;
	float loftMod = 0.0;
	swing(float speedIn, float angleIn, float pathIn, float smashIn) {
		speed = speedIn;
		attackAngle = angleIn;
		path = pathIn;
		smash = smashIn;
	}
};

class ball {
public:
	float radM = .020574;
	float radCM = 2.0574;
	float massKG = .0456;
	float massG = 45.6;
	float area = 3.14159265 * (radM * radM);
	float coLift = .71;//.45
	float coDrag = .55;//.63
	float spin = 0.0;
	float calcSpin(swing swingIn, club clubIn) {
		cout << "angle" << endl;
		cout << clubIn.attack << endl;
		return swingIn.speed*(clubIn.loft+swingIn.loftMod-clubIn.attack)*clubIn.spinMod;
	}
};

class shot {
public:
	point path3D[1000];
	float maxHeight = 0.0;
	float distance = 0.0;
	float curve = 0.0;
	float time = 0.0;
};

// functions ---------------------------------------------------------------------------

void loadClubData() {
	cout << "loading club data..." << endl;
	std::ifstream fin;
	fin.open("clubdata.csv");

	if (fin) {
		cout << "file exists" << endl;

		vector<string> row;
		string line, seg, temp, name;
		float loft, smash, spin, attack;


		while (fin.good()) {
			getline(fin, line, ',');
			name = line;
			getline(fin, line, ',');
			loft = std::stof(line);
			getline(fin, line, ',');
			smash = std::stof(line);
			getline(fin, line, ',');
			spin = std::stof(line);
			getline(fin, line, '\n');
			attack = std::stof(line);
			cout << name << " " << loft << " " << smash << " " << spin << " " << endl;
			clubs.emplace_back(club(name, loft, smash, spin, attack));



		}
	}
	else {
		cout << "file doesn't exist" << endl;
	}




}

//Basic simulation - No drag/lift 
shot simulateBasic(shot shotIn, swing swingIn, club clubIn, climate climateIn) {
	float degToRad = 3.1415926 / 180.0;
	float velocityInit = swingIn.speed*clubIn.smash*.44704;
	float angleInit = (clubIn.loft-swingIn.loftMod+swingIn.attackAngle)*degToRad;
	cout << "Initial Velocity: " << velocityInit << " m/s" << endl;
	cout << "Initial angle: " << clubIn.loft-swingIn.loftMod+swingIn.attackAngle << " deg (" << angleInit << " rad)" << endl;
	cout << "Position" << endl;
	cout << "  x        y" << endl;
	cout << "-----------------------" << endl;
	//basic projectile motion - step by 1/10 of second until hits ground
	float tempX = 0.0, tempY = 0.0, time = 0.0, heightMax = 0.0;
	int count = 0;
	while(tempY >= 0.0){
		tempX = velocityInit * time * cos(angleInit);
		tempY = velocityInit * time * sin(angleInit) - .5*climateIn.gravity*(time*time);
		shotIn.path3D[count] = point(0.0, tempY, tempX);
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


//Full simulation
shot simulate(shot shotIn, swing swingIn, club clubIn, ball ballIn, climate climateIn) {
	//Calculate launch angle and speed
	float degToRad = 3.1415926 / 180.0;
	float velocityInit = swingIn.speed * clubIn.smash * .44704;
	//float angleInit = (clubIn.loft - swingIn.loftMod + swingIn.attackAngle) * degToRad;
	float angleInit = (cos(clubIn.loft * degToRad) * clubIn.loft + clubIn.attack * 2) * degToRad;

	float spinRPM = ballIn.calcSpin(swingIn, clubIn);
	cout << spinRPM << endl;

	//output info to console
	cout << "Initial Velocity: " << velocityInit << " m/s" << endl;
	cout << "Initial angle: " << cos(clubIn.loft * degToRad) * clubIn.loft + clubIn.attack * 2 << " deg (" << angleInit << " rad)" << endl;
	//cout << "Position" << endl;
	//cout << "  x        y" << endl;
	//cout << "-----------------------" << endl;

	//Temporary variables
	float tempX = 0.0, tempY = 0.0, time = 0.0, heightMax = 0.0, xChange = 0.0, yChange = 0.0;
	float vSquare = 0.0, vLength = 0.0, gravForce = 0.0, dragForce = 0.0, liftForce = 0.0;
	float yVelocity = 0.0, xVelocity = 0.0;

	//controls time interval
	float timeDelta = .1;
	int count = 0;

	//calculate initial velocity in x and y directions
	xVelocity = velocityInit * cos(angleInit);
	yVelocity= velocityInit * sin(angleInit);
	while (tempY >= 0.0) {

		//update position
		tempX += xVelocity * timeDelta;
		tempY += yVelocity * timeDelta;


		vSquare = xVelocity * xVelocity + yVelocity * yVelocity;
		vLength = sqrt(vSquare);

		gravForce = climateIn.gravity * ballIn.massKG;
		liftForce = (.5 * climateIn.airDensity * ballIn.area * (ballIn.coLift + (spinRPM / 40000.0)))/ ballIn.massKG * (vLength/(1.0+count/10.0));
		dragForce = (.5 * climateIn.airDensity * ballIn.area * (ballIn.coDrag + (spinRPM / 50000.0)))/ ballIn.massKG * vLength;
		

		//Calculate velocity change
		xChange = (-dragForce * xVelocity / vLength);
		yChange = (-dragForce * yVelocity / vLength) - gravForce +liftForce;

		//calculate new velocities
		xVelocity += xChange * timeDelta / ballIn.massKG;
		yVelocity += yChange * timeDelta / ballIn.massKG;

		//store ball position
		shotIn.path3D[count] = point(0.0, tempY, tempX);
		//step time interval
		time += timeDelta;

		count++;
		if (tempY > heightMax) {
			heightMax = tempY;
		}
		//cout << tempX << "," << tempY << endl;
	}

	//create and return stats
	float strikeDist = tempX;
	//cout << "strike distance: " << strikeDist << endl;
	shotIn.maxHeight = heightMax;
	shotIn.time = time;
	shotIn.distance = strikeDist;

	return shotIn;
}

// main --------------------------------------------------------------------------------

int main() {

	loadClubData();

	shot shot1, shot2;
	climate climate1 = climate(75.0, 1.2, 0.0, 0.0);
	vector<swing> swings;
	swings.emplace_back(swing(113.0, 0.0, 0.0, 1.48));
	swings.emplace_back(swing(107.0, 0.0, 0.0, 1.48));
	swings.emplace_back(swing(103.0, 0.0, 0.0, 1.48));
	swings.emplace_back(swing(101.0, 0.0, 0.0, 1.48));
	swings.emplace_back(swing(100.0, 0.0, 0.0, 1.48));
	swings.emplace_back(swing(98.0, 0.0, 0.0, 1.48));
	swings.emplace_back(swing(96.0, 0.0, 0.0, 1.48));
	swings.emplace_back(swing(94.0, 0.0, 0.0, 1.48));
	swings.emplace_back(swing(92.0, 0.0, 0.0, 1.48));
	swings.emplace_back(swing(90.0, 0.0, 0.0, 1.48));
	swings.emplace_back(swing(87.0, 0.0, 0.0, 1.48));
	swings.emplace_back(swing(85.0, 0.0, 0.0, 1.48));
	swings.emplace_back(swing(83.0, 0.0, 0.0, 1.48));
	//club club1(clubs[12]);
	ball ball1;

	//shot1 = simulateBasic(shot1, swing1, club1, climate1);
	//shot2 = simulate(shot1, swing1, club1, ball1, climate1);

	//temporary stat display

	for (int i = 0; i < 13;i++) {
		shot2 = simulate(shot1, swings[i], clubs[i], ball1, climate1);
		cout << "---------- " << clubs[i].name << " ------------ " << endl;
		cout << "Flight time: " << shot2.time << " seconds" << endl;
		cout << "Distance: " << shot2.distance << " m" << endl;
		cout << "Max Height: " << shot2.maxHeight << " m" << endl;
	}


	system("pause>0");
}

