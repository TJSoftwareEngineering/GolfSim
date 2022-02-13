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

#include <SFML/Graphics.hpp>

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
	float temperature = 100.0; // deg F
	float gravity = 9.8;
	float airDensity = 1.2;
	float windSpeed = 0.0;// m/s
	float windDirection = 0.0;// degrees 
	climate(float tempIn, float densityIn, float windSpeedIn, float windDirIn) {
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

		ballSpeed = clubSpeed * smash;
	}
};

class ball {
public:
	float radM = .020574;
	float radCM = 2.0574;
	float massKG = .0456;
	float massG = 45.6;
	float area = 3.14159265 * (radM * radM);
	float coLift = .54;//.45
	float coDrag = .48;//.63
	float coRestY = .34;
	float coRestX = .87;
	float spin = 0.0;
	float calcSpin(swing swingIn, club clubIn) {
		cout << "angle" << endl;
		cout << clubIn.attack << endl;
		return swingIn.clubSpeed * (clubIn.loft + swingIn.loftMod - clubIn.attack) * clubIn.spinMod;
	}
};

class shot {
public:
	vector<point> path3D;
	float maxHeight = 0.0;
	float distance = 0.0;
	float curveInAir = 0.0;
	float curveTotal = 0.0;
	float timeInAir = 0.0;
	float timeTotal = 0.0;
	float carryDist = 0.0;
	float totalDist = 0.0;
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
		float loft, smash, spin, attack, loftMod;


