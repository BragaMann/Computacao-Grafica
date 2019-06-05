#include "headers/Struct.h"

Struct::Struct(){
}

Struct::Struct(vector<Point*> L) {
	LP = L;
}

vector<Point*> Struct::getLP() {
	return LP;
}
	
void Struct::setLP(vector<Point*> L) {
	LP = L;
}

//Responsavel por gerar os pontos dos (2) tri�ngulos pertencentes ao plano.
void Struct::genPlane(float size)
{
	float h = size/2;
	LP.push_back(new Point(-h, 0, h));
	LP.push_back(new Point(h, 0, h));
	LP.push_back(new Point(-h, 0, -h));
	LP.push_back(new Point(-h, 0, -h));
	LP.push_back(new Point(h, 0, h));
	LP.push_back(new Point(h, 0, -h));
}

//Responsavel por gerar os pontos dos tri�ngulos relativos ao cilindro.
void Struct::genCylinder(float radius, float height, int slices) {
	float beta = atan(radius / height);
	float alfa = (float) (2 * (M_PI / slices));
	for (int i = 0; i < slices; i++) {
		float x0 = sin((i + 1)*alfa)*radius;
		float x1 = sin(i*alfa)*radius;
		float z0 = cos((i + 1)*alfa)*radius;
		float z1 = cos(i*alfa)*radius;

		LP.push_back(new Point(x0, -height / 2, z0));
		LP.push_back(new Point(x1, -height / 2, z1));
		LP.push_back(new Point(0.0f, -height / 2, 0.0f));

		LP.push_back(new Point(0.0f, height / 2, 0.0f));
		LP.push_back(new Point(x1, height / 2, z1));
		LP.push_back(new Point(x0, height / 2, z0));
		
		LP.push_back(new Point(x0, -height / 2, z0));
		LP.push_back(new Point(x0, height / 2, z0));
		LP.push_back(new Point(x1, height / 2, z1));
		
		LP.push_back(new Point(x1, -height / 2, z1));
		LP.push_back(new Point(x0, -height / 2, z0));
		LP.push_back(new Point(x1, height / 2, z1));
	}
}

float xxx = 0, yyy = 0, zzz = 0;

// Responsavel por gerar os pontos dos triangulos relativos a esfara.
void Struct::genSphere(float radius, int slices, int stacks) {

	float beta = (float) (M_PI) / stacks; //angulo beta
	float alfa = (float) (2 * M_PI) / slices; //angulo alfa

	//para cada slice percorrem-se as respectivas stacks
	for (int i = 0; i < slices; i++) {

		//calculam-se os pontos por slice
		for (int j = 0; j < stacks; j++) {


			//s� foram consideradas 
			// express�es que se repitam mais do que uma 
			// vez por "if"
			//porque, salvo certas excep��es, 
			// o mesmo "if" � executado uma vez por ciclo
			float x1 = radius * cos((j + 1)*beta);
			float x2 = radius * cos(i*alfa)*sin((j + 1)*beta);
			float x3 = radius * sin(i*alfa)*sin((j + 1)*beta);
			float x4 = radius * cos((j + 1)*beta);
			float x5 = radius * cos((i + 1)*alfa)*sin((j + 2)*beta);
			float x6 = radius * cos((j + 2)*beta);
			float x7 = radius * sin((i + 1)*alfa)*sin((j + 2)*beta);

			//come�a-se a construir a esfera por cima
			//j = 0 corresponde ao topo (polo norte) da esfera 
			//porque como podemos ver (0,radius,0)
			//o que indica que estamos no topo
			if (j == 0) {

				LP.push_back(new Point(0+xxx, radius+yyy, 0+zzz));
				LP.push_back(new Point(radius*cos((i + 1)*alfa)*sin((j + 1)*beta)+xxx, x1+yyy, radius*sin((i + 1)*alfa)*sin((j + 1)*beta) +zzz));
				LP.push_back(new Point(x2+xxx, x1+yyy, x3+zzz));
			}

			//estamos no polo sul da esfera
			if (j == stacks - 1) {

				LP.push_back(new Point(0+xxx, -radius+yyy, 0+zzz));
				LP.push_back(new Point(x2 + radius*cos(i*alfa)*sin(beta) + xxx, -(radius*cos(beta)) + yyy, x3 + radius*sin(i*alfa)*sin(beta) + zzz));
				LP.push_back(new Point(radius*cos((i + 1)*alfa)*sin((j + 1)*beta) + radius*cos((i + 1)*alfa)*sin(beta) + xxx,
										-(radius * cos(beta)) + yyy, 
										radius*sin((i + 1)*alfa)*sin((j + 1)*beta) + radius * sin((i + 1)*alfa)*sin(beta) + zzz));
			}//para os n�veis intermedios entre os polos
			else {

				LP.push_back(new Point(radius*cos((i + 1)*alfa)*sin((j + 1)*beta) + xxx, x4+yyy, radius*sin((i + 1)*alfa)*sin((j + 1)*beta)+zzz));
				LP.push_back(new Point(x5+xxx, x6+yyy, x7+zzz));
				LP.push_back(new Point(x2+xxx, x4+yyy, x3+zzz));

				LP.push_back(new Point(x2+xxx, x4+yyy, x3+zzz));
				LP.push_back(new Point(x5+xxx, x6+yyy,x7+zzz));
				LP.push_back(new Point(radius*cos(i*alfa)*sin((j + 2)*beta)+xxx, x6+yyy, radius*sin(i*alfa)*sin((j + 2)*beta)+zzz));
			}
		}
	}
}

