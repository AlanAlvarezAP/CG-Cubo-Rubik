#pragma once

#include "Shape.h"

class Face : public ShapeNode {
public:
    unsigned int sector_Start, lines_Start, points_Start;
	Point a,b,c,d;
    std::vector<RGB> triColors;
    std::vector<RGB> lineColors;
    std::vector<RGB> pointColors;

    Face(World* world, Point A, Point B, Point C, Point D);
    void Generate() override;
    void DrawGeometry(const Matrix& parent) override;
};


class Cube: public ShapeNode{
public:
	Point center;
	unsigned int sector_Start,lines_Start,points_Start;
	std::vector<RGB> triColors;
	std::vector<RGB> lineColors;
	std::vector<RGB> pointColors;
	int type;
public:
	Cube(World* world, const Point& cent,int tp=0,std::string name="Cube");
	void Generate() override;
	void DrawGeometry(const Matrix& parent) override {}
	void printMenu() override;
	void handleKey(int key, int mods,char CURRENT_AXIS) override;
};