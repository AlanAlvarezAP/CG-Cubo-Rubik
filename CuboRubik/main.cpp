/*
	Pasos basicos para OpenGL
	1. Inicializar GLFW y el perfil a usar
	2. Configurar Ventana para hacer contexto esa ventana
	3. Cargar GLAD
	4. Colocar Callbacks
	5. Bucle principal de rendering 
	5.1 Swapear buffers
	5.2 Buscar eventos
*/

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include <iostream>
#include <string>
#include <sstream>
#include <queue>
#include <map>
#include <random>

#include "Shape.h"
#include "Builder.h"
#include "Kociemba.h"
#include "solver.hpp"

World* mundito=nullptr;
GLuint VAO,VBO,EBO;
unsigned int NUM_REBANADAS=4,SELECT_REBANDA=0;
char CURRENT_AXIS = 'z';
Camera* cam=nullptr;
Animator* anim=nullptr;
Rubik* rubik=nullptr;
bool Target_free=false;
float dt=0.0f,lastX=0.0f,lastY=0.0f;

Camera_Status camMode = TARGETING;
int currentSceneIndex = 5;

// for rubik animations
class Movement{
public:
    char type; // v / h
    float angle; // 90 / -90
    int layer; // 1..6
};
std::queue<Movement> rubikAnimations;
std::queue<Movement> solutionQueue;

//------------- SECCION DE TESTS ---------------//
void tests_anim(){

	// CUIDADO CON DOBLE RELEASE
	// TEST CAMARA
	//Animation_Step* moveRobot = new Animation_Step(robot, 4.0f, 'a', 10.0f, 'x', 'W');

	// rotar la cámara 90 grados en yaw en 4 segundos
	Animation_Step* rotateCam = new Animation_Step(cam, 4.0f, 'o', 360.0f, 'y', 'W');
	Animation_Step* rotateCam1 = new Animation_Step(cam, 4.0f, 'o', 360.0f, 'y', 'W');

	// rotar la cámara 90 grados en yaw en 4 segundos
	Animation_Step* rotateCam2 = new Animation_Step(cam, 4.0f, 'o', 360.0f, 'x', 'W');

	//Pequeño movimiento para ver Z
	Animation_Step* movCam1= new Animation_Step(cam,4.0f,'a',1.0f,'x','W');
	Animation_Step* rotateCam3 = new Animation_Step(cam, 4.0f, 'o', 360.0f, 'z', 'W');

	// zoom suave
	Animation_Step* zoomCam = new Animation_Step(cam, 4.0f, 'g', -40.0f, 'z', 'W');

	anim->Add_Animations(std::vector<Animation_Step*>{rotateCam}, 'S');
	//anim->Add_Animations(std::vector<Animation_Step*>{moveRobot,rotateCam1}, 'S');
	anim->Add_Animations(std::vector<Animation_Step*>{rotateCam2}, 'S');
	anim->Add_Animations(std::vector<Animation_Step*>{movCam1,rotateCam3}, 'S');
}

void rotation_H_rubik(){

	// CUIDADO CON DOBLE RELEASE
	// TEST CAMARA

	// rotar la cámara 90 grados en yaw en 4 segundos
	Animation_Step* rotateCam = new Animation_Step(cam, 1.5f, 'o', 360.0f, 'y', 'W');

	anim->Add_Animations(std::vector<Animation_Step*>{rotateCam}, 'N');
}

void rotation_V_rubik(){
	Animation_Step* rotateCam2 = new Animation_Step(cam, 1.5f, 'o', 360.0f, 'x', 'W');

	anim->Add_Animations(std::vector<Animation_Step*>{rotateCam2}, 'N');
}

