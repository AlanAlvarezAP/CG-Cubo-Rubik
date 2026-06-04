#pragma once

#include "Cube.h"
#include "Colors.h"
#include "Camera.h"
#include "Rubik.h"

class Builder{
public:
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
	
	static Rubik* BuildRubik(World* &mundito){
		Rubik* cubo_rubik=new Rubik(mundito,{0.0f,0.0f,0.0f});
		mundito->root->AddChildren(cubo_rubik);
		cubo_rubik->Generate();
		std::cout << "GEN Cubo Rubik" << std::endl;
		return cubo_rubik;
	}
};