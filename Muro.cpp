#include "Muro.hpp"

Muro::Muro(int x, int y, Texture &texture){
	sprites.resize(5);
	state.resize(5,{0,0});
	for(int i = 0; i < 5; i++){
		sprites[i].setTexture(texture);
		sprites[i].setTextureRect(IntRect(16*8+9+state[i].first*9,14*8+14+state[i].second*9,8,8));
		sprites[i].setScale(3,3);
	}
	sprites[0].setPosition(x,y);
	sprites[1].setPosition(x+24,y);
	sprites[2].setPosition(x+48,y);
	sprites[3].setPosition(x,y+24);
	sprites[4].setPosition(x+48,y+24);
}

void Muro::Update(){
	for(int i = 0; i < 5; i++){
		sprites[i].setTextureRect(IntRect(16*8+9+state[i].first*9,14*8+14+state[i].second*9,8,8));
	}
}

void Muro::Pos(vector<pair<int,Vector2f>> &pos){
	pos.clear();
	for(int i = 0; i < 5; i++){
		if(state[i].first+state[i].second<5){
			pos.push_back({i,sprites[i].getPosition()});
		}
	}	
}

void Muro::Colision(int indice, bool up){
	if(up) state[indice].first++;
	else state[indice].second++;
}

void Muro::draw(RenderTarget &rt, RenderStates rs) const{
	for(int i = 0; i < 5; i++){
		if(state[i].first+state[i].second<5){
			rt.draw(sprites[i], rs);
		}
	}
}