// to mess
void rubik_add_animations(int nMoves = 15) {
    while (!rubikAnimations.empty())
        rubikAnimations.pop();
    static std::random_device rd;
    static std::mt19937 gen(rd());
    // 0=v - 1=h - 2=d
    std::uniform_int_distribution<int> typeDist(0, 2);
    // 0 = -90 - 1 = 90
    std::uniform_int_distribution<int> angleDist(0, 1);
    for (int i = 0; i < nMoves; i++) {
        char type;
        int layer;
        switch (typeDist(gen)) {
            case 0:
                type = 'v';
                layer = std::uniform_int_distribution<int>(4, 6)(gen);
                break;
            case 1:
                type = 'h';
                layer = std::uniform_int_distribution<int>(1, 3)(gen);
                break;
            case 2:
                type = 'd';
                layer = std::uniform_int_distribution<int>(7, 9)(gen);
                break;
        }
        float angle = angleDist(gen) ? 90.0f : -90.0f;
        rubikAnimations.push({type, angle, layer});
    }
}
/*
void rubik_add_animations() {
	while(!rubikAnimations.empty())
		rubikAnimations.pop();

	rubikAnimations.push({'v',90,4});
	rubikAnimations.push({'v',-90,4});
	rubikAnimations.push({'h',90,1,});
	rubikAnimations.push({'h',90,2});
	rubikAnimations.push({'v',-90,5});
	rubikAnimations.push({'v',-90,6});
	rubikAnimations.push({'v',-90,5});
	rubikAnimations.push({'v',-90,6});
	rubikAnimations.push({'h',90,3});
	rubikAnimations.push({'d',90,8});
	rubikAnimations.push({'d',-90,7});
	rubikAnimations.push({'h',90,3});
	rubikAnimations.push({'h',-90,2});
	rubikAnimations.push({'h',-90,2});
	rubikAnimations.push({'h',-90,1});
	rubikAnimations.push({'d',-90,7});
	rubikAnimations.push({'d',90,8});
	rubikAnimations.push({'h',90,1});
	rubikAnimations.push({'v',-90,4});
	rubikAnimations.push({'v',-90,4});
	rubikAnimations.push({'d',-90,7});
	rubikAnimations.push({'d',90,8});
	rubikAnimations.push({'d',-90,9});
	rubikAnimations.push({'d',-90,9});
}
*/

