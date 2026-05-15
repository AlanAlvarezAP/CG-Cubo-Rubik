#include "Robot.h"

#include "Cube.h"
#include "Sphere.h"
#include "Piramid.h"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

Robot::Robot(World* world):ShapeNode(world, GL_TRIANGLES, "Robot"){}

void Robot::Generate() {

	// Creación
	// Cabeza
	
	ShapeNode* Cabeza = new ShapeNode(this->world,GL_TRIANGLES,"Cara");
	
	Cube* cube2 = new Cube(world, {0, 0, 0});
    cube2->Generate();
    cube2->Mat.UpdateView('a', 0.0f, 0.5f, 0.0f, 'y','W');
	cube2->Mat.UpdateView('g', 1.0f, 0.5f, 1.0f, 'y','L');
    Cabeza->AddChildren(cube2);
	
	Sphere* sphere1 = new Sphere(world, {0, 0, 0}, 0.1, 12, 12);
	sphere1->Mat.UpdateView('a',-0.1f, 0.55f, 0.15f,'x','W');
	sphere1->Mat.UpdateView('g',0.4f, 0.3f, 1.0f,'x','L');
    sphere1->Generate();
	Cabeza->AddChildren(sphere1);
	
	Sphere* sphere2 = new Sphere(world, {0, 0, 0}, 0.1, 12, 12);
	sphere2->Mat.UpdateView('a',0.1f, 0.55f, 0.15f,'x','W');
	sphere2->Mat.UpdateView('g',0.4f, 0.3f, 1.0f,'x','L');
    sphere2->Generate();
	Cabeza->AddChildren(sphere2);
	
	Cube* cube3 = new Cube(world, {0, 0, 0});
    cube3->Generate();
    cube3->Mat.UpdateView('a', 0.0f, 0.45f, 0.04f, 'y','W');
	cube3->Mat.UpdateView('g', 0.1f, 0.05f, 1.0f, 'y','L');
    Cabeza->AddChildren(cube3);
	this->AddChildren(Cabeza);

	//------------------ Torso -------------------
    Cube* cube1 = new Cube(world, {0, 0, 0});
    cube1->Generate();
	cube1->Mat.UpdateView('a', 0.0f, 0.1f, 0.0f, 'y','W');
	cube1->Mat.UpdateView('g', 1.0f, 1.25f, 1.0f, 'y','W');
    this->AddChildren(cube1);


	// -------------- BRAZOS --------------------
	// Brazo izq
	ShapeNode* Brazo_grande_izq=new ShapeNode(this->world,GL_TRIANGLES,"Brazo grande izq");
	
	// AnteBrazo
	ShapeNode* AnteBrazo_izq= new ShapeNode(this->world,GL_TRIANGLES,"Ante Brazo izq");
	
	Cube* cubo_AB_izq = new Cube(world, {0, 0, 0});
	cubo_AB_izq->Mat.UpdateView('a',-0.32f,0.2f,0.0f,'x','W');
	cubo_AB_izq->Mat.UpdateView('g',0.4f,0.8f,0.5f,'x','L');
    cubo_AB_izq->Generate();
	
	Sphere* sphere_AB_izq = new Sphere(world, {0, 0, 0}, 0.1, 12, 12);
	sphere_AB_izq->Mat.UpdateView('a',-0.32f,0.35f,0.0f,'x','W');
	sphere_AB_izq->Mat.UpdateView('g',1.4f,1.1f,1.7f,'x','L');
    sphere_AB_izq->Generate();
	
	// Brazo Brazo
	ShapeNode* BrazoBrazo_izq= new ShapeNode(this->world,GL_TRIANGLES,"Brazo Brazo izq");
	
	Sphere* sphere_BB_izq = new Sphere(world, {0, 0, 0}, 0.1, 12, 12);
	sphere_BB_izq->Mat.UpdateView('a',-0.32f,0.0f,0.0f,'x','W');
	sphere_BB_izq->Mat.UpdateView('g',1.4f,1.1f,1.7f,'x','L');
    sphere_BB_izq->Generate();
	
	Cube* cubo_BB_izq = new Cube(world, {0, 0, 0});
	cubo_BB_izq->Mat.UpdateView('a',-0.32f,-0.2f,0.0f,'x','W');
	cubo_BB_izq->Mat.UpdateView('g',0.4f,0.8f,0.5f,'x','L');
    cubo_BB_izq->Generate();
	
	Piramid* pyramid_BB_izq = new Piramid(world, {0.0f, 0.0f, 0.0f, 0.0f},4,0.2f, 0.5f);
    pyramid_BB_izq->Generate();
	pyramid_BB_izq->Mat.UpdateView('a',-0.32f,-0.38f,0.0f,'x','W');
	pyramid_BB_izq->Mat.UpdateView('g',0.5f,0.5f,0.5f,'x','L');
	pyramid_BB_izq->Mat.UpdateView('d', 180.0f, 0.0f, 0.0f, 'z','L');
	
	//------ ADD BRAZO IZQ
	this->AddChildren(Brazo_grande_izq);
	Brazo_grande_izq->AddChildren(AnteBrazo_izq);
	Brazo_grande_izq->AddChildren(BrazoBrazo_izq);
	AnteBrazo_izq->AddChildren(sphere_AB_izq);
	AnteBrazo_izq->AddChildren(cubo_AB_izq);
	BrazoBrazo_izq->AddChildren(sphere_BB_izq);
	BrazoBrazo_izq->AddChildren(cubo_BB_izq);
	BrazoBrazo_izq->AddChildren(pyramid_BB_izq);
	
	
	// Brazo der
	ShapeNode* Brazo_grande_der=new ShapeNode(this->world,GL_TRIANGLES,"Brazo grande der");
	
	// AnteBrazo
	ShapeNode* AnteBrazo_der= new ShapeNode(this->world,GL_TRIANGLES,"Ante Brazo der");
	
	Cube* cubo_AB_der = new Cube(world, {0, 0, 0});
	cubo_AB_der->Mat.UpdateView('a',0.32f,0.2f,0.0f,'x','W');
	cubo_AB_der->Mat.UpdateView('g',0.4f,0.8f,0.5f,'x','L');
    cubo_AB_der->Generate();
	
	Sphere* sphere_AB_der = new Sphere(world, {0, 0, 0}, 0.1, 12, 12);
	sphere_AB_der->Mat.UpdateView('a',0.32f,0.35f,0.0f,'x','W');
	sphere_AB_der->Mat.UpdateView('g',1.4f,1.1f,1.7f,'x','L');
    sphere_AB_der->Generate();
	
	
	// Brazo Brazo
	ShapeNode* BrazoBrazo_der= new ShapeNode(this->world,GL_TRIANGLES,"Brazo Brazo der");
	
	Sphere* sphere_BB_der = new Sphere(world, {0, 0, 0}, 0.1, 12, 12);
	sphere_BB_der->Mat.UpdateView('a',0.32f,0.0f,0.0f,'x','W');
	sphere_BB_der->Mat.UpdateView('g',1.4f,1.1f,1.7f,'x','L');
    sphere_BB_der->Generate();
	
	Cube* cubo_BB_der = new Cube(world, {0, 0, 0});
	cubo_BB_der->Mat.UpdateView('a',0.32f,-0.2f,0.0f,'x','W');
	cubo_BB_der->Mat.UpdateView('g',0.4f,0.8f,0.5f,'x','L');
    cubo_BB_der->Generate();
	
    Piramid* pyramid_BB_der = new Piramid(world, {0.0f, 0.0f, 0.0f, 0.0f},4,0.2f, 0.5f);
    pyramid_BB_der->Generate();
	pyramid_BB_der->Mat.UpdateView('a',0.32f,-0.38f,0.0f,'x','W');
	pyramid_BB_der->Mat.UpdateView('g',0.5f,0.5f,0.5f,'x','L');
	pyramid_BB_der->Mat.UpdateView('d', 180.0f, 0.0f, 0.0f, 'z','L');
	
	
	//------ ADD BRAZO DER
	this->AddChildren(Brazo_grande_der);
	Brazo_grande_der->AddChildren(AnteBrazo_der);
	Brazo_grande_der->AddChildren(BrazoBrazo_der);
	AnteBrazo_der->AddChildren(sphere_AB_der);
	AnteBrazo_der->AddChildren(cubo_AB_der);
	BrazoBrazo_der->AddChildren(sphere_BB_der);
	BrazoBrazo_der->AddChildren(cubo_BB_der);
	BrazoBrazo_der->AddChildren(pyramid_BB_der);
	
	// ------------------- PIERNAS ---------------------
	// Pierna izq
	ShapeNode* Pierna_izq=new ShapeNode(world,GL_TRIANGLES,"Pierna izq");
	
	//Muslo
	ShapeNode* Muslo_izq= new ShapeNode(world,GL_TRIANGLES,"Muslo izq");
	
	Sphere* sphere_muslo_izq = new Sphere(world, {0, 0, 0}, 0.1, 12, 12);
	sphere_muslo_izq->Mat.UpdateView('a',-0.1f,-0.18f,0.0f,'x','W');
	sphere_muslo_izq->Mat.UpdateView('g',1.1f,1.1f,1.7f,'x','L');
    sphere_muslo_izq->Generate();
	
	Cube* cubo_muslo_izq = new Cube(world, {0, 0, 0});
	cubo_muslo_izq->Mat.UpdateView('a',-0.1f,-0.35f,0.0f,'x','W');
	cubo_muslo_izq->Mat.UpdateView('g',0.4f,0.8f,0.5f,'x','L');
    cubo_muslo_izq->Generate();
	
	// Pierna Pierna
	ShapeNode* PiernaPierna_izq= new ShapeNode(world,GL_TRIANGLES,"Pierna Pierna izq");
	
	Sphere* sphere_PP_izq = new Sphere(world, {0, 0, 0}, 0.1, 12, 12);
	sphere_PP_izq->Mat.UpdateView('a',-0.1f,-0.58f,0.0f,'x','W');
	sphere_PP_izq->Mat.UpdateView('g',1.1f,1.1f,1.7f,'x','L');
    sphere_PP_izq->Generate();
	
	Cube* cubo_PP_izq = new Cube(world, {0, 0, 0});
	cubo_PP_izq->Mat.UpdateView('a',-0.1f,-0.75f,0.0f,'x','W');
	cubo_PP_izq->Mat.UpdateView('g',0.4f,0.8f,0.5f,'x','L');
    cubo_PP_izq->Generate();
	
	Piramid* pyramid_PP_izq = new Piramid(world, {0.0f, 0.0f, 0.0f, 0.0f},4,0.2f, 0.5f);
    pyramid_PP_izq->Generate();
	pyramid_PP_izq->Mat.UpdateView('a',-0.1f,-0.83f,0.12f,'x','W');
	pyramid_PP_izq->Mat.UpdateView('g',0.38f,0.4f,0.5f,'x','L');
	pyramid_PP_izq->Mat.UpdateView('d', 270.0f, 0.0f, 0.0f, 'x','L');
	
	
	//------ ADD PIERNA IZQ
	this->AddChildren(Pierna_izq);
	Pierna_izq->AddChildren(Muslo_izq);
	Pierna_izq->AddChildren(PiernaPierna_izq);
	Muslo_izq->AddChildren(sphere_muslo_izq);
	Muslo_izq->AddChildren(cubo_muslo_izq);
	PiernaPierna_izq->AddChildren(sphere_PP_izq);
	PiernaPierna_izq->AddChildren(cubo_PP_izq);
	PiernaPierna_izq->AddChildren(pyramid_PP_izq);
	
	
	// Pierna der
	ShapeNode* Pierna_der=new ShapeNode(world,GL_TRIANGLES,"Pierna der");
	
	//Muslo
	ShapeNode* Muslo_der= new ShapeNode(world,GL_TRIANGLES,"Muslo der");
	
	Sphere* sphere_muslo_der = new Sphere(world, {0, 0, 0}, 0.1, 12, 12);
	sphere_muslo_der->Mat.UpdateView('a',0.1f,-0.18f,0.0f,'x','W');
	sphere_muslo_der->Mat.UpdateView('g',1.1f,1.1f,1.7f,'x','L');
    sphere_muslo_der->Generate();
	
	Cube* cubo_muslo_der = new Cube(world, {0, 0, 0});
	cubo_muslo_der->Mat.UpdateView('a',0.1f,-0.35f,0.0f,'x','W');
	cubo_muslo_der->Mat.UpdateView('g',0.4f,0.8f,0.5f,'x','L');
    cubo_muslo_der->Generate();
	
	// Pierna Pierna
	ShapeNode* PiernaPierna_der= new ShapeNode(world,GL_TRIANGLES,"Pierna Pierna der");
	
	Sphere* sphere_PP_der = new Sphere(world, {0, 0, 0}, 0.1, 12, 12);
	sphere_PP_der->Mat.UpdateView('a',0.1f,-0.58f,0.0f,'x','W');
	sphere_PP_der->Mat.UpdateView('g',1.1f,1.1f,1.7f,'x','L');
    sphere_PP_der->Generate();
	
	Cube* cubo_PP_der = new Cube(world, {0, 0, 0});
	cubo_PP_der->Mat.UpdateView('a',0.1f,-0.75f,0.0f,'x','W');
	cubo_PP_der->Mat.UpdateView('g',0.4f,0.8f,0.5f,'x','L');
    cubo_PP_der->Generate();
	
	Piramid* pyramid_PP_der = new Piramid(world, {0.0f, 0.0f, 0.0f, 0.0f},4,0.2f, 0.5f);
    pyramid_PP_der->Generate();
	pyramid_PP_der->Mat.UpdateView('a',0.1f,-0.83f,0.12f,'x','W');
	pyramid_PP_der->Mat.UpdateView('g',0.38f,0.4f,0.5f,'x','L');
	pyramid_PP_der->Mat.UpdateView('d', 270.0f, 0.0f, 0.0f, 'x','L');
	
	
	//------ ADD PIERNA IZQ
	this->AddChildren(Pierna_der);
	Pierna_der->AddChildren(Muslo_der);
	Pierna_der->AddChildren(PiernaPierna_der);
	Muslo_der->AddChildren(sphere_muslo_der);
	Muslo_der->AddChildren(cubo_muslo_der);
	PiernaPierna_der->AddChildren(sphere_PP_der);
	PiernaPierna_der->AddChildren(cubo_PP_der);
	PiernaPierna_der->AddChildren(pyramid_PP_der);
	
}

