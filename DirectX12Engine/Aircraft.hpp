#pragma once
#include "Entity.hpp"

class Aircraft : public Entity
{
public:

	enum class Type
	{
		Eagle,
		Raptor,
	};

public:
	Aircraft(Type type, Game* game);

private:
	virtual void	drawCurrent() const;
	virtual void	buildCurrent();

private:
	Type	m_Type;
	std::string		m_Sprite;
	RenderItem* m_AircraftRender;
};