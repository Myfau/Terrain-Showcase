#include <iostream>
#include <fstream>
#include <Libraries/include/include_fade25d/Fade_2D.h>
#include <Libraries/include/include_fade25d/Point2.h>
#define SRTM_SIZE 1000

class triangulation{	
public:
	triangulation(const char* heightDataFilename);
private: 
	short height[SRTM_SIZE][SRTM_SIZE] = { 0 };
	std::vector<GEOM_FADE25D::Point2> inputPointsVector;

	int hgt_loader(const char * filename);
	std::vector<int> iCourse;
	std::vector<int> jCourse;

	void createInputVector();
	void terrain_full();
};

