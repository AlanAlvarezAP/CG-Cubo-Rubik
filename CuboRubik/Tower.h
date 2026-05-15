#pragma once 

#include "Shape.h" 

class Cube;
class Sphere;
class Piramid;

class Tower: public ShapeNode{
public:
    Tower(World* world);
    void Generate() override;
	void DrawGeometry(const Matrix& parent) override {}
    float GetTopY() const;
	void printMenu() override;
    void handleKey(int key, int mods, char axis) override;
};