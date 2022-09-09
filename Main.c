#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <string.h>
#include <time.h>

/* macro definition for console clearing */
#ifdef _WIN32
# define CLEAR_SCREEN system ("cls")
#else
# define CLEAR_SCREEN puts("\x1b[H\x1b[2J")
#endif

#define distx 200;
#define trilha1 -400;
#define trilha2 -200;
#define trilha3  0;
#define trilha4  200;
#define trilha5  400;

int win;					//variável da janela
int projecao=0; 			//Variável Lógica para Definir o Tipo de Projeção (Perspectiva ou Ortogonal) 
int posx=0, posy=10, posz=20; //Variáveis que definem a posição da câmera
int oy=0,ox=0,oz=-20;         //Variáveis que definem para onde a câmera olha
int lx=0, ly=1,  lz=0;         //Variáveis que definem qual eixo estará na vertical do monitor.
int navex = 0;				   //Variavel Responsavel pelo eixo x da nave
int navey = -10;
int navez = 400;
int naverotY = 0;			//variavel que altera o angulo da inclinacao da nave(em relacao ao eixo y)
int tiro=0;					//variavel responsavel por transladar o projetil
int tiroi[3]={0,0,0};
int value=0;				//variavel da funcao glutimerfunc
int shoot = 0;				//tipo de uma flag pra realizar o tiro
int a=0, d=0;				//variaveis para controle do movimento da nave
int i = 0;
int gerax[100];
int b=0;
int x1[1], z=360;				//variaveis que controlam a bala
int xi[3], zi[3]={-360,-360,-360}, disparoi[3]={0,0,0};
int rotacao=0;
int ValorX();
int cont=0;
int geraz[100];
int disparo=0;
int modo=0; 					//2 = 2D	3 = 3D
int posaleatoria;
int xaleatorio=0;			//posição em x em que as naves/asteroide são gerados
int zasteroide=-1200;			//posição em z em que as naves/asteroide são gerados
int tamasteroide[3]={50,50,50};
int comeco = 0;
int asteroide[3];
int qntasteroides;			//0 a 2
int gameover = 0;
int rot[3];					//indica se os asteroides vão girar para a esquerda(0) ou direita(1)
char Pressione[25]={'P','R','E','S','S','I','O','N','E',' ','K',' ','P','A','R','A',' ','I','N','I','C','I','A','R','!'};
char Espaco[15]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
char GameOver[10]={'G','A','M','E',' ','O','V','E','R','!'};
char Espac[15]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
char Inimigos[10]={'I','N','I','M','I','G','O','S',':',' '};
char Espa[15]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
char Ganhou[12]={'V','O','C','E',' ','G','A','N','H','O','U','!'};
char Esp[15]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
char Fase[5]={'F','A','S','E',':'};
char Es[20]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
char HP[3]={'H','P',':'};
char E[15]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
int perspecX=0;
int perspecY=0;
int perspecZ=0;
int pause=0;				//variável para pausar a execução
int naveix[3];
int naveiz[3]={-600,-600,-600};
int qntnaves;				//1 a 3
int testex[1];
int continimigos=5;
int estrelaX[50];
int estrelaY[50];
int estrelaZ[50]={700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700,700};
int estrela=90;
int vidas = 3;
int contador=120;
char inimigos[3];
char fases[3];
char hp[3];
int ganhou=0;
int fase=1;
int flag=0;
int chefx=0, chefy=0, chefz=-1200;
int tirochefe[4]={0,0,0,0}, tiroz=-600, dartiro=0, tz=0;
int vidachefe=20;

void Chefao();
void DesenhaChefao();
void Perspectiva(int x, int y, int z);
void Vida(int i);
void Colisao();
void ColisaoTiro(int i);						//tiro da nave inimiga
void ColisaoNave();								//destruir a nave inimiga
void ColisaoChefe(int i);
void Escrever(char palavra[], float tamanho, int x, int y, int z, int cor);
void DesenhoAsteroide(int i);
void Inicializa();   							//funcao responsavel pela luz do ambiente
void DISPLAY ();
void keyboard (unsigned char key,int x,int y);  //controle de teclas
void DesenhoNave();  							//desenho da nave aliada
void DesenhoNaveInimiga(int i); 						//desenho da nave inimiga
void DisparoNave();									//funcao que realiza os desparos
void DisparoNaveInimiga(int i);
void GerarAleatorio();
void Eixos(int x, int y, int z);
void GerarEstrela();
void DesenhoEstrelas();
void timer(){                                //funcao para 60fps
	glutPostRedisplay();
	glutTimerFunc(1000/60,timer,0);
}


// Inicializa parâmetros de rendering
void DISPLAY()
{   
	itoa(continimigos,inimigos,10);
	itoa(fase,fases,10);
	itoa(vidachefe,hp,10);
	
	if(modo==2)
    {
    	glClearColor(0, 0, 0, 0);
	    glEnable(GL_DEPTH_TEST);
	    glMatrixMode(GL_PROJECTION);							 //Ativa matriz de projeção
	    glLoadIdentity();										//"Limpa" ou "transforma" a matriz em identidade, reduzindo possíveis erros.    
	    glOrtho(-550, 550, -550, 550, -5000, 5000);	
	    glMatrixMode(GL_MODELVIEW);								//Ativa matriz de visualização
		glLoadIdentity();										//"Limpa" ou "transforma" a matriz em identidade, reduzindo possíveis erros.
	 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		 // "limpa" um buffer particular ou combinações de buffers, onde buffer é uma área de armazenamento para informações da imagem
		                                                                      
		gluLookAt(posx,posy,posz,ox,oy,oz,lx,ly,lz);
		
		//GerarEstrela();
		//DesenhoEstrelas();
		
		if((comeco==0 && gameover==0) && ganhou==0)
		{
			Escrever(Pressione,0.3,-250, 0, 0, 1);
			
			Escrever(Fase,0.3,-80, 0, -400, 1);
			Escrever(fases,0.3,40, 0, -400, 2);
			
			perspecY=1;
			Perspectiva(perspecX, perspecY, perspecZ);
		}
		
		if(comeco==1)
		{	
			if(fase!=5)//fase!=5
			{
				DisparoNave();	
				DesenhoNave();
				GerarAleatorio();
				Colisao();
				ColisaoNave();
				
				Escrever(Inimigos,0.3,320, 0, -450, 1);
				Escrever(inimigos,0.3,490, 0, -450, 2);
				
				for(int i=0; i<qntasteroides+1; i++)
				{
					DesenhoAsteroide(i);
				}
				
				for(int i=0; i<qntnaves; i++)
				{
					DesenhoNaveInimiga(i);
					DisparoNaveInimiga(i);
					ColisaoTiro(i);
				}
				
				for(int i=0; i<vidas; i++)
				{
					Vida(i);
				}
				
				if(pause==0)
				{	
					if(naverotY > 0)
					{
					 	naverotY -= 0.00000000000001;
					}
					 
					else if(naverotY < 0)
					{
					 	naverotY += 0.00000000000001;
					}
					 
					if(rotacao<360)
					{
					 	if(cont==7)
					 	{
					 		rotacao += 3;
					 		cont=0;
					 		zasteroide+=40;
						}
						else
						{
							cont++;
						}
					}
					else
					{
						rotacao=0;
					}
				}
			}
			
			else if(fase==5)
			{
				DisparoNave();	
				DesenhoNave();
				DesenhaChefao();
				Chefao();
				
				for(int i=0; i<3; i++)
				{
					ColisaoChefe(i);
				}
				
				for(int i=0; i<vidas; i++)
				{
					Vida(i);
				}
				
				Escrever(HP,0.3,350, 0, 150, 1);
				Escrever(hp,0.3,420, 0, 150, 2);
				
				if(pause==0)
				{	
					if(naverotY > 0)
					{
					 	naverotY -= 0.00000000000001;
					}
					 
					else if(naverotY < 0)
					{
					 	naverotY += 0.00000000000001;
					}
					 
					if(rotacao<360)
					{
					 	if(cont==7)
					 	{
					 		rotacao += 3;
					 		cont=0;
						}
						else
						{
							cont++;
						}
					}
					else
					{
						rotacao=0;
					}
				}
			}
		}
		else
		{
			if(gameover==1)
			Escrever(GameOver,0.3,-125,0,0,2);
			
			if(ganhou==1)
			Escrever(Ganhou,0.3,-125,0,0,3);
		}
	}

	else if(modo==3)
	{
		glClearColor(0, 0, 0, 0);
	    glEnable(GL_DEPTH_TEST);
	    glMatrixMode(GL_PROJECTION);							 //Ativa matriz de projeção
	    glLoadIdentity();										//"Limpa" ou "transforma" a matriz em identidade, reduzindo possíveis erros.    
	    glOrtho(-550, 550, -550, 550, -5000, 5000);	
	    glMatrixMode(GL_MODELVIEW);								//Ativa matriz de visualização
		glLoadIdentity();										//"Limpa" ou "transforma" a matriz em identidade, reduzindo possíveis erros.
	 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		 // "limpa" um buffer particular ou combinações de buffers, onde buffer é uma área de armazenamento para informações da imagem
		posx=0, posy=5, posz=20;                                                                      
		gluLookAt(posx,posy,posz,ox,oy,oz,lx,ly,lz);
		
		//GerarEstrela();
		//DesenhoEstrelas();
		
		if((comeco==0 && gameover==0) && ganhou==0)
		{
			Escrever(Pressione,0.3,-250, 0, 0, 1);
			
			Escrever(Fase,0.3,-80, 0, -400, 1);
			Escrever(fases,0.3,40, 0, -400, 2);
			
			perspecZ=1;
			Perspectiva(perspecX, perspecY, perspecZ);
		}
		
		if(comeco==1)
		{	
			if(fase!=5)//fase!=5
			{
				DisparoNave();	
				DesenhoNave();
				GerarAleatorio();
				Colisao();
				ColisaoNave();
				
				Escrever(Inimigos,0.3,320, 250, -450, 1);
				Escrever(inimigos,0.3,490, 250, -450, 2);
				
				for(int i=0; i<qntasteroides+1; i++)
				{
					DesenhoAsteroide(i);
				}
				
				for(int i=0; i<qntnaves; i++)
				{
					DesenhoNaveInimiga(i);
					DisparoNaveInimiga(i);
					ColisaoTiro(i);
				}
				
				for(int i=0; i<vidas; i++)
				{
					Vida(i);
				}
				
				if(pause==0)
				{	
					if(naverotY > 0)
					{
					 	naverotY -= 0.00000000000001;
					}
					 
					else if(naverotY < 0)
					{
					 	naverotY += 0.00000000000001;
					}
					 
					if(rotacao<360)
					{
					 	if(cont==7)
					 	{
					 		rotacao += 3;
					 		cont=0;
					 		zasteroide+=80;
						}
						else
						{
							cont++;
						}
					}
					else
					{
						rotacao=0;
					}
				}
			}
			
			else if(fase==5)
			{
				DisparoNave();	
				DesenhoNave();
				DesenhaChefao();
				Chefao();
				
				for(int i=0; i<3; i++)
				{
					ColisaoChefe(i);
				}
				
				for(int i=0; i<vidas; i++)
				{
					Vida(i);
				}
				
				Escrever(HP,0.3,350, 0, 150, 1);
				Escrever(hp,0.3,420, 0, 150, 2);
				
				if(pause==0)
				{	
					if(naverotY > 0)
					{
					 	naverotY -= 0.00000000000001;
					}
					 
					else if(naverotY < 0)
					{
					 	naverotY += 0.00000000000001;
					}
					 
					if(rotacao<360)
					{
					 	if(cont==7)
					 	{
					 		rotacao += 3;
					 		cont=0;
						}
						else
						{
							cont++;
						}
					}
					else
					{
						rotacao=0;
					}
				}
			}
		}
		else
		{
			if(gameover==1)
			Escrever(GameOver,0.3,-125,0,0,2);
			
			if(ganhou==1)
			Escrever(Ganhou,0.3,-125,0,0,3);
			
			if(comeco==1)
			{
				gameover=0;
				ganhou=0;
				comeco=0;
				fase=1;
			}
		}
	}
	Eixos(perspecX, perspecY, perspecZ);
	
	 glutSwapBuffers(); //Executa a Cena. SwapBuffers dá suporte para mais de um buffer, permitindo execução de animações sem cintilações. 
}

