#include "Rubik.h"

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

Rubik::Rubik(World* world,const Point &center) :
	ShapeNode(world,GL_TRIANGLES,"Cubo Rubik"),
	centro(center),
	do_permutation(false),
	permutation_option(0),
	permutation_axi(1)
	{
		animation_time = 0.25f;
		/*
		for(int i = 0; i < 9; i++) {
			facelets[i] = '0'; // UP
			facelets[i + 9] = '1'; // RIGHT
			facelets[i + 18] = '2'; // FRONT
			facelets[i + 27] = '3'; // DOWN
			facelets[i + 36] = '4'; // LEFT
			facelets[i + 45] = '5'; // BACK
		}
		*/
		for(int i = 0; i < 9; i++) {
			facelets[i] = 'U';
			facelets[i + 9] = 'R';
			facelets[i + 18] = 'F';
			facelets[i + 27] = 'D';
			facelets[i + 36] = 'L';
			facelets[i + 45] = 'B';
		}
		facelets[54]='\0';
	}

// Camadas
// Horizontal -> 0
// Vertical -> 1

// Tipos
// Esquinas -> 0 (3 colores)
// Medio centrados -> 1 (2 colores)
// Centro -> 2 (1 color)
void Rubik::Generate() {
	
	int types[27]={
		ESQUINA,MEDIO_CENTRO,ESQUINA,MEDIO_CENTRO,ESQUINA,
		MEDIO_CENTRO,ESQUINA,MEDIO_CENTRO,MEDIO_CENTRO,CENTRO,
		MEDIO_CENTRO,CENTRO,MEDIO_CENTRO,CENTRO,MEDIO_CENTRO,
		CENTRO,ESQUINA,MEDIO_CENTRO,ESQUINA,MEDIO_CENTRO,
		ESQUINA,MEDIO_CENTRO,ESQUINA,MEDIO_CENTRO,
		CENTRO,CENTRO,CENTRO
	};
	
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
		Cubee* tmp=new Cubee(this->world,{0.0f,0.0f,0.0f},rubikColors[i],types[i],std::string{"Cubee"+std::to_string(i)});
		tmp->Generate();
		cubes.push_back(tmp);
		this->AddChildren(tmp);
	}

	horizontal_set.resize(3);
	vertical_set.resize(3);
	deep_set.resize(3);

	// Añadiendo todos los cubos
	// Actualizando pos
	// front
	cubes[0]->Mat.UpdateView('a',-0.42,-0.42, 0.42,'x','W');
	cubes[1]->Mat.UpdateView('a',-0.42, 0.0 , 0.42,'x','W');
	cubes[2]->Mat.UpdateView('a',-0.42, 0.42, 0.42,'x','W');

	cubes[8]->Mat.UpdateView( 'a',0.0,-0.42, 0.42,'x','W');
	cubes[9]->Mat.UpdateView( 'a',0.0, 0.0 , 0.42,'x','W');
	cubes[10]->Mat.UpdateView('a',0.0, 0.42, 0.42,'x','W');

	cubes[16]->Mat.UpdateView('a',0.42,-0.42, 0.42,'x','W');
	cubes[17]->Mat.UpdateView('a',0.42, 0.0 , 0.42,'x','W');
	cubes[18]->Mat.UpdateView('a',0.42, 0.42, 0.42,'x','W');

	// mid
	cubes[7]->Mat.UpdateView( 'a',-0.42,-0.42, 0.0,'x','W');
	cubes[25]->Mat.UpdateView('a',-0.42, 0.0 , 0.0,'x','W');
	cubes[3]->Mat.UpdateView( 'a',-0.42, 0.42, 0.0,'x','W');

	cubes[15]->Mat.UpdateView('a',0.0,-0.42, 0.0,'x','W');
	cubes[26]->Mat.UpdateView('a',0.0, 0.0 , 0.0,'x','W');
	cubes[11]->Mat.UpdateView('a',0.0, 0.42, 0.0,'x','W');

	cubes[23]->Mat.UpdateView('a',0.42,-0.42, 0.0,'x','W');
	cubes[24]->Mat.UpdateView('a',0.42, 0.0 , 0.0,'x','W');
	cubes[19]->Mat.UpdateView('a',0.42, 0.42, 0.0,'x','W');

	// back
	cubes[6]->Mat.UpdateView('a',-0.42,-0.42,-0.42,'x','W');
	cubes[5]->Mat.UpdateView('a',-0.42, 0.0 ,-0.42,'x','W');
	cubes[4]->Mat.UpdateView('a',-0.42, 0.42,-0.42,'x','W');

	cubes[14]->Mat.UpdateView('a',0.0,-0.42,-0.42,'x','W');
	cubes[13]->Mat.UpdateView('a',0.0, 0.0 ,-0.42,'x','W');
	cubes[12]->Mat.UpdateView('a',0.0, 0.42,-0.42,'x','W');

	cubes[22]->Mat.UpdateView('a',0.42,-0.42,-0.42,'x','W');
	cubes[21]->Mat.UpdateView('a',0.42, 0.0 ,-0.42,'x','W');
	cubes[20]->Mat.UpdateView('a',0.42, 0.42,-0.42,'x','W');

	// DUMMY centrado en el medio
	
	// front - green
	// left - orange
	// up - yellow

	// Camada CH1 - bottom
	horizontal_set[0]={
		// mini-Ch1
		cubes[6],cubes[7],cubes[0],
		// mini-Ch2
		cubes[14],cubes[15],cubes[8],
		// mini-Ch3
		cubes[22],cubes[23],cubes[16]
	};

	// Camada CH2 - mid
	horizontal_set[1]={
		// mini-Ch1
		cubes[5],cubes[25],cubes[1],
		// mini-Ch2
		cubes[13],cubes[26],cubes[9],
		// mini-Ch3
		cubes[21],cubes[24],cubes[17]
	};

	// Camada CH3 - up
	horizontal_set[2]={
		// mini-Ch1
		cubes[4],cubes[3],cubes[2],
		// mini-Ch2
		cubes[12],cubes[11],cubes[10],
		// mini-Ch3
		cubes[20],cubes[19],cubes[18]
	};
	
	// Camada CV1 - left
	vertical_set[0]={
		// mini-Cv1
		cubes[0],cubes[7],cubes[6],
		// mini-Cv2
		cubes[1],cubes[25],cubes[5],
		// mini-Cv3
		cubes[2],cubes[3],cubes[4]
	};
	
	// Camada CV2 - mid
	vertical_set[1]={
		// mini-Cv1
		cubes[8],cubes[15],cubes[14],
		// mini-Cv2
		cubes[9],cubes[26],cubes[13],
		// mini-Cv3
		cubes[10],cubes[11],cubes[12]
	};
	
	// Camada CV3 - right
	vertical_set[2]={
		// mini-Cv1
		cubes[16],cubes[23],cubes[22],
		// mini-Cv2
		cubes[17],cubes[24],cubes[21],
		// mini-Cv3
		cubes[18],cubes[19],cubes[20]
	};
	
	// Camada CP1 - front
	deep_set[0]={
		// mini-CP1
		cubes[16],cubes[8],cubes[0],
		// mini-CP2
		cubes[17],cubes[9],cubes[1],
		// mini-CP3
		cubes[18],cubes[10],cubes[2],
	};
	
	// Camada CP2 - mid
	deep_set[1]={
		// mini-CP1
		cubes[23],cubes[15],cubes[7],
		// mini-CP2
		cubes[24],cubes[26],cubes[25],
		// mini-CP3
		cubes[19],cubes[11],cubes[3],
	};
	
	// Camada CP2 - back
	deep_set[2]={
		// mini-CP1
		cubes[22],cubes[14],cubes[6],
		// mini-CP2
		cubes[21],cubes[13],cubes[5],
		// mini-CP3
		cubes[20],cubes[12],cubes[4],
	};
}

