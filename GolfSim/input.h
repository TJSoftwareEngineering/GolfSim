#pragma once

bool input = false;
bool left = false, right = false, up = false, down = false, forward = false, backward = false ;
bool rotLeft = false, rotRight = false;
const float moveSpeed = 50.0;
const float rotSpeed = 50.0;

float delta;
void update() {

	end = clock();
	delta = end - start;
	start = clock();

	

	if (left) {
		cam.x -= sin((cam.angleX + 90.0) * degToRad) * moveSpeed * (delta / 1000.0);
		cam.z -= cos((cam.angleX - 90.0) * degToRad) * moveSpeed * (delta / 1000.0);
	}
	if (right) {
		cam.x += sin((cam.angleX + 90.0) * degToRad) * moveSpeed * (delta / 1000.0);
		cam.z += cos((cam.angleX - 90.0) * degToRad) * moveSpeed * (delta / 1000.0);
	}


	if (up) cam.y += moveSpeed * (delta / 1000.0);
	if (down) cam.y -= moveSpeed * (delta / 1000.0);


	if (forward) {
		cam.x -= sin(cam.angleX * degToRad) * moveSpeed * (delta / 1000.0);
		cam.z += cos(cam.angleX * degToRad) * moveSpeed * (delta / 1000.0);
	}

	if (backward) {
		cam.x += sin(cam.angleX * degToRad) * moveSpeed * (delta / 1000.0);
		cam.z -= cos(cam.angleX * degToRad) * moveSpeed * (delta / 1000.0);
	}

	if (rotLeft) cam.angleX += rotSpeed * (delta / 1000.0);
	if (rotRight) cam.angleX -= rotSpeed * (delta / 1000.0);

	if (left || right || up || down || forward || backward) input = true; else input = false;
}