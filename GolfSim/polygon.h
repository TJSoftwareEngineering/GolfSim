#pragma once

const float degToRad = 3.14159265 / 180;
class polygon {
public:
	vector <point> points3D;
	vector <point> points3DRotated;
	vector <point> points2D;
	int r = 255, g = 255, b = 255;
	bool draw = true;
	polygon(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int rIn, int gIn, int bIn) {
		points3D.emplace_back(point(x1, y1, z1));
		points3D.emplace_back(point(x2, y2, z2));
		points3D.emplace_back(point(x3, y3, z3));
		r = rIn;
		g = gIn;
		b = bIn;
	}

	void rotate() {
		points3DRotated.clear();
		for (int i = 0; i < points3D.size(); i++) {
			float tempX = (points3D[i].x - cam.x) * cos(cam.angleX * degToRad) + (points3D[i].z - cam.z) * sin(cam.angleX * degToRad);
			float tempY = points3D[i].y - cam.y;
			float tempZ = (points3D[i].z - cam.z) * cos(cam.angleX * degToRad) - (points3D[i].x - cam.x) * sin(cam.angleX * degToRad);
			points3DRotated.emplace_back(point(tempX, tempY, tempZ));
		}
	}

	void convert2D() {
		points2D.clear();
		//3D rotation
		rotate();
		float scaler = 800.0;
		for (int i = 0; i < points3D.size(); i++) {
			//3D to 2D projection
			if ((points3DRotated[0].z) > 0.0 && (points3DRotated[1].z) > 0.0 && (points3DRotated[2].z) > 0.0) {
				float tempX = (points3DRotated[i].x) / (points3DRotated[i].z ) * scaler + halfWidth;
				float tempY = -(points3DRotated[i].y ) / (points3DRotated[i].z ) * scaler + halfHeight;
				points2D.emplace_back(point(tempX, tempY, 0));
			}
		}
	}
};


vector<polygon> makeTrail(shot shotIn) {
	vector<polygon> tempVec;
	if (shotIn.path3D.size() > 0) {
		for (int i = 0; i < shotIn.path3D.size()-1; i++) {
			//horizontal side
			tempVec.emplace_back(polygon(shotIn.path3D[i].x, shotIn.path3D[i].y, shotIn.path3D[i].z, shotIn.path3D[i+1].x, shotIn.path3D[i+1].y, shotIn.path3D[i+1].z, shotIn.path3D[i+1].x+.5, shotIn.path3D[i+1].y, shotIn.path3D[i+1].z, 255,50,20));
			tempVec.emplace_back(polygon(shotIn.path3D[i].x, shotIn.path3D[i].y, shotIn.path3D[i].z, shotIn.path3D[i].x +.5, shotIn.path3D[i].y, shotIn.path3D[i].z, shotIn.path3D[i + 1].x + .5, shotIn.path3D[i + 1].y, shotIn.path3D[i + 1].z, 255, 50, 20));
			//vertical side
			tempVec.emplace_back(polygon(shotIn.path3D[i].x, shotIn.path3D[i].y, shotIn.path3D[i].z, shotIn.path3D[i + 1].x, shotIn.path3D[i + 1].y, shotIn.path3D[i + 1].z, shotIn.path3D[i + 1].x, shotIn.path3D[i + 1].y + .5, shotIn.path3D[i + 1].z, 255, 50, 20));
			tempVec.emplace_back(polygon(shotIn.path3D[i].x, shotIn.path3D[i].y, shotIn.path3D[i].z, shotIn.path3D[i].x, shotIn.path3D[i].y +.5 , shotIn.path3D[i].z, shotIn.path3D[i + 1].x, shotIn.path3D[i + 1].y +.5, shotIn.path3D[i + 1].z, 255, 50, 20));
		}
	}
	return tempVec;
}