void Rubik::clockwise_permutation(std::vector<Cubee*> &cam){ // horario
	std::vector<Cubee*> tmp=cam;
	cam[2]=tmp[0];
	cam[5]=tmp[1];
	cam[8]=tmp[2];
	cam[1]=tmp[3];
	cam[7]=tmp[5];
	cam[0]=tmp[6];
	cam[3]=tmp[7];
	cam[6]=tmp[8];
}

void Rubik::counterClockwise_permutation(std::vector<Cubee*> &cam){ // antihorario
	std::vector<Cubee*> tmp=cam;
	cam[6]=tmp[0];
	cam[3]=tmp[1];
	cam[0]=tmp[2];
	cam[7]=tmp[3];
	cam[1]=tmp[5];
	cam[8]=tmp[6];
	cam[5]=tmp[7];
	cam[2]=tmp[8];
}
/*
void Rubik::Update_contrary(int option,int offset,std::vector<Cubee*> &camada_changed,std::vector<std::vector<Cubee*>> &camadas){
	int block=option-offset;
	for(int i=0;i<camadas.size();i++){ 
		auto& row=camadas[i];
		for(int j=0;j<3;j++){ 
			int col=j+(block*3);
			if(col>=0 && col<row.size()){
                row[col]=camada_changed[(i*3)+j];
			}else{
				std::cout << "Acceso ilegal cuidao" << std::endl;
			}
		} 
	}
}
*/
// deep and vertical
void Rubik::sync_from_horizontal(int h_idx) {
    for(int v = 0; v < 3; v++) {
        for(int d = 0; d < 3; d++) {
            Cubee* c = horizontal_set[h_idx][v * 3 + (2 - d)]; 
            vertical_set[v][h_idx * 3 + d] = c;
            deep_set[d][h_idx * 3 + (2 - v)] = c;
        }
    }
}