void ColisaoChefe(int i)
{
	if(contador<120)
	{
		contador++;
	}
	
	//tiro da nave
	//verificação para x
	else if(tirochefe[i] == navex)
	{
		//verificação para z
		if(tiroz >= 330 && tiroz <= 450)
		{
			if(vidas==1)
			{
				gameover=1;
				comeco=0;
			}
			else
			{
				if(contador==120)
				{
					vidas--;
					contador=0;
				}
			}
		}	
	}
	
	if(modo==2)
	{
		//tiro na nave
		//verificação para z
		if(((z < -500) && (z > -1000)) && chefz>-750)
		{
			if(contador==120)
			{
				vidachefe--;
				contador=0;
			}
		}
	}
	
	else if(modo==3)
	{
		//tiro na nave
		//verificação para z
		if(((z < -900) && (z > -1200)) && chefz>-1100)
		{
			if(contador==120)
			{
				vidachefe--;
				contador=0;
			}
		}
	}
	
	
	if(vidachefe==0)
	{
		ganhou=1;
		comeco=0;
	}
}

void Chefao()
{	
	if(dartiro==0)
	{
		for(int i=0; i<4; i++)
		{
			posaleatoria = rand()%5;
		
			if(i==0)
			{
				if(posaleatoria == 0)
				{
					tirochefe[0] = trilha1;	
				}
				
				else if(posaleatoria == 1)
				{
					tirochefe[0] = trilha2;	
				}
				
				else if(posaleatoria == 2)
				{
					tirochefe[0] = trilha3;	
				}
				
				else if(posaleatoria == 3)
				{
					tirochefe[0] = trilha4;	
				}
					
				else if(posaleatoria == 4)
				{
					tirochefe[0] = trilha5;	
				}
			}
			
			else if(i==1)
			{
				if(posaleatoria == 0)
				{
					if(tirochefe[0] != -400)
					tirochefe[1] = -400;	
					
					else
					i--;
				}
				
				else if(posaleatoria == 1)
				{
					if(tirochefe[0] != -200)
					tirochefe[1] = -200;	
					
					else
					i--;	
				}
				
				else if(posaleatoria == 2)
				{
					if(tirochefe[0] != 0)
					tirochefe[1] = 0;	
					
					else
					i--;
				}
				
				else if(posaleatoria == 3)
				{
					if(tirochefe[0] != 200)
					tirochefe[1] = 200;	
					
					else
					i--;	
				}
					
				else if(posaleatoria == 4)
				{
					if(tirochefe[0] != 400)
					tirochefe[1] = 400;	
					
					else
					i--;
				}
			}
			
			else if(i==2)
			{
				if(posaleatoria == 0)
				{
					if(tirochefe[0] != -400 && tirochefe[1]!= -400)
					tirochefe[2] = -400;	
					
					else
					i--;
				}
				
				else if(posaleatoria == 1)
				{
					if(tirochefe[0] != -200 && tirochefe[1]!= -200)
					tirochefe[2] = -200;	
					
					else
					i--;	
				}
				
				else if(posaleatoria == 2)
				{
					if(tirochefe[0] != 0 && tirochefe[1]!= 0)
					tirochefe[2] = 0;	
					
					else
					i--;
				}
				
				else if(posaleatoria == 3)
				{
					if(tirochefe[0] != 200 && tirochefe[1]!= 200)
					tirochefe[2] = 200;	
					
					else
					i--;	
				}
					
				else if(posaleatoria == 4)
				{
					if(tirochefe[0] != 400 && tirochefe[1]!= 400)
					tirochefe[2] = 400;	
					
					else
					i--;
				}
			}
			
			else if(i==3)
			{
				if(posaleatoria == 0)
				{
					if((tirochefe[0] != -400 && tirochefe[1]!= -400) && tirochefe[2]!= -400)
					tirochefe[3] = -400;	
					
					else
					i--;
				}
				
				else if(posaleatoria == 1)
				{
					if((tirochefe[0] != -200 && tirochefe[1]!= -200) && tirochefe[2]!= -200)
					tirochefe[3] = -200;	
					
					else
					i--;	
				}
				
				else if(posaleatoria == 2)
				{
					if((tirochefe[0] != 0 && tirochefe[1]!= 0) && tirochefe[2]!= 0)
					tirochefe[3] = 0;	
					
					else
					i--;
				}
				
				else if(posaleatoria == 3)
				{
					if((tirochefe[0] != 200 && tirochefe[1]!= 200) && tirochefe[2]!= 200)
					tirochefe[3] = 200;	
					
					else
					i--;	
				}
					
				else if(posaleatoria == 4)
				{
					if((tirochefe[0] != 400 && tirochefe[1]!= 400) && tirochefe[2]!= 400)
					tirochefe[3] = 400;	
					
					else
					i--;
				}
			}
		}
		
		dartiro=1;
	}
	
	if(modo==2 && chefz<-700)
	{
		chefz+=0.2;
	}
	
	else if(modo==3 && chefz<-1000)
	{
		chefz+=0.2;
	}
	
	if((modo==2 && chefz>=-800) && dartiro==1)
	{			
		if(pause==0)
		tiroz+=tz*0.02;
			
		glPushMatrix();
		glColor3f(1,0,0);
		glTranslatef(tirochefe[0],0,tiroz);
		glRotatef(naverotY,0,0,1);
		glScalef(1,1,4);
		glutSolidTorus(4,8,8,8);
		glPopMatrix();
		
		glPushMatrix();
		glColor3f(1,0,0);
		glTranslatef(tirochefe[1],0,tiroz);
		glRotatef(naverotY,0,0,1);
		glScalef(1,1,4);
		glutSolidTorus(4,8,8,8);
		glPopMatrix();
		
		glPushMatrix();
		glColor3f(1,0,0);
		glTranslatef(tirochefe[2],0,tiroz);
		glRotatef(naverotY,0,0,1);
		glScalef(1,1,4);
		glutSolidTorus(4,8,8,8);
		glPopMatrix();
		
		glPushMatrix();
		glColor3f(1,0,0);
		glTranslatef(tirochefe[3],0,tiroz);
		glRotatef(naverotY,0,0,1);
		glScalef(1,1,4);
		glutSolidTorus(4,8,8,8);
		glPopMatrix();
		
		if(tiroz>600)
		{
			tiroz=-600;
			tz=0;
			dartiro=0;
		}
		
		else
		{
			if(pause==0)
			{
				tz +=5;
			}
		}	
	}
	
	else if((modo==3 && chefz>=-1000) && dartiro==1)
	{
		if(pause==0)
		tiroz+=tz*0.08;
			
		glPushMatrix();
		glColor3f(1,0,0);
		glTranslatef(tirochefe[0],0,tiroz);
		glRotatef(60,1,0,0);
		glRotatef(naverotY,0,0,1);
		glScalef(1,1,4);
		glutSolidTorus(4,8,8,8);
		glPopMatrix();
		
		glPushMatrix();
		glColor3f(1,0,0);
		glTranslatef(tirochefe[1],0,tiroz);
		glRotatef(60,1,0,0);
		glRotatef(naverotY,0,0,1);
		glScalef(1,1,4);
		glutSolidTorus(4,8,8,8);
		glPopMatrix();
		
		glPushMatrix();
		glColor3f(1,0,0);
		glTranslatef(tirochefe[2],0,tiroz);
		glRotatef(60,1,0,0);
		glRotatef(naverotY,0,0,1);
		glScalef(1,1,4);
		glutSolidTorus(4,8,8,8);
		glPopMatrix();
		
		glPushMatrix();
		glColor3f(1,0,0);
		glTranslatef(tirochefe[3],0,tiroz);
		glRotatef(60,1,0,0);
		glRotatef(naverotY,0,0,1);
		glScalef(1,1,4);
		glutSolidTorus(4,8,8,8);
		glPopMatrix();
		
		if(tiroz>1000)
		{
			tiroz=-1000;
			tz=0;
			dartiro=0;
		}
		
		else
		{
			if(pause==0)
			{
				tz +=5;
			}
		}	
	}
}

