#pragma once

vector <polygon> heightMap;

//create heightmap of defines width and depth, divided into grid based on scale size
void createHeightMap(int width, int depth, int scale) {

	for (float d = 0.0f-(depth/2.0f); d < (depth/2.0f); d += scale) {
		for (float w = 0.0f-(width/2.0f); w < (width/2.0f); w += scale) {
			//back-left poly
			heightMap.emplace_back(polygon(w, 0.0f, d, w + scale, 0.0f, d, w, 0.0f, d - scale));
			//fron-right poly
			heightMap.emplace_back(polygon(w, 0.0f, d - scale, w + scale, 0.0f, d, w + scale, 0.0f, d - scale));
		}
	}

}