// to solve
void load_solution_kociemba(const std::vector<std::string>& solution) {
    while(!solutionQueue.empty())
        solutionQueue.pop();

    for (const std::string& move : solution) {
        if (move.empty())
			continue;

        char face = move[0];
        char type = 'h';
        int layer = 1;
        float angle = 90.0f;
        int repetitions = 1;
		// mapping
        switch (face) {
            case 'U':
                type = 'h'; 
                layer = 3; // h_set[2]
				angle = -90.0f;
                break;
            case 'D':
                type = 'h'; 
                layer = 1; // h_set[0]
        		angle = 90.0f;
                break;
            case 'R':
                type = 'v'; 
                layer = 6; // v_set[2]
				angle = -90.0f;
                break;
            case 'L':
                type = 'v'; 
                layer = 4; // v_set[0]
        		angle = 90.0f;
                break;
            case 'F':
                type = 'd'; 
                layer = 7; // d_set[0]
				angle = -90.0f;
                break;
            case 'B':
                type = 'd'; 
                layer = 9; // d_set[2]
        		angle = 90.0f;
                break;
            default:
                continue; 
        }
		// check ' or 2
		float final_angle = angle;
        if (move.size() > 1) {
            if (move[1] == '\'')
                final_angle = -angle;
            else if (move[1] == '2') {
                repetitions = 2;
            }
        }
		// to queue
        for (int i = 0; i < repetitions; i++) {
            Movement mov;
            mov.type = type;
            mov.angle = final_angle;
            mov.layer = layer;
            solutionQueue.push(mov);
        }
    }
}
// send to kociemba
void pass_rubik_state_kociemba() {

	char normalized[55];
	std::map<char,char> remap;

	remap[rubik->facelets[4]] = 'U';
	remap[rubik->facelets[13]] = 'R';
	remap[rubik->facelets[22]] = 'F';
	remap[rubik->facelets[31]] = 'D';
	remap[rubik->facelets[40]] = 'L';
	remap[rubik->facelets[49]] = 'B';

	for(int i=0;i<54;i++)
		normalized[i] = remap[ rubik->facelets[i] ];

	normalized[54] = '\0';
	
	std::string facelets_r(rubik->facelets, 54);
	std::string facelets_n(normalized, 54);

	std::cout<<"\nRubik facelets:\n"<<facelets_r<<"\n";
	std::cout<<"\nNormalized facelets:\n"<<facelets_n<<"\n";

	Cube c = Cube::fromString(facelets_n);
	std::string sol = solve(c);

	std::vector<std::string> solution;

	std::istringstream ss(sol);
	std::string mov;

	while (ss >> mov)
		solution.push_back(mov);
	
	std::cout<<"Solution:\n";
	for(auto& s : solution)
		std::cout<<s<<"-";
	std::cout<<"\n";
	
	/*	
	std::string real_facelets(54, ' ');
	// UP
	for(int i = 0; i < 9; i++)
		real_facelets[i] = rubik->facelets[i];
	// BACK
	real_facelets[9]  = rubik->facelets[45]; real_facelets[10] = rubik->facelets[46]; real_facelets[11] = rubik->facelets[47];
	real_facelets[21] = rubik->facelets[48]; real_facelets[22] = rubik->facelets[49]; real_facelets[23] = rubik->facelets[50];
	real_facelets[33] = rubik->facelets[51]; real_facelets[34] = rubik->facelets[52]; real_facelets[35] = rubik->facelets[53];
	// LEFT
	real_facelets[12] = rubik->facelets[36]; real_facelets[13] = rubik->facelets[37]; real_facelets[14] = rubik->facelets[38];
	real_facelets[24] = rubik->facelets[39]; real_facelets[25] = rubik->facelets[40]; real_facelets[26] = rubik->facelets[41];
	real_facelets[36] = rubik->facelets[42]; real_facelets[37] = rubik->facelets[43]; real_facelets[38] = rubik->facelets[44];
	// FRONT
	real_facelets[15] = rubik->facelets[18]; real_facelets[16] = rubik->facelets[19]; real_facelets[17] = rubik->facelets[20];
	real_facelets[27] = rubik->facelets[21]; real_facelets[28] = rubik->facelets[22]; real_facelets[29] = rubik->facelets[23];
	real_facelets[39] = rubik->facelets[24]; real_facelets[40] = rubik->facelets[25]; real_facelets[41] = rubik->facelets[26];
	// RIGHT
	real_facelets[18] = rubik->facelets[9];  real_facelets[19] = rubik->facelets[10]; real_facelets[20] = rubik->facelets[11];
	real_facelets[30] = rubik->facelets[12]; real_facelets[31] = rubik->facelets[13]; real_facelets[32] = rubik->facelets[14];
	real_facelets[42] = rubik->facelets[15]; real_facelets[43] = rubik->facelets[16]; real_facelets[44] = rubik->facelets[17];
	// DOWN
	for(int i = 0; i < 9; i++)
		real_facelets[45 + i] = rubik->facelets[27 + i];
	*/
	
	/*
	KociembaSolver::initTables();
	std::string send_facelets(rubik->facelets);
	std::cout<<"\nrubik facelets:\n"<<send_facelets<<"\n";
	//std::cout<<"\ntranslated:\n"<<real_facelets<<"\n";
	std::vector<std::string> solution = KociembaSolver::solve(send_facelets);
	std::cout<<"\nsolution:\n";
	for(auto& s : solution)
		std::cout<<s<<" ";
	std::cout<<"\n";
	*/

	//std::vector<std::string> solution1{"U","D","R","L","F","B"};
	//std::vector<std::string> solution1{"U'","D'","R'","L'","F'","B'"};
	//std::vector<std::string> solution1{"U2","D2","R2","L2","F2","B2"};

	load_solution_kociemba(solution);
}

