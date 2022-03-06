#pragma once


class polygon {
public:
	vector <point> points3D;
	vector <point> points2D;
	bool draw = true;
	polygon(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
		points3D.emplace_back(point(x1, y1, z1));
		points3D.emplace_back(point(x2, y2, z2));
		points3D.emplace_back(point(x3, y3, z3));
	}
	void convert2D() {
		points2D.clear();
		float scaler = 700.0;
		for (int i = 0; i < points3D.size(); i++) {
			if (points3D[i].z > 0.0) {
				float tempX = points3D[i].x / points3D[i].z * scaler + halfWidth;
				float tempY = points3D[i].y / points3D[i].z * scaler + halfHeight;
				points2D.emplace_back(point(tempX, tempY, 0));
			}
		}
	}
};