void Struct::genCone(float radius, float height, int slices, int stacks) {
	float alfa = (float) (2 * M_PI) / slices; //angulo alfa
	float alturaStack = height / stacks; // altura de uma stack 
	float tangenteBeta = height / radius;

	float alturaBaixo = 0; //altura parte de baixo
	float alturaCima; //altura parte de cima
	float raio2; //raio do circulo superior

	//um slice pode ser resumido em quatro pontos 
	//ligados entre a camada superior e inferior
	//   __
	//  /\/
	//   
	float x1, x2, x3, x4, z1, z2, z3, z4;

	float alfaI;
	float alfaIMais1;

	//este ciclo exterior trata das camadas
	//o que faz com que o cone seja
	//desenhado por "andares" 
	//que convergem no topo do cilindro
	for (int j = 1; j <= stacks; j++) {
		alturaCima = alturaStack * j;
		raio2 = (height - alturaCima) / tangenteBeta;

		//este ciclo foca-se num slice particular
		//portanto, quando 
		for (int i = 1; i <= slices + 1; i++) {

			alfaI = alfa * i;
			alfaIMais1 = alfa * (i + 1);

			x1 = radius * sin(alfaI);
			z1 = radius * cos(alfaI);

			x2 = radius * sin(alfaIMais1);
			z2 = radius * cos(alfaIMais1);

			x3 = raio2 * sin(alfaI);
			z3 = raio2 * cos(alfaI);

			x4 = raio2 * sin(alfaIMais1);
			z4 = raio2 * cos(alfaIMais1);

			if (j == 1) {

				//triangulo da base
				LP.push_back(new Point(0, 0, 0));
				LP.push_back(new Point(x2, 0, z2));
				LP.push_back(new Point(x1, 0, z1));

				//triangulos do lado
				LP.push_back(new Point(x1, 0, z1));
				LP.push_back(new Point(x2, 0, z2));
				LP.push_back(new Point(x3, alturaCima, z3));

				LP.push_back(new Point(x2, 0, z2));
				LP.push_back(new Point(x4, alturaCima, z4));
				LP.push_back(new Point(x3, alturaCima, z3));

			}// estamos no topo do cone
			else if (j == stacks) {

				//um unico triangulo no topo (por slice)
				LP.push_back(new Point(x1, alturaBaixo, z1));
				LP.push_back(new Point(x2, alturaBaixo, z2));
				LP.push_back(new Point(0, height, 0));
			}
			else {
				//se nao estivermos no topo ou na base estamos a desenhar os lados do cone

				//triangulos relativos aos lados do cone		
				LP.push_back(new Point(x1, alturaBaixo, z1));
				LP.push_back(new Point(x2, alturaBaixo, z2));
				LP.push_back(new Point(x3, alturaCima, z3));
		
				LP.push_back(new Point(x2, alturaBaixo, z2));
				LP.push_back(new Point(x4, alturaCima, z4));
				LP.push_back(new Point(x3, alturaCima, z3));
			}
		}

		//subimos de patamar
		alturaBaixo = alturaCima;
		//consequentemente tambem muda o circulo da base
		radius = raio2;
	}
}