// deep and horizontal
void Rubik::sync_from_vertical(int v_idx) {
    for(int h = 0; h < 3; h++) {
        for(int d = 0; d < 3; d++) {
            Cubee* c = vertical_set[v_idx][h * 3 + d];
            horizontal_set[h][v_idx * 3 + (2 - d)] = c;
            deep_set[d][h * 3 + (2 - v_idx)] = c;
        }
    }
}

// vertical and horizontal
void Rubik::sync_from_deep(int d_idx) {
    for(int h = 0; h < 3; h++) {
        for(int v = 0; v < 3; v++) {
            Cubee* c = deep_set[d_idx][h * 3 + (2 - v)];
            horizontal_set[h][v * 3 + (2 - d_idx)] = c;
            vertical_set[v][h * 3 + d_idx] = c;
        }
    }
}

void Rubik::horizontal_rotation(float value_rot,int option,Animator* &anim){
	if(!anim->animations.empty())
        return;
	if(do_permutation)
		return;
	if(option <= 0 || option > 3){
		std::cout << "Opcion de rotacion vertical invalida :( " << std::endl;
		return;
	}
	
	std::vector<Animation_Step*> steps;
	for(auto* cube : horizontal_set[option-1])
		steps.push_back(new Animation_Step(cube, animation_time, 'd', value_rot, 'y', 'W'));
	anim->Add_Animations(steps, 'N');
	
	do_permutation = true;
    permutation_option = option;
    permutation_axi = 1;
	permutation_clockwise = (value_rot > 0);

	update_facelets_from_horizontal(option-1);
}

void Rubik::vertical_rotation(float value_rot,int option,Animator* &anim){
	if(!anim->animations.empty())
        return;
	if(do_permutation)
		return;
	if(option <= 3 || option > 6){
		std::cout << "Opcion de rotacion horizontal invalida :( " << std::endl;
		return;
	}
	
	std::vector<Animation_Step*> steps;
	for(auto* cube : vertical_set[option-4])
		steps.push_back(new Animation_Step(cube, animation_time, 'd', value_rot, 'x', 'W'));
	anim->Add_Animations(steps, 'N');

	do_permutation = true;
    permutation_option = option;
    permutation_axi = 2;
	permutation_clockwise = (value_rot > 0);

	update_facelets_from_vertical(option-4);
}

