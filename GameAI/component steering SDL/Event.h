#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <trackable.h>

using namespace std;

enum EventType
{
	INVALID_EVENT_TYPE = -1,
	KEY_DOWN_EVENT,
	MOUSE_DOWN_EVENT, //Input system events

	ESC,
	D_KEY,
	S_KEY,
	C_KEY,
	A_KEY,
	W_KEY,
	G_KEY,
	UP_ARROW,//Input translator events, focused on buttons/keys
	DOWN_ARROW,
	LEFT_ARROW,
	RIGHT_ARROW,
	ENTER,	
	MOUSE_LEFT,
	PLAYER_MOVE_TO_MESSAGE,

	NUM_EVENT_TYPES //Tracks the number of event types
};

const string EVENT_NAMES[NUM_EVENT_TYPES] = {	
	"Key Down Event",											
	"Mouse Down Event",
	"Escape Key Event",
	"Up Arrow Event",
	"Down Arrow Event",
	"Left Arrow Event",
	"Right Arrow Event",
	"Space Event",
};

class Event:public Trackable
{
public:
	Event();
	Event( EventType type );
	virtual ~Event();

	void setType(EventType type);
	EventType getType() const { return mType; };
	const string& getEventName() const;

private:
	EventType mType;

};

#endif // !EVENT_H
