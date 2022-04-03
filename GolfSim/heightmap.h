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
			heightMap.emplace_back(polygon(w, setY, d + scale, w + scale, setY, d, w + scale, setY, d + scale,103,143,43));
		}
	}
	return heightMap;
}

vector<polygon> makeYardMarkers() {
	vector<polygon> markers;
	const float markSize = .25;
	int r = 200, g = 210, b = 150;
	for (float d = 0.0f - (mapSizeZ / 2.0f); d <= (mapSizeZ / 2.0f); d += 100.0) {
		for (float w = 0.0f - (mapSizeX / 2.0f); w < (mapSizeX / 2.0f); w += mapScale) {
			markers.emplace_back(polygon(w, 0, d - markSize, w, 0, d + markSize, w + mapScale, 0, d + markSize, r, g, b));
			markers.emplace_back(polygon(w, 0, d - markSize, w + mapScale, 0, d - markSize, w + mapScale, 0, d + markSize, r, g, b));

			markers.emplace_back(polygon(w, -markSize, d, w, markSize, d, w + mapScale, markSize, d, r, g, b));
			markers.emplace_back(polygon(w, -markSize, d, w + mapScale, -markSize, d, w + mapScale, markSize, d, r, g, b));
		}
	}
	return markers;
}