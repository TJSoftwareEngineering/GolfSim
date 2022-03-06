#pragma once
class club {
public:
	string name;
	float speed = 0.0;
	float launch = 0.0;
	float smash = 0.0;
	float spin = 0.0;
	float path = 0.0;
	float attack = 0.0;
	club(string nameIn, float speedIn, float smashIn, float launchIn, float attackIn, float pathIn, float spinIn) {
		name = nameIn;
		speed = speedIn;
		smash = smashIn;
		launch = launchIn;
		attack = attackIn;
		path = pathIn;
		spin = spinIn;
		
	}
	club() {

	}

};