#pragma once

#include "Shape.h"
#include "Cube.h"

enum Orientation{HORIZONTAL,VERTICAL};
enum Types{ESQUINA,MEDIO_CENTRO,CENTRO};



class Rubik: public ShapeNode{
public:
	Point centro;
	std::vector<std::vector<Cubee*>> vertical_set;
	std::vector<std::vector<Cubee*>> horizontal_set;
	std::vector<std::vector<Cubee*>> deep_set;
	std::vector<Cubee*> cubes;
	bool do_permutation;
	int permutation_option;
	int permutation_axi;
	bool permutation_clockwise;
	char facelets[55];
	float animation_time;
	// Solver
public:
	Rubik(World* world, const Point& cent);
	void Generate() override;
	void DrawGeometry(const Matrix& parent) override {}
	void printMenu() override;
	void handleKey(int key, int mods,char CURRENT_AXIS) override;
	void clockwise_permutation(std::vector<Cubee*> &cam);
	void counterClockwise_permutation(std::vector<Cubee*> &cam);	
	//void Update_contrary(int option,int offset,std::vector<Cubee*> &camada_changed,std::vector<std::vector<Cubee*>> &camadas);
	void sync_from_horizontal(int h_idx);
	void sync_from_vertical(int v_idx);
	void sync_from_deep(int d_idx);
	void vertical_rotation(float value_rot,int option,Animator* &anim);
	void horizontal_rotation(float value_rot,int option,Animator* &anim);
	void deep_rotation(float value_rot,int option,Animator* &anim);
	void update_facelets_from_horizontal(int idx);
	void update_facelets_from_vertical(int idx);
	void update_facelets_from_deep(int idx);
	void PrintCamadas();
};