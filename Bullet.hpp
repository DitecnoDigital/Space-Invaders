#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Bullet : public Drawable {
	private:
		Sprite sprite;
		int vel;
	public:
		Bullet(int x, int y, Texture &texture, IntRect intRect, int v);
		void Update();
		Vector2f Pos();
		virtual void draw(RenderTarget &rt, RenderStates rs) const;
};