void DesenhaChefao()
{
	if(modo==2)
	{
		glPushMatrix();
		glTranslatef(chefx,chefy,chefz);
		glScalef(4,4,4);   
		
		glPushMatrix();	
		glColor3f(0.8f,0.8f,0.8f);		
		glTranslatef(0,10,0);
		glScalef(1,0,1);
		glutSolidSphere(140,50,50);
		glPopMatrix();
		
		glPushMatrix();	
		glColor3f(0.5,0.5,0.5);		
		glTranslatef(0,40,90);
		glScalef(1,0,1);
		glutSolidSphere(15,50,50);
		glPopMatrix();
		
		glPushMatrix();	
		glColor3f(0.5,0.5,0.5);			
		glTranslatef(0,40,55);
		glScalef(1,0,1);
		glutSolidSphere(15,50,50);
		glPopMatrix();
		
		glPushMatrix();	
		glColor3f(0.5,0.5,0.5);			
		glTranslatef(35,40,45);
		glScalef(1,0,1);
		glutSolidSphere(15,50,50);
		glPopMatrix();
		
		glPushMatrix();	
		glColor3f(0.5,0.5,0.5);			
		glTranslatef(-35,40,45);
		glScalef(1,0,1);
		glutSolidSphere(15,50,50);
		glPopMatrix();
		
		glPushMatrix();	
		glColor3f(0.5,0.5,0.5);			
		glTranslatef(-45,40,80);
		glScalef(1,0,1);
		glutSolidSphere(15,50,50);
		glPopMatrix();
		
		glPushMatrix();	
		glColor3f(0.5,0.5,0.5);			
		glTranslatef(45,40,80);
		glScalef(1,0,1);
		glutSolidSphere(15,50,50);
		glPopMatrix();
		
		glPopMatrix();
	}
	
	else if(modo==3)
	{
		glPushMatrix();
		glTranslatef(chefx,chefy,chefz);
		glScalef(4,4,6);   
		glRotatef(45,1,0,0);
		
		glPushMatrix();	
		glColor3f(0.8f,0.8f,0.8f);		
		glTranslatef(0,10,0);
		glScalef(1,0,1);
		glutSolidSphere(140,50,50);
		glPopMatrix();
		
		glPushMatrix();	
		glColor3f(0.5,0.5,0.5);		
		glTranslatef(0,40,90);
		glScalef(1,0,1);
		glutSolidSphere(15,50,50);
		glPopMatrix();
		
		glPushMatrix();	
		glColor3f(0.5,0.5,0.5);			
		glTranslatef(0,40,55);
		glScalef(1,0,1);
		glutSolidSphere(15,50,50);
		glPopMatrix();
		
		glPushMatrix();	
		glColor3f(0.5,0.5,0.5);			
		glTranslatef(35,40,45);
		glScalef(1,0,1);
		glutSolidSphere(15,50,50);
		glPopMatrix();
		
		glPushMatrix();	
		glColor3f(0.5,0.5,0.5);			
		glTranslatef(-35,40,45);
		glScalef(1,0,1);
		glutSolidSphere(15,50,50);
		glPopMatrix();
		
		glPushMatrix();	
		glColor3f(0.5,0.5,0.5);			
		glTranslatef(-45,40,80);
		glScalef(1,0,1);
		glutSolidSphere(15,50,50);
		glPopMatrix();
		
		glPushMatrix();	
		glColor3f(0.5,0.5,0.5);			
		glTranslatef(45,40,80);
		glScalef(1,0,1);
		glutSolidSphere(15,50,50);
		glPopMatrix();
		
		glPopMatrix();
	}
	
}