void Rubik::deep_rotation(float value_rot,int option,Animator* &anim){
	if(!anim->animations.empty())
        return;
	if(do_permutation)
		return;
	if(option <= 6 || option > 9){
		std::cout << "Opcion de rotacion profunda invalida :( " << std::endl;
		return;
	}
	
	std::vector<Animation_Step*> steps;
	for(auto* cube : deep_set[option-7])
		steps.push_back(new Animation_Step(cube, animation_time, 'd', value_rot, 'z', 'W'));
	anim->Add_Animations(steps, 'N');
	
	do_permutation = true;
    permutation_option = option;
    permutation_axi = 3;
	permutation_clockwise = (value_rot > 0);

	update_facelets_from_deep(option-7);
}

void Rubik::update_facelets_from_horizontal(int idx) {
    char tmp[54];
    for(int i = 0; i < 54; i++) tmp[i] = facelets[i];

    bool cw = permutation_clockwise;

    auto rotate_face = [&](int base, bool is_cw) {
        if (is_cw) {
            facelets[base+0]=tmp[base+6]; facelets[base+1]=tmp[base+3]; facelets[base+2]=tmp[base+0];
            facelets[base+3]=tmp[base+7]; facelets[base+4]=tmp[base+4]; facelets[base+5]=tmp[base+1];
            facelets[base+6]=tmp[base+8]; facelets[base+7]=tmp[base+5]; facelets[base+8]=tmp[base+2];
        } else {
            facelets[base+0]=tmp[base+2]; facelets[base+1]=tmp[base+5]; facelets[base+2]=tmp[base+8];
            facelets[base+3]=tmp[base+1]; facelets[base+4]=tmp[base+4]; facelets[base+5]=tmp[base+7];
            facelets[base+6]=tmp[base+0]; facelets[base+7]=tmp[base+3]; facelets[base+8]=tmp[base+6];
        }
    };

    auto rotate_ring = [&](const int r1[3], const int r2[3], const int r3[3], const int r4[3], bool is_cw) {
        if (is_cw) {
            for(int i=0; i<3; i++) {
                facelets[r2[i]]=tmp[r1[i]]; facelets[r3[i]]=tmp[r2[i]];
                facelets[r4[i]]=tmp[r3[i]]; facelets[r1[i]]=tmp[r4[i]];
            }
        } else {
            for(int i=0; i<3; i++) {
                facelets[r4[i]]=tmp[r1[i]]; facelets[r3[i]]=tmp[r4[i]];
                facelets[r2[i]]=tmp[r3[i]]; facelets[r1[i]]=tmp[r2[i]];
            }
        }
    };

    if (idx == 0) { // Down - face 27
        const int r1[3]={24,25,26}, r2[3]={15,16,17}, r3[3]={51,52,53}, r4[3]={42,43,44};
        rotate_ring(r1, r2, r3, r4, cw);
        rotate_face(27, cw);
    } 
    else if (idx == 1) { // Mid (E)
        const int r1[3]={21,22,23}, r2[3]={12,13,14}, r3[3]={48,49,50}, r4[3]={39,40,41};
        rotate_ring(r1, r2, r3, r4, cw);
    } 
    else if (idx == 2) { // Up - face 0
        const int r1[3]={18,19,20}, r2[3]={9,10,11}, r3[3]={45,46,47}, r4[3]={36,37,38};
        rotate_ring(r1, r2, r3, r4, cw);
        rotate_face(0, !cw); // opuesto
    }
}

