#pragma once

#include "EventListener.h"
#include "EventSystem.h"


EventListener::EventListener()
{
}

EventListener::~EventListener()
{
		EventSystem::getInstance()->removeListenerFromAllEvents(this);
		EventSystem::getInstance()->cleanupInstance();
}