void Vida(int i)
{
	if(modo==2)
	{
		glPushMatrix();	
		if(i==0)
		{
			glTranslatef(350,0,500);
		}
		else if(i==1)
		{
			glTranslatef(400,0,500);
		}
		else if(i==2)
		{
			glTranslatef(450,0,500);
		}
		glScalef(0.2,0.2,0.2);
		glRotatef(-90,1,0,0);
			
			glBegin(GL_TRIANGLES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(0,0,0);
			glVertex3f(100,120,0);
			glVertex3f(-100,120,0);
		glEnd(); 
			glBegin(GL_QUADS);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(100,120,0);
			glVertex3f(-100,120,0);
			glVertex3f(-105,180,0);
			glVertex3f(105,180,0);
		glEnd(); 
		
			glBegin(GL_QUADS);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(105,180,0);
			glVertex3f(-105,180,0);
			glVertex3f(-85,210,0);
			glVertex3f(85,210,0);
		glEnd(); 
			glBegin(GL_QUADS);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(-85,210,0);
			glVertex3f(-65,240,0);
			glVertex3f(-20,240,0);
			glVertex3f(-20,210,0);
		glEnd(); 
		
			glBegin(GL_QUADS);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(85,210,0);
			glVertex3f(65,240,0);
			glVertex3f(20,240,0);
			glVertex3f(20,210,0);
		glEnd(); 
			
			glBegin(GL_TRIANGLES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(20,210,0);
			glVertex3f(20,240,0);
			glVertex3f(0,210,0);
		glEnd(); 
		
			glBegin(GL_TRIANGLES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(-20,210,0);
			glVertex3f(-20,240,0);
			glVertex3f(0,210,0);
		glEnd(); 
		glPopMatrix();
	}
	
	else if(modo==3)
	{
		glPushMatrix();	
		if(i==0)
		{
			glTranslatef(350,-500,0);
		}
		else if(i==1)
		{
			glTranslatef(400,-500,0);
		}
		else if(i==2)
		{
			glTranslatef(450,-500,0);
		}
		glScalef(0.2,0.2,0.2);
			
			glBegin(GL_TRIANGLES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(0,0,0);
			glVertex3f(100,120,0);
			glVertex3f(-100,120,0);
		glEnd(); 
			glBegin(GL_QUADS);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(100,120,0);
			glVertex3f(-100,120,0);
			glVertex3f(-105,180,0);
			glVertex3f(105,180,0);
		glEnd(); 
		
			glBegin(GL_QUADS);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(105,180,0);
			glVertex3f(-105,180,0);
			glVertex3f(-85,210,0);
			glVertex3f(85,210,0);
		glEnd(); 
			glBegin(GL_QUADS);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(-85,210,0);
			glVertex3f(-65,240,0);
			glVertex3f(-20,240,0);
			glVertex3f(-20,210,0);
		glEnd(); 
		
			glBegin(GL_QUADS);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(85,210,0);
			glVertex3f(65,240,0);
			glVertex3f(20,240,0);
			glVertex3f(20,210,0);
		glEnd(); 
			
			glBegin(GL_TRIANGLES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(20,210,0);
			glVertex3f(20,240,0);
			glVertex3f(0,210,0);
		glEnd(); 
		
			glBegin(GL_TRIANGLES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(-20,210,0);
			glVertex3f(-20,240,0);
			glVertex3f(0,210,0);
		glEnd(); 
		glPopMatrix();
	}
}

void GerarEstrela()
{	
	for(int i=0; i<50; i++)
	{
		if(estrelaZ[i] > 600)
		{
			estrelaX[i] = -550 + rand()%1100;
			estrelaZ[i] = -800 + rand()%200;
		}
		
		estrelaZ[i] += estrela; 
	}		
}


void DesenhoEstrelas()
{
	for(int i=0; i<50; i++)
	{
		glPushMatrix();
		glColor3f(1,1,1);
		glTranslatef(estrelaX[i],-200,estrelaZ[i]);
		glutSolidSphere(2,50,50);
		glPopMatrix();
	}
}

void Perspectiva(int x, int y, int z)
{
	if(modo==2)
	{
		//Olhar para X
		if(x==1)
		{
			ox=20;
		 	posx=-20;
		 	oy=0;
		 	posy=0;
		 	oz=0;
		 	posz=0;
		 	lz=0;
		 	ly=1;
		 	lx=0;
		}
		
		//Olhar para Y
		else if(y==1)
		{
			ox=0;
		 	posx=0;
		 	oy=-20;
		 	posy=20;
		 	oz=0;
		 	posz=0;
		 	lz=-1;
		 	ly=0;
		 	lx=0;
		}
		
		//Olhar para Z
		else if(z==1)
		{
			ox=0;
		 	posx=0;
		 	oy=0;
		 	posy=0;
		 	oz=-20;
		 	posz=20;
		 	lz=0;
		 	ly=1;
		 	lx=0;
		}
	}
	
	else if(modo==3)
	{
		//Olhar para X
		if(x==1)
		{
			ox=20;
		 	posx=-20;
		 	oy=-20;
		 	posy=20;
		 	oz=0;
		 	posz=0;
		 	lz=0;
		 	ly=1;
		 	lx=0;
		}
		
		//Olhar para Y
		else if(y==1)
		{
			ox=0;
		 	posx=0;
		 	oy=-20;
		 	posy=20;
		 	oz=0;
		 	posz=0;
		 	lz=-1;
		 	ly=0;
		 	lx=0;
		}
		
		//Olhar para Z
		else if(z==1)
		{
			ox=0;
		 	posx=0;
		 	oy=-20;
		 	posy=20;
		 	oz=-20;
		 	posz=20;
		 	lz=0;
		 	ly=1;
		 	lx=0;
		}
	}
}

void Colisao()
{
	if(contador<120)
	{
		contador++;
	}
	
	int tamA, tamB, tamC;
	//verificando a quantidade de asteroides
	if(qntasteroides==0)
	{
		//verificação para x
		if(asteroide[0] == navex)
		{
			//verificação para y
			tamA = tamasteroide[0]/2;
			
			if(zasteroide+tamA >= 320 && zasteroide-tamA <= 450)
			{
				if(vidas==0)
				{
					gameover=1;
					comeco=0;
				}
				else
				{
					if(contador==120)
					{
						vidas--;
						contador=0;
					}
				}
			}
		}
	}
	//verificando a quantidade de asteroides
	else if(qntasteroides==1)
	{
		//verificação para x
		if(asteroide[0] == navex)
		{
			//verificação para y
			tamA = tamasteroide[0]/2;
			
			if(zasteroide+tamA >= 320 && zasteroide-tamA <= 450)
			{
				if(vidas==0)
				{
					gameover=1;
					comeco=0;
				}
				else
				{
					if(contador==120)
					{
						vidas--;
						contador=0;
					}
				}
			}
		}
		//verificação para x
		else if(asteroide[1] == navex)
		{
			//verificação para y
			tamB = tamasteroide[1]/2;
			
			if(zasteroide+tamB >= 320 && zasteroide+tamB <= 450)
			{
				if(vidas==0)
				{
					gameover=1;
					comeco=0;
				}
				else
				{
					if(contador==120)
					{
						vidas--;
						contador=0;
					}
				}
			}
		}
	}
	//verificando a quantidade de asteroides
	else if(qntasteroides==2)
	{
		//verificação para x
		if(asteroide[0] == navex)
		{
			//verificação para y
			tamA = tamasteroide[0]/2;
			
			if(zasteroide+tamA >= 320 && zasteroide-tamA <= 450)
			{
				if(vidas==0)
				{
					gameover=1;
					comeco=0;
				}
				else
				{
					if(contador==120)
					{
						vidas--;
						contador=0;
					}
				}
			}
		}
		//verificação para x
		else if(asteroide[1] == navex)
		{
			//verificação para y
			tamB = tamasteroide[1]/2;
			
			if(zasteroide+tamB >= 320 && zasteroide+tamB <= 450)
			{
				if(vidas==0)
				{
					gameover=1;
					comeco=0;
				}
				else
				{
					if(contador==120)
					{
						vidas--;
						contador=0;
					}
				}
			}
		}
		//verificação para x
		else if(asteroide[2] == navex)
		{
			//verificação para y
			tamC = tamasteroide[2]/2;
			
			if(zasteroide+tamC >= 320 && zasteroide+tamC <= 450)
			{
				if(vidas==0)
				{
					gameover=1;
					comeco=0;
				}
				else
				{
					if(contador==120)
					{
						vidas--;
						contador=0;
					}
				}
			}
		}
	}
}

void ColisaoTiro(int i)
{
	if(contador<120)
	{
		contador++;
	}
	
	//verificação para x
	if(xi[i] == navex)
	{
		//verificação para z
		if(zi[i]+8 >= 330 && zi[i]-8 <= 450)
		{
			if(vidas==0)
				{
					gameover=1;
					comeco=0;
				}
				else
				{
					if(contador==120)
					{
						vidas--;
						contador=0;
					}
				}
		}	
	}
}

void ColisaoNave()
{
	if(qntnaves==1)
	{
		//destruir nave 1
		//verificação para x
		if(naveix[0] == x1[0])
		{
			//verificação para z
			if((z < -300) && (z > -360))
			{
				qntnaves--;
				continimigos--;
				z = 360;
				tiro = 0;
				shoot = 0;
				disparo=0;
			}
		}
	}
	
	else if(qntnaves==2)
	{
		//destruir nave 1
		//verificação para x
		if(naveix[0] == x1[0])
		{
			//verificação para z
			if((z < -300) && (z > -360))
			{
				naveix[0] = naveix[1];
				xi[0] = xi[1]; 
				zi[0] = zi[1];
				tiroi[0] = tiroi[1];
				qntnaves--;
				continimigos--;
				z = 360;
				tiro = 0;
				shoot = 0;
				disparo=0;
			}
		}
		
		//destruir nave 2
		//verificação para x
		else if(naveix[1] == x1[0])
		{
			//verificação para z
			if((z < -300) && (z > -360))
			{
				qntnaves--;
				continimigos--;
				z = 360;
				tiro = 0;
				shoot = 0;
				disparo=0;
			}
		}
	}
	
	else if(qntnaves==3)
	{
		//destruir nave 1
		//verificação para x
		if(naveix[0] == x1[0])
		{
			//verificação para z
			if((z < -300) && (z > -360))
			{
				naveix[0] = naveix[2];
				xi[0] = xi[2]; 
				zi[0] = zi[2];
				tiroi[0] = tiroi[2];
				qntnaves--;
				continimigos--;
				z = 360;
				tiro = 0;
				shoot = 0;
				disparo=0;
			}
		}
		
		//destruir nave 2
		//verificação para x
		else if(naveix[1] == x1[0])
		{
			//verificação para z
			if((z < -300) && (z > -360))
			{
				naveix[1] = naveix[2];
				xi[1] = xi[2]; 
				zi[1] = zi[2];
				tiroi[1] = tiroi[2];
				qntnaves--;
				continimigos--;
				z = 360;
				tiro = 0;
				shoot = 0;
				disparo=0;
			}
		}
		
		//destruir nave 3
		//verificação para x
		else if(naveix[2] == x1[0])
		{
			//verificação para z
			if((z < -300) && (z > -360))
			{
				qntnaves--;
				continimigos--;
				z = 360;
				tiro = 0;
				shoot = 0;
				disparo=0;
			}
		}
	}
	
	if(continimigos==0 && fase!=5)
	{
		fase++;
		flag=0;
		comeco=0;
		
		if(fase==1 && flag==0)
		{
			zasteroide=-1200;
			qntnaves=0;
			qntasteroides=0;
			continimigos=5;
			flag=1;
			z = 360;
			tiro = 0;
			shoot = 0;
			disparo=0;
		}
			
		else if(fase==2 && flag==0)
		{
			zasteroide=-1200;
			qntnaves=0;
			qntasteroides=0;
			continimigos=10;
			if(flag==0)
			{
				if(vidas<3)
				vidas++;
				
				flag=1;
			}
			navex=0;
			z = 360;
			tiro = 0;
			shoot = 0;
			disparo=0;
		}
			
		else if(fase==3 && flag==0)
		{
			zasteroide=-1200;
			qntnaves=0;
			qntasteroides=0;
			continimigos=15;
			if(flag==0)
			{
				if(vidas<3)
				vidas++;
				
				flag=1;
			}
			navex=0;
			z = 360;
			tiro = 0;
			shoot = 0;
			disparo=0;
		}
		
		else if(fase==4 && flag==0)
		{
			zasteroide=-1200;
			qntnaves=0;
			qntasteroides=0;
			continimigos=20;
			if(flag==0)
			{
				if(vidas<3)
				vidas++;
				
				flag=1;
			}
			navex=0;
			z = 360;
			tiro = 0;
			shoot = 0;
			disparo=0;
		}
			
		else if(fase==5 && flag==0)
		{
			zasteroide=-1200;
			qntnaves=0;
			qntasteroides=0;
			continimigos=1;
			if(flag==0)
			{
				if(vidas<3)
				vidas++;
				
				flag=1;
			}
			navex=0;
			z = 360;
			tiro = 0;
			shoot = 0;
			disparo=0;
		}
	}
	
	if(continimigos==0 && fase==5)
	{
		ganhou=1;
		comeco=0;
	}
}

void GerarAleatorio(){	
	
	if(modo==2)
	{
		int teste = rand()%1000;
	
		if(testex[0]==0)
		{
			testex[0] = 0;
		}
		
		//naves inimigas
		if(navex != testex[0]) //a nave mudou de posição
		{
			testex[0] = navex;
			
			int test = rand()%50;
			
			if(test<30)
			{
				if(((qntasteroides + qntnaves) < 4) && qntnaves<3) //se naves+asteroides não superam 5
				{	
					if(qntasteroides==0) //1 asteroide 3 naves
					{
						if(qntnaves==0) //nave 1
						{
							if(asteroide[0] != navex) 
							{
								naveix[qntnaves] = navex;
								naveiz[qntnaves] = -600;
								zi[qntnaves] = -360;
								tiroi[qntnaves] = 0;
								disparoi[qntnaves] = 0;
								qntnaves++;
							}
						}
						
						else if(qntnaves==1) //nave 2
						{
							if((naveix[0] != navex) && (asteroide[0] != navex))
							{
								naveix[qntnaves] = navex;
								naveiz[qntnaves] = -600;
								zi[qntnaves] = -360;
								tiroi[qntnaves] = 0;
								disparoi[qntnaves] = 0;
								qntnaves++;
							}
						}
						
						else if(qntnaves==2) //nave 3
						{
							if(((naveix[0] != navex) && (naveix[1] != navex)) && (asteroide[0] != navex))
							{
								naveix[qntnaves] = navex;
								naveiz[qntnaves] = -600;
								zi[qntnaves] = -360;
								tiroi[qntnaves] = 0;
								disparoi[qntnaves] = 0;
								qntnaves++;
							}
						}
					}
					else if(qntasteroides==1) //2 asteroides 3 naves
					{
						if(qntnaves==0) //nave 1
						{
							if((asteroide[0] != navex) && (asteroide[1] != navex)) 
							{
								naveix[qntnaves] = navex;
								naveiz[qntnaves] = -600;
								zi[qntnaves] = -360;
								tiroi[qntnaves] = 0;
								disparoi[qntnaves] = 0;
								qntnaves++;
							}
						}
						
						else if(qntnaves==1) //nave 2
						{
							if((naveix[0] != navex) && ((asteroide[0] != navex) && (asteroide[1] != navex)))
							{
								naveix[qntnaves] = navex;
								naveiz[qntnaves] = -600;
								zi[qntnaves] = -360;
								tiroi[qntnaves] = 0;
								disparoi[qntnaves] = 0;
								qntnaves++;
							}
						}
						
						else if(qntnaves==2) //nave 3
						{
							if(((naveix[0] != navex) && (naveix[1] != navex)) && ((asteroide[0] != navex) && (asteroide[1] != navex)))
							{
								naveix[qntnaves] = navex;
								naveiz[qntnaves] = -600;
								zi[qntnaves] = -360;
								tiroi[qntnaves] = 0;
								disparoi[qntnaves] = 0;
								qntnaves++;
							}
						}
					}
					else if(qntasteroides==2) //3 asteroides 2 naves
					{
						if(qntnaves==0) //nave 1
						{
							if(((asteroide[0] != navex) && (asteroide[1] != navex)) && (asteroide[2] != navex)) 
							{
								naveix[qntnaves] = navex;
								naveiz[qntnaves] = -600;
								zi[qntnaves] = -360;
								tiroi[qntnaves] = 0;
								disparoi[qntnaves] = 0;
								qntnaves++;
							}
						}
						
						else if(qntnaves==1) //nave 2
						{
							if(((naveix[0] != navex) && (asteroide[0] != navex)) && ((asteroide[1] != navex) && (asteroide[2] != navex)))
							{
								naveix[qntnaves] = navex;
								naveiz[qntnaves] = -600;
								zi[qntnaves] = -360;
								tiroi[qntnaves] = 0;
								disparoi[qntnaves] = 0;
								qntnaves++;
							}
						}
					}
				}
			}
		}
	
		//asteroides
		if(teste < 10 && zasteroide > 600)
		{
			zasteroide = -600;
			
			qntasteroides = rand()%3;
			
			if((qntasteroides + qntnaves) > 4)
			{
				qntasteroides = 4 - qntnaves;
			}
			
			for(int i=0; i<qntasteroides+1; i++)
			{
				posaleatoria = rand()%5;
				
				tamasteroide[i] = 50;
			
				if(posaleatoria == 0)
				{
					xaleatorio = trilha1;	
				}
				
				else if(posaleatoria == 1)
				{
					xaleatorio = trilha2;	
				}
				
				else if(posaleatoria == 2)
				{
					xaleatorio = trilha3;	
				}
				
				else if(posaleatoria == 3)
				{
					xaleatorio = trilha4;	
				}
				
				else if(posaleatoria == 4)
				{
					xaleatorio = trilha5;	
				}
				
				//Evitar gerar mais de 1 asteroide na mesma posição e na mesma trilha que as naves inimigas
				if(qntnaves==0)//nenhuma nave 3 asteroides
				{
					if(qntasteroides==0)//1 asteroides
					{
						asteroide[i] = xaleatorio;
					}
					else if(qntasteroides==1)//2 asteroides
					{
						if(i==0)
						{
							asteroide[i] = xaleatorio;
						}
						else
						{
							if(asteroide[i-1] != xaleatorio)
							{
								asteroide[i] = xaleatorio;
							}
						}
					}
					else if(qntasteroides==2)//3 asteroides
					{
						if(i==0)
						{
							asteroide[i] = xaleatorio;
						}
						else
						{
							if(i==1)
							{
								if(asteroide[i-1] != xaleatorio)
								{
									asteroide[i] = xaleatorio;
								}
							}
							else
							{
								if(asteroide[i-2] != xaleatorio)
								{
									if(asteroide[i-1] != xaleatorio)
									{
										asteroide[i] = xaleatorio;
									}
								}
							}
						}
					}
				}
				else if(qntnaves==1)//1 nave 3 asteroides
				{
					if(qntasteroides==0)//1 asteroide
					{
						if(naveix[0] != xaleatorio)
						{
							asteroide[i] = xaleatorio;
						}
					}
					else if(qntasteroides==1)//2 asteroides
					{
						if(i==0)
						{
							if(naveix[0] != xaleatorio)
							{
								asteroide[i] = xaleatorio;
							}
						}
						else
						{
							if((asteroide[i-1] != xaleatorio) && (naveix[0] != xaleatorio))
							{
								asteroide[i] = xaleatorio;
							}
						}
					}
					else if(qntasteroides==2)//3 asteroides
					{
						if(i==0)
						{
							if(naveix[0] != xaleatorio)
							{
								asteroide[i] = xaleatorio;
							}
						}
					
						else if(i==1)
						{
							if((asteroide[i-1] != xaleatorio) && (naveix[0] != xaleatorio))
							{
								asteroide[i] = xaleatorio;
							}
						}
						else if(i==2)
						{
							if(((asteroide[i-2] != xaleatorio) && (asteroide[i-1] != xaleatorio)) && (naveix[0] != xaleatorio))
							{
									asteroide[i] = xaleatorio;
							}
						}
					}
				}
				else if(qntnaves==2)//2 naves 3 asteroides
				{
					if(qntasteroides==0)//1 asteroide
					{
						if((naveix[0] != xaleatorio) && (naveix[1] != xaleatorio))
						{
							asteroide[i] = xaleatorio;
						}
					}
					else if(qntasteroides==1)//2 asteroides
					{
						if(i==0)
						{
							if((naveix[0] != xaleatorio) && (naveix[1] != xaleatorio))
							{
								asteroide[i] = xaleatorio;
							}
						}
						else
						{
							if((asteroide[i-1] != xaleatorio) && ((naveix[0] != xaleatorio) && (naveix[1] != xaleatorio)))
							{
								asteroide[i] = xaleatorio;
							}
						}
					}
					else if(qntasteroides==2)//3 asteroides
					{
						if(i==0)
						{
							if((naveix[0] != xaleatorio) && (naveix[1] != xaleatorio))
							{
								asteroide[i] = xaleatorio;
							}
						}
					
						else if(i==1)
						{
							if((asteroide[i-1] != xaleatorio) && ((naveix[0] != xaleatorio) && (naveix[1] != xaleatorio)))
							{
								asteroide[i] = xaleatorio;
							}
						}
						else if(i==2)
						{
							if(((asteroide[i-2] != xaleatorio) && (asteroide[i-1] != xaleatorio)) && ((naveix[0] != xaleatorio) && (naveix[1] != xaleatorio)))
							{
									asteroide[i] = xaleatorio;
							}
						}
					}
				}
				else if(qntnaves==3)//3 naves 2 asteroides
				{
					if(qntasteroides==0)//1 asteroide
					{
						if(((naveix[0] != xaleatorio) && (naveix[1] != xaleatorio)) && (naveix[2] != xaleatorio))
						{
							asteroide[i] = xaleatorio;
						}
					}
					else if(qntasteroides==1)//2 asteroides
					{
						if(i==0)
						{
							if(((naveix[0] != xaleatorio) && (naveix[1] != xaleatorio)) && (naveix[2] != xaleatorio))
							{
								asteroide[i] = xaleatorio;
							}
						}
						else
						{
							if(((asteroide[i-1] != xaleatorio) && (naveix[0] != xaleatorio)) && ((naveix[1] != xaleatorio) && (naveix[2] != xaleatorio)))
							{
								asteroide[i] = xaleatorio;
							}
						}
					}
				}
				
				tamasteroide[i] += rand()%41;
					
				rot[i] = rand()%2;
			}
		}
	}
	
	else if(modo==3)
	{
		int teste = rand()%1000;
	
		if(testex[0]==0)
		{
			testex[0] = 0;
		}
		
		//naves inimigas
		if(navex != testex[0]) //a nave mudou de posição
		{
			testex[0] = navex;
			
			int test = rand()%50;
			
			if(test<30)
			{
				if(((qntasteroides + qntnaves) < 4) && qntnaves<3) //se naves+asteroides não superam 5
				{	
					if(qntasteroides==0) //1 asteroide 3 naves
					{
						if(qntnaves==0) //nave 1
						{
							if(asteroide[0] != navex) 
							{
								naveix[qntnaves] = navex;
								naveiz[qntnaves] = -1000;
								zi[qntnaves] = -360;
								tiroi[qntnaves] = 0;
								disparoi[qntnaves] = 0;
								qntnaves++;
							}
						}
						
						else if(qntnaves==1) //nave 2
						{
							if((naveix[0] != navex) && (asteroide[0] != navex))
							{
								naveix[qntnaves] = navex;
								naveiz[qntnaves] = -1000;
								zi[qntnaves] = -360;
								tiroi[qntnaves] = 0;
								disparoi[qntnaves] = 0;
								qntnaves++;
							}
						}
						
						else if(qntnaves==2) //nave 3
						{
							if(((naveix[0] != navex) && (naveix[1] != navex)) && (asteroide[0] != navex))
							{
								naveix[qntnaves] = navex;
								naveiz[qntnaves] = -1000;
								zi[qntnaves] = -360;
								tiroi[qntnaves] = 0;
								disparoi[qntnaves] = 0;
								qntnaves++;
							}
						}
					}
					else if(qntasteroides==1) //2 asteroides 3 naves
					{
						if(qntnaves==0) //nave 1
						{
							if((asteroide[0] != navex) && (asteroide[1] != navex)) 
							{
								naveix[qntnaves] = navex;
								naveiz[qntnaves] = -1000;
								zi[qntnaves] = -360;
								tiroi[qntnaves] = 0;
								disparoi[qntnaves] = 0;
								qntnaves++;
							}
						}
						
						else if(qntnaves==1) //nave 2
						{
							if((naveix[0] != navex) && ((asteroide[0] != navex) && (asteroide[1] != navex)))
							{
								naveix[qntnaves] = navex;
								naveiz[qntnaves] = -1000;
								zi[qntnaves] = -360;
								tiroi[qntnaves] = 0;
								disparoi[qntnaves] = 0;
								qntnaves++;
							}
						}
						
						else if(qntnaves==2) //nave 3
						{
							if(((naveix[0] != navex) && (naveix[1] != navex)) && ((asteroide[0] != navex) && (asteroide[1] != navex)))
							{
								naveix[qntnaves] = navex;
								naveiz[qntnaves] = -1000;
								zi[qntnaves] = -360;
								tiroi[qntnaves] = 0;
								disparoi[qntnaves] = 0;
								qntnaves++;
							}
						}
					}
					else if(qntasteroides==2) //3 asteroides 2 naves
					{
						if(qntnaves==0) //nave 1
						{
							if(((asteroide[0] != navex) && (asteroide[1] != navex)) && (asteroide[2] != navex)) 
							{
								naveix[qntnaves] = navex;
								naveiz[qntnaves] = -1000;
								zi[qntnaves] = -360;
								tiroi[qntnaves] = 0;
								disparoi[qntnaves] = 0;
								qntnaves++;
							}
						}
						
						else if(qntnaves==1) //nave 2
						{
							if(((naveix[0] != navex) && (asteroide[0] != navex)) && ((asteroide[1] != navex) && (asteroide[2] != navex)))
							{
								naveix[qntnaves] = navex;
								naveiz[qntnaves] = -1000;
								zi[qntnaves] = -360;
								tiroi[qntnaves] = 0;
								disparoi[qntnaves] = 0;
								qntnaves++;
							}
						}
					}
				}
			}
		}
	
		//asteroides
		if(teste < 10 && zasteroide > 1200)
		{
			zasteroide = -1200;
			
			qntasteroides = rand()%3;
			
			if((qntasteroides + qntnaves) > 4)
			{
				qntasteroides = 4 - qntnaves;
			}
			
			for(int i=0; i<qntasteroides+1; i++)
			{
				posaleatoria = rand()%5;
				
				tamasteroide[i] = 50;
			
				if(posaleatoria == 0)
				{
					xaleatorio = trilha1;	
				}
				
				else if(posaleatoria == 1)
				{
					xaleatorio = trilha2;	
				}
				
				else if(posaleatoria == 2)
				{
					xaleatorio = trilha3;	
				}
				
				else if(posaleatoria == 3)
				{
					xaleatorio = trilha4;	
				}
				
				else if(posaleatoria == 4)
				{
					xaleatorio = trilha5;	
				}
				
				//Evitar gerar mais de 1 asteroide na mesma posição e na mesma trilha que as naves inimigas
				if(qntnaves==0)//nenhuma nave 3 asteroides
				{
					if(qntasteroides==0)//1 asteroides
					{
						asteroide[i] = xaleatorio;
					}
					else if(qntasteroides==1)//2 asteroides
					{
						if(i==0)
						{
							asteroide[i] = xaleatorio;
						}
						else
						{
							if(asteroide[i-1] != xaleatorio)
							{
								asteroide[i] = xaleatorio;
							}
						}
					}
					else if(qntasteroides==2)//3 asteroides
					{
						if(i==0)
						{
							asteroide[i] = xaleatorio;
						}
						else
						{
							if(i==1)
							{
								if(asteroide[i-1] != xaleatorio)
								{
									asteroide[i] = xaleatorio;
								}
							}
							else
							{
								if(asteroide[i-2] != xaleatorio)
								{
									if(asteroide[i-1] != xaleatorio)
									{
										asteroide[i] = xaleatorio;
									}
								}
							}
						}
					}
				}
				else if(qntnaves==1)//1 nave 3 asteroides
				{
					if(qntasteroides==0)//1 asteroide
					{
						if(naveix[0] != xaleatorio)
						{
							asteroide[i] = xaleatorio;
						}
					}
					else if(qntasteroides==1)//2 asteroides
					{
						if(i==0)
						{
							if(naveix[0] != xaleatorio)
							{
								asteroide[i] = xaleatorio;
							}
						}
						else
						{
							if((asteroide[i-1] != xaleatorio) && (naveix[0] != xaleatorio))
							{
								asteroide[i] = xaleatorio;
							}
						}
					}
					else if(qntasteroides==2)//3 asteroides
					{
						if(i==0)
						{
							if(naveix[0] != xaleatorio)
							{
								asteroide[i] = xaleatorio;
							}
						}
					
						else if(i==1)
						{
							if((asteroide[i-1] != xaleatorio) && (naveix[0] != xaleatorio))
							{
								asteroide[i] = xaleatorio;
							}
						}
						else if(i==2)
						{
							if(((asteroide[i-2] != xaleatorio) && (asteroide[i-1] != xaleatorio)) && (naveix[0] != xaleatorio))
							{
									asteroide[i] = xaleatorio;
							}
						}
					}
				}
				else if(qntnaves==2)//2 naves 3 asteroides
				{
					if(qntasteroides==0)//1 asteroide
					{
						if((naveix[0] != xaleatorio) && (naveix[1] != xaleatorio))
						{
							asteroide[i] = xaleatorio;
						}
					}
					else if(qntasteroides==1)//2 asteroides
					{
						if(i==0)
						{
							if((naveix[0] != xaleatorio) && (naveix[1] != xaleatorio))
							{
								asteroide[i] = xaleatorio;
							}
						}
						else
						{
							if((asteroide[i-1] != xaleatorio) && ((naveix[0] != xaleatorio) && (naveix[1] != xaleatorio)))
							{
								asteroide[i] = xaleatorio;
							}
						}
					}
					else if(qntasteroides==2)//3 asteroides
					{
						if(i==0)
						{
							if((naveix[0] != xaleatorio) && (naveix[1] != xaleatorio))
							{
								asteroide[i] = xaleatorio;
							}
						}
					
						else if(i==1)
						{
							if((asteroide[i-1] != xaleatorio) && ((naveix[0] != xaleatorio) && (naveix[1] != xaleatorio)))
							{
								asteroide[i] = xaleatorio;
							}
						}
						else if(i==2)
						{
							if(((asteroide[i-2] != xaleatorio) && (asteroide[i-1] != xaleatorio)) && ((naveix[0] != xaleatorio) && (naveix[1] != xaleatorio)))
							{
									asteroide[i] = xaleatorio;
							}
						}
					}
				}
				else if(qntnaves==3)//3 naves 2 asteroides
				{
					if(qntasteroides==0)//1 asteroide
					{
						if(((naveix[0] != xaleatorio) && (naveix[1] != xaleatorio)) && (naveix[2] != xaleatorio))
						{
							asteroide[i] = xaleatorio;
						}
					}
					else if(qntasteroides==1)//2 asteroides
					{
						if(i==0)
						{
							if(((naveix[0] != xaleatorio) && (naveix[1] != xaleatorio)) && (naveix[2] != xaleatorio))
							{
								asteroide[i] = xaleatorio;
							}
						}
						else
						{
							if(((asteroide[i-1] != xaleatorio) && (naveix[0] != xaleatorio)) && ((naveix[1] != xaleatorio) && (naveix[2] != xaleatorio)))
							{
								asteroide[i] = xaleatorio;
							}
						}
					}
				}
				
				tamasteroide[i] += rand()%41;
					
				rot[i] = rand()%2;
			}
		}
	}
}

void DesenhoAsteroide(int i)
{     
	if(modo==2)
	{
		glPushMatrix();               
	    glTranslatef(asteroide[i],0,zasteroide);
	    glColor3f(0.9, 0.9, 0.9); 
	    if(rot[i]==1)
	    {
	    	glRotatef(rotacao,1,1,0);       //ajusta direcao da rotação	
		}
		else if(rot[i]==0)
		{
			glRotatef(rotacao,-1,-1,0);       //ajusta direcao da rotação	
		}
	    glScalef(tamasteroide[i],tamasteroide[i],tamasteroide[i]);
	    glShadeModel(GL_SMOOTH);
	    glutSolidIcosahedron();
	    glPopMatrix();
	}
	else if(modo==3)
	{
		glPushMatrix();               
	    glTranslatef(asteroide[i],0,zasteroide);
	    glColor3f(0.9, 0.9, 0.9); 
	    if(rot[i]==1)
	    {
	    	glRotatef(rotacao,1,1,0);       //ajusta direcao da rotação	
		}
		else if(rot[i]==0)
		{
			glRotatef(rotacao,-1,-1,0);       //ajusta direcao da rotação	
		}
	    glScalef(tamasteroide[i],tamasteroide[i],tamasteroide[i]);
	    glShadeModel(GL_SMOOTH);
	    glutSolidIcosahedron();
	    glPopMatrix();
	}
}

void DesenhoNaveInimiga(int i)
{   
	if(modo==2)
	{
		glPushMatrix();
		glTranslatef(naveix[i],0,naveiz[i]);
		
		
		//corpo
		glPushMatrix();	
		glColor3f(0.5f,0.5f,0.5f);		
		glutSolidSphere(30,30,30);
		glPopMatrix();
		
		
		//arma(circulo vermelho)
		glPushMatrix();
		glTranslatef(0,30,0);
		glRotatef(-90,1,0,0);
		glScalef(1,1,0);
		glColor3f(1.0f,0.0f,0.0f);	
		glutSolidSphere(15,30,30);
		glPopMatrix();
		
		//asas direita
		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(25,12.5,12.5);
		glVertex3f(25,12.5,-12.5);
		glVertex3f(85,0,0);
	glEnd(); 
	
			glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(25,-12.5,12.5);
		glVertex3f(25,-12.5,-12.5);
		glVertex3f(85,0,0);
	glEnd(); 
		
		//asas esquerda
			glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-25,12.5,12.5);
		glVertex3f(-25,12.5,-12.5);
		glVertex3f(-85,0,0);
	glEnd(); 
	
			glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-25,-12.5,12.5);
		glVertex3f(-25,-12.5,-12.5);
		glVertex3f(-85,0,0);
	glEnd(); 
		
		
		//Escudo da asa direita
		glBegin(GL_LINE_STRIP);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(85,0,-60);
		glVertex3f(85,60,-30);
		
		glVertex3f(85,60,30);
		glVertex3f(85,0,60);
		
		glVertex3f(85,-60,30);
		glVertex3f(85,-60,-30);
		
		glVertex3f(85,0,-60);
		
	glEnd(); 	
	
		//detalhe dentro da asa direita
		glBegin(GL_LINES);
		glVertex3f(85,60,-30);
		glVertex3f(85,-60,30);
		
		glVertex3f(85,60,30);
		glVertex3f(85,-60,-30);
		
		glVertex3f(85,0,60);
		glVertex3f(85,0,-60);
		
		
	glEnd(); 
	
		//Escudo da asa esquerda
		
		glScalef(-1,1,1);  // inverte lado da asa
		
		glBegin(GL_LINE_STRIP);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(85,0,-60);
		glVertex3f(85,60,-30);
		
		glVertex3f(85,60,30);
		glVertex3f(85,0,60);
		
		glVertex3f(85,-60,30);
		glVertex3f(85,-60,-30);
		
		glVertex3f(85,0,-60);
		
	glEnd(); 	
	
		//detalhe dentro da asa esquerda
		glBegin(GL_LINES);
		glVertex3f(85,60,-30);
		glVertex3f(85,-60,30);
		
		glVertex3f(85,60,30);
		glVertex3f(85,-60,-30);
		
		glVertex3f(85,0,60);
		glVertex3f(85,0,-60);
		
		
	glEnd(); 
		
		glPopMatrix(); // fim do translate	
	}
	
	else if(modo==3)
	{
		glPushMatrix();
		glTranslatef(naveix[i],0,naveiz[i]);
		
		
		//corpo
		glPushMatrix();	
		glColor3f(0.5f,0.5f,0.5f);		
		glutSolidSphere(30,30,30);
		glPopMatrix();
		
		
		//arma(circulo vermelho)
		glPushMatrix();
		glTranslatef(0,0,30);
		glScalef(1,1,0);
		glColor3f(1.0f,0.0f,0.0f);	
		glutSolidSphere(15,30,30);
		glPopMatrix();
		
		//asas direita
		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(25,12.5,12.5);
		glVertex3f(25,12.5,-12.5);
		glVertex3f(85,0,0);
	glEnd(); 
	
			glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(25,-12.5,12.5);
		glVertex3f(25,-12.5,-12.5);
		glVertex3f(85,0,0);
	glEnd(); 
		
		//asas esquerda
			glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-25,12.5,12.5);
		glVertex3f(-25,12.5,-12.5);
		glVertex3f(-85,0,0);
	glEnd(); 
	
			glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-25,-12.5,12.5);
		glVertex3f(-25,-12.5,-12.5);
		glVertex3f(-85,0,0);
	glEnd(); 
		
		
		//Escudo da asa direita
		glBegin(GL_LINE_STRIP);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(85,0,-60);
		glVertex3f(85,60,-30);
		
		glVertex3f(85,60,30);
		glVertex3f(85,0,60);
		
		glVertex3f(85,-60,30);
		glVertex3f(85,-60,-30);
		
		glVertex3f(85,0,-60);
		
	glEnd(); 	
	
		//detalhe dentro da asa direita
		glBegin(GL_LINES);
		glVertex3f(85,60,-30);
		glVertex3f(85,-60,30);
		
		glVertex3f(85,60,30);
		glVertex3f(85,-60,-30);
		
		glVertex3f(85,0,60);
		glVertex3f(85,0,-60);
		
		
	glEnd(); 
	
		//Escudo da asa esquerda
		
		glScalef(-1,1,1);  // inverte lado da asa
		
		glBegin(GL_LINE_STRIP);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(85,0,-60);
		glVertex3f(85,60,-30);
		
		glVertex3f(85,60,30);
		glVertex3f(85,0,60);
		
		glVertex3f(85,-60,30);
		glVertex3f(85,-60,-30);
		
		glVertex3f(85,0,-60);
		
	glEnd(); 	
	
		//detalhe dentro da asa esquerda
		glBegin(GL_LINES);
		glVertex3f(85,60,-30);
		glVertex3f(85,-60,30);
		
		glVertex3f(85,60,30);
		glVertex3f(85,-60,-30);
		
		glVertex3f(85,0,60);
		glVertex3f(85,0,-60);
		
		
	glEnd(); 
		
		glPopMatrix(); // fim do translate		
	}
	
 }	
 
 void Eixos(int x, int y, int z)
 {
 	//olhando para x
 	if(x==1)
 	{
		//Eixo y
		glPushMatrix();	
		glColor3f(0,1,0);
		glTranslatef(0,-450,500);
		
		glPushMatrix();	
		glTranslatef(0,27,0);
		glRotatef(-90,0,1,0);
		glScalef(0.9,1,1);
		glutSolidCone(6,1,3,2); 
		glPopMatrix();
		
		glScalef(1,12,0.8);
		glutSolidCube(4); 
		glPopMatrix();
		
		//Eixo z
		glPushMatrix();	
		glColor3f(0,0,1);
		glTranslatef(0,-450,499);
		
		glPushMatrix();	
		glTranslatef(0,0,20);
		glRotatef(90,1,0,0);
		glRotatef(-90,0,1,0);
		glutSolidCone(6,1,3,2); 
		glPopMatrix();
		
		glScalef(1,1,9);
		glutSolidCube(4); 
		glPopMatrix();
	}
	
	//olhando para y
	else if(y==1)
 	{
 		//Eixo x
		glPushMatrix();	
		glColor3f(1,0,0);
		glTranslatef(500,-400,450);
		
		glPushMatrix();	
		glTranslatef(20,0,0);
		glRotatef(-90,0,1,0);
		glRotatef(-90,1,0,0);
		glutSolidCone(6,1,3,2); 
		glPopMatrix();
		
		glScalef(9,1,1);
		glutSolidCube(4); 
		glPopMatrix();
		
		//Eixo z
		glPushMatrix();	
		glColor3f(0,0,1);
		glTranslatef(500,-400,450);
		
		glPushMatrix();	
		glTranslatef(0,0,25);
		glRotatef(180,0,0,1);
		glRotatef(90,1,0,0);
		glutSolidCone(6,1,3,2); 
		glPopMatrix();
		
		glScalef(0.75,1,15);
		glutSolidCube(4); 
		glPopMatrix();
	}
	
	//olhando para z
	else if(z==1)
 	{
 		//Eixo x
		glPushMatrix();	
		glColor3f(1,0,0);
		glTranslatef(500,-450,+40);
		
		glPushMatrix();	
		glTranslatef(20,0,0);
		glRotatef(-90,0,0,1);
		glutSolidCone(6,1,3,2); 
		glPopMatrix();
		
		glScalef(9,1,1);
		glutSolidCube(4); 
		glPopMatrix();
		
		//Eixo y
		glPushMatrix();	
		glColor3f(0,1,0);
		glTranslatef(500,-450,40);
		
		glPushMatrix();	
		glTranslatef(0,27,0);
		glScalef(0.9,1,1);
		glutSolidCone(6,1,3,2); 
		glPopMatrix();
		
		glScalef(0.8,12,1);
		glutSolidCube(4); 
		glPopMatrix();
	}
 }
 
 void DesenhoNave()
 {
	if(modo==2)
	{	
		glPushMatrix();
		glTranslatef(navex,navey,navez);
		glScalef(0.5,0.5,0.5);
		
		if(d==1 || a==1)
		{
		glRotatef(naverotY,0,0,1);	
		}
		
		glPushMatrix();	
		glColor3f(1,1,1);		
		glScalef(1.5,1,8);
		glutSolidCube(20);
		glPopMatrix();
		
		//asas
		glPushMatrix();		
		glColor3f(0.0f,0.5f,1.8);	
		glBegin(GL_POLYGON);                                              
	                glVertex3f(15, 11, 20);                          
	                glVertex3f( 15, 11, 60);                        
	                glVertex3f( 110,31, 50);                          
	                glVertex3f(110,31, 30);                          
	        glEnd();
		glFlush(); 
	
		glBegin(GL_POLYGON);                                              
	                glVertex3f(-15, 11, 20);                          
	                glVertex3f( -15, 11, 60);                        
	                glVertex3f( -110,31, 50);                          
	                glVertex3f(-110,31, 30);                        
	        glEnd();
		glFlush();
		
		glBegin(GL_POLYGON);                                              
	                glVertex3f(-15, -10, 20);                          
	                glVertex3f(-15, -10, 60);                        
	                glVertex3f(-110,-30, 50);                          
	                glVertex3f(-110,-30, 30);                          
	        glEnd();
		glFlush();    
	
		glBegin(GL_POLYGON);                                              
	                glVertex3f(15, -10, 20);                          
	                glVertex3f(15, -10, 60);                        
	                glVertex3f(110,-30, 50);                          
	                glVertex3f(110,-30, 30);                          
	        glEnd();
		glFlush(); 
		
		//aerofolio superior
		glBegin(GL_POLYGON);                                              
	                glVertex3f(5, 10, 20);                          
	                glVertex3f(5, 10, 60);                        
	                glVertex3f(5,30, 50);                          
	                glVertex3f(5,30, 30);                          
	        glEnd();
		glFlush();                
	       	
		glBegin(GL_POLYGON);                                              
	                glVertex3f(-5, 10, 20);                          
	                glVertex3f(-5, 10, 60);                        
	                glVertex3f(-5,30, 50);                          
	                glVertex3f(-5,30, 30);                          
	        glEnd();
		glFlush();                
	       	glPopMatrix();
		
		//armas	
		glPushMatrix();				
		glTranslatef(20,0,-30);	
		glScalef(4,1,1);
		glutSolidCube(10);
		glPopMatrix();
		
		glPushMatrix();				
		glTranslatef(35,0,-65);	
		glScalef(1,1,8);
		glutSolidCube(10);
		glPopMatrix();
	
		glPushMatrix();				
		glTranslatef(-20,0,-30);	
		glScalef(4,1,1);
		glutSolidCube(10);
		glPopMatrix();
		
		glPushMatrix();				
		glTranslatef(-35,0,-65);	
		glScalef(1,1,8);
		glutSolidCube(10);
		glPopMatrix();
		
		//turbinas
		glPushMatrix();	
		glColor3f(0.5f,0.5f,0.5f);
		glTranslatef(30,-5,40);
		glutWireTorus(7.5, 5, 20, 20);
		glPopMatrix();
		
		glPushMatrix();
		glColor3f(0.5f,0.5f,0.5f);
		glTranslatef(-30,-5,40);
		glutWireTorus(7.5, 5, 20, 20);
		glPopMatrix();
		
		glPopMatrix();
	}
	
	else if(modo==3)
	{
		glPushMatrix();
		glTranslatef(navex,navey,navez);
		glScalef(0.5,0.5,0.5);
		
		if(d==1 || a==1)
		{
		glRotatef(naverotY,0,0,1);	
		}
		
		glPushMatrix();	
		glColor3f(1,1,1);		
		glScalef(1.5,1,8);
		glutSolidCube(20);
		glPopMatrix();
		
		//asas
		glPushMatrix();		
		glColor3f(0.0f,0.5f,1.8);	
		glBegin(GL_POLYGON);                                              
	                glVertex3f(15, 11, 20);                          
	                glVertex3f( 15, 11, 60);                        
	                glVertex3f( 110,31, 50);                          
	                glVertex3f(110,31, 30);                          
	        glEnd();
		glFlush(); 
	
		glBegin(GL_POLYGON);                                              
	                glVertex3f(-15, 11, 20);                          
	                glVertex3f( -15, 11, 60);                        
	                glVertex3f( -110,31, 50);                          
	                glVertex3f(-110,31, 30);                        
	        glEnd();
		glFlush();
		
		glBegin(GL_POLYGON);                                              
	                glVertex3f(-15, -10, 20);                          
	                glVertex3f(-15, -10, 60);                        
	                glVertex3f(-110,-30, 50);                          
	                glVertex3f(-110,-30, 30);                          
	        glEnd();
		glFlush();    
	
		glBegin(GL_POLYGON);                                              
	                glVertex3f(15, -10, 20);                          
	                glVertex3f(15, -10, 60);                        
	                glVertex3f(110,-30, 50);                          
	                glVertex3f(110,-30, 30);                          
	        glEnd();
		glFlush(); 
		
		//aerofolio superior
		glBegin(GL_POLYGON);                                              
	                glVertex3f(5, 10, 20);                          
	                glVertex3f(5, 10, 60);                        
	                glVertex3f(5,30, 50);                          
	                glVertex3f(5,30, 30);                          
	        glEnd();
		glFlush();                
	       	
		glBegin(GL_POLYGON);                                              
	                glVertex3f(-5, 10, 20);                          
	                glVertex3f(-5, 10, 60);                        
	                glVertex3f(-5,30, 50);                          
	                glVertex3f(-5,30, 30);                          
	        glEnd();
		glFlush();                
	       	glPopMatrix();
		
		//armas	
		glPushMatrix();				
		glTranslatef(20,0,-30);	
		glScalef(4,1,1);
		glutSolidCube(10);
		glPopMatrix();
		
		glPushMatrix();				
		glTranslatef(35,0,-65);	
		glScalef(1,1,8);
		glutSolidCube(10);
		glPopMatrix();
	
		glPushMatrix();				
		glTranslatef(-20,0,-30);	
		glScalef(4,1,1);
		glutSolidCube(10);
		glPopMatrix();
		
		glPushMatrix();				
		glTranslatef(-35,0,-65);	
		glScalef(1,1,8);
		glutSolidCube(10);
		glPopMatrix();
		
		//turbinas
		glPushMatrix();	
		glColor3f(0.5f,0.5f,0.5f);
		glTranslatef(30,-5,40);
		glutWireTorus(7.5, 5, 20, 20);
		glPopMatrix();
		
		glPushMatrix();
		glColor3f(0.5f,0.5f,0.5f);
		glTranslatef(-30,-5,40);
		glutWireTorus(7.5, 5, 20, 20);
		glPopMatrix();
		
		glPopMatrix();
	}
 }


 void keyboard (unsigned char key, int x, int y)
{	
	  
	  if (key=='a' && comeco==1) //move lado esquerdo
      { 	
      	 testex[0] = navex;
      
		 if(navex>-400 && pause==0)
		 {
		 	navex-=distx;	 //Variavel Responsavel pelo eixo x da nave
	      	naverotY = 30;   //variavel que altera o angulo da inclinacao
	      	d=1;
		 }
   }
	  
      if (key=='d' && comeco==1) //move lado direito
      {
    	 testex[0] = navex;
      	
		 if(navex<400 && pause==0)
		 {
		 	navex+=distx;
      	 	naverotY = -30;
      		a=1;
		 }
	 }
	  
	 if (key == 'k'){
	 	//tiros
		
		if(disparo==0 && comeco==1)
		{
			if(pause==0)
			{
				shoot=1;
	 			x1[0] = navex;	
			}
		}
		
		if(gameover==0)
		{
			comeco=1;
		}
	 }	
	 
	 if (key == 'x')
	 {
	 	perspecX=1;
	 	perspecY=0;
	 	perspecZ=0;
	 	Perspectiva(1, 0, 0);
	 }
	 
	 if (key == 'y')
	 {	
		perspecX=0;
	 	perspecY=1;
	 	perspecZ=0;
	 	Perspectiva(0, 1, 0);
	 }
	 
	 if (key == 'z')
	 {
	 	perspecX=0;
	 	perspecY=0;
	 	perspecZ=1;
	 	Perspectiva(0, 0, 1);
	 }
	 
	 if (key == 32)// Espaço
	 {
	 	if(pause==0)
	 	{
	 		pause=1;
		}
		
		else
	 	{
	 		pause=0;
		}
	 }
	 
	 if (key == 27)// Esc
	 {
	 	glutDestroyWindow(win);
    	exit (0);
	 }
}


void DisparoNaveInimiga(int i){	
	if(modo==2 && naveiz[i]==-360)
	{
		if((naveix[i] == navex) || (disparoi[i]==1))
		{	
			xi[i] = naveix[i];
			glPushMatrix();
			glTranslatef(xi[i]-16,-10,40);
				
			if(pause==0)
			zi[i] += tiroi[i]*0.015;
				
			glPushMatrix();
			glColor3f(1,0,0);
			glTranslatef(18+lz,0,zi[i]+10);
			glRotatef(naverotY,0,0,1);
			glScalef(1,1,4);
			glutSolidTorus(4,8,8,8);
			glPopMatrix();
			
			if(zi[i]>600)
			{
				zi[i] = -360;
				tiroi[i] = 0;
				disparoi[i] = 0;
			}
			else
			{
				if(pause==0)
				{
					tiroi[i] +=5;
					disparoi[i] = 1;
				}
			}	
					
			glPopMatrix();
		}
	}
	
	else if(modo==3 && naveiz[i]==-360)
	{
		if((naveix[i] == navex) || (disparoi[i]==1))
		{	
			xi[i] = naveix[i];
			glPushMatrix();
			glTranslatef(xi[i]-16,-10,40);
				
			if(pause==0)
			zi[i] += tiroi[i]*0.04;
				
			glPushMatrix();
			glColor3f(1,0,0);
			glTranslatef(18+lz,0,zi[i]+10);
			glRotatef(90,0,1,0);
			glRotatef(naverotY,0,0,1);
			glScalef(1,1,2);
			glutSolidTorus(4,8,8,8);
			glPopMatrix();
			
			if(zi[i]>600)
			{
				zi[i] = -360;
				tiroi[i] = 0;
				disparoi[i] = 0;
			}
			else
			{
				if(pause==0)
				{
					tiroi[i] +=5;
					disparoi[i] = 1;
				}
			}	
					
			glPopMatrix();
		}
	}
	
	if(naveiz[i]<-360 && modo==2)
	{
		naveiz[i]+=5;
	}
	
	else if(naveiz[i]<-360 && modo==3)
	{
		naveiz[i]+=10;
	}
}

void DisparoNave(){	
	if(modo==2)
	{
		if(shoot==1)
		{	
			glPushMatrix();
			glTranslatef(x1[0],0,0);
				
			if(pause==0)
			z -= tiro*0.015;
				
			glPushMatrix();
			glColor3f(0,0,1);
			glTranslatef(18+lz,0,z-10);
			glRotatef(naverotY,0,0,1);
			glutSolidSphere(4,8,8);
			glPopMatrix();
				
			glPushMatrix();
			glColor3f(0,0,1);
			glTranslatef(-17+lz,0,z-10);
			glRotatef(naverotY,0,0,1);
			glutSolidSphere(4,8,8);
			glPopMatrix();
			
			if(z<-600)
			{
				z = 360;
				tiro = 0;
				shoot = 0;
				disparo=0;
			}
			else
			{
				if(pause==0)
				{
					tiro +=5;
					disparo=1;
				}
			}	
					
			glPopMatrix();
		}
	}
	
	else if(modo==3)
	{
		if(shoot==1)
		{	
			glPushMatrix();
			glTranslatef(x1[0],0,0);
				
			if(pause==0)
			z -= tiro*0.05;
				
			glPushMatrix();
			glColor3f(0,0,1);
			glTranslatef(18+lz,0,z-10);
			glRotatef(naverotY,0,0,1);
			glutSolidSphere(4,8,8);
			glPopMatrix();
				
			glPushMatrix();
			glColor3f(0,0,1);
			glTranslatef(-17+lz,0,z-10);
			glRotatef(naverotY,0,0,1);
			glutSolidSphere(4,8,8);
			glPopMatrix();
			
			if(z<-1200)
			{
				z = 360;
				tiro = 0;
				shoot = 0;
				disparo=0;
			}
			else
			{
				if(pause==0)
				{
					tiro +=5;
					disparo=1;
				}
			}	
					
			glPopMatrix();
		}	
	}
}

void Escrever(char palavra[], float tamanho, int x, int y, int z, int cor)
{
	if(modo==2)
	{
		glPushMatrix();
		glTranslatef(x,y,z);
		glRotatef(-90,1,0,0);
		glScalef(tamanho,tamanho,tamanho);
		glLineWidth(5); // Define a espessura da linha
		if(cor==1)
		glColor3f(1,1,1);
		else if(cor==2)
		glColor3f(1,0,0);
		else if(cor==3)
		glColor3f(0,1,0);
		
		for(int i = 0; i < (int)strlen(palavra); i++)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, palavra[i]);
		}
		glPopMatrix();
	}

	if(modo==3)
	{
		glPushMatrix();
		glTranslatef(x,y,z);
		glRotatef(-45,1,0,0);
		glScalef(tamanho,tamanho,tamanho);
		glLineWidth(5); // Define a espessura da linha
		if(cor==1)
		glColor3f(1,1,1);
		else if(cor==2)
		glColor3f(1,0,0);
		
		for(int i = 0; i < (int)strlen(palavra); i++)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, palavra[i]);
		}
		glPopMatrix();
	}
}

