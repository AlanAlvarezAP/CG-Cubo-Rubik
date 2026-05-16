#include "Trapecio.h"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

Trapecio::Trapecio(World* world):ShapeNode(world,GL_TRIANGLES,"Trapecio"){
	ModifiedShaderColor(ColorTable[ROJO].r,ColorTable[ROJO].g,ColorTable[ROJO].b);
}

void Trapecio::Generate(){
	
	
	std::vector<float> vertices;
    std::vector<unsigned int> indices;

    std::vector<unsigned int> lineIdx;
    std::vector<unsigned int> pointIdx;

	unsigned int base = world->all_vertices.size() / 3;

    vertices.insert(vertices.end(), {-0.2f,0.0f, 0.0f, // Esquina izq inf
	-0.1f, 0.4f, 0.0f, // Esquina izq arriba
	0.2,0.0f,0.0f, // Esquina der arriba
	0.1f,0.4f,0.0f}); // Esquina der inf
    indices = {base, base+1, base+3,base, base+2, base+3};
    lineIdx = {base, base+1,base+1, base+3,base+3, base+2,base+2, base};
    pointIdx = {base, base+1, base+2, base+3};

    sector_Start = 0;
    lines_Start = indices.size();
    points_Start = lines_Start + lineIdx.size();

    indices.insert(indices.end(), lineIdx.begin(), lineIdx.end());
    indices.insert(indices.end(), pointIdx.begin(), pointIdx.end());

    EBOs_range = world->Add_Batch(vertices, indices, offset);
    IsDrawable = true;


	
}

void Trapecio::DrawGeometry(const Matrix& parent){
	Shader.use();
    Shader.SetMatrix(parent);

    for(int i = sector_Start, t = 0; i < lines_Start; i += 6, t++) {
        this->Shader.SetColor(ColorTable[NARANJA].r,ColorTable[NARANJA].g,ColorTable[NARANJA].b);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,(void*)((offset + i) * sizeof(unsigned int)));
    }

    for(int i = lines_Start, l = 0; i < points_Start; i += 2, l++) {
        this->Shader.SetColor(ColorTable[VERDE].r,ColorTable[VERDE].g,ColorTable[VERDE].b);

        glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT,(void*)((offset + i) * sizeof(unsigned int)));
    }

    for(int i = points_Start, p = 0; i < EBOs_range.size(); i += 1, p++) {
        this->Shader.SetColor(ColorTable[ROJO].r,ColorTable[ROJO].g,ColorTable[ROJO].b);

        glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT,(void*)((offset + i) * sizeof(unsigned int)));
    }
	
	
}


void Trapecio::handleKey(int key, int mods,char CURRENT_AXIS){
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

void Trapecio::printMenu(){
	std::cout << "===================================" << std::endl;
    std::cout << "|        Bienvenido a             |" << std::endl;
    std::cout << "|     Simulador de Trapecio           |" << std::endl;
    std::cout << "|                                 |" << std::endl;
    std::cout << "|  Q. Generar Trapecio (default=4)    |" << std::endl;
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