void Robot::handleKey(int key, int mods, char CURRENT_AXIS){
	ShapeNode* target = this;

    if(selected_part != -1){
        target = children[selected_part];
    }

    Matrix* mat = &(target->Mat);

    switch(key){
		case GLFW_KEY_UP:{
			mat->UpdateView('a',0.0f,0.1f,0.0f,CURRENT_AXIS,'W');
			break;
		}
		case GLFW_KEY_DOWN:{
			mat->UpdateView('a',0.0f,-0.1f,0.0f,CURRENT_AXIS,'W');
			break;
		}
		case GLFW_KEY_RIGHT:{
			mat->UpdateView('a',0.1f,0.0f,0.0f,CURRENT_AXIS,'W');
			break;
		}
		case GLFW_KEY_LEFT:{
			mat->UpdateView('a',-0.1f,0.0f,0.0f,CURRENT_AXIS,'W');
			break;
		}
		case GLFW_KEY_PAGE_UP:{
			mat->UpdateView('a',0.0f,0.0f,0.1f,CURRENT_AXIS,'W');
			break;
		}
		case GLFW_KEY_PAGE_DOWN:{
			mat->UpdateView('a',0.0f,0.0f,-0.1f,CURRENT_AXIS,'W');
			break;
		}
		case GLFW_KEY_D:{
			mat->UpdateView('d',10.0f,0.0f,0.0f,CURRENT_AXIS,'W');
			break;
		}
		case GLFW_KEY_F:{
			mat->UpdateView('f',10.0f,0.0f,0.0f,CURRENT_AXIS,'W');
			break;
		}
		case GLFW_KEY_G:{
			mat->UpdateView('g', 1.1f, 1.1f,1.1f,CURRENT_AXIS,'W');
			break;
		}
		case GLFW_KEY_H:{
			mat->UpdateView('g', 0.9f, 0.9f,0.9f,CURRENT_AXIS,'W');
			break;
		}
		default:{
			break;
		}
	}
}

