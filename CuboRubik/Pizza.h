#pragma once

#include "Shape.h"

class Sector;
class Pizza;

class Piña: public ShapeNode{
public:
	Sector *sectorOwn;
	Pizza* pizzaOwn;
    int target_index;
public:
	Piña(World* world,Sector& sec,Pizza& piz,int idx);
	void Generate() override;
	void DrawGeometry(const Matrix& parent) override;
};

class Pepperoni: public ShapeNode{
public:
	Sector *sectorOwn;
	float radius;
	Pizza* pizzaOwn;
    int target_index;
public:
	Pepperoni(World* world,Sector& sec,Pizza& piz,int idx,const float& rad);
	void Generate() override;
	void DrawGeometry(const Matrix& parent) override;
};

class Oregano: public ShapeNode{
public:
	Sector *sectorOwn;
public:
	Oregano(World* world,Sector& sec);
	void Generate() override;
	void DrawGeometry(const Matrix& parent) override;
};

class Sector: public ShapeNode {
public:
	Point startPoint,endPoint,center;
	float radius;
	int number_segments;
	unsigned int sector_Start,lines_Start;
public:
	Sector(World* world,const Point& start_point,const Point& end_point,const Point& centro,const float &rad,const int &num_seg);
	void Generate() override;
	void DrawGeometry(const Matrix& parent) override;
};

class Pizza: public ShapeNode{
public:
	Point center;
	int amount_sector;
	float RADIUS;
public:
	Pizza(World* world,const Point& cent,const int& amount_sec,const float& rad);
	// No necesidad de Pizza destructor ?
	int Get_Sector(Point evaluate,const float radio);
	void Generate() override;
	void DrawGeometry(const Matrix& parent) override {}
	void printMenu() override;
	void handleKey(int key, int mods,char CURRENT_AXIS) override;
};
