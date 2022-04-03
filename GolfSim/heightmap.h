#pragma once


const float setY = 0.0f;
//create heightmap of defines width and depth, divided into grid based on scale size
vector<polygon> createHeightMap(int width, int depth, int scale) {
	vector <polygon> heightMap;
	for (float d = 0.0f-(depth/2.0f); d < (depth/2.0f); d += scale) {
		for (float w = 0.0f-(width/2.0f); w < (width/2.0f); w += scale) {
			//back-left poly
			heightMap.emplace_back(polygon(w, setY, d, w + scale, setY, d, w, setY, d + scale, 100, 140,40));
			//fron-right poly
			heightMap.emplace_back(polygon(w, setY, d + scale, w + scale, setY, d, w + scale, setY, d + scale,110,150,50));
		}
	}
	return heightMap;
}