#include <AIGame/Graphics/TileDrawer.hpp>

// Constructors
TileDrawer::TileDrawer() :
	spritebatch() {
}

TileDrawer::~TileDrawer() {
}

// Draw
void TileDrawer::Draw(sf::RenderTarget* _target, sf::Texture* _texture, int _tilewidth, int _tileheight, float _destx, float _desty, float _destwidth, float _destheight, unsigned short* _data, int _datax, int _datay, int _datawidth, int _dataheight, int _totaldatawidth, int _totaldataheight) {
	// Calculate the size of an individual tile
    int amounttilesx = (int)_texture->getSize().x / _tilewidth;
    int amounttilesy = (int)_texture->getSize().y / _tileheight;

	// Loop through the tiles and draw them.
	for (int x = _datax; x < _datax + _datawidth; ++x) {
		if (x < 0) {
			x = -1;
			continue;
		}
		if (x >= _totaldatawidth) {
			break;
		}
		for (int y = _datay; y < _datay + _dataheight; ++y) {
			if (y < 0) {
				y = -1;
				continue;
			}
			if (y >= _totaldataheight) {
				break;
			}

			// Figure out the portion of the texture to draw.
			unsigned short id = _data[y * _totaldatawidth + x];

			// Don't draw anything if it's 0
			if (id == 0)
				continue;
			id--; // An id of 1, is the first tile.

			int tilex = (id % amounttilesx) * _tilewidth;
			int tiley = (id / amounttilesx) * _tileheight;

			// Figure out the position and scale to draw the tile.
			sf::FloatRect rect((float)tilex, (float)tiley, (float)_tilewidth, (float)_tileheight);
            spritebatch.Draw(_target,
							 _texture,
							 sf::Vector2f(_destx + (x * _tilewidth), _desty + (y * _tileheight)),
                             sf::Vector2f(0, 0),
                             sf::Vector2f(1, 1),
                             0,
                             sf::Color::White,
                             &rect);
		}
	}

	// Flush
	spritebatch.Flush(_target);

	/*// Flip the buffers for the render texture.
	rendertarget.display();

	// Now draw the render texture to the window
	sf::Texture* rendertexture = const_cast<sf::Texture*>(&(rendertarget.getTexture()));
	sf::VertexArray vertexarray(sf::PrimitiveType::Quads, 4);
	vertexarray[0] = sf::Vertex(sf::Vector2f(0, 0), sf::Color::White, sf::Vector2f(0, 0));
	vertexarray[1] = sf::Vertex(sf::Vector2f(1, 0), sf::Color::White, sf::Vector2f(1, 0));
	vertexarray[2] = sf::Vertex(sf::Vector2f(1, 1), sf::Color::White, sf::Vector2f(1, 1));
	vertexarray[3] = sf::Vertex(sf::Vector2f(0, 1), sf::Color::White, sf::Vector2f(0, 1));
	
	// Set the texture.
	sf::RenderStates state = sf::RenderStates::Default;
	state.texture = rendertexture;
	state.transform = sf::Transform::Identity;

	// Disable the current view
	sf::View oldview = _target->getView();
	_target->setView(sf::View(sf::FloatRect(0, 0, 1, 1)));

	// Draw it.
	_target->draw(vertexarray);

	// Set it back
	_target->setView(oldview);*/
}


void TileDrawer::DrawFlickerless(sf::RenderTarget* _target, sf::Texture* _texture, int _tilewidth, int _tileheight, float _destx, float _desty, float _destwidth, float _destheight, unsigned short* _data, int _datax, int _datay, int _datawidth, int _dataheight, int _totaldatawidth, int _totaldataheight) {
   	// Calculate the size the render texture needs to be.
	int texwidth = _datawidth * _tilewidth;
	int texheight = _dataheight * _tileheight;

	// Do we have to re-create the texture?
	if (rendertarget.getSize().x != texwidth || rendertarget.getSize().y != texheight)
		rendertarget.create(texwidth, texheight); 

	// Clear the render texture
	rendertarget.clear(sf::Color::Red);
	
	// Calculate the size of an individual tile
    int amounttilesx = (int)_texture->getSize().x / _tilewidth;
    int amounttilesy = (int)_texture->getSize().y / _tileheight;

	// Loop through the tiles and draw them.
	for (int x = _datax; x < _datawidth; ++x) {
		if (x < 0) {
			x = -1;
			continue;
		}
		if (x >= _totaldatawidth) {
			break;
		}
		for (int y = _datay; y < _dataheight; ++y) {
			if (y < 0) {
				y = -1;
				continue;
			}
			if (y >= _totaldataheight) {
				break;
			}

			// Figure out the portion of the texture to draw.
			unsigned short id = _data[y * _totaldatawidth + x];

			// Don't draw anything if it's 0
			if (id == 0)
				continue;
			id--; // An id of 1, is the first tile.

			int tilex = (id % amounttilesx) * _tilewidth;
			int tiley = (id / amounttilesx) * _tileheight;

			// Figure out the position and scale to draw the tile.
			sf::FloatRect rect((float)tilex, (float)tiley, (float)_tilewidth, (float)_tileheight);
            spritebatch.Draw(&rendertarget,
							 _texture,
							 sf::Vector2f((float)(x * _tilewidth), (float)(y * _tileheight)),
                             sf::Vector2f(0, 0),
                             sf::Vector2f(1, 1),
                             0,
                             sf::Color::White,
                             &rect);
		}
	}

	// Flush
	spritebatch.Flush(&rendertarget);

	// Flip the buffers for the render texture.
	rendertarget.display();

	// Now draw the render texture to the window
	sf::Texture* rendertexture = const_cast<sf::Texture*>(&(rendertarget.getTexture()));
	sf::VertexArray vertexarray(sf::PrimitiveType::Quads, 4);
	vertexarray[0] = sf::Vertex(sf::Vector2f(0, 0), sf::Color::White, sf::Vector2f(0, 0));
	vertexarray[1] = sf::Vertex(sf::Vector2f(1, 0), sf::Color::White, sf::Vector2f(1, 0));
	vertexarray[2] = sf::Vertex(sf::Vector2f(1, 1), sf::Color::White, sf::Vector2f(1, 1));
	vertexarray[3] = sf::Vertex(sf::Vector2f(0, 1), sf::Color::White, sf::Vector2f(0, 1));
	
	// Set the texture.
	sf::RenderStates state = sf::RenderStates::Default;
	state.texture = rendertexture;
	state.transform = sf::Transform::Identity;

	// Disable the current view
	sf::View oldview = _target->getView();
	_target->setView(sf::View(sf::FloatRect(0, 0, 1, 1)));

	// Draw it.
	_target->draw(vertexarray);

	// Set it back
	_target->setView(oldview);
}
