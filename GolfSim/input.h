#pragma once

bool input = false;
bool left = false, right = false, up = false, down = false, forward = false, backward = false ;
const float moveSpeed = .25;
void update() {
	if (left) cam.x -= moveSpeed;
	if (right) cam.x += moveSpeed;
	if (up) cam.y += moveSpeed;
	if (down) cam.y -= moveSpeed;
	if (forward) cam.z += moveSpeed;
	if (backward) cam.z -= moveSpeed;

	if (left || right || up || down || forward || backward) input = true; else input = false;
}