#include "Rubik.h"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

Rubik::Rubik(World* world,const Point &center):ShapeNode(world,GL_TRIANGLES,"Cubo Rubik"),centro(center),do_permutation(false),perm_option(0),perm_horizontal(true){}


// Camadas
// Horizontal -> 0
// Vertical -> 1

// Tipos
// Esquinas -> 0 (3 colores)
// Medio centrados -> 1 (2 colores)
// Centro -> 2 (1 color)
void Rubik::Generate() {
	
	int types[27]={ESQUINA,MEDIO_CENTRO,ESQUINA,MEDIO_CENTRO,ESQUINA,MEDIO_CENTRO,ESQUINA,MEDIO_CENTRO,MEDIO_CENTRO,CENTRO,MEDIO_CENTRO,CENTRO,MEDIO_CENTRO,CENTRO,MEDIO_CENTRO,CENTRO,ESQUINA,MEDIO_CENTRO,ESQUINA,MEDIO_CENTRO,ESQUINA,MEDIO_CENTRO,ESQUINA,MEDIO_CENTRO,CENTRO,CENTRO,CENTRO};
	
	std::vector<std::vector<RGB>> rubikColors = { // orden de facesIdx en Generate
		{ColorTable[NEGRO],ColorTable[VERDE],ColorTable[BLANCO],ColorTable[NEGRO],ColorTable[NARANJA],ColorTable[NEGRO]}, // 1
		{ColorTable[NEGRO],ColorTable[VERDE],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NARANJA],ColorTable[NEGRO]}, // 2
		{ColorTable[NEGRO],ColorTable[VERDE],ColorTable[NEGRO],ColorTable[AMARILLO],ColorTable[NARANJA],ColorTable[NEGRO]}, // 3
		{ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[AMARILLO],ColorTable[NARANJA],ColorTable[NEGRO]}, // 4
		{ColorTable[AZUL],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[AMARILLO],ColorTable[NARANJA],ColorTable[NEGRO]}, // 5
		{ColorTable[AZUL],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NARANJA],ColorTable[NEGRO]}, // 6
		{ColorTable[AZUL],ColorTable[NEGRO],ColorTable[BLANCO],ColorTable[NEGRO],ColorTable[NARANJA],ColorTable[NEGRO]}, // 7
		{ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[BLANCO],ColorTable[NEGRO],ColorTable[NARANJA],ColorTable[NEGRO]}, // 8
		{ColorTable[NEGRO],ColorTable[VERDE],ColorTable[BLANCO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO]}, // 9
		{ColorTable[NEGRO],ColorTable[VERDE],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO]}, // 10
		{ColorTable[NEGRO],ColorTable[VERDE],ColorTable[NEGRO],ColorTable[AMARILLO],ColorTable[NEGRO],ColorTable[NEGRO]}, // 11
		{ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[AMARILLO],ColorTable[NEGRO],ColorTable[NEGRO]}, // 12
		{ColorTable[AZUL],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[AMARILLO],ColorTable[NEGRO],ColorTable[NEGRO]}, // 13
		{ColorTable[AZUL],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO]}, // 14
		{ColorTable[AZUL],ColorTable[NEGRO],ColorTable[BLANCO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO]}, // 15
		{ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[BLANCO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO]}, // 16
		{ColorTable[NEGRO],ColorTable[VERDE],ColorTable[BLANCO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[ROJO]}, // 17
		{ColorTable[NEGRO],ColorTable[VERDE],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[ROJO]}, // 18
		{ColorTable[NEGRO],ColorTable[VERDE],ColorTable[NEGRO],ColorTable[AMARILLO],ColorTable[NEGRO],ColorTable[ROJO]}, // 19
		{ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[AMARILLO],ColorTable[NEGRO],ColorTable[ROJO]}, // 20
		{ColorTable[AZUL],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[AMARILLO],ColorTable[NEGRO],ColorTable[ROJO]}, // 21
		{ColorTable[AZUL],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[ROJO]}, // 22
		{ColorTable[AZUL],ColorTable[NEGRO],ColorTable[BLANCO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[ROJO]}, // 23
		{ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[BLANCO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[ROJO]}, // 24
		{ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[ROJO]}, // 25
		{ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NARANJA],ColorTable[NEGRO]}, // 26
		{ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO],ColorTable[NEGRO]} // 27
	};
	
	for(int i=0;i<27;i++){
		Cube* tmp=new Cube(this->world,{0.0f,0.0f,0.0f},rubikColors[i],types[i],std::string{"Cube"+std::to_string(i)});
		tmp->Generate();
		cubos.push_back(tmp);
		this->AddChildren(tmp);
	}

	camada_horits.resize(3);
	camada_verts.resize(3);

	// Añadiendo todos los cubos
	// Actualizando pos
	cubos[0]->Mat.UpdateView('a',-0.42,-0.42, 0.42,'x','W');
	cubos[1]->Mat.UpdateView('a',-0.42, 0.0 , 0.42,'x','W');
	cubos[2]->Mat.UpdateView('a',-0.42, 0.42, 0.42,'x','W');
	cubos[3]->Mat.UpdateView('a',-0.42, 0.42, 0.0 ,'x','W');
	cubos[4]->Mat.UpdateView('a',-0.42, 0.42,-0.42,'x','W');
	cubos[5]->Mat.UpdateView('a',-0.42, 0.0 ,-0.42,'x','W');
	cubos[6]->Mat.UpdateView('a',-0.42,-0.42,-0.42,'x','W');
	cubos[7]->Mat.UpdateView('a',-0.42,-0.42, 0.0 ,'x','W');

	cubos[8]->Mat.UpdateView('a',0.0,-0.42, 0.42,'x','W');
	cubos[9]->Mat.UpdateView('a',0.0, 0.0 , 0.42,'x','W');
	cubos[10]->Mat.UpdateView('a',0.0, 0.42, 0.42,'x','W');
	cubos[11]->Mat.UpdateView('a',0.0, 0.42, 0.0 ,'x','W');
	cubos[12]->Mat.UpdateView('a',0.0, 0.42,-0.42,'x','W');
	cubos[13]->Mat.UpdateView('a',0.0, 0.0 ,-0.42,'x','W');
	cubos[14]->Mat.UpdateView('a',0.0,-0.42,-0.42,'x','W');
	cubos[15]->Mat.UpdateView('a',0.0,-0.42, 0.0 ,'x','W');

	cubos[16]->Mat.UpdateView('a',0.42,-0.42, 0.42,'x','W');
	cubos[17]->Mat.UpdateView('a',0.42, 0.0 , 0.42,'x','W');
	cubos[18]->Mat.UpdateView('a',0.42, 0.42, 0.42,'x','W');
	cubos[19]->Mat.UpdateView('a',0.42, 0.42, 0.0 ,'x','W');
	cubos[20]->Mat.UpdateView('a',0.42, 0.42,-0.42,'x','W');
	cubos[21]->Mat.UpdateView('a',0.42, 0.0 ,-0.42,'x','W');
	cubos[22]->Mat.UpdateView('a',0.42,-0.42,-0.42,'x','W');
	cubos[23]->Mat.UpdateView('a',0.42,-0.42, 0.0 ,'x','W');

	// DUMMY centrado en el medio
	cubos[24]->Mat.UpdateView('a',0.42,0.0, 0.0,'x','W');
	cubos[25]->Mat.UpdateView('a',-0.42,0.0, 0.0,'x','W');
	cubos[26]->Mat.UpdateView('a',0.0, 0.0, 0.0,'x','W');
	
	// Camada CH1
	// mini-CV1
	camada_verts[0]={cubos[0],cubos[7],cubos[6],
	// mini-CV2
	cubos[8],cubos[15],cubos[14],
	// mini-CV3
	cubos[16],cubos[23],cubos[22]};

	// Camada CH2
	// mini-CV1
	camada_verts[1]={cubos[1],cubos[25],cubos[5],
	// mini-CV2
	cubos[9],cubos[26],cubos[13],
	// mini-CV3
	cubos[17],cubos[24],cubos[21]};

	
	// Camada CH3
	// mini-CV1
	camada_verts[2]={cubos[2],cubos[3],cubos[4],
	// mini-CV2
	cubos[10],cubos[11],cubos[12],
	// mini-CV3
	cubos[18],cubos[19],cubos[20]};
	
	// Camada CV1
	// mini-CH1
	camada_horits[0] = {cubos[0],cubos[7],cubos[6],
	// mini-CH2
	cubos[1],cubos[25],cubos[5],
	// mini-CH3
	cubos[2],cubos[3],cubos[4]};
	
	// Camada CV2
	// mini-CH1
	camada_horits[1]={cubos[8],cubos[15],cubos[14],
	// mini-CH2
	cubos[9],cubos[26],cubos[13],
	// mini-CH3
	cubos[10],cubos[11],cubos[12]};
	
	// Camada CV3
	// mini-CH1
	camada_horits[2]={cubos[16],cubos[23],cubos[22],
	// mini-CH2
	cubos[17],cubos[24],cubos[21],
	// mini-CH3
	cubos[18],cubos[19],cubos[20]};
	
}

