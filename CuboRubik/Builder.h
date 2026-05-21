#pragma once

#include "Pizza.h"
#include "Piramid.h"
#include "Cube.h"
#include "Sphere.h"
#include "Tower.h"
#include "Colors.h"
#include "Robot.h"
#include "Camera.h"
#include "Trapecio.h"
#include "Rubik.h"

class Builder{
public:
	static Pizza* BuildPizzaScene(World* &mundito,int NUM_REBANADAS){
		Point centro = {0.0f, 0.0f, 0.0f};
		Pizza* pizza = new Pizza(mundito, centro, NUM_REBANADAS, 0.5f);

		mundito->root->AddChildren(pizza);
		pizza->Generate();

		for(int i = 0; i < (int)pizza->children.size(); i++){
			Sector* sec = dynamic_cast<Sector*>(pizza->children[i]);
			if(!sec){
				continue;
			}
			
			for(int j = 0; j < 3; j++){
				Pepperoni* pep = new Pepperoni(mundito, *sec, *pizza, i, 0.05f);
				sec->AddChildren(pep);
				pep->Generate();
			}
			
			for(int j = 0; j < 2; j++){
				Piña* pina = new Piña(mundito, *sec, *pizza, i);
				sec->AddChildren(pina);
				pina->Generate();
			}
			
			for(int j = 0; j < 20; j++){
				Oregano* ore = new Oregano(mundito, *sec);
				sec->AddChildren(ore);
				ore->Generate();
			}
		}
		std::cout << "GEN 4 " << std::endl;
		return pizza;
	}
	
	static Piramid* BuildPyramidScene(World* &mundito,const float& height){
		
		Point centro={0.0f,0.0f,height/2,0.0f};
		Piramid* pir=new Piramid(mundito,centro,3,0.5f,height);
		mundito->root->AddChildren(pir);
		pir->Generate();
		
		return pir;
	}
	
	static Cube* BuildCubeScene(World* &mundito, const Point& center = {0.0f, 0.0f, 0.0f}) {

        Cube* cube = new Cube(mundito, center);
        mundito->root->AddChildren(cube);
        cube->Generate();

        std::cout << "GEN CUBE" << std::endl;

        return cube;
    }
	
	static Sphere* BuildSphereScene(World* &mundito, float radius) {

        Point centro = {0.0f, 0.0f, 0.0f};
        Sphere* s = new Sphere(mundito, centro, radius, 18, 18);
        mundito->root->AddChildren(s);
        s->Generate();

		std::cout << "GEN SPHERE" << std::endl;
        return s;
    }
	
	static Tower* BuildTowerScene(World* &mundito){
		Tower* tow=new Tower(mundito);
		mundito->root->AddChildren(tow);
		tow->Generate();
		
		std::cout << "GEN TOWER" << std::endl;
		return tow;
	}
	
	static Robot* BuildRobotScene(World* &mundito){
		Robot* rot=new Robot(mundito);
		mundito->root->AddChildren(rot);
		rot->Generate();
		
		std::cout << "GEN ROBOT" << std::endl;
		return rot;
	}
	
	static Camera* BuildCamera(){
		Camera* cama=new Camera();
		std::cout << "GEN Camera" << std::endl;
		return cama;
	}
	
	static Animator* BuildAnimator(){
		Animator* anim=new Animator();
		std::cout << "GEN Animator" << std::endl;
		return anim;
	}
	static Trapecio* BuildTrapecio(World* &mundito){
		Trapecio* trape=new Trapecio(mundito);
		mundito->root->AddChildren(trape);
		trape->Generate();
		std::cout << "GEN Trapecio" << std::endl;
		return trape;
	}
	
	static Rubik* BuildRubik(World* &mundito){
		Rubik* cubo_rubik=new Rubik(mundito,{0.0f,0.0f,0.0f});
		mundito->root->AddChildren(cubo_rubik);
		cubo_rubik->Generate();
		std::cout << "GEN Cubo Rubik" << std::endl;
		return cubo_rubik;
	}
};