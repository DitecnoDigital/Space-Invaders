#include "Bullet.hpp"

Bullet::Bullet(int x, int y, Texture &texture, IntRect intRect, int v){
	sprite.setTexture(texture);
	sprite.setTextureRect(intRect);
	sprite.setPosition(x,y);
	sprite.setScale(3,3);
	vel=v;
}

void Bullet::Update(){
	sprite.move(0,vel);
}

Vector2f Bullet::Pos(){
	return sprite.getPosition();
}
	
void Bullet::draw(RenderTarget &rt, RenderStates rs) const{
	rt.draw(sprite,rs);
}