void Struct:: genBox(float cX, float cY, float cZ, int div){
    
    
    
    float x = cX/2;
    float y = cY/2;
    float z = cZ/2;
    
    float pX = (float)cX/div;
    float pY = (float)cY/div;
    float pZ = (float)cZ/div;
    
    
    
    for(int i=0;i<div;i++){
        for(int j=0;j<div;j++){
            //Face da frente
            LP.push_back(new Point(-x + (j*pX),-y + (i*pY),z));
            LP.push_back(new Point((-x+pX) + (j*pX),-y + (i*pY),z));
            LP.push_back(new Point(-x + (j*pX),(-y+pY) + (i*pY),z));
            
            
            
            LP.push_back(new Point(-x + (j*pX),(-y+pY) + (i*pY),z));
            LP.push_back(new Point((-x+pX) + (j*pX),-y + (i*pY),z));
            LP.push_back(new Point((-x+pX) + (j*pX),(-y+pY) + (i*pY),z));
            
            //Face traseira
            
            LP.push_back(new Point(-x + (j*pX),-y + (i*pY),-z));
            LP.push_back(new Point(-x + (j*pX),(-y+pY) + (i*pY),-z));
            LP.push_back(new Point((-x+pX) + (j*pX),-y + (i*pY),-z));
            
            
            LP.push_back(new Point(-x + (j*pX),(-y+pY) + (i*pY),-z));
            LP.push_back(new Point((-x+pX) + (j*pX),(-y+pY) + (i*pY),-z));
            LP.push_back(new Point((-x+pX) + (j*pX),-y + (i*pY),-z));
            
            
            //Face direita
            LP.push_back(new Point(x,-y + (i*pY),-z + (j*pZ)));
            LP.push_back(new Point(x,(-y+pY) + (i*pY),-z +(j*pZ)));
            LP.push_back(new Point(x,-y + (i*pY),(-z+pZ) + (j*pZ)));
            
            
            LP.push_back(new Point(x,(-y+pY) + (i*pY),-z + (j*pZ)));
            LP.push_back(new Point(x,(-y+pY) + (i*pY),(-z+pZ) + (j*pZ)));
            LP.push_back(new Point(x,-y + (i*pY),(-z+pZ) + (j*pZ)));
            
            
            //Face esquerda
            LP.push_back(new Point(-x,-y + (i*pY),-z + (j*pZ)));
            LP.push_back(new Point(-x,-y + (i*pY),(-z+pZ) + (j*pZ)));
            LP.push_back(new Point(-x,(-y+pY) + (i*pY),-z +(j*pZ)));
            
            
            LP.push_back(new Point(-x,(-y+pY) + (i*pY),-z + (j*pZ)));
            LP.push_back(new Point(-x,-y + (i*pY),(-z+pZ) + (j*pZ)));
            LP.push_back(new Point(-x,(-y+pY) + (i*pY),(-z+pZ) + (j*pZ)));
            
            //Topo
            LP.push_back(new Point(-x + (j*pX),y,-z + (i*pZ)));
            LP.push_back(new Point(-x + (j*pX),y,(-z+pZ) + (i*pZ)));
            LP.push_back(new Point((-x+pX) + (j*pX),y,-z + (i*pZ)));
            
            
            LP.push_back(new Point(-x + (j*pX),y,(-z+pZ) + (i*pZ)));
            LP.push_back(new Point((-x+pX) + (j*pX),y,(-z+pZ) + (i*pZ)));
            LP.push_back(new Point((-x+pX) + (j*pX),y,-z + (i*pZ)));
            
            //Base
            LP.push_back(new Point(-x + (j*pX),-y,-z + (i*pZ)));
            LP.push_back(new Point((-x+pX) + (j*pX),-y,-z + (i*pZ)));
            LP.push_back(new Point(-x + (j*pX),-y,(-z+pZ) + (i*pZ)));
            
            
            LP.push_back(new Point(-x + (j*pX),-y,(-z+pZ) + (i*pZ)));
            LP.push_back(new Point((-x+pX) + (j*pX),-y,-z + (i*pZ)));
            LP.push_back(new Point((-x+pX) + (j*pX),-y,(-z+pZ) + (i*pZ)));
            
        }
    }
    
}


