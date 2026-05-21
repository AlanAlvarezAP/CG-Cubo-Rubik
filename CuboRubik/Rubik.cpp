#include "Rubik.h"
#include "Cube.h"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

Camada::Camada(World* world,std::string camada_name,bool hori_vert):ShapeNode(world,GL_TRIANGLES,camada_name){
	this->Camada_name=camada_name;
	this->Horizontal_vert=hori_vert;
}


Rubik::Rubik(World* world,const Point &center):ShapeNode(world,GL_TRIANGLES,"Cubo Rubik"),centro(center){}



// Horizontal -> 0
// Vertical -> 1
void Rubik::Generate() {
	std::vector<Cube*> cubos;
	for(int i=0;i<27;i++){
		cubos.push_back(new Cube(this->world,{0.0f,0.0f,0.0f}));
		
	}

	for(int i=0;i<3;i++){
		Camada* camad=new Camada(this->world,"CH"+std::to_string(i),0);
		this->camada_verts.push_back(camad);
		this->AddChildren(camad);
	}
	for(int i=0;i<3;i++){
		Camada* camad=new Camada(this->world,"CV"+std::to_string(i),1);
		this->camada_horits.push_back(camad);
		this->AddChildren(camad);
	}

	
	
}


void Rubik::handleKey(int key, int mods,char CURRENT_AXIS){
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

void Rubik::printMenu(){
	std::cout << "===================================" << std::endl;
    std::cout << "|        Bienvenido a             |" << std::endl;
    std::cout << "|     Simulador de Rubik          |" << std::endl;
    std::cout << "|                                 |" << std::endl;
    std::cout << "|  1. Escalar esfera              |" << std::endl;
    std::cout << "|  2. Traslacion opuesta cubos    |" << std::endl;
    std::cout << "|  3. Traslacion opuesta triangulos|" << std::endl;
	std::cout << "|  4. Triangulos rotan todo       |" << std::endl;
	std::cout << "|  5. Cubos rotan todo            |" << std::endl;
    std::cout << "|  ESC/CTRL+C. Salir              |" << std::endl;
    std::cout << "===================================" << std::endl;
}


