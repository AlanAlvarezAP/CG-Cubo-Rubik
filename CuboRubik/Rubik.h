#pragma once

#include "Shape.h"
#include "Cube.h"

enum Orientation{HORIZONTAL,VERTICAL};
enum Types{ESQUINA,MEDIO_CENTRO,CENTRO};



class Rubik: public ShapeNode{
public:
	Point centro;
	std::vector<std::vector<Cube*>> camada_verts;
	std::vector<std::vector<Cube*>> camada_horits;
	std::vector<std::vector<Cube*>> camada_prof;
	std::vector<Cube*> cubos;
	bool do_permutation;
	int perm_option;
	int perm_eje;
	bool perm_direccion_horaria;
	// Solver
public:
	Rubik(World* world, const Point& cent);
	void Generate() override;
	void DrawGeometry(const Matrix& parent) override {}
	void printMenu() override;
	void handleKey(int key, int mods,char CURRENT_AXIS) override;
	void Permutation_horaria(std::vector<Cube*> &cam);
	void Permutation_antihoraria(std::vector<Cube*> &cam);	
	//void Update_contrary(int option,int offset,std::vector<Cube*> &camada_changed,std::vector<std::vector<Cube*>> &camadas);
	void sync_from_hori(int h);
	void sync_from_verti(int v);
	void sync_from_prof(int p);
	void Rotation_verti(float value_rot,int option,Animator* &anim);
	void Rotation_hori(float value_rot,int option,Animator* &anim);
	void Rotation_prof(float value_rot,int option,Animator* &anim);
	void PrintCamadas();
};