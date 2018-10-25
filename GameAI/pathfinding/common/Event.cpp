#include "Event.h"

Event::Event()
{
	mType = INVALID_EVENT_TYPE;
}

Event::Event(EventType type)
:mType(type)
{
}

Event::~Event()
{
}

void Event::setType(EventType type)
{
	mType = type;
}

const string& Event::getEventName() const
{
	if( mType > INVALID_EVENT_TYPE && mType < NUM_EVENT_TYPES )
	{
		return EVENT_NAMES[mType];
	}
	else
	{
		static const string nullString;
		return nullString;
	}
}