void Rubik::update_facelets_from_vertical(int idx) {
    char tmp[54];
    for(int i = 0; i < 54; i++) tmp[i] = facelets[i];

    bool cw = permutation_clockwise;

    auto rotate_face = [&](int base, bool is_cw) {
        if (is_cw) {
            facelets[base+0]=tmp[base+6]; facelets[base+1]=tmp[base+3]; facelets[base+2]=tmp[base+0];
            facelets[base+3]=tmp[base+7]; facelets[base+4]=tmp[base+4]; facelets[base+5]=tmp[base+1];
            facelets[base+6]=tmp[base+8]; facelets[base+7]=tmp[base+5]; facelets[base+8]=tmp[base+2];
        } else {
            facelets[base+0]=tmp[base+2]; facelets[base+1]=tmp[base+5]; facelets[base+2]=tmp[base+8];
            facelets[base+3]=tmp[base+1]; facelets[base+4]=tmp[base+4]; facelets[base+5]=tmp[base+7];
            facelets[base+6]=tmp[base+0]; facelets[base+7]=tmp[base+3]; facelets[base+8]=tmp[base+6];
        }
    };

    auto rotate_ring = [&](const int r1[3], const int r2[3], const int r3[3], const int r4[3], bool is_cw) {
        if (is_cw) {
            for(int i=0; i<3; i++) {
                facelets[r2[i]]=tmp[r1[i]]; facelets[r3[i]]=tmp[r2[i]];
                facelets[r4[i]]=tmp[r3[i]]; facelets[r1[i]]=tmp[r4[i]];
            }
        } else {
            for(int i=0; i<3; i++) {
                facelets[r4[i]]=tmp[r1[i]]; facelets[r3[i]]=tmp[r4[i]];
                facelets[r2[i]]=tmp[r3[i]]; facelets[r1[i]]=tmp[r2[i]];
            }
        }
    };

    if (idx == 0) { // Left - face 36
        const int r1[3]={0,3,6}, r2[3]={18,21,24}, r3[3]={27,30,33}, r4[3]={53,50,47};
        rotate_ring(r1, r2, r3, r4, cw);
        rotate_face(36, cw);
    } 
    else if (idx == 1) { // Mid (M)
        const int r1[3]={1,4,7}, r2[3]={19,22,25}, r3[3]={28,31,34}, r4[3]={52,49,46};
		//printf("\ntest M movement\n");
        //const int r4[3]={46,49,52};
        rotate_ring(r1, r2, r3, r4, cw);
    } 
    else if (idx == 2) { // Right - face 9
        const int r1[3]={2,5,8}, r2[3]={20,23,26}, r3[3]={29,32,35}, r4[3]={51,48,45};
        rotate_ring(r1, r2, r3, r4, cw);
        rotate_face(9, !cw); 
    }
}

void Rubik::update_facelets_from_deep(int idx) {
    char tmp[54];
    for(int i = 0; i < 54; i++) tmp[i] = facelets[i];

    bool cw = permutation_clockwise;

    auto rotate_face = [&](int base, bool is_cw) {
        if (is_cw) {
            facelets[base+0]=tmp[base+6]; facelets[base+1]=tmp[base+3]; facelets[base+2]=tmp[base+0];
            facelets[base+3]=tmp[base+7]; facelets[base+4]=tmp[base+4]; facelets[base+5]=tmp[base+1];
            facelets[base+6]=tmp[base+8]; facelets[base+7]=tmp[base+5]; facelets[base+8]=tmp[base+2];
        } else {
            facelets[base+0]=tmp[base+2]; facelets[base+1]=tmp[base+5]; facelets[base+2]=tmp[base+8];
            facelets[base+3]=tmp[base+1]; facelets[base+4]=tmp[base+4]; facelets[base+5]=tmp[base+7];
            facelets[base+6]=tmp[base+0]; facelets[base+7]=tmp[base+3]; facelets[base+8]=tmp[base+6];
        }
    };

    auto rotate_ring = [&](const int r1[3], const int r2[3], const int r3[3], const int r4[3], bool is_cw) {
        if (is_cw) {
            for(int i=0; i<3; i++) {
                facelets[r2[i]]=tmp[r1[i]]; facelets[r3[i]]=tmp[r2[i]];
                facelets[r4[i]]=tmp[r3[i]]; facelets[r1[i]]=tmp[r4[i]];
            }
        } else {
            for(int i=0; i<3; i++) {
                facelets[r4[i]]=tmp[r1[i]]; facelets[r3[i]]=tmp[r4[i]];
                facelets[r2[i]]=tmp[r3[i]]; facelets[r1[i]]=tmp[r2[i]];
            }
        }
    };

    if (idx == 0) { // Front - face 18
        const int r1[3]={6,7,8}, r2[3]={44,41,38}, r3[3]={29,28,27}, r4[3]={9,12,15};
        rotate_ring(r1, r2, r3, r4, cw);
        rotate_face(18, !cw); 
    } 
    else if (idx == 1) { // Standing (S)
        const int r1[3]={3,4,5}, r2[3]={43,40,37}, r3[3]={32,31,30}, r4[3]={10,13,16};
        rotate_ring(r1, r2, r3, r4, cw);
    } 
    else if (idx == 2) { // Back - face 45
        const int r1[3]={0,1,2}, r2[3]={42,39,36}, r3[3]={35,34,33}, r4[3]={11,14,17};
        rotate_ring(r1, r2, r3, r4, cw);
        rotate_face(45, cw);
    }
}

