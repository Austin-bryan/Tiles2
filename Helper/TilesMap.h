#pragma once
#include "ForwardDeclares.h"
#include <map>

struct FCoord_Less
{
	bool operator() (const FCoordPtr& lhs, const FCoordPtr& rhs) const
	{
		return const_cast<FCoord*>(lhs.Get()) < const_cast<FCoord*>(rhs.Get());
	}
};
 
using Tiles = std::map<FCoordPtr, ATile*, FCoord_Less>;
