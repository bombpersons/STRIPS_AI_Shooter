#ifndef AI_TILEDRAWER_HPP
#define AI_TILEDRAWER_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <AIGame/Macros.hpp>
#include <AIGame/Graphics/Spritebatch.hpp>

class TileDrawer {
public:
	// Constructors
	TileDrawer();
	virtual ~TileDrawer();

	// Draw
	void Draw(sf::RenderTarget* _target, sf::Texture* _texture, int _tilewidth, int _tileheight, float _destx, float _desty, float _destwidth, float _destheight, unsigned short* _data, int _datax, int _datay, int _datawidth, int _dataheight, int _totaldatawidth, int _totaldataheight);
	void DrawFlickerless(sf::RenderTarget* _target, sf::Texture* _texture, int _tilewidth, int _tileheight, float _destx, float _desty, float _destwidth, float _destheight, unsigned short* _data, int _datax, int _datay, int _datawidth, int _dataheight, int _totaldatawidth, int _totaldataheight);

private:
	// Vars
	SpriteBatch spritebatch;
	sf::RenderTexture rendertarget;

	// Make it so we can't copy this class.
	UNCOPYABLE(TileDrawer)
};

#endif