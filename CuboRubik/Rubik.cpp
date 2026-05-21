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


// Camadas
// Horizontal -> 0
// Vertical -> 1

// Tipos
// Esquinas -> 0 (3 colores)
// Medio centrados -> 1 (2 colores)
// Centro -> 2 (1 color)
void Rubik::Generate() {
	std::vector<Cube*> cubos;
	std::vector<int> types={ESQUINA,MEDIO_CENTRO,ESQUINA,MEDIO_CENTRO,ESQUINA,MEDIO_CENTRO,ESQUINA,MEDIO_CENTRO,MEDIO_CENTRO,CENTRO,MEDIO_CENTRO,CENTRO,MEDIO_CENTRO,CENTRO,MEDIO_CENTRO,CENTRO,ESQUINA,MEDIO_CENTRO,ESQUINA,MEDIO_CENTRO,ESQUINA,MEDIO_CENTRO,ESQUINA,MEDIO_CENTRO,CENTRO,CENTRO,CENTRO};
	
	for(int i=0;i<27;i++){
		Cube* tmp=new Cube(this->world,{0.0f,0.0f,0.0f},types[i],std::string{"Cube"+std::to_string(i)});
		tmp->Generate();
		cubos.push_back(tmp);
	}

	for(int i=0;i<3;i++){
		Camada* camad=new Camada(this->world,"CH-"+std::to_string(i),HORIZONTAL);
		this->camada_verts.push_back(camad);
		this->AddChildren(camad);
	}
	for(int i=0;i<3;i++){
		Camada* camad=new Camada(this->world,"CV-"+std::to_string(i),VERTICAL);
		this->camada_horits.push_back(camad);
		this->AddChildren(camad);
	}

	// Añadiendo todos los cubos
	// Actualizando pos
	cubos[0]->Mat.UpdateView('a',-0.4,-0.4,0.0,'x','W');
	cubos[1]->Mat.UpdateView('a',-0.4,0.0,0.0,'x','W');
	cubos[2]->Mat.UpdateView('a',-0.4,0.4,0.0,'x','W');
	cubos[3]->Mat.UpdateView('a',-0.4,0.4,-0.4,'x','W');
	cubos[4]->Mat.UpdateView('a',-0.4,0.4,-0.8,'x','W');
	cubos[5]->Mat.UpdateView('a',-0.4,0.0,-0.8,'x','W');
	cubos[6]->Mat.UpdateView('a',-0.4,-0.4,-0.8,'x','W');
	cubos[7]->Mat.UpdateView('a',-0.4,-0.4,-0.4,'x','W');

	cubos[8]->Mat.UpdateView('a',0.0,-0.4,0.0,'x','W');
	cubos[9]->Mat.UpdateView('a',0.0,0.0,0.0,'x','W');
	cubos[10]->Mat.UpdateView('a',0.0,0.4,0.0,'x','W');
	cubos[11]->Mat.UpdateView('a',0.0,0.4,-0.4,'x','W');
	cubos[12]->Mat.UpdateView('a',0.0,0.4,-0.8,'x','W');
	cubos[13]->Mat.UpdateView('a',0.0,0.0,-0.8,'x','W');
	cubos[14]->Mat.UpdateView('a',0.0,-0.4,-0.8,'x','W');
	cubos[15]->Mat.UpdateView('a',0.0,-0.4,-0.4,'x','W');

	cubos[16]->Mat.UpdateView('a',0.4,-0.4,0.0,'x','W');
	cubos[17]->Mat.UpdateView('a',0.4,0.0,0.0,'x','W');
	cubos[18]->Mat.UpdateView('a',0.4,0.4,0.0,'x','W');
	cubos[19]->Mat.UpdateView('a',0.4,0.4,-0.4,'x','W');
	cubos[20]->Mat.UpdateView('a',0.4,0.4,-0.8,'x','W');
	cubos[21]->Mat.UpdateView('a',0.4,0.0,-0.8,'x','W');
	cubos[22]->Mat.UpdateView('a',0.4,-0.4,-0.8,'x','W');
	cubos[23]->Mat.UpdateView('a',0.4,-0.4,-0.4,'x','W');

	cubos[24]->Mat.UpdateView('a',0.4,0.0,-0.4,'x','W');
	cubos[25]->Mat.UpdateView('a',-0.4,0.0,-0.4,'x','W');
	cubos[26]->Mat.UpdateView('a',0.0,0.0,-0.4,'x','W'); // DUMMY
	// Camada CH1
	
	camada_verts[0]->AddChildren(cubos[0]);
	camada_verts[0]->AddChildren(cubos[1]);
	camada_verts[0]->AddChildren(cubos[2]);
	camada_verts[0]->AddChildren(cubos[3]);
	camada_verts[0]->AddChildren(cubos[4]);
	camada_verts[0]->AddChildren(cubos[5]);
	camada_verts[0]->AddChildren(cubos[6]);
	camada_verts[0]->AddChildren(cubos[7]);

	// Camada CH2
	camada_verts[1]->AddChildren(cubos[9]);
	camada_verts[1]->AddChildren(cubos[10]);
	camada_verts[1]->AddChildren(cubos[11]);
	camada_verts[1]->AddChildren(cubos[12]);
	camada_verts[1]->AddChildren(cubos[13]);
	camada_verts[1]->AddChildren(cubos[14]);
	camada_verts[1]->AddChildren(cubos[15]);
	camada_verts[1]->AddChildren(cubos[16]);
	
	// Camada CH3
	camada_verts[2]->AddChildren(cubos[17]);
	camada_verts[2]->AddChildren(cubos[18]);
	camada_verts[2]->AddChildren(cubos[19]);
	camada_verts[2]->AddChildren(cubos[20]);
	camada_verts[2]->AddChildren(cubos[21]);
	camada_verts[2]->AddChildren(cubos[22]);
	camada_verts[2]->AddChildren(cubos[23]);
	camada_verts[2]->AddChildren(cubos[24]);
	
	// Camada CV1
	camada_horits[0]->AddChildren(cubos[1]);
	camada_horits[0]->AddChildren(cubos[2]);
	camada_horits[0]->AddChildren(cubos[3]);
	camada_horits[0]->AddChildren(cubos[9]);
	camada_horits[0]->AddChildren(cubos[11]);
	camada_horits[0]->AddChildren(cubos[17]);
	camada_horits[0]->AddChildren(cubos[18]);
	camada_horits[0]->AddChildren(cubos[19]);
	
	// Camada CV2
	camada_horits[1]->AddChildren(cubos[4]);
	camada_horits[1]->AddChildren(cubos[8]);
	camada_horits[1]->AddChildren(cubos[12]);
	camada_horits[1]->AddChildren(cubos[16]);
	camada_horits[1]->AddChildren(cubos[20]);
	camada_horits[1]->AddChildren(cubos[24]);
	camada_horits[1]->AddChildren(cubos[25]);
	camada_horits[1]->AddChildren(cubos[26]);
	
	// Camada CV3
	camada_horits[2]->AddChildren(cubos[5]);
	camada_horits[2]->AddChildren(cubos[6]);
	camada_horits[2]->AddChildren(cubos[7]);
	camada_horits[2]->AddChildren(cubos[13]);
	camada_horits[2]->AddChildren(cubos[15]);
	camada_horits[2]->AddChildren(cubos[21]);
	camada_horits[2]->AddChildren(cubos[22]);
	camada_horits[2]->AddChildren(cubos[23]);
	
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


