#include "Piramid.h"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

Piramid::Piramid(World* world,const Point& cent,const int& amount_sec,const float& rad,const float& heig):ShapeNode(world,0,"Piramide"),center(cent),amount_sector(amount_sec),RADIUS(rad),HEIGHT(heig){}

void Piramid::printMenu(){
	std::cout << "===================================" << std::endl;
    std::cout << "|        Bienvenido a             |" << std::endl;
    std::cout << "|     Simulador de Piramide       |" << std::endl;
    std::cout << "|                                 |" << std::endl;
    std::cout << "|  Q. Generar Piramide (default=4)|" << std::endl;
    std::cout << "|  W. Configurar                  |" << std::endl;
    std::cout << "|  R. Seleccion parte (0-N)       |" << std::endl;
	std::cout << "|  d. Rotar (0.1)                 |" << std::endl;
	std::cout << "|  f. Rotar inverso (0.1)         |" << std::endl;
	std::cout << "|  g. Escalar (1.1)               |" << std::endl;
	std::cout << "|  h. Escalar inverso (0.9)       |" << std::endl;
	std::cout << "|  x. Usar eje x                  |" << std::endl;
	std::cout << "|  y. Usar eje y                  |" << std::endl;
	std::cout << "|  z. Usar eje z                  |" << std::endl;
    std::cout << "|  4. Mover arriba (Flecha arr)   |" << std::endl;
    std::cout << "|  5. Mover abajo (Flecha abj)    |" << std::endl;
    std::cout << "|  6. Mover derecha (Flecha der)  |" << std::endl;
	std::cout << "|  7. Mover izquierda (Flecha izq)|" << std::endl;
    std::cout << "|  8. Salir (ESC o CTRL+C)        |" << std::endl;
    std::cout << "===================================" << std::endl;
	std::cout << " AL TERMINAR DE ESCRIBIR LA PARTE O CONFIGURACIÓN DE REBANDAS CONFIRMAR CON ENTER " << std::endl;
}

void Piramid::handleKey(int key, int mods,char CURRENT_AXIS){
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


void Piramid::Generate(){
	float final_sectors_display = 360.0f / (float)amount_sector;
	const float TRANSFORM = PI / 180.0f;


	Point apice = {center.x,center.y + HEIGHT,center.z,0.0f};

	for(float i = 0.0f; i <= 360.0f; i += final_sectors_display){
		Point start = {center.x + RADIUS * std::cos(i * TRANSFORM),center.y,center.z + RADIUS * std::sin(i * TRANSFORM),i * TRANSFORM};
		Point end = {center.x + RADIUS * std::cos((i + final_sectors_display) * TRANSFORM),
			center.y,
			center.z + RADIUS * std::sin((i + final_sectors_display) * TRANSFORM),
			(i + final_sectors_display) * TRANSFORM
		};
		Sector_Piramid* sec = new Sector_Piramid(world, start, end, apice, RADIUS, HEIGHT);
		this->AddChildren(sec);
		sec->Generate();
	}

	Base_Piramid* bas = new Base_Piramid(world, center, RADIUS, amount_sector, final_sectors_display);
	this->AddChildren(bas);
	bas->Generate();
}


Base_Piramid::Base_Piramid(World* world,const Point& centro,const float& rad,const int &sid,const float& ste)
	:ShapeNode(world,GL_TRIANGLES,"Base"),center(centro),radius(rad),sides(sid),steps(ste),sector_Start(0),lines_Start(0){
	ModifiedShaderColor(ColorTable[ARENA].r,ColorTable[ARENA].g,ColorTable[ARENA].b);
}

void Base_Piramid::Generate(){
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<unsigned int> borderVertex;
	std::vector<unsigned int> pointsVertex;

	unsigned int base = this->world->all_vertices.size() / 3;
	const float TRANSFORM = PI / 180.0f;

	vertices.push_back(center.x);
	vertices.push_back(center.y);
	vertices.push_back(center.z);

	for(int i = 0; i <= sides; i++){
		float ang = i * steps * TRANSFORM;
		vertices.push_back(center.x + radius * std::cos(ang));
		vertices.push_back(center.y);
		vertices.push_back(center.z + radius * std::sin(ang));
	}

	for(int i = 0; i < sides; i++){
		indices.push_back(base);
		indices.push_back(base + 1 + i);
		indices.push_back(base + 2 + i);

		borderVertex.push_back(base + 1 + i);
		borderVertex.push_back(base + 2 + i);

		pointsVertex.push_back(base + 1 + i);
	}

	this->sector_Start = 0;
	this->lines_Start  = indices.size();
	this->points_Start = indices.size() + borderVertex.size();

	indices.insert(indices.end(), borderVertex.begin(), borderVertex.end());
	indices.insert(indices.end(), pointsVertex.begin(), pointsVertex.end());

	this->EBOs_range = this->world->Add_Batch(vertices, indices, offset);
	this->IsDrawable = true;

	const int COLOR_COUNT = ARENA + 1;
    int &colorIdx = world->globalColorCounter;
    triColors.clear();
    lineColors.clear();
    pointColors.clear();
	triColors.push_back(ColorTable[colorIdx++ % COLOR_COUNT]);
    lineColors.push_back(ColorTable[colorIdx++ % COLOR_COUNT]);
    lineColors.push_back(ColorTable[colorIdx++ % COLOR_COUNT]);
    pointColors.push_back(ColorTable[colorIdx++ % COLOR_COUNT]);
    pointColors.push_back(ColorTable[colorIdx++ % COLOR_COUNT]);
    pointColors.push_back(ColorTable[colorIdx++ % COLOR_COUNT]);
    world->globalColorCounter = colorIdx;
}

void Base_Piramid::DrawGeometry(const Matrix& parent){
	this->Shader.use();
    this->Shader.SetMatrix(parent);

    for(int i = sector_Start; i < (int)lines_Start; i += 3){
        RGB c = ColorTable[GRIS];
        this->Shader.SetColor(c.r, c.g, c.b);
        glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,(void*)((offset + i) * sizeof(unsigned int)));
    }

	
    for(int i = lines_Start, l = 0; i < (int)points_Start; i += 2, l++){
        RGB c = ColorTable[VERDE];
        this->Shader.SetColor(c.r, c.g, c.b);
        glDrawElements(GL_LINES,2,GL_UNSIGNED_INT,(void*)((offset + i) * sizeof(unsigned int)));
    }

    for(int i = points_Start, p = 0; i < (int)EBOs_range.size(); i += 1, p++){
        RGB c = ColorTable[ROJO];
        this->Shader.SetColor(c.r, c.g, c.b);
        glDrawElements(GL_POINTS,1,GL_UNSIGNED_INT,(void*)((offset + i) * sizeof(unsigned int)));
    }
}

