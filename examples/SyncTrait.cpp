//
// LockPolicy.cpp
//
// Created: 2021-12-17
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#include <thread>
#include <chrono>
#include <iostream>
#include <list>
#include <Rr/Key.hpp>
#include <Rr/Module.hpp>
#include <Rr/SyncTrait.hpp>

namespace Topic {
struct Default;
}  // namespace Topic

template <class TfnSignature, class Ttopic = Topic::Default, class Tsync = Rr::DefaultMutexTrait>
using Key = typename Rr::Key<TfnSignature, Ttopic, std::list, Tsync>;

template <class TfnSignature, class Ttopic = Topic::Default, class Tsync = Rr::DefaultMutexTrait>
using Module = typename Rr::Module<TfnSignature, Ttopic, std::list, Tsync>;

struct DeviceInfo {
	const char *name;
	enum class Status {
		Disabled,
		Active,
	} status;
};

using DeviceModule = Module<DeviceInfo(), Topic::Default, Rr::DefaultGroupMutexTrait<1>>;
// using DeviceModule = Module<DeviceInfo(), Topic::Default, Rr::MockGroupMutexTrait<1>>;

struct Device {
	DeviceInfo di;
	DeviceModule key;
	DeviceInfo getDeviceInfo()
	{
		std::cout << "DeviceInfo, Status: " << (int)di.status << " ; name: " << di.name << std::endl;
		return di;
	}
	Device(const char *aName, DeviceInfo::Status aStatus): di{aName, aStatus}, key{&Device::getDeviceInfo, this}
	{
	}
	void setEnabled(bool aEnabled)
	{
		di.status = aEnabled ? DeviceInfo::Status::Active : DeviceInfo::Status::Disabled;
	}
	~Device()
	{
		key.setEnabled(false);
	}
};

struct Camera : Device {
	Camera(): Device("Camera", DeviceInfo::Status::Active)
	{
	}
};

struct SdCard : Device {
	SdCard(): Device("SD card", DeviceInfo::Status::Disabled)
	{
	}
};

void countActiveDevices()
{
	auto nEnabled = 0;

	for (auto &callable : DeviceModule::getIterators()) {
		nEnabled += callable().status == DeviceInfo::Status::Active;
	}

	std::cout << "Active devices: " << nEnabled << std::endl;
}

void deviceTask(Device &aDevice)
{
	bool fEnabled = false;

	for (auto i = 30; i < 42; ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(i));
		aDevice.setEnabled(fEnabled);
		fEnabled = !fEnabled;
	}
}

void counterTask()
{
	for (auto i = 20; i < 30; ++i) {
		countActiveDevices();
		std::this_thread::sleep_for(std::chrono::milliseconds(i));
	}
}

int main(void)
{
	Camera cam;
	SdCard sd;

	std::thread thrCam{deviceTask, std::ref(cam)};
	std::thread thrSd{deviceTask, std::ref(sd)};
	std::thread thrCnt{counterTask};
	std::thread thrCnt2{counterTask};

	thrCam.join();
	thrSd.join();
	thrCnt.join();
	thrCnt2.join();
}
