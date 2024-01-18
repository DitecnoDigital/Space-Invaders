#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>
#include "Player.hpp"
#include "Bullet.hpp"
#include "Enemie.hpp"
#include "Muro.hpp"

using namespace std;
using namespace sf;

void UpdatePlayer(Player &player, Bullet &bulletPlayer);
void UpdateBulletPlayer(Bullet &bulletPlayer, vector<vector<Enemie>> &enemies);
void UpdateEnemies(vector<vector<Enemie>> &enemies);
void UpdateBulletsEnemies(Player &player);
void UpdateMuro(vector<Muro> &muro,Bullet &bulletPlayer);

Texture spritesheet;

int timer=0;
int cadencia=125;

int dirEnemies=1;
int maxX,minX;
int cantEnemies;

vector<Bullet> bulletsEnemies;

vector<pair<int,Vector2f>> posicionMuro;

Vector2f sectionSpritesheet;

IntRect playerRect; 
IntRect bulletRect;
IntRect enemieRect;
IntRect muroRect;

bool bulletActive = false;


int main(){
	
	if(!spritesheet.loadFromFile("spritesheet.png")){
		cout<<"Error al cargar la textura\n";
	}
	
	Player player(288,555,spritesheet);
	

	Bullet bulletPlayer(0,0,spritesheet,IntRect(0,0,0,0),0);

	
	vector<vector<Enemie>> enemies(7,vector<Enemie>(12,Enemie(0,0,spritesheet,Vector2f(0,0))));

	
	 
	for(int i = 0; i < (int)enemies.size(); i++){
		for(int j = 0; j < (int)enemies[i].size(); j++){
			if(i==0){
				sectionSpritesheet = Vector2f(0,0);
			}else if(i<3){
				sectionSpritesheet = Vector2f(0,9+(8*4+4));
			}else if(i<5){
				sectionSpritesheet = Vector2f(0,18+(8*4+4)*2);
			}else if(i<7){
				sectionSpritesheet = Vector2f(0,27+(8*4+4)*3);
			}
			enemies[i][j]=Enemie(j*30+24,i*30+24,spritesheet,sectionSpritesheet);
		}
	}

	vector<Muro> muro(3,Muro(0,0,spritesheet));
	
	for(int i = 0; i < 3; i++){
		muro[i]=Muro(70+200*i,460,spritesheet);
	}

	
	RenderWindow window(VideoMode(600,600),"Space Invaders");
	window.setFramerateLimit(60);

	
	while(window.isOpen()){
		
		Event event;
		while(window.pollEvent(event)){
			if(event.type==Event::Closed) window.close();
		}
		
		UpdatePlayer(player,bulletPlayer);
				
		UpdateBulletPlayer(bulletPlayer, enemies);
		
		UpdateEnemies(enemies);
		
		UpdateBulletsEnemies(player);
		
		UpdateMuro(muro,bulletPlayer);
		
		if(!player.Vivo()){ system("clear"); cout<<"Perdiste\n"; window.close();}
		
		for(int i = 0; i < (int)enemies.size(); i++){
			for(int j = 0; j < (int)enemies[i].size(); j++){
				if(enemies[i][j].Pos().y>=480){
					system("clear");
					cout<<"Perdiste\n";
					window.close();
					return 0;
				}
			}
		}
		
		cantEnemies=0;
		
		for(int i = 0; i < (int)enemies.size(); i++) cantEnemies+=(int)enemies[i].size();
		
		if(cantEnemies==0){
			system("clear");
			cout<<"Ganaste\n";
			window.close();
		}
		
		window.clear();
		
		for(int i = 0; i < (int)bulletsEnemies.size(); i++){
			window.draw(bulletsEnemies[i]);
		}
		
		
		if(bulletActive) window.draw(bulletPlayer);

		for(int i = 0; i < (int)enemies.size(); i++){
			for(int j = 0; j < (int)enemies[i].size(); j++){
				window.draw(enemies[i][j]);
			}
		}		
		
		for(int i = 0; i < 3; i++) window.draw(muro[i]);
		
		window.draw(player);
		
		
		window.display();
	}
	return 0;
}

void UpdatePlayer(Player &player, Bullet &bulletPlayer){
		
		player.Update();
		
		if(player.Shoot()&&!bulletActive){
			Bullet bullet(player.Pos().x+24,player.Pos().y+12,spritesheet,IntRect(13*8+16,6*8+6,8,8),-10);
			bulletPlayer = bullet;
			bulletActive=true;
		}
}


