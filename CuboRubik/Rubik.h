#pragma once

#include "Shape.h"
#include "Cube.h"

enum Orientation{HORIZONTAL,VERTICAL};
enum Types{ESQUINA,MEDIO_CENTRO,CENTRO};



class Rubik: public ShapeNode{
public:
	Point centro;
	std::vector<std::vector<Cube*>> vertical_set;
	std::vector<std::vector<Cube*>> horizontal_set;
	std::vector<std::vector<Cube*>> deep_set;
	std::vector<Cube*> cubes;
	bool do_permutation;
	int permutation_option;
	int permutation_axi;
	bool permutation_clockwise;
	// Solver
public:
	Rubik(World* world, const Point& cent);
	void Generate() override;
	void DrawGeometry(const Matrix& parent) override {}
	void printMenu() override;
	void handleKey(int key, int mods,char CURRENT_AXIS) override;
	void clockwise_permutation(std::vector<Cube*> &cam);
	void counterClockwise_permutation(std::vector<Cube*> &cam);	
	//void Update_contrary(int option,int offset,std::vector<Cube*> &camada_changed,std::vector<std::vector<Cube*>> &camadas);
	void sync_from_horizontal(int h_idx);
	void sync_from_vertical(int v_idx);
	void sync_from_deep(int d_idx);
	void vertical_rotation(float value_rot,int option,Animator* &anim);
	void horizontal_rotation(float value_rot,int option,Animator* &anim);
	void deep_rotation(float value_rot,int option,Animator* &anim);
	void PrintCamadas();
};