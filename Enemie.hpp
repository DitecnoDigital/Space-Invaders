#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Enemie : public Drawable{
	private:
		Sprite sprite;
		int vel;
		int state;
		int cadencia;
		int timer;
		Vector2f point;
	public:
		Enemie(int x, int y, Texture &texture, Vector2f p);
		void Update();
		void ChangeDir();
		Vector2f Pos();
		void AumentarCadencia();
		virtual void draw(RenderTarget &rt, RenderStates rs) const;
};