void UpdateBulletPlayer(Bullet &bulletPlayer, vector<vector<Enemie>> &enemies){
	
	if(bulletActive){
		bulletPlayer.Update();
		if(bulletPlayer.Pos().y<-24) bulletActive = false;
			
		bulletRect=IntRect(bulletPlayer.Pos().x,bulletPlayer.Pos().y,3,8);
		for(int i = 0; i < (int)enemies.size(); i++){
			for(int j = 0; j < (int)enemies[i].size(); j++){
				enemieRect=IntRect(enemies[i][j].Pos().x,enemies[i][j].Pos().y,24,24);
				if(enemieRect.intersects(bulletRect)){
					enemies[i].erase(enemies[i].begin()+j);
					bulletActive=false;
					
					break;
				}
			}
			if(!bulletActive) break;
		}
		if(!bulletActive){
			for(int i = 0; i < (int)enemies.size(); i++){
				for(int j = 0; j < (int)enemies[i].size(); j++){
					enemies[i][j].AumentarCadencia();
					
				}
			}
			cadencia--;
		}
	}
}


void UpdateEnemies(vector<vector<Enemie>> &enemies){
	
	maxX=0;
	minX=600;
	for(int i = 0; i < (int)enemies.size(); i++){
		for(int j = 0; j < (int)enemies[i].size(); j++){
			maxX=max(maxX,(int)enemies[i][j].Pos().x+24*dirEnemies);
			minX=min(minX,(int)enemies[i][j].Pos().x+24*dirEnemies);
		}
	}
		
	if(minX<24||maxX>576){
		for(int i = 0; i < (int)enemies.size(); i++){
			for(int j = 0; j < (int)enemies[i].size(); j++){
				enemies[i][j].ChangeDir();
			}
		}
		dirEnemies*=-1;
	}
	
	for(int i = 0; i < (int)enemies.size(); i++){
		for(int j = 0; j < (int)enemies[i].size(); j++){
			enemies[i][j].Update();		
		}
	}
	
	timer++;
	
	if(timer>=cadencia&&(int)enemies[0].size()>0){
		timer=0;
		srand(time(NULL));
		int enem = rand()%(int)enemies[0].size();
		Bullet bullet = Bullet(enemies[0][enem].Pos().x+9,enemies[0][enem].Pos().y+24,spritesheet,IntRect(13*8+8,8*2+2,8,8),10);
		bulletsEnemies.push_back(bullet);
	}
	
}


void UpdateBulletsEnemies(Player &player){
	
	for(int i = 0; i < (int)bulletsEnemies.size(); i++){
		bulletsEnemies[i].Update();
	}
	
	for(int i = 0; i < (int)bulletsEnemies.size(); i++){
		if(bulletsEnemies[i].Pos().y>600){
			bulletsEnemies.erase(bulletsEnemies.begin()+i);
		}
	}
	
	playerRect=IntRect(player.Pos().x,player.Pos().y+9,48,15);
	for(int i = 0; i < (int)bulletsEnemies.size(); i++){
		bulletRect = IntRect(bulletsEnemies[i].Pos().x,bulletsEnemies[i].Pos().y,3,24);
		if(playerRect.intersects(bulletRect)){
			bulletsEnemies.erase(bulletsEnemies.begin()+i);
			player.QuitarVida();
		}
	}

}


void UpdateMuro(vector<Muro> &muro,Bullet &bulletPlayer){
	if(bulletActive){
		bulletRect=IntRect(bulletPlayer.Pos().x,bulletPlayer.Pos().y,3,8);
		for(int i = 0; i < 3; i++){
			muro[i].Pos(posicionMuro);
			for(int j = 0; j < (int) posicionMuro.size(); j++){
				muroRect=IntRect(posicionMuro[j].second.x,posicionMuro[j].second.y,24,24);
				if(muroRect.intersects(bulletRect)){
					muro[i].Colision(posicionMuro[j].first,false);
					bulletActive=false;
				}
			}
			if(!bulletActive) break;
		}
	}
	
	bool elim=false;
	
	for(int h = 0; h < (int)bulletsEnemies.size(); h++){
		bulletRect=IntRect(bulletsEnemies[h].Pos().x,bulletsEnemies[h].Pos().y,3,8);
		for(int i = 0; i < 3; i++){
			muro[i].Pos(posicionMuro);
			for(int j = 0; j < (int) posicionMuro.size(); j++){
				muroRect=IntRect(posicionMuro[j].second.x,posicionMuro[j].second.y,24,24);
				if(muroRect.intersects(bulletRect)){
					muro[i].Colision(posicionMuro[j].first,true);
					bulletsEnemies.erase(bulletsEnemies.begin()+h);
					elim=true;
					break;
				}
			}
			if(elim) break;
		}
	}
	
	for(int i = 0; i < 3; i++) muro[i].Update();
}

