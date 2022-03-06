#pragma once
//Load club data from file
vector<club> clubs;
vector<string> titles;
void loadClubData() {
	cout << "loading club data..." << endl;
	std::ifstream fin;
	fin.open("clubdata.csv");

	if (fin) {
		cout << "file exists" << endl;

		vector<string> row;
		string line, seg, temp, name;
		float speed, launch, smash, spin, attack, path;


		while (fin.good()) {
			getline(fin, line, ',');
			name = line;
			getline(fin, line, ',');
			speed = std::stof(line);
			getline(fin, line, ',');
			smash = std::stof(line);
			getline(fin, line, ',');
			launch = std::stof(line);
			getline(fin, line, ',');
			attack = std::stof(line);
			getline(fin, line, ',');
			path = std::stof(line);
			getline(fin, line, '\n');
			spin = std::stof(line);
			//cout << name << " " << loft << " " << smash << " " << spin << " " << endl;
			clubs.emplace_back(club(name, speed, smash, launch, attack, path, spin));
			titles.emplace_back(name);
		}
	}
	else {
		cout << "file doesn't exist" << endl;
	}




}