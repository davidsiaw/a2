
#include "map.hpp"
#include "entity.hpp"
#include "printlog.hpp"
#include "floortile.hpp"
#include "block.hpp"
#include "softblock.hpp"
#include "bonus.hpp"
#include "umpire.hpp"

#include <iostream>

namespace bomberman {


Map::Map(int w, int h) : 
	_width(w),
	_height(h),
	_map(w)
{
	for (int i = 0;i < w;i++)
	{
		_map[i] = std::vector<EntitySet>(h);
	}
}
    

Map::PositionCheck Map::CheckPosition(int x, int y) const
{
	if (x >= 0 && x < _width && y >= 0 && y < _height)
	{
		for (auto ntt : GetEntities(x, y))
		{
			if (
				typeid(*ntt) != typeid(architecture::FloorTile) &&
				typeid(*ntt) != typeid(Umpire)
				)
			{
				if (typeid(*ntt) == typeid(architecture::Block))
				{
					return HARD_OCCUPIED;
				}
				else if (typeid(*ntt) == typeid(architecture::SoftBlock))
				{
					return SOFT_OCCUPIED;
				}
				else
				{
					return KILLABLE_OCCUPIED;
				}
			}
		}

		return FREE;
	}
	
	return BOUNDARY;
}

const EntitySet &Map::GetEntities(int x, int y) const
{
	return _map[x][y];
}

EntitySet &Map::GetEntities(int x, int y)
{
	return _map[x][y];
}

bool Map::SetEntity(const EntityPtr &ntt)
{
    if (typeid(*ntt) == typeid(Umpire))
    {
        _umpire = ntt;
    }
    
	int x = ntt->GetX();
	int y = ntt->GetY();

	if (CheckPosition(x, y) == BOUNDARY)
	{
		return false;
	}
	
	_map[x][y].insert(ntt);

	if (ntt->id)
	{
		_entityIndex[ntt->id] = ntt;
	}

	return true;
}

EntityConstPtr Map::GetEntity(EntityID id) const
{
	auto entityIter = _entityIndex.find(id);
	if (entityIter == _entityIndex.end())
	{
		return EntityConstPtr();
	}
	return entityIter->second;
}

void Map::Clear()
{
	ForeachTile([&](int, int, const EntitySet &tile)
	{
		const_cast<EntitySet &>(tile).clear();
	});
}

void Map::ForeachTile(std::function<void(int, int, const EntitySet &)> func) const
{
	for (int x = 0; x < _width; x++)
	{
		for (int y = 0; y < _height; y++)
		{
			func(x, y, _map[x][y]);
		}
	}
}

void Map::ForeachEntity(std::function<void(const EntityConstPtr &)> func) const
{
	ForeachTile([&](int, int, const EntitySet &entities)
	{
		for (auto entity : entities)
		{
			func(entity);
		}
	});
}

void Map::ForeachEntity(std::function<void(const EntityPtr &)> func)
{
	ForeachTile([&](int, int, const EntitySet &entities)
	{
		for (auto entity : entities)
		{
			func(entity);
		}
	});
}

}
