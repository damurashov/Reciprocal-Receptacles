#include <iostream>
#include <string>
#include <Rr/Subscription.hpp>

using namespace std;

namespace Topic {
struct Critical{};
struct Info{};
};

void unknownEvent(void)
{
	cout << "onEvent" << endl;
}

class Subscriber {
public:
	// For multiple types: Topic, SyncTrait, Types...
	using KeyParam = Rr::Subscription::KeyBase<Topic::Info, Rr::Subscription::StlSyncTrait, const std::string &, int>;

private:
	struct {
		// For one type: Type, [Topic, [SyncTrait]]
		Rr::Subscription::Key<void, Topic::Info> info;
		Rr::Subscription::Key<void> unknown;
		KeyParam parameter;
	} key;

public:
	Subscriber(): 
		key{{&Subscriber::onEvent, this},
			{&Subscriber::onEvent, this},
			{&Subscriber::updateParameter, this}}
		{
		}

	void updateParameter(const std::string &aName, int aValue)
	{
		cout << "updateParameter: " << aName << " " << aValue << endl;
	}

	void onEvent(void) {
		cout << "Subscriber::onEvent" << endl;
	}

	void enableSubscribe(bool aEnable)
	{
		key.info.enableSubscribe(aEnable);
		key.unknown.enableSubscribe(aEnable);
		key.parameter.enableSubscribe(aEnable);
	}
};

void notifyAll()
{
	std::string paramDuration{"duration"};
	int durationMs{42};
	Subscriber::KeyParam::notify(paramDuration, durationMs);
	Rr::Subscription::Key<void, Topic::Info>::notify();
	Rr::Subscription::Key<void>::notify();
}

int main(void)
{
	Rr::Subscription::Key<void, Topic::Info> keyUnkEvent{unknownEvent};
	Subscriber subscriber;

	cout << '1' << endl;
	notifyAll();

	subscriber.enableSubscribe(false);

	cout << '2' << endl;
	notifyAll();

	subscriber.enableSubscribe(true);
	keyUnkEvent.enableSubscribe(false);
	
	cout << '3' << endl;
	notifyAll();

	return 0;
}