void Rubik::Permutation_horizo(std::vector<Cube*> &cam){
	std::vector<Cube*> tmp=cam;
	
	cam[2]=tmp[0];
	cam[5]=tmp[1];
	cam[8]=tmp[2];
	cam[1]=tmp[3];
	cam[7]=tmp[5];
	cam[0]=tmp[6];
	cam[3]=tmp[7];
	cam[6]=tmp[8];
}

void Rubik::Permutation_verti(std::vector<Cube*> &cam){
	std::vector<Cube*> tmp=cam;

	cam[6]=tmp[0];
	cam[3]=tmp[1];
	cam[0]=tmp[2];
	cam[7]=tmp[3];
	cam[1]=tmp[5];
	cam[8]=tmp[6];
	cam[5]=tmp[7];
	cam[2]=tmp[8];
}
void Rubik::Update_contrary(int option,int offset,std::vector<Cube*> &camada_changed,std::vector<std::vector<Cube*>> &camadas){
	int block=option-offset;
	for(int i=0;i<camadas.size();i++){ 
		auto& row = camadas[i];
		for(int j=0;j<3;j++){ 
			int col=j+block*3;
			if(col>=0 && col<row.size()){
                row[col]=camada_changed[i*3+j];
			}else{
				std::cout << "Acceso ilegal cuidao" << std::endl;
			}
		} 
	}
}

