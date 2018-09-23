#ifndef EVENT_SYS_H
#define EVENT_SYS_H

#include <map>
#include <cassert>
#include <Trackable.h>

class Event;
class EventListener;
enum EventType;

using namespace std;

class EventSystem:public Trackable
{
public:
	
	//Static class functions
	static EventSystem* getInstance()
	{
		assert(mESInstance != NULL);
		return mESInstance;
	}

	static bool hasInstance()
	{
		return mESInstance;
	}

	static void initInstance()
	{
		mESInstance = new EventSystem;
	}


	static void cleanupInstance()
	{
		if (mESInstance != NULL)
		{
			delete mESInstance;
			mESInstance = NULL;
		}
	}

	EventSystem();
	~EventSystem();

	void fireEvent( const Event& theEvent );
	void addListener( EventType type, EventListener* pListener );
	void removeListener( EventType type, EventListener* pListener );
	void removeListenerFromAllEvents( EventListener* pListener );

private:
	static EventSystem* mESInstance;

	multimap< EventType, EventListener* > mListenerMap;

	void dispatchAllEvents( const Event& theEvent );

};

#endif // !EVENT_SYS_H