void orbit(){
	// rotar la cámara 90 grados en yaw en 4 segundos
	Animation_Step* rotateCam = new Animation_Step(cam, 4.0f, 'o', 360.0f, 'y', 'W');
	Animation_Step* rotateCam1 = new Animation_Step(cam, 4.0f, 'o', 360.0f, 'y', 'W');

	anim->Add_Animations(std::vector<Animation_Step*>{rotateCam}, 'S');
	anim->Add_Animations(std::vector<Animation_Step*>{rotateCam1}, 'S');
}

void general_Menu(){
	std::cout << "===================================" << std::endl;
    std::cout << "|        Bienvenido a             |" << std::endl;
    std::cout << "|     Simulador de Camara         |" << std::endl;
    std::cout << "|                                 |" << std::endl;
    std::cout << "|  1. Empezar animacion (Solo robot)|" << std::endl;
	std::cout << "|  2. Empezar test camara         |" << std::endl;
	std::cout << "|  xyz. Rotar ejes                |" << std::endl;
    std::cout << "|  P. Rotar obj (Respecto mundo)  |" << std::endl;
	std::cout << "|  B. Cambiar modo camara         |" << std::endl;
	std::cout << "|  E. Cambiar modo hijo escena activa|" << std::endl;
	std::cout << "|  Flechas. Mover objeto activo   |" << std::endl;
	std::cout << "|  J. Camara izquierda            |" << std::endl;
	std::cout << "|  I. Camara adelante             |" << std::endl;
	std::cout << "|  K. Camara atras                |" << std::endl;
	std::cout << "|  L. Camara derecha              |" << std::endl;
    std::cout << "|  ESC/CTRL+C. Salir              |" << std::endl;
    std::cout << "===================================" << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window,int width,int height){
	glViewport(0,0,width,height);
}
void set_Vs(){
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&VBO);
	glGenBuffers(1,&EBO);
	
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,mundito->all_vertices.size()*sizeof(float),mundito->all_vertices.data(),GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,mundito->all_EBOs.size()*sizeof(unsigned int),mundito->all_EBOs.data(),GL_DYNAMIC_DRAW);
	
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);
	
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	static bool firstMouse = true;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        return;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    if (camMode == FREE) {
        cam->ProcessMouse(xoffset, yoffset);
    }
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    cam->ProcessScroll((float)yoffset);
}