void Robot::printMenu(){
	std::cout << "===================================" << std::endl;
    std::cout << "|        Bienvenido a             |" << std::endl;
    std::cout << "|     Simulador de Robot          |" << std::endl;
    std::cout << "|                                 |" << std::endl;
    std::cout << "|  1. Empezar animacion           |" << std::endl;
    std::cout << "|  2. Empezar animacion+rotacion  |" << std::endl;
    std::cout << "|  ESC/CTRL+C. Salir              |" << std::endl;
    std::cout << "===================================" << std::endl;
}


void Robot::Walk(Animator* anim){
    ShapeNode* Brazo_izq_AB = dynamic_cast<ShapeNode*>(this->children[2]->children[0]);
    ShapeNode* Brazo_izq_BB = dynamic_cast<ShapeNode*>(this->children[2]->children[1]);
    ShapeNode* Brazo_der_AB = dynamic_cast<ShapeNode*>(this->children[3]->children[0]);
    ShapeNode* Brazo_der_BB = dynamic_cast<ShapeNode*>(this->children[3]->children[1]);
    ShapeNode* Muslo_izq = dynamic_cast<ShapeNode*>(this->children[4]->children[0]);
    ShapeNode* PiernaPierna_izq = dynamic_cast<ShapeNode*>(this->children[4]->children[1]);
    ShapeNode* Muslo_der = dynamic_cast<ShapeNode*>(this->children[5]->children[0]);
    ShapeNode* PiernaPierna_der = dynamic_cast<ShapeNode*>(this->children[5]->children[1]);

    const float BB_steps = 90.0f;
    const float AB_steps = 10.0f;
	const float Muslo_steps=70.0f;
	const float PP_steps=70.0f;

    const float stepTime = 3.0f;
    const float giroPorPaso = 360.0f;


	std::vector<Animation_Step*> group_anim;

    group_anim.push_back(new Animation_Step(Brazo_izq_AB, stepTime, 'd',  AB_steps, 'x','W'));
	group_anim.push_back(new Animation_Step(Brazo_izq_BB, stepTime, 'd', -BB_steps, 'x','W'));

	group_anim.push_back(new Animation_Step(Muslo_der, stepTime, 'd', Muslo_steps, 'x','W'));
	group_anim.push_back(new Animation_Step(PiernaPierna_der, stepTime, 'd',  PP_steps, 'x','W'));

	group_anim.push_back(new Animation_Step(Brazo_der_AB, stepTime, 'd', -AB_steps, 'x','W'));
	group_anim.push_back(new Animation_Step(Brazo_der_BB, stepTime, 'd',  BB_steps, 'x','W'));

	group_anim.push_back(new Animation_Step(Muslo_izq, stepTime, 'd',  -Muslo_steps, 'x','W'));
	group_anim.push_back(new Animation_Step(PiernaPierna_izq, stepTime, 'd', -PP_steps, 'x','W'));
	group_anim.push_back(new Animation_Step(world->root, stepTime, 'd', giroPorPaso, 'y','W'));

	anim->Add_Animations(group_anim,'S');

    std::cout << "Termino animacion" << std::endl;
}


void Robot::Move(Animator* anim){

	std::vector<Animation_Step*> group_anim;
    group_anim.push_back(new Animation_Step(this, 5.0f, 'a',  10.0f, 'x','W'));
	group_anim.push_back(new Animation_Step(this, 5.0f, 'a', -10.0f, 'y','W'));

	anim->Add_Animations(group_anim,'S');

    std::cout << "Termino animacion" << std::endl;
}
