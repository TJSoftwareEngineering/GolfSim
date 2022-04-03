//-----------------------------------------------------------------------------
// Pennstate Capstone Project - Golf Simulator - Tim Myotte - 2022
//-----------------------------------------------------------------------------

const int screenWidth = 1280, screenHeight = 768;
const int halfWidth = screenWidth / 2, halfHeight = screenHeight / 2;
const int mapSizeX = 50, mapSizeZ = 400, mapScale = 25;


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
#include "utilityfunctions.h"
#include "simulator.h"
#include "loader.h"

#include "camera.h"
camera cam = camera(0.0f, 30.0f, -250.0f, 0.0f);

#include "polygon.h"
#include "heightmap.h"
#include "input.h"




// main window loop --------------------------------------------------------------------------------

int main() {

	loadClubData();
	vector<polygon> heightMap = createHeightMap(50.0f,400.0f,10.0f);


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

	vector <polygon> shotPolygons;
	vector <polygon> scene;


	//Create window
	sf::RenderWindow window(sf::VideoMode(1280, 768), "Golf Simulator - Pennstate Capstone");
	ImGui::SFML::Init(window);
	sf::Clock deltaClock;


	// Window render loop
	while (window.isOpen()) {

		//update
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			//keyboard input
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) left = true; else left = false;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) right = true; else right = false;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) up = true; else up = false;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) down = true; else down = false;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) forward = true; else forward = false;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) backward = true; else backward = false;

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

		//run simulation on button click
		if (ImGui::Button("Simulate")) {
			lastShot = simulate(lastShot, swing1, clubs[0], ball1, climate1);
			shotPolygons.clear();
			shotPolygons = makeTrail(lastShot);

		}
		ImGui::End();
		//climate window

		//Stat window
		ImGui::Begin("Stats");
		ImGui::Text("Distances:\n");

		ImGui::Text("Carry Distance: %f", lastShot.carryDist*unitConvert);
		ImGui::Text("Total Distance: %f", lastShot.totalDist*unitConvert);
		ImGui::Text("Ball Speed(mph): %f", lastShot.ballSpeed);
		
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

		//update variables from keyboard input
		update();

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
		window.clear(sf::Color(20, 50, 60)); // Color background

		//draw polygon


		//after any input, recalculate 2d conversions


			for (int i = 0; i < heightMap.size(); i++) {
				heightMap[i].convert2D();
			}


		//create poly object via convex class
		sf::ConvexShape convex;
		//convex.setOutlineColor(sf::Color(180, 210, 100));
		//convex.setFillColor(sf::Color(100, 150, 20));
		//convex.setOutlineThickness(1.0f);
		convex.setPointCount(3);

		for (int i = 0; i < heightMap.size(); i++) {

			if (heightMap[i].points2D.size() > 0) {
				convex.setFillColor(sf::Color(heightMap[i].r, heightMap[i].g, heightMap[i].b));
				// define the points
				convex.setPoint(0, sf::Vector2f(int(heightMap[i].points2D[0].x), int(heightMap[i].points2D[0].y)));
				convex.setPoint(1, sf::Vector2f(int(heightMap[i].points2D[1].x), int(heightMap[i].points2D[1].y)));
				convex.setPoint(2, sf::Vector2f(int(heightMap[i].points2D[2].x), int(heightMap[i].points2D[2].y)));

				window.draw(convex);
			}
		}



			for (int i = 0; i < shotPolygons.size(); i++) {
				shotPolygons[i].convert2D();
			}

		for (int i = 0; i < shotPolygons.size(); i++) {

			if (shotPolygons[i].points2D.size() > 0) {
				convex.setFillColor(sf::Color(shotPolygons[i].r, shotPolygons[i].g, shotPolygons[i].b));
				// define the points
				convex.setPoint(0, sf::Vector2f(int(shotPolygons[i].points2D[0].x), int(shotPolygons[i].points2D[0].y)));
				convex.setPoint(1, sf::Vector2f(int(shotPolygons[i].points2D[1].x), int(shotPolygons[i].points2D[1].y)));
				convex.setPoint(2, sf::Vector2f(int(shotPolygons[i].points2D[2].x), int(shotPolygons[i].points2D[2].y)));

				window.draw(convex);
			}
		}

		//window.draw(shape);
		ImGui::SFML::Render(window);
		window.display();
	}


	ImGui::SFML::Shutdown();
	system("pause>0");
}