void Inicializa (void)
{ 
	GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0}; 
	GLfloat posicaoLuz[4]={0.0, 50.0, 50.0, 1.0};
	
	// Especifica que a cor de fundo da janela será preta
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);
	
	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
	
	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente); 
	glLightfv(GL_LIGHT0, 0x1202, posicaoLuz);
	
	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);  
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);
}

void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Stroing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not acheived 
    while (clock() < start_time + milli_seconds) 
    ; 
} 

int main(int argc,char **argv)
{	
	srand(time(NULL));
	
	while(modo!=2 && modo!=3)
	{
		printf("Teclas:\n\n");
		printf("A = movimenta para a esquerda\n\n");
		printf("D = movimenta para a direita\n\n");
		printf("K = atira\n\n");
		printf("Espaco = pausa\n\n");
		printf("Esc = sair do jogo\n\n");
		printf("X = faz a camera olhar para o eixo X\n\n");
		printf("Y = faz a camera olhar para o eixo Y\n\n");
		printf("Z = faz a camera olhar para o eixo Z\n\n");
		
		printf("\n\nDigite o modo de jogo:	[2]2D	[3]3D\n");
		scanf("%d",&modo);
		printf("");
		
		if(modo!=2 && modo!=3)
		{
			CLEAR_SCREEN;
			printf("ERRO: Valor invalido!");
	        delay(2); 
			CLEAR_SCREEN;
		}
	}
	
    glutInit(&argc, argv); // Initializes glut
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    /*Avisa a GLUT que modo de exibição deve ser usado quando a janela é criada. 
    //  Nesse caso, permite: uma única janela; cores compostas por Verm. Verde e Azul; e, */
    glutInitWindowSize(1280,720);
    glutInitWindowPosition(150, 0);
    glutGameModeString("1280x720:16@60");
    win = glutEnterGameMode();
    glutDisplayFunc(DISPLAY);
    glutIdleFunc(DISPLAY);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    Inicializa();
    glutMainLoop();
    return 0;
}
