#include "Cube.h"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

Face::Face(World* world, Point A, Point B, Point C, Point D):ShapeNode(world, GL_TRIANGLES, "Face"),a(A), b(B), c(C), d(D),sector_Start(0), lines_Start(0), points_Start(0) {}


void Face::Generate() {

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    std::vector<unsigned int> lineIdx;
    std::vector<unsigned int> pointIdx;

    unsigned int base = world->all_vertices.size() / 3;

    vertices.insert(vertices.end(), {a.x, a.y, a.z,b.x, b.y, b.z,c.x, c.y, c.z,d.x, d.y, d.z});
    indices = {base, base+1, base+2,base, base+2, base+3};
    lineIdx = {base, base+1,base+1, base+2,base+2, base+3,base+3, base};
    pointIdx = {base, base+1, base+2, base+3};

    sector_Start = 0;
    lines_Start = indices.size();
    points_Start = lines_Start + lineIdx.size();

    indices.insert(indices.end(), lineIdx.begin(), lineIdx.end());
    indices.insert(indices.end(), pointIdx.begin(), pointIdx.end());

    EBOs_range = world->Add_Batch(vertices, indices, offset);
    IsDrawable = true;

    int &idx = world->globalColorCounter;
    const int COLOR_COUNT = ARENA + 1;

    //triColors.clear(); // elimina el envio del vector
    lineColors.clear();
    pointColors.clear();

	if(triColors.empty()) {
		triColors.push_back(ColorTable[idx++ % COLOR_COUNT]);
		triColors.push_back(ColorTable[idx++ % COLOR_COUNT]);
	}

    lineColors.push_back(ColorTable[idx++ % COLOR_COUNT]);
    lineColors.push_back(ColorTable[idx++ % COLOR_COUNT]);
    lineColors.push_back(ColorTable[idx++ % COLOR_COUNT]);
    lineColors.push_back(ColorTable[idx++ % COLOR_COUNT]);

    pointColors.push_back(ColorTable[9]);
    pointColors.push_back(ColorTable[9]);
    pointColors.push_back(ColorTable[9]);
    pointColors.push_back(ColorTable[9]);
}

void Face::DrawGeometry(const Matrix& parent) {

    Shader.use();
    Shader.SetMatrix(parent);

    for(int i = sector_Start, t = 0; i < lines_Start; i += 6, t++) {
        RGB c = triColors[t % triColors.size()];
        Shader.SetColor(c.r, c.g, c.b);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,(void*)((offset + i) * sizeof(unsigned int)));
    }

    for(int i = lines_Start, l = 0; i < points_Start; i += 2, l++) {
        RGB c = ColorTable[NEGRO];
        Shader.SetColor(c.r, c.g, c.b);

        glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT,(void*)((offset + i) * sizeof(unsigned int)));
    }

	
    for(int i = points_Start, p = 0; i < EBOs_range.size(); i += 1, p++) {
        RGB c = pointColors[p % pointColors.size()];
        Shader.SetColor(c.r, c.g, c.b);

        glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT,(void*)((offset + i) * sizeof(unsigned int)));
    }
	
}

Cube::Cube(World* world, const Point& cent, std::vector<RGB> colors, int tp, std::string name) :
	ShapeNode(world, 0, name), center(cent), triColors(colors), sector_Start(0), lines_Start(0), points_Start(0), type(tp) {}

void Cube::Generate() {

    float s = 0.2f;

    Point v[8];

    v[0] = {center.x - s, center.y - s, center.z - s};
    v[1] = {center.x + s, center.y - s, center.z - s};
    v[2] = {center.x + s, center.y + s, center.z - s};
    v[3] = {center.x - s, center.y + s, center.z - s};

    v[4] = {center.x - s, center.y - s, center.z + s};
    v[5] = {center.x + s, center.y - s, center.z + s};
    v[6] = {center.x + s, center.y + s, center.z + s};
    v[7] = {center.x - s, center.y + s, center.z + s};

    int facesIdx[6][4] = {{0,1,2,3},{4,5,6,7},{0,1,5,4},{2,3,7,6},{0,3,7,4},{1,2,6,5}};

    for(int i = 0; i < 6; i++) {
        Face* f = new Face(world,v[facesIdx[i][0]],v[facesIdx[i][1]],v[facesIdx[i][2]],v[facesIdx[i][3]]);
		f->triColors = {triColors[i],triColors[i]};
        this->AddChildren(f);
        f->Generate();
    }
}

void Cube::handleKey(int key, int mods,char CURRENT_AXIS){
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

void Cube::printMenu(){
	std::cout << "===================================" << std::endl;
    std::cout << "|        Bienvenido a             |" << std::endl;
    std::cout << "|     Simulador de Cube           |" << std::endl;
    std::cout << "|                                 |" << std::endl;
    std::cout << "|  Q. Generar Cube (default=4)    |" << std::endl;
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