void key_callback(GLFWwindow* window,int key,int scan,int action,int mods){
	if(action != GLFW_PRESS){
		return;
	}
	
	switch(key){
		case GLFW_KEY_ESCAPE:{
			// std::cout << "ESC presionado saliendo..." << std::endl;
			glfwSetWindowShouldClose(window,GLFW_TRUE);
			break;
		}
		
		case GLFW_KEY_C:{
			if(mods & GLFW_MOD_CONTROL){
				std::cout << "CTRL+C presionado saliendo..." << std::endl;
				glfwSetWindowShouldClose(window,GLFW_TRUE);
			}
			break;
		}
		
		case GLFW_KEY_A:{
			// rubik horizontal rotation animnation
			rotation_H_rubik();
			break;
		}
		case GLFW_KEY_S:{
			// rubik vertical rotation animnation
			rotation_V_rubik();
			break;
		}
		case GLFW_KEY_Q:{
			// rubik sequence of movements
			rubik_add_animations(8);
			break;
		}
		case GLFW_KEY_W:{
			// rubik solver
			pass_rubik_state_kociemba();
			break;
		}
		
		case GLFW_KEY_1:
		case GLFW_KEY_2:
		case GLFW_KEY_3:{
			if(mundito->activeSceneNode==rubik){
				float angle = (mods & GLFW_MOD_SHIFT) ? -90.0f : 90.0f;
				rubik->horizontal_rotation(angle,key-GLFW_KEY_0,anim);
			}else{
				std::cout << "No es rubiks no se puede mover" << std::endl;
			}
			break;
		}
		case GLFW_KEY_4:
		case GLFW_KEY_5:
		case GLFW_KEY_6:{
			if(mundito->activeSceneNode==rubik){
				float angle = (mods & GLFW_MOD_SHIFT) ? -90.0f : 90.0f;
				rubik->vertical_rotation(angle,key-GLFW_KEY_0,anim);
			}else{
				std::cout << "No es rubiks no se puede mover" << std::endl;
			}
			break;
		}
		case GLFW_KEY_7:
		case GLFW_KEY_8:
		case GLFW_KEY_9:{
			if(mundito->activeSceneNode==rubik){
				float angle = (mods & GLFW_MOD_SHIFT) ? -90.0f : 90.0f;
				rubik->deep_rotation(angle,key-GLFW_KEY_0,anim);
			}else{
				std::cout << "No es rubiks no se puede mover" << std::endl;
			}
			break;
		}
		
		case GLFW_KEY_X:{
			CURRENT_AXIS='x';
			std::cout << "Eje actual: X" << std::endl;
			break;
		}
		case GLFW_KEY_Y:{
			CURRENT_AXIS='y';
			std::cout << "Eje actual: Y" << std::endl;
			break;
		}
		case GLFW_KEY_Z:{
			CURRENT_AXIS='z';
			std::cout << "Eje actual: Z" << std::endl;
			break;
		}
		case GLFW_KEY_J:{
			cam->ProcessKeyboard(LEFT, dt);
			break;
		}
		case GLFW_KEY_I:{
			cam->ProcessKeyboard(FORWARD, dt);
			break;
		}
		case GLFW_KEY_K:{
			cam->ProcessKeyboard(BACKWARD, dt);
			break;
		}
		case GLFW_KEY_L:{
			cam->ProcessKeyboard(RIGHT, dt);
			break;
		}
		case GLFW_KEY_P:{
			currentSceneIndex =(currentSceneIndex+1)%mundito->root->children.size();
			mundito->activeSceneNode=mundito->root->children[currentSceneIndex];
			std::cout << "Escena actual: "<< mundito->activeSceneNode->name << " con index " << currentSceneIndex << std::endl;
			break;
		}
		case GLFW_KEY_E:{
            mundito->activeSceneNode->SelectNextChild();
            break;
        }
		case GLFW_KEY_B: {
			if (camMode == FREE){
				// std::cout << "Cambiando a modo TARGETING " << std::endl;
				camMode = TARGETING;
				cam->UpdateCam(TARGETING,mundito->activeSceneNode->GetWorldPosition());
			}
			else{
				// std::cout << "Cambiando a modo FREE " << std::endl;
				camMode = FREE;
				cam->UpdateCam(FREE);
			}
			break;
		}
		default:{
			break;
		}
	}
	if(mundito && mundito->activeSceneNode){
		mundito->activeSceneNode->handleKey(key,mods,CURRENT_AXIS);
	}
	
}


