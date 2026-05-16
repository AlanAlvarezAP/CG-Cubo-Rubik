#include "Tower.h"

#include "Cube.h"
#include "Sphere.h"
#include "Piramid.h"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

Tower::Tower(World* world):ShapeNode(world, GL_TRIANGLES, "Tower"){}

void Tower::Generate() {

    Cube* cube1 = new Cube(world, {0, 0, 0});
    cube1->Generate();
    cube1->Mat.UpdateView('a', 0.0f, 0.5f, 0.0f, 'y','W');
    this->AddChildren(cube1);
	
	Cube* cube2 = new Cube(world, {0, 0, 0});
    cube2->Generate();
    cube2->Mat.UpdateView('a', 0.0f, -0.5f, 0.0f, 'y','W');
    this->AddChildren(cube2);



    Sphere* sphere = new Sphere(world, {0, 0, 0}, 0.2, 12, 12);
    sphere->Generate();
    this->AddChildren(sphere);


	Point pyrCenter = {0.0f, 0.0f, 0.0f, 0.0f};
    Piramid* pyramid1 = new Piramid(world, pyrCenter, 4,0.2f, 0.5f);
    pyramid1->Generate();
	pyramid1->Mat.UpdateView('a', -0.25f, 0.0f, 0.0f, 'x','L');
	pyramid1->Mat.UpdateView('d', 90.0f, 0.0f, 0.0f, 'z','L');
    
	this->AddChildren(pyramid1);
	
	Point pyrCenter2 = {0.0f, 0.0f, 0.0f, 0.0f};
    Piramid* pyramid2 = new Piramid(world, pyrCenter2, 4,0.2f, 0.5f);
    pyramid2->Generate();
	pyramid2->Mat.UpdateView('a', 0.25f, 0.0f, 0.0f, 'x','L');
	pyramid2->Mat.UpdateView('f', 90.0f, 0.0f, 0.0f, 'z','L');
    this->AddChildren(pyramid2);
}

void Tower::handleKey(int key, int mods, char CURRENT_AXIS){
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
			mat->PrintMatrix();
			mat->UpdateView('a',0.1f,0.0f,0.0f,CURRENT_AXIS,'W');
			mat->PrintMatrix();
			break;
		}
		case GLFW_KEY_LEFT:{
			mat->PrintMatrix();
			mat->UpdateView('a',-0.1f,0.0f,0.0f,CURRENT_AXIS,'W');
			mat->PrintMatrix();
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

void Tower::printMenu(){
	std::cout << "===================================" << std::endl;
    std::cout << "|        Bienvenido a             |" << std::endl;
    std::cout << "|     Simulador de Tower          |" << std::endl;
    std::cout << "|                                 |" << std::endl;
    std::cout << "|  1. Escalar esfera              |" << std::endl;
    std::cout << "|  2. Traslacion opuesta cubos    |" << std::endl;
    std::cout << "|  3. Traslacion opuesta triangulos|" << std::endl;
	std::cout << "|  4. Triangulos rotan todo       |" << std::endl;
	std::cout << "|  5. Cubos rotan todo            |" << std::endl;
    std::cout << "|  ESC/CTRL+C. Salir              |" << std::endl;
    std::cout << "===================================" << std::endl;
}