Sector_Piramid::Sector_Piramid(World* world,const Point& start_point,const Point& end_point,
	const Point& centro,const float &rad,const float& heigh)
	:ShapeNode(world,GL_TRIANGLES,"Sector"),
	startPoint(start_point),endPoint(end_point),center(centro),
	radius(rad),sector_Start(0),lines_Start(0),height(heigh){
	ModifiedShaderColor(ColorTable[ARENA].r,ColorTable[ARENA].g,ColorTable[ARENA].b);
}

void Sector_Piramid::Generate(){
	std::vector<float> vectores;
	std::vector<unsigned int> indices;
	std::vector<unsigned int> borderVertex;
	std::vector<unsigned int> pointsVertex;

	unsigned int base = this->world->all_vertices.size() / 3;

	vectores.push_back(center.x);
	vectores.push_back(center.y);
	vectores.push_back(center.z);

	vectores.push_back(startPoint.x);
	vectores.push_back(startPoint.y);
	vectores.push_back(startPoint.z);

	vectores.push_back(endPoint.x);
	vectores.push_back(endPoint.y);
	vectores.push_back(endPoint.z);

	indices.push_back(base);
	indices.push_back(base + 1);
	indices.push_back(base + 2);

	borderVertex.push_back(base);
	borderVertex.push_back(base + 1);
	borderVertex.push_back(base);
	borderVertex.push_back(base + 2);

	pointsVertex.push_back(base);
	pointsVertex.push_back(base + 1);
	pointsVertex.push_back(base + 2);

	this->sector_Start = 0;
	this->lines_Start  = indices.size();
	this->points_Start = indices.size() + borderVertex.size();

	indices.insert(indices.end(), borderVertex.begin(), borderVertex.end());
	indices.insert(indices.end(), pointsVertex.begin(), pointsVertex.end());

	this->EBOs_range = this->world->Add_Batch(vectores, indices, offset);
	this->IsDrawable = true;

	const int COLOR_COUNT = ARENA + 1;
    int &colorIdx = world->globalColorCounter;
    triColors.clear();
    lineColors.clear();
    pointColors.clear();
    triColors.push_back(ColorTable[colorIdx++ % COLOR_COUNT]);
    lineColors.push_back(ColorTable[colorIdx++ % COLOR_COUNT]);
    lineColors.push_back(ColorTable[colorIdx++ % COLOR_COUNT]);
    pointColors.push_back(ColorTable[colorIdx++ % COLOR_COUNT]);
    pointColors.push_back(ColorTable[colorIdx++ % COLOR_COUNT]);
    pointColors.push_back(ColorTable[colorIdx++ % COLOR_COUNT]);
    world->globalColorCounter = colorIdx;
}

void Sector_Piramid::DrawGeometry(const Matrix& parent){
	this->Shader.use();
    this->Shader.SetMatrix(parent);

	int conta=0;
    for(int i = sector_Start, tri = 0; i < (int)lines_Start; i += 3, tri++){
		RGB c;
		if(conta++ == 0 || conta++ == 1){
			std::cout << "Entre 1" << std::endl;
			c = ColorTable[AMARILLO];
		}else if(conta++ == 2 || conta++ == 3){
			std::cout << "Entre 2" << std::endl;
			c = ColorTable[CYAN];
		}
		else if(conta++ == 4 || conta++ == 5){
			std::cout << "Entre 3" << std::endl;
			c = ColorTable[MAGENTA];
			
		}else{
			conta=0;
		}
		
        this->Shader.SetColor(c.r, c.g, c.b);
        glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,(void*)((offset + i) * sizeof(unsigned int)));
    }

    for(int i = lines_Start, l = 0; i < (int)points_Start; i += 2, l++){
        RGB c = ColorTable[VERDE];
        this->Shader.SetColor(c.r, c.g, c.b);
        glDrawElements(GL_LINES,2,GL_UNSIGNED_INT,(void*)((offset + i) * sizeof(unsigned int)));
    }

    for(int i = points_Start, p = 0; i < (int)EBOs_range.size(); i += 1, p++){
        RGB c = ColorTable[ROJO];
        this->Shader.SetColor(c.r, c.g, c.b);
        glDrawElements(GL_POINTS,1,GL_UNSIGNED_INT,(void*)((offset + i) * sizeof(unsigned int)));
    }
}