int main(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window=glfwCreateWindow(800,800,"Animación robot",NULL,NULL);
	if(!window){
		std::cout << "Windows didn't charge" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	if(!gladLoadGL(glfwGetProcAddress)){
		std::cout << "GLAD failed :( " << std::endl;
		return -1;
	}
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glfwSetKeyCallback(window,key_callback);
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	mundito = new World();
	cam = Builder::BuildCamera();
	anim = Builder::BuildAnimator();
	
	std::cout << "CONSTRUYENDO RUBIK " << std::endl;
	auto inicio = std::chrono::high_resolution_clock::now();
	rubik=Builder::BuildRubik(mundito);
	auto fin = std::chrono::high_resolution_clock::now();
	std::cout << "BuildRubik tardo "<< std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio).count()<< " ms" <<std::endl;
	
	// Ojo aca cambiar escena inicial :D
	mundito->activeSceneNode= rubik;

	mundito->activeSceneNode->printMenu();
	//general_Menu();

	set_Vs();
	mundito->print(mundito->root);
	glEnable(GL_DEPTH_TEST);
	

	float lastTime=glfwGetTime();
	double fpsTime = 0.0;
	int fpsFrames = 0;
	//rubik->PrintCamadas();
	while(!glfwWindowShouldClose(window)){
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		float NowTime=glfwGetTime();
		dt=NowTime-lastTime;
		lastTime=NowTime;
		
		fpsTime += dt;
		fpsFrames++;

		if (fpsTime >= 1.0) {
			double fps = fpsFrames / fpsTime;

			std::string title = "Cubo Rubik OpenGL - FPS: " + std::to_string((int)fps);
			glfwSetWindowTitle(window, title.c_str());

			fpsFrames = 0;
			fpsTime = 0.0;
		}
		
		glBindVertexArray(VAO);
		glPointSize(4.0f);
		glLineWidth(4.0f);
		
		
		glfwPollEvents();
		anim->Execute_animations(dt);
		if(anim->animations.empty() && rubik->do_permutation){
			if(rubik->permutation_axi==1){ // horizontal
				int h = rubik->permutation_option - 1;
				if(rubik->permutation_clockwise)
					rubik->clockwise_permutation(rubik->horizontal_set[h]);
				else
					rubik->counterClockwise_permutation(rubik->horizontal_set[h]);
				rubik->sync_from_horizontal(h);
			} else if(rubik->permutation_axi==2) { // vertical
				int v = rubik->permutation_option - 4;
				if(rubik->permutation_clockwise)
					rubik->clockwise_permutation(rubik->vertical_set[v]);
				else
					rubik->counterClockwise_permutation(rubik->vertical_set[v]);
				rubik->sync_from_vertical(v);
			} else if(rubik->permutation_axi==3) { // deep
				int p = rubik->permutation_option - 7;
				if(rubik->permutation_clockwise)
					rubik->counterClockwise_permutation(rubik->deep_set[p]);
				else
					rubik->clockwise_permutation(rubik->deep_set[p]);
				rubik->sync_from_deep(p);
			}
			rubik->do_permutation = false;
			//rubik->PrintCamadas();

			// print facelets
			for(int i = 0; i < 54; i++) {
				if(!(i % 9))
					std::cout<<" - ";
				std::cout<<rubik->facelets[i];
			}
			std::cout<<"\n";
		}
		
		// for rubik sequence animation
		if(anim->animations.empty() && !rubik->do_permutation && !rubikAnimations.empty()){
			Movement mov = rubikAnimations.front();
			rubikAnimations.pop();
			if(mov.type == 'h')
				rubik->horizontal_rotation(mov.angle, mov.layer, anim);
			else if(mov.type == 'v')
				rubik->vertical_rotation(mov.angle, mov.layer, anim);
			else if(mov.type == 'd')
				rubik->deep_rotation(mov.angle, mov.layer, anim);
		}
		// for rubik solver animation
		if(anim->animations.empty() && !rubik->do_permutation && !solutionQueue.empty()){
			Movement mov = solutionQueue.front();
			solutionQueue.pop();
			if(mov.type == 'h')
				rubik->horizontal_rotation(mov.angle, mov.layer, anim);
			else if(mov.type == 'v')
				rubik->vertical_rotation(mov.angle, mov.layer, anim);
			else if(mov.type == 'd')
				rubik->deep_rotation(mov.angle, mov.layer, anim);
		}
		
		// Para seguir
		if (camMode == TARGETING) {
			cam->UpdateCam(TARGETING, mundito->activeSceneNode->GetWorldPosition());
		}
		else {
			cam->UpdateCam(FREE);
		}
        mundito->DrawShape(cam->GetLookAt(),cam->GetProjection(800.0f, 800.0f, 0.1f, 100.0f));
		
		glBindVertexArray(0);
		
        glfwSwapBuffers(window);
        
    }
	delete mundito;
	return 0;
}