void Rubik::handleKey(int key, int mods,char CURRENT_AXIS){
	ShapeNode* target = this;

    if(selected_part != -1)
        target = children[selected_part];
	
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
	std::cout << "=======================================" << std::endl;
    std::cout << "|           Bienvenido a              |" << std::endl;
    std::cout << "|        Simulador de Rubik           |" << std::endl;
    std::cout << "|                                     |" << std::endl;
    std::cout << "|     1. Mover Camada Horizontal 1    |" << std::endl;
    std::cout << "|     2. Mover Camada Horizontal 2    |" << std::endl;
    std::cout << "|     3. Mover Camada Horizontal 3    |" << std::endl;
	std::cout << "|     4. Mover Camada Vertical 1      |" << std::endl;
	std::cout << "|     5. Mover Camada Vertical 2      |" << std::endl;
	std::cout << "|     6. Mover Camada Vertical 3      |" << std::endl;
	std::cout << "|     7. Mover Camada Profunda 1      |" << std::endl;
	std::cout << "|     8. Mover Camada Profunda 2      |" << std::endl;
	std::cout << "|     9. Mover Camada Profunda 3      |" << std::endl;
	std::cout << "|     A. Animacion horizontal         |" << std::endl;
	std::cout << "|     S. Animacion vertical           |" << std::endl;
	std::cout << "|     Q. Animacion de camadas         |" << std::endl;
    std::cout << "|     ESC/CTRL+C. Salir               |" << std::endl;
    std::cout << "=======================================" << std::endl;
}

void Rubik::PrintCamadas(){
	std::cout << "-------------------------------" << std::endl;
	std::cout << " PARA CAMADAS VERTICALES " << std::endl;
	for(int i=0;i<horizontal_set.size();i++){
		std::cout << "Camada vertical -> " << i << std::endl;
		for(int j=0;j<horizontal_set[i].size();j++){
			std::cout << "Cubo # " << horizontal_set[i][j]->name << std::endl;
		}
	}
	std::cout << " PARA CAMADAS HORIZONTAL " << std::endl;
	for(int i=0;i<vertical_set.size();i++){
		std::cout << "Camada horizontal -> " << i << std::endl;
		for(int j=0;j<vertical_set[i].size();j++){
			std::cout << "Cubo # " << vertical_set[i][j]->name << std::endl;
		}
	}
	
	std::cout << " PARA CAMADAS PROFUNDAS " << std::endl;
	for(int i=0;i<deep_set.size();i++){
		std::cout << "Camada horizontal -> " << i << std::endl;
		for(int j=0;j<deep_set[i].size();j++){
			std::cout << "Cubo # " << deep_set[i][j]->name << std::endl;
		}
	}
	std::cout << "-------------------------------" << std::endl;
}