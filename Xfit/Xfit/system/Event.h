#pragma once

#include "../system/Error.h"

template <bool autoReset,bool norifyAll>
class Event {
	mutex mutex;
	condition_variable value;
	bool trigger;
public:
	Event(bool _trigger=false) :trigger(_trigger) {}

	void Wait() {
		unique_lock<::mutex> lock(mutex);
		while (!trigger)value.wait(lock);
		if constexpr (autoReset)trigger = false;
	}
	void Set() {
		mutex.lock();
		trigger = true;
		if constexpr (norifyAll)value.notify_all();
		else value.notify_one();
		mutex.unlock();
	}
	void Reset() {
		mutex.lock();
		trigger = false;
		mutex.unlock();
	}
};