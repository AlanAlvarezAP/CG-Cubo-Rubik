#include "Pizza.h"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// PIZZA
Pizza::Pizza(World* world,const Point& cent,const int& amount_sec,const float& rad):ShapeNode(world,0,"Pizza"),center(cent),amount_sector(amount_sec),RADIUS(rad){}

void Pizza::printMenu(){
	std::cout << "===================================" << std::endl;
    std::cout << "|        Bienvenido a             |" << std::endl;
    std::cout << "|     Simulador de Pizza          |" << std::endl;
    std::cout << "|                                 |" << std::endl;
    std::cout << "|  Q. Generar pizza (default=4)   |" << std::endl;
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

	
void Pizza::handleKey(int key, int mods, char CURRENT_AXIS){

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

// PARA 2D -> TODO ver en 3D
int Pizza::Get_Sector(Point evaluate,const float radio){
	if(this->children.empty()){
		return -1;
	}
	
	if (evaluate.x*evaluate.x + evaluate.y*evaluate.y > radio*radio){
        return -1;
	}
	
	// Evaluar esto tdv por el caso en z
	float ang=std::atan2(evaluate.y,evaluate.x);
	if(ang < 0){
		ang+=2*PI;
	}
	for(int i=0;i<this->children.size();i++){
		Sector *sect=dynamic_cast<Sector*>(this->children[i]);
		if(!sect){
			continue;
		}
		Point* start=&(sect->startPoint);
		Point* end=&(sect->endPoint);
		if (start->angle <= end->angle){
            if (ang > start->angle && ang < end->angle){
                return i;
			}
        }
        else{
            if (ang > start->angle || ang < end->angle){
                return i;
			}
        }
	}
	return -1;
}
void Pizza::Generate(){
	float final_sectors_display=360.0f/(float)amount_sector;
	
	const int NUMBER_SEGMENTS=10;
	const float TRANSFORM=PI/180.0f;
	
	int sector_index = 0;
	for(float i=0.0f;i<360.0f;i+=final_sectors_display,sector_index++){
		Point start={
			center.x+RADIUS*std::cos(i*TRANSFORM),
			center.y+RADIUS*std::sin(i*TRANSFORM),
			0.0f,
			i*TRANSFORM
		};
		Point end={center.x+RADIUS*std::cos((i+final_sectors_display)*TRANSFORM),
			center.y+RADIUS*std::sin((i+final_sectors_display)*TRANSFORM),
			360.0f,
			(i+final_sectors_display)*TRANSFORM
		};
		Sector *sec = new Sector(world,start,end,center,RADIUS,NUMBER_SEGMENTS);
		this->AddChildren(sec);
		
		sec->Generate();
		
	}
	
}

Sector::Sector(World* world,const Point& start_point,const Point& end_point,const Point& centro,const float &rad,const int &num_seg):ShapeNode(world,GL_TRIANGLES,"Sector"),startPoint(start_point),endPoint(end_point),center(centro),number_segments(num_seg),radius(rad),sector_Start(0),lines_Start(0){
	ModifiedShaderColor(ColorTable[ARENA].r,ColorTable[ARENA].g,ColorTable[ARENA].b);
}

void Sector::Generate(){
	std::vector<float> vectores;
	std::vector<unsigned int> indices;
	std::vector<unsigned int> borderVertex;
	float angle_between=(endPoint.angle-startPoint.angle)/number_segments;
	
	unsigned int base = this->world->all_vertices.size()/3;
	unsigned int base_tmp_triang=base+1;
	unsigned int base_tmp_lines=base+1;
	
	vectores.push_back(center.x);
	vectores.push_back(center.y);
	vectores.push_back(center.z);
	
	vectores.push_back(startPoint.x);
	vectores.push_back(startPoint.y);
	vectores.push_back(startPoint.z);
	
	borderVertex.push_back(base);
	borderVertex.push_back(base_tmp_lines);
	
	for(float i=1;i<=number_segments;i++){
		float ang = startPoint.angle + i * angle_between;
		vectores.push_back(center.x+radius*std::cos(ang));
        vectores.push_back(center.y+radius*std::sin(ang));
        vectores.push_back(0.0f);
        
        indices.push_back(base);
        indices.push_back(base_tmp_triang);
		indices.push_back(++base_tmp_triang);
		
		
	}
	for(int i=0;i<number_segments;i++){
		borderVertex.push_back(base_tmp_lines+i);
		borderVertex.push_back(base_tmp_lines+i+1);
	}
	borderVertex.push_back(base_tmp_lines+number_segments);
	borderVertex.push_back(base);	
	/*std::cout << "------------------ VERTICES --------------------" << std::endl;
	for(int i=0;i<vectores.size();i+=3){
		std::cout << "Vertice " << i/3 << " : " << vectores[i] << " - " << vectores[i+1] << " - " << vectores[i+2] << std::endl;
	}
	std::cout << "-------------------------------------------------" << std::endl;
	
	std::cout << "----------------- INDICES SECTOR ---------------" << std::endl;;
	for(auto p:indices){
		std::cout << p << std::endl;
	}
	std::cout << "-------------------------------------------------" << std::endl;
	
	std::cout << "----------------- INDICES BORDE ---------------" << std::endl;;
	for(auto p:borderVertex){
		std::cout << p << std::endl;
	}
	std::cout << "-------------------------------------------------" << std::endl;*/
	
	this->sector_Start=indices.size();
	indices.insert(indices.end(),borderVertex.begin(),borderVertex.end());
	this->EBOs_range = this->world->Add_Batch(vectores, indices,offset);
	this->lines_Start=indices.size()-this->sector_Start;
	this->IsDrawable=true;
	/*std::cout << "VALOR DEBUG " << std::endl;
	std::cout << "TAM ACTUAL INDICES " << indices.size() << std::endl;
	std::cout << "lines_Start START " << this->lines_Start << std::endl;
    for(auto son : children){
        son->Generate();
    }*/
}

void Sector::DrawGeometry(const Matrix& parent){
	this->Shader.use();
	this->Shader.SetMatrix(parent);
	this->Shader.SetColor(ColorTable[NARANJA].r,ColorTable[NARANJA].g,ColorTable[NARANJA].b);
	
	glDrawElements(primitive,EBOs_range.size()-lines_Start,GL_UNSIGNED_INT,(void*)((offset)*sizeof(unsigned int)));	
	
	this->Shader.SetColor(ColorTable[VERDE].r,ColorTable[VERDE].g,ColorTable[VERDE].b);
	glDrawElements(GL_LINES,EBOs_range.size()-sector_Start,GL_UNSIGNED_INT,(void*)((offset+(EBOs_range.size()-lines_Start))*sizeof(unsigned int)));
	
	
	this->Shader.SetColor(ColorTable[ROJO].r,ColorTable[ROJO].g,ColorTable[ROJO].b);
	glDrawElements(GL_POINTS,EBOs_range.size()-lines_Start,GL_UNSIGNED_INT,(void*)((offset)*sizeof(unsigned int)));
}

Oregano::Oregano(World* world,Sector& sec):ShapeNode(world,GL_POINTS,"Oregano"),sectorOwn(&sec){
	ModifiedShaderColor(ColorTable[OLIVA].r,ColorTable[OLIVA].g,ColorTable[OLIVA].b);
}

void Oregano::Generate(){
	std::random_device rd;
    std::mt19937 gen(rd());
	float starting_radius=0.1f;
	
	Sector* pat=this->sectorOwn;
	
	Point start_center;
	float radius;
	float angulo;
	std::uniform_real_distribution<> dist_angle(pat->startPoint.angle, pat->endPoint.angle);
	angulo = dist_angle(gen);
	
	std::uniform_real_distribution<> dist_radius(0.2f, 0.4f);
	radius = dist_radius(gen);
	
	start_center=Point{radius*std::cos(angulo),radius*std::sin(angulo),0.0f,angulo};
	
	unsigned int tama=world->all_vertices.size()/3;
	unsigned int inicio = world->all_EBOs.size();
	world->all_vertices.push_back(start_center.x);
	world->all_vertices.push_back(start_center.y);
	world->all_vertices.push_back(start_center.z);
	
	world->all_EBOs.push_back(tama);
	this->offset = inicio;
	this->EBOs_range = std::vector<unsigned int>(world->all_EBOs.end()-1, world->all_EBOs.end());
	this->IsDrawable=true;
}

void Oregano::DrawGeometry(const Matrix& parent){
	this->Shader.use();
	this->Shader.SetMatrix(parent);
	this->Shader.SetColor(ColorTable[ROJO].r,ColorTable[ROJO].g,ColorTable[ROJO].b);
	
	glDrawElements(primitive,EBOs_range.size(),GL_UNSIGNED_INT,(void*)(offset*sizeof(unsigned int)));
	
	
}

Pepperoni::Pepperoni(World* world,Sector& sec,Pizza& piz,int idx,const float& rad):ShapeNode(world,GL_TRIANGLES,"Pepperoni"),sectorOwn(&sec),pizzaOwn(&piz),target_index(idx),radius(rad){
	ModifiedShaderColor(ColorTable[RUST].r,ColorTable[RUST].g,ColorTable[RUST].b);
}

void Pepperoni::Generate(){
    std::random_device rd;
    std::mt19937 gen(rd());

    Sector* pat=this->sectorOwn;
    float starting_radius=radius;
    float MIN_RADIUS=0.005f;
    int intentos=0;
    Point start_center;
    bool  result = false;
    do {
        std::uniform_real_distribution<float> dist_angle(pat->startPoint.angle,pat->endPoint.angle);
        std::uniform_real_distribution<float> dist_radius(starting_radius,pat->radius - starting_radius);
        float angulo=dist_angle(gen);
        float radius=dist_radius(gen);
        start_center={radius * std::cos(angulo),radius * std::sin(angulo),0.0f,angulo};
        bool Cond_1=pizzaOwn->Get_Sector({start_center.x + starting_radius,start_center.y, 0.0f, angulo}, pat->radius)== target_index;
        bool Cond_2=pizzaOwn->Get_Sector({start_center.x - starting_radius,start_center.y, 0.0f, angulo}, pat->radius)== target_index;
        bool Cond_3 = pizzaOwn->Get_Sector({start_center.x,start_center.y + starting_radius, 0.0f, angulo},pat->radius)== target_index;
        bool Cond_4 = pizzaOwn->Get_Sector({start_center.x,start_center.y - starting_radius, 0.0f, angulo},pat->radius)== target_index;
        result=Cond_1 && Cond_2 && Cond_3 && Cond_4;
        if(!result && intentos > 20){
            starting_radius *= 0.5f;
            if(starting_radius < MIN_RADIUS) return;
            intentos = 0;
        }
        intentos++;

    } while(!result);
    Circle* circ = new Circle(world, start_center, starting_radius, 10);
    circ->Generate();
	this->EBOs_range=circ->EBOs_range;
	this->offset=circ->offset;
	this->IsDrawable=true;
	delete circ;
}

void Pepperoni::DrawGeometry(const Matrix& parent){
	this->Shader.use();
	this->Shader.SetMatrix(parent);
	this->Shader.SetColor(ColorTable[VERDE].r,ColorTable[VERDE].g,ColorTable[VERDE].b);
	
	glDrawElements(primitive,EBOs_range.size(),GL_UNSIGNED_INT,(void*)(offset*sizeof(unsigned int)));
	
	
	this->Shader.SetColor(ColorTable[ROJO].r,ColorTable[ROJO].g,ColorTable[ROJO].b);
	glDrawElements(GL_POINTS,EBOs_range.size(),GL_UNSIGNED_INT,(void*)(offset*sizeof(unsigned int)));
}

Piña::Piña(World* world, Sector& sec, Pizza& piz, int idx):ShapeNode(world,GL_TRIANGLES,"Piña"),sectorOwn(&sec),pizzaOwn(&piz),target_index(idx) {
	ModifiedShaderColor(ColorTable[DORADO].r,ColorTable[DORADO].g,ColorTable[DORADO].b);
}

void Piña::Generate(){
	std::random_device rd;
    std::mt19937 gen(rd());
	float starting_radius=0.1f;
	float MIN_RADIUS = 0.005f;
	int intentos=0;
	Sector* pat=this->sectorOwn;
	
	Point start_center;
	float radius;
	float angulo;
	bool result=false;
	do{
		std::uniform_real_distribution<> dist_angle(pat->startPoint.angle, pat->endPoint.angle);
		angulo = dist_angle(gen);
		
		std::uniform_real_distribution<> dist_radius(0.2f, 0.4f);
		radius = dist_radius(gen);
		
		start_center=Point{radius*std::cos(angulo),radius*std::sin(angulo),0.0f,angulo};
		result = true;
		bool Cond_1 = pizzaOwn->Get_Sector({start_center.x + starting_radius, start_center.y, 0.0f, angulo}, pat->radius) == target_index;
		bool Cond_2 = pizzaOwn->Get_Sector({start_center.x - starting_radius, start_center.y, 0.0f, angulo}, pat->radius) == target_index;
		bool Cond_3 = pizzaOwn->Get_Sector({start_center.x, start_center.y + starting_radius, 0.0f, angulo}, pat->radius) == target_index;
		bool Cond_4 = pizzaOwn->Get_Sector({start_center.x, start_center.y - starting_radius, 0.0f, angulo}, pat->radius) == target_index;
		result = Cond_1 && Cond_2 && Cond_3 && Cond_4;
		if(!result && intentos > 20){
			starting_radius*=0.5f;
			if(starting_radius < MIN_RADIUS){
				return;
			}
		}
		intentos++;
	}while(!result);
	
	unsigned int tama=world->all_vertices.size()/3;
	unsigned int inicio = world->all_EBOs.size();
	world->all_vertices.push_back(start_center.x);
	world->all_vertices.push_back(start_center.y);
	world->all_vertices.push_back(start_center.z);
	
	world->all_vertices.push_back(start_center.x - starting_radius);
	world->all_vertices.push_back(start_center.y);
	world->all_vertices.push_back(start_center.z);
	
	world->all_vertices.push_back(start_center.x);
	world->all_vertices.push_back(start_center.y + starting_radius);
	world->all_vertices.push_back(start_center.z);
	
	world->all_EBOs.push_back(tama++);
	world->all_EBOs.push_back(tama++);
	world->all_EBOs.push_back(tama);
	this->offset = inicio;
	this->EBOs_range = std::vector<unsigned int>(world->all_EBOs.end()-3, world->all_EBOs.end());
	this->IsDrawable=true;
}

void Piña::DrawGeometry(const Matrix& parent){
	this->Shader.use();
	this->Shader.SetMatrix(parent);
	this->Shader.SetColor(color.r, color.g, color.b);
	
	this->Shader.SetColor(ColorTable[NARANJA].r,ColorTable[NARANJA].g,ColorTable[NARANJA].b);
	glDrawElements(primitive,EBOs_range.size(),GL_UNSIGNED_INT,(void*)(offset*sizeof(unsigned int)));
	
	this->Shader.SetColor(ColorTable[ROJO].r,ColorTable[ROJO].g,ColorTable[ROJO].b);
	glDrawElements(GL_POINTS,EBOs_range.size(),GL_UNSIGNED_INT,(void*)(offset*sizeof(unsigned int)));
}