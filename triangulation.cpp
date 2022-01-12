#include "triangulation.h"

triangulation::triangulation(const char* heightDataFilename) {
    int result = hgt_loader(heightDataFilename);
    if (result != 0) {
        std::cout << "Error reading file!" << std::endl;
    }
    else {
        createInputVector();
        terrain_full();
    }
}

int triangulation::hgt_loader(const char * filename){
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file){
        std::cout << "Error opening file!" << std::endl;
        return -1;
    }

    unsigned char buffer[2];
    std::cout << "Start reading file!" << std::endl;
    int counter = 0;
    for (int i = 0; i < 3601; ++i){
        for (int j = 0; j < 3601; ++j){            
            if (!file.read(reinterpret_cast <char*>(buffer), sizeof(buffer))){
                std::cout << "Error reading file!" << std::endl;
                return -1;
            }
            if (i < SRTM_SIZE && j < SRTM_SIZE) {
                height[i][j] = (buffer[0] << 8) | buffer[1];
            }
        }
    }
    return 0;
}

void triangulation::createInputVector(){
    std::cout << "Start creating vector!" << std::endl;
    for (int i = 0; i < SRTM_SIZE; ++i){
        for (int j = 0; j < SRTM_SIZE; ++j){
            GEOM_FADE25D::Point2 * point = new GEOM_FADE25D::Point2(i, j, height[i][j]);
            inputPointsVector.push_back(*point);
            delete point;
        }
    }
}

void triangulation::terrain_full()
{
    std::cout << "Start triangulate!" << std::endl;
    GEOM_FADE25D::Fade_2D dt;
    dt.setFastMode(true);
    dt.setNumCPU(0);
    GEOM_FADE25D::CloudPrepare cloudPrep;
    cloudPrep.add(inputPointsVector);
    dt.insert(&cloudPrep, true);

    GEOM_FADE25D::FadeExport fadeExport;
    bool bCustomIndices(false);
    bool bClear(true); 
    dt.exportTriangulation(fadeExport, bCustomIndices, bClear);
    fadeExport.writeObj("terrain2.obj");
}
