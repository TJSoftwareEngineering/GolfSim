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
#include <imgui.h>
#include <imgui-SFML.h>

#include "point.h"
#include "climate.h"
#include "club.h"
#include "swing.h"
#include "ball.h"
#include "shot.h"
#include "simulator.h"
#include "loader.h"
#include "utilityfunctions.h"


// main window loop --------------------------------------------------------------------------------

int main() {

	loadClubData();

	static const char* clubName[]{"Driver","3 Wood","5 Wood","7 Wood", "hybrid","3 Iron","4 Iron","5 Iron" ,"6 Iron" ,"7 Iron" ,"8 Iron" ,"9 Iron", "PW"};

	shot shot1, shot2, lastShot;
	climate climate1 = climate(100.0, 0.0, 0.0, 45.0);
	swing swing1 = swing(120.0, 1.48, 12.0, 0.0, -3.0, 2300.0);
	ball ball1;
	vector<swing> swings;

	float unitConvert = 1.0;
	bool isYards = false;
	int selectedClub = 0;
	int selectedClubNew = 0;


	//Create window
	sf::RenderWindow window(sf::VideoMode(1280, 768), "Golf Simulator - Pennstate Capstone");
	ImGui::SFML::Init(window);
	sf::Clock deltaClock;

	//test shape
	//sf::CircleShape shape(150.0, 100);
	//shape.setFillColor(sf::Color(80, 135, 30)); // Color circle
	//shape.setPosition(200, 100); // Center circle

	// Window render loop
	while (window.isOpen()) {

		//update
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());
		//swing window
		ImGui::Begin("Swing Data");

		ImGui::Combo("Load default club data", &selectedClub, clubName, IM_ARRAYSIZE(clubName));

		ImGui::Text("\nSwing Variables:");
		ImGui::SliderFloat("Swing Speed (mph)", &swing1.clubSpeed, 0.0F, 150.0F);
		ImGui::SliderFloat("Smash Factor", &swing1.smash, 0.0F, 1.5F);
		ImGui::SliderFloat("Launch angle (deg)", &swing1.launchAngle, 0.0F, 89.0F);
		ImGui::SliderFloat("Spin (rpm)", &swing1.spin, 0.0F, 15000.0F);
		ImGui::SliderFloat("Spin axis (deg)", &swing1.spinAxis, -20.0F, 20.0F);
		ImGui::Text("\n");
		ImGui::Separator();
		ImGui::Text("\nClimate Variables:");
		ImGui::BulletText("Sections below are demonstrating many aspects of the library.");
		ImGui::SliderFloat("Temperature (f)", &climate1.temperature, 0.0F, 120.0F);
		ImGui::SliderFloat("Altitude (ft)", &climate1.altitude, 0.0F, 15000.0F);
		ImGui::SliderFloat("Wind Speed (m/s)", &climate1.windSpeed, 0.0F, 20.0F);
		ImGui::SliderFloat("Wind Direction (deg)", &climate1.windDirection, 0.0F, 360.0F);
		ImGui::Text("\n");
		ImGui::Separator();
		ImGui::Text("\n");
		if (ImGui::Button("Simulate")) {
			lastShot = simulate(lastShot, swing1, clubs[0], ball1, climate1);
		}
		ImGui::End();
		//climate window

		//Stat window
		ImGui::Begin("Stats");
		ImGui::Text("Distances:\n");

		ImGui::Text("Carry Distance: %f", lastShot.carryDist*unitConvert);
		ImGui::Text("Total Distance: %f", lastShot.totalDist*unitConvert);
		
		ImGui::Text("\n");
		ImGui::Separator();
		ImGui::Text("Wind and Spin:\n");
		ImGui::Text("Curve (in air): %f", lastShot.curveInAir*unitConvert);
		ImGui::Text("Curve total: %f", lastShot.curveTotal*unitConvert);
		
		ImGui::Text("\n");
		ImGui::Separator();
		ImGui::Text("Trajectory:\n");
		ImGui::Text("Max Height: %f", lastShot.maxHeight*unitConvert);
		ImGui::Text("\n");
		ImGui::Separator();

		ImGui::Text("Time:\n");
		ImGui::Text("Flight time (s): %f", lastShot.timeInAir*unitConvert);
		ImGui::Text("Total time (s): %f", lastShot.timeTotal*unitConvert);

		ImGui::Checkbox("Yards", &isYards);
		ImGui::End();


		//see is yards checkbox is checked
		if (isYards == true) {
			unitConvert = 1.093613;
		}
		else {
			unitConvert = 1.0;
		}
		//check for change in club selection
		if (selectedClubNew != selectedClub) {
			cout << "CLUB CHANGE" <<endl;
			selectedClubNew = selectedClub;
			swing1.clubSpeed = clubs[selectedClub].speed;
			swing1.calcBallSpeed();
			swing1.launchAngle = clubs[selectedClub].launch;
			swing1.smash = clubs[selectedClub].smash;
			swing1.spin = clubs[selectedClub].spin;
			swing1.spinAxis = 0.0;
		}

		//clear and draw

		window.clear(sf::Color(20, 60, 70)); // Color background
		//window.draw(shape);
		ImGui::SFML::Render(window);
		window.display();
	}



	// swing(float speedIn, float smashIn, float launchIn, float angleIn, float pathIn, float spinIn)
	//driver
	//swings.emplace_back(swing(113.0, 1.48, 10.9, -1.3, 0.0, 2686.0));
	//woods
	//swings.emplace_back(swing(107.0, 1.48, 9.2, -2.9, 0.0, 3655.0));
	//swings.emplace_back(swing(103.0, 1.47, 9.4, -3.3, 0.0, 4350.0));
	//swings.emplace_back(swing(101.0, 1.46, 10.0, -3.4, 0.0, 4400.0));
	//hybrid
	//swings.emplace_back(swing(100.0, 1.46, 10.2, -3.5, 0.0, 4437.0));
	//irons
	//swings.emplace_back(swing(98.0, 1.45, 10.4, -3.1, 0.0, 4630.0));
	//swings.emplace_back(swing(96.0, 1.43, 11.0, -3.4, 10.0, 4836.0));
	//swings.emplace_back(swing(94.0, 1.41, 12.1, -3.7, 0.0, 5361.0));
	//swings.emplace_back(swing(92.0, 1.38, 14.1, -4.1, 0.0, 6231.0));
	//swings.emplace_back(swing(90.0, 1.33, 16.3, -4.3, 0.0, 7097.0));
	//swings.emplace_back(swing(87.0, 1.32, 18.1, -4.5, 0.0, 7998.0));
	//swings.emplace_back(swing(85.0, 1.28, 20.4, -4.7, 0.0, 8647.0));
	//swings.emplace_back(swing(83.0, 1.23, 24.2, -5.0, 2.0, 9304.0));


	ImGui::SFML::Shutdown();
	system("pause>0");
}
