#pragma once

#include "Shape.h"

class Camada : public ShapeNode {
public:
	bool Horizontal_vert;
	std::string Camada_name;
public:
    Camada(World* world,std::string camada_name,bool hori_vert);
};


class Rubik: public ShapeNode{
public:
	Point centro;
	std::vector<Camada*> camada_verts;
	std::vector<Camada*> camada_horits;
	// Solver
public:
	Rubik(World* world, const Point& cent);
	void Generate() override;
	void DrawGeometry(const Matrix& parent) override {}
	void printMenu() override;
	void handleKey(int key, int mods,char CURRENT_AXIS) override;
};