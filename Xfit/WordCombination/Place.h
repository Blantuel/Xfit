#pragma once

#include "math/Point.h"
#include "data/Array.h"

class PlaceEvent;

class Place;

struct PlacePack {
	Place* place;
	unsigned moveStamina;
};

class Place {
public:
	const unsigned nameIndex;
	Array<PlacePack*> otherPlacePacks;

	Array<PlaceEvent*> events;
};