void Struct::genTorus(float tamanhoCoroa, float raio, int faces, int aneis) {

	float lado = (float) (2 * M_PI) / faces;
	float anel =  (float) (2 * M_PI) / aneis;

	for (int i = 0; i < aneis; i++) {

		float cosI = (float) cos(i * anel);
		float sinI = (float) sin(i * anel);
		float cosIanel = (float) cos(i * anel + anel);
		float sinIanel = (float) sin(i * anel + anel);

		for (int j = 0; j < faces + 1; j++) {

			float jj = j * lado;
			float jjUm = (j + 1)*lado;

			float tcos = tamanhoCoroa * cos(jj) + raio;
			float tsin = tamanhoCoroa * sin(jj);

			float tcosUm = tamanhoCoroa * (cos(jjUm)) + raio;
			float tsinUm = tamanhoCoroa * sin(jjUm);

			LP.push_back(new Point(cosI*tcos, sinI*tcos, tsin));
			LP.push_back(new Point(cosIanel*tcos, sinIanel*tcos, tsin));
			LP.push_back(new Point(cosI*tcosUm, sinI*tcosUm, tsinUm));
			
			LP.push_back(new Point(cosI*tcosUm, sinI*tcosUm, tsinUm));
			LP.push_back(new Point(cosIanel*tcos, sinIanel*tcos, tsin));
			LP.push_back(new Point(cosIanel*tcosUm, sinIanel*tcosUm, tsinUm));
		}
	}
}

void Struct::genCintura(float tamanhoCoroa, float raio, int faces, int aneis) {

	float lado = (float)(2 * M_PI) / faces;
	float anel = (float)(2 * M_PI) / aneis;


	for (int i = 0; i < aneis; i++) {


		float cosI = (float)cos(i * anel);
		float sinI = (float)sin(i * anel);
		float cosIanel = (float)cos(i * anel + anel);
		float sinIanel = (float)sin(i * anel + anel);

		for (int j = 0; j < faces + 1; j++) {

			float jj = j * lado;
			float jjUm = (j + 1)*lado;

			float tcos = tamanhoCoroa * cos(jj) + raio;
			float tsin = tamanhoCoroa * sin(jj);

			float tcosUm = tamanhoCoroa * (cos(jjUm)) + raio;
			float tsinUm = tamanhoCoroa * sin(jjUm);

			//(float radius, int slices, int stacks)
			float radius = (float) 0.01; int slices = 4; int stacks = 2;

			//LP.push_back(new Point(cosI*tcos, sinI*tcos, tsin));
			xxx = cosI*tcos; yyy = sinI*tcos; zzz = tsin;
			genSphere(radius, slices, stacks);

			//LP.push_back(new Point(cosIanel*tcos, sinIanel*tcos, tsin));
			xxx = cosIanel * tcos; yyy = sinIanel * tcos;
			genSphere(radius, slices, stacks);
			//LP.push_back(new Point(cosI*tcosUm, sinI*tcosUm, tsinUm));
			xxx = cosI * tcosUm; yyy = sinI * tcosUm; zzz = tsinUm;
			genSphere(radius, slices,stacks);
			//LP.push_back(new Point(cosI*tcosUm, sinI*tcosUm, tsinUm));
			xxx = cosI * tcosUm; yyy = sinI * tcosUm;
			genSphere(radius, slices, stacks);
			//LP.push_back(new Point(cosIanel*tcos, sinIanel*tcos, tsin));
			xxx = cosIanel * tcos; yyy = sinIanel * tcos; zzz = tsin;
			genSphere(radius, slices, stacks);
			//LP.push_back(new Point(cosIanel*tcosUm, sinIanel*tcosUm, tsinUm));
			xxx = cosIanel * tcosUm; yyy = sinIanel * tcosUm; zzz = tsinUm;
			genSphere(radius, slices, stacks);
		}
	}
}