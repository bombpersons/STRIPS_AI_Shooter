#include <AIGame/Entities/EntityList.hpp>

// Constructors
EntityList::EntityList() {
}

EntityList::~EntityList() {
	Clear();
}

// Draw
void EntityList::Draw(Game* _game, float _delta, sf::RenderTarget* _target) {
	for (EntityListType::iterator it = entities.begin(); it != entities.end(); ++it) {
		for (int i = 0; i < (int)(*it).second.size(); ++i) {
			(*it).second[i]->Draw(_game, _delta, _target);
		}
	}
}

// Update
void EntityList::Update(Game* _game, float _delta) {
	for (EntityListType::iterator it = entities.begin(); it != entities.end();) {
		for (std::vector<Entity*>::iterator it2 = it->second.begin(); it2 != it->second.end();) {
			// Check if this entity is still alive.
			if ((*it2)->GetAlive() == false) {
				// Remove this entity from the list.
				delete (*it2);
				it2 = (*it).second.erase(it2);
			} else {
				// Update
				(*it2)->Update(_game, _delta);
			
				// Increment
				it2++;
			}
		}

		// If this list is empty, remove it.
		if ((*it).second.size() == 0) {
			it = entities.erase(it);
		} else {
			it++;
		}
	}
}

// Add / Remove entities
void EntityList::AddEntity(Entity* _entity) {
	// See if a sublist with the name of this entity exists.
	EntityListType::iterator iterator = entities.find(_entity->GetName());
	if (iterator == entities.end()) {
		// Add an entry
		entities.insert(std::pair<std::string, std::vector<Entity*> >(_entity->GetName(), std::vector<Entity*>()));

		// Get the iterator to that.
		iterator = entities.find(_entity->GetName());
	}

	// Add to the list
	iterator->second.push_back(_entity);
}

// Get entities
std::vector<Entity*>* EntityList::GetEntities(const std::string& _name) {
	EntityListType::iterator it = entities.find(_name);
	if (it == entities.end())
		return NULL;
	return &((*it).second);
}
Entity* EntityList::GetEntity(const std::string& _name) {
	std::vector<Entity*>* list = GetEntities(_name);
	if (list == NULL)
		return NULL;
	if (list->size() == 0)
		return NULL;
	return (*list)[0];
}

// Clear
void EntityList::Clear() {
	// Delete all of the entities
	for (EntityListType::iterator it = entities.begin(); it != entities.end(); ++it) {
		for (int i = 0; i < (int)(*it).second.size(); ++i) {
			delete (*it).second[i];
		}
	}
	entities.clear();
}