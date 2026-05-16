#include "Rubik.h"

Camada::Camada(World* world,std::string camada_name,bool hori_vert):ShapeNode(world,GL_TRIANGLES,camada_name){
	this->Camada_name=camada_name;
	this->Horizontal_vert=hori_vert;
}


Rubik::Rubik(World* world):ShapeNode(world,GL_TRIANGLES,"Cubo Rubik"){}



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