		while (fin.good()) {
			getline(fin, line, ',');
			name = line;
			getline(fin, line, ',');
			loft = std::stof(line);
			getline(fin, line, ',');
			smash = std::stof(line);
			getline(fin, line, ',');
			spin = std::stof(line);
			getline(fin, line, ',');
			attack = std::stof(line);
			getline(fin, line, '\n');
			loftMod = std::stof(line);
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
	float velocityInit = swingIn.ballSpeed * .44704;
	float angleInit = (clubIn.loft - swingIn.loftMod + swingIn.attackAngle) * degToRad;
	cout << "Initial Velocity: " << velocityInit << " m/s" << endl;
	cout << "Initial angle: " << clubIn.loft - swingIn.loftMod + swingIn.attackAngle << " deg (" << angleInit << " rad)" << endl;
	cout << "Position" << endl;
	cout << "  x        y" << endl;
	cout << "-----------------------" << endl;
	//basic projectile motion - step by 1/10 of second until hits ground
	float tempX = 0.0, tempY = 0.0, time = 0.0, heightMax = 0.0;
	int count = 0;
	while (tempY >= 0.0) {
		tempX = velocityInit * time * cos(angleInit);
		tempY = velocityInit * time * sin(angleInit) - .5 * climateIn.gravity * (time * time);
		shotIn.path3D[count] = point(0.0, tempY, tempX);
		time += .1;
		count++;
		if (tempY > heightMax) {
			heightMax = tempY;
		}
		cout << tempX << "   " << tempY << endl;
	}

	//create and return stats
	float strikeDist = (velocityInit * velocityInit) / climateIn.gravity * sin(2 * angleInit);
	cout << "strike distance: " << strikeDist << endl;
	shotIn.maxHeight = heightMax;
	shotIn.timeTotal = time;
	shotIn.distance = strikeDist;

	return shotIn;
}


//Full simulation
shot simulate(shot shotIn, swing swingIn, club clubIn, ball ballIn, climate climateIn) {
	//Calculate launch angle and speed
	float degToRad = 3.1415926 / 180.0;
	float velocityInit = swingIn.ballSpeed * .44704;
	//float angleInitDeg = (cos(clubIn.loft * degToRad) * clubIn.loft + clubIn.attack*2.0);
	float angleInitDeg = swingIn.launchAngle;
	float angleInit = angleInitDeg * degToRad;

	float spinRPM = swingIn.spin;
	float coLift = ballIn.coLift + spinRPM / 40000.0;
	float coDrag = ballIn.coDrag + spinRPM / 60000.0;
	float coTemp = (climateIn.temperature - 100.0) * .001 + 1;


	//Temporary variables
	float tempX = 0.0, tempY = 0.0, tempZ = 0.0, time = 0.0, heightMax = 0.0, xChange = 0.0, yChange = 0.0, zChange = 0;
	float vSquare = 0.0, vLength = 0.0, gravForce = 0.0, dragForce = 0.0, liftForce = 0.0;
	float yVelocity = 0.0, xVelocity = 0.0, zVelocity = 0.0;
	float oldX, oldY, oldZ, carryDist = 0.0, totalDist;
	float windForceX = 0.0, windForceZ = 0.0, spinForceX = 0.0;
	float groundDrag = 0.0;
	bool bounce = false;

	//controls time interval
	float timeDelta = .1;
	int count = 0;

	//calculate initial velocity in x and y directions
	zVelocity = velocityInit * cos(angleInit);
	yVelocity = velocityInit * sin(angleInit);

	//push start position to path
	shotIn.path3D.emplace_back(point(0.0,0.0,0.0));

	while (zVelocity > 0.0) {

		//update position
		oldX = tempX;
		oldY = tempY;
		oldZ = tempZ;
		tempZ += zVelocity * timeDelta * coTemp;
		tempY += yVelocity * timeDelta;
		tempX += xVelocity * timeDelta;


		//Bounce if impacting ground
		if (tempY <= 0.0) {
			bounce = true;
			tempZ = oldZ + (tempZ - oldZ) * abs((0.0 - oldY) / (tempY - oldY +.0001));
			tempY = 0.0;
			yVelocity = -yVelocity * ballIn.coRestY;
			zVelocity = zVelocity * (ballIn.coRestX - (swingIn.spin/15000.0)) - .1 + windForceZ;
			//save distance, curve, time of first impact
			if (carryDist == 0.0) {
				carryDist = tempZ;
				shotIn.curveInAir = tempX;
				shotIn.timeInAir = time;
			}
		}

		//if no movement set ball to rest
		if (oldZ == tempZ) {
			zVelocity = 0.0;
		}

		vSquare = zVelocity * zVelocity + yVelocity * yVelocity;
		vLength = sqrt(vSquare);

		gravForce = climateIn.gravity * ballIn.massKG;
		liftForce = (.5 * climateIn.airDensity * ballIn.area * coLift) / ballIn.massKG * (vLength / (1.0 + count / 10.0));
		dragForce = (.5 * climateIn.airDensity * ballIn.area * coDrag) / ballIn.massKG * vLength;
		windForceX = .5 * climateIn.windSpeed * coDrag * cos(climateIn.windDirection*degToRad);
		windForceZ = .5 * climateIn.windSpeed * coDrag * sin(climateIn.windDirection*degToRad);
		spinForceX = sin(swingIn.spinAxis*degToRad)*18.0;
		//cout << spinForceX << endl;

		if (bounce == true) {
			windForceX *= .5;
			spinForceX *= .5;
			windForceZ = 0.0;
		}

		//cout << windForceX << "," << windForceZ << endl;


		//Calculate velocity change
		zChange = (-dragForce * zVelocity / vLength);
		yChange = (-dragForce * yVelocity / vLength) - gravForce + liftForce;

		//calculate new velocities
		zVelocity += zChange * timeDelta / ballIn.massKG + (windForceZ*timeDelta);
		yVelocity += yChange * timeDelta / ballIn.massKG;
		xVelocity = windForceX + spinForceX;

		//cout << xVelocity << endl;

		//store ball position
		shotIn.path3D.emplace_back(point(tempX, tempY, tempZ));
		//step time interval
		time += timeDelta;

		count++;
		if (tempY > heightMax) {
			heightMax = tempY;
		}
		cout << tempZ << "," << tempY <<"," << tempX << endl;
	}

	//create and return stats
	
	totalDist = tempZ;
	//cout << "strike distance: " << strikeDist << endl;
	shotIn.curveTotal = tempX;
	shotIn.maxHeight = heightMax;
	shotIn.timeTotal = time;
	shotIn.carryDist = carryDist;
	shotIn.totalDist = totalDist;

	return shotIn;
}

// main --------------------------------------------------------------------------------

int main() {

	sf::RenderWindow window(sf::VideoMode(800, 600), "Window Title");
	sf::CircleShape shape(150.0, 100);
	shape.setFillColor(sf::Color(80, 135, 30)); // Color circle
	shape.setPosition(200, 100); // Center circle


	

	while (window.isOpen()){
		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear(sf::Color(30, 40, 30)); // Color background
		window.draw(shape);
		window.display();
	}

	loadClubData();

	shot shot1, shot2;
	climate climate1 = climate(100.0, 1.2, 0.0, -45.0);
	vector<swing> swings;

	// swing(float speedIn, float smashIn, float launchIn, float angleIn, float pathIn, float spinIn)
	//driver
	swings.emplace_back(swing(113.0, 1.48, 10.9, -1.3, 0.0, 2686.0));
	//woods
	swings.emplace_back(swing(107.0, 1.48, 9.2, -2.9, 0.0, 3655.0));
	swings.emplace_back(swing(103.0, 1.47, 9.4, -3.3, 0.0, 4350.0));
	swings.emplace_back(swing(101.0, 1.46, 10.0, -3.4, 0.0, 4400.0));
	//hybrid
	swings.emplace_back(swing(100.0, 1.46, 10.2, -3.5, 0.0, 4437.0));
	//irons
	swings.emplace_back(swing(98.0, 1.45, 10.4, -3.1, 0.0, 4630.0));
	swings.emplace_back(swing(96.0, 1.43, 11.0, -3.4, 10.0, 4836.0));
	swings.emplace_back(swing(94.0, 1.41, 12.1, -3.7, 0.0, 5361.0));
	swings.emplace_back(swing(92.0, 1.38, 14.1, -4.1, 0.0, 6231.0));
	swings.emplace_back(swing(90.0, 1.33, 16.3, -4.3, 0.0, 7097.0));
	swings.emplace_back(swing(87.0, 1.32, 18.1, -4.5, 0.0, 7998.0));
	swings.emplace_back(swing(85.0, 1.28, 20.4, -4.7, 0.0, 8647.0));
	swings.emplace_back(swing(83.0, 1.23, 24.2, -5.0, 2.0, 9304.0));

	ball ball1;


	//temporary stat display

	for (int i = 0; i < 13;i++) {
		shot2 = simulate(shot1, swings[i], clubs[i], ball1, climate1);
		cout << "---------- " << clubs[i].name << " ------------ " << endl;
		cout << "Ball speed: " << swings[i].ballSpeed << " m/s" << endl;
		cout << "launch Angle: " << swings[i].launchAngle << " deg" << endl;
		cout << "Spin: " << swings[i].spin << " rpm" << endl;
		cout << "Flight time: " << shot2.timeInAir << " seconds" << endl;
		cout << "Total time: " << shot2.timeTotal << " seconds" << endl;
		cout << "Carry Distance: " << shot2.carryDist << " m" << endl;
		cout << "Total Distance: " << shot2.totalDist << " m" << endl;
		cout << "Max Height: " << shot2.maxHeight << " m" << endl;
		cout << "Curve in air: " << shot2.curveInAir << " m" << endl;
		cout << "Total curve: " << shot2.curveTotal << " m" << endl;
	}


	system("pause>0");
}
