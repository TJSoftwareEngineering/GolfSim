#pragma once
//Full simulation
shot simulate(shot shotIn, swing swingIn, club clubIn, ball ballIn, climate climateIn) {
	//Calculate launch angle and speed
	swingIn.calcBallSpeed();
	float degToRad = 3.1415926 / 180.0;
	float velocityInit = swingIn.ballSpeed * .44704;
	shotIn.ballSpeed = swingIn.ballSpeed;
	//float angleInitDeg = (cos(clubIn.loft * degToRad) * clubIn.loft + clubIn.attack*2.0);
	float angleInitDeg = swingIn.launchAngle;
	float angleInit = angleInitDeg * degToRad;

	float spinRPM = swingIn.spin;
	float coLift = ballIn.coLift + spinRPM / 40000.0;
	float coDrag = ballIn.coDrag + spinRPM / 60000.0;
	float coTemp = (climateIn.temperature - 100.0) * .001 + 1;
	float coAlt = 1.0 + climateIn.altitude * .0000116;


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
	shotIn.path3D.emplace_back(point(0.0, 0.0, 0.0));

	while (zVelocity > 0.0) {

		//update position
		oldX = tempX;
		oldY = tempY;
		oldZ = tempZ;
		tempZ += zVelocity * timeDelta * coTemp * coAlt;
		tempY += yVelocity * timeDelta;
		tempX += xVelocity * timeDelta;

		//if no movement set ball to rest
		if (oldZ == tempZ) {
			zVelocity = 0.0;
		}

		//Bounce if impacting ground
		if (tempY <= 0.0) {
			bounce = true;
			tempZ = oldZ + (tempZ - oldZ) * abs((0.0 - oldY) / (tempY - oldY + .0001));
			tempY = 0.0;
			yVelocity = -yVelocity * ballIn.coRestY;
			zVelocity = zVelocity * (ballIn.coRestX - (swingIn.spin / 15000.0)) - .1 + windForceZ;
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
		windForceX = .5 * climateIn.windSpeed * coDrag * cos(climateIn.windDirection * degToRad);
		windForceZ = .5 * climateIn.windSpeed * coDrag * sin(climateIn.windDirection * degToRad);
		spinForceX = sin(swingIn.spinAxis * degToRad) * 18.0;
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
		zVelocity += zChange * timeDelta / ballIn.massKG + (windForceZ * timeDelta);
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
		cout << tempZ << "," << tempY << "," << tempX << endl;
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