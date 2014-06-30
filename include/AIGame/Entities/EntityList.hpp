#ifndef AI_ENTITYLIST_HPP
#define AI_ENTITYLIST_HPP

#include <SFML/Graphics.hpp>

#include <AIGame/Macros.hpp>
#include <AIGame/Entities/Entity.hpp>

#include <unordered_map>
#include <vector>

class EntityList {
public:
	// Constructors
	EntityList();
	virtual ~EntityList();

	// Draw
	virtual void Draw(Game* _game, float _delta, sf::RenderTarget* _target);

	// Update 
	virtual void Update(Game* _game, float _delta);

	// Add / Remove
	void AddEntity(Entity* _entity);

	// Get entities
	std::vector<Entity*>* GetEntities(const std::string& _name);
	
	// Get all entities of a particular type.
	template <typename T>
	std::vector<T*> GetEntities() {
		std::vector<T*> list;
		for (EntityListType::iterator it = entities.begin(); it != entities.end(); ++it) {
			for (int i = 0; i < (int)(*it).second.size(); ++i) {
				T* entity = dynamic_cast<T*>((*it).second[i]);
				if (entity != NULL)
					list.push_back(entity);
			}
		}

		return list;
	}

	Entity* GetEntity(const std::string& _name);

	// Clear
	void Clear();

private:
	// A list of entities by name
	typedef std::unordered_map<std::string, std::vector<Entity*>> EntityListType;
	EntityListType entities;

	// Make sure we can't copy this class.
	UNCOPYABLE(EntityList)
};

#endif