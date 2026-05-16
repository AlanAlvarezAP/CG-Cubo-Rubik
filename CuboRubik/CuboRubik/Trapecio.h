#pragma once

#include "Shape.h"

class Trapecio: public ShapeNode{
public:
	unsigned int sector_Start, lines_Start, points_Start;
public:
	Trapecio(World* world);
	void Generate() override;
	void DrawGeometry(const Matrix& parent) override;
	void printMenu() override;
	void handleKey(int key, int mods,char CURRENT_AXIS) override;
};