void Rubik::Rotation_hori(float value_rot,int option,Animator* &anim){
	if(option <= 0 || option > 3){
		std::cout << "Opcion de rotacion horizontal invalida :( " << std::endl;
		return;
	}
	
	std::vector<Animation_Step*> steps;

	for (auto* cube : camada_horits[option-1]) {
		steps.push_back(new Animation_Step(cube, 1.0f, 'd', value_rot, 'x', 'W'));
	}
	anim->Add_Animations(steps, 'N');

	do_permutation = true;
    perm_option = option;
    perm_horizontal = true;
	
}

void Rubik::Rotation_verti(float value_rot,int option,Animator* &anim){
	if(option <= 3 || option > 6){
		std::cout << "Opcion de rotacion vertical invalida :( " << std::endl;
		return;
	}
	
	std::vector<Animation_Step*> steps;
	for (auto* cube : camada_verts[option-4]) {
		steps.push_back(new Animation_Step(cube, 1.0f, 'd', value_rot, 'y', 'W'));
	}
	anim->Add_Animations(steps, 'N');
	
	do_permutation = true;
    perm_option = option;
    perm_horizontal = false;
	
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
    std::cout << "|  1. Mover Camada Hori 1         |" << std::endl;
    std::cout << "|  2. Mover Camada Hori 2         |" << std::endl;
    std::cout << "|  3. Mover Camada Hori 3         |" << std::endl;
	std::cout << "|  4. Mover Camada Verti 1        |" << std::endl;
	std::cout << "|  5. Mover Camada Verti 2        |" << std::endl;
	std::cout << "|  6. Mover Camada Verti 3        |" << std::endl;
    std::cout << "|  ESC/CTRL+C. Salir              |" << std::endl;
    std::cout << "===================================" << std::endl;
}

void Rubik::PrintCamadas(){
	std::cout << "-------------------------------" << std::endl;
	std::cout << " PARA CAMADAS VERTICALES " << std::endl;
	for(int i=0;i<camada_verts.size();i++){
		std::cout << "Camada vertical -> " << i << std::endl;
		for(int j=0;j<camada_verts[i].size();j++){
			std::cout << "Cubo # " << camada_verts[i][j]->name << std::endl;
		}
	}
	std::cout << " PARA CAMADAS HORIZONTAL " << std::endl;
	for(int i=0;i<camada_verts.size();i++){
		std::cout << "Camada horizontal -> " << i << std::endl;
		for(int j=0;j<camada_verts[i].size();j++){
			std::cout << "Cubo # " << camada_verts[i][j]->name << std::endl;
		}
	}
	std::cout << "-------------------------------" << std::endl;
}