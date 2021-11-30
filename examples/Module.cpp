#include <Rr/Module.hpp>
#include <iostream>

using namespace std;

template <typename T>
using Module = Rr::Module::Mod<T, Rr::Module::Stl11SyncTrait>;

struct Camera : Module<Camera> {
	Camera()
	{
	}

	void configure(int mode)
	{
		std::cout << "configured" << endl;
	}

	int getMode() const 
	{
		std::cout << "get mode" << std::endl;
		return 42;
	}
};

void initCamera()
{
	static Camera camera;
	Module<Camera>::setInstance(camera);
}

int main()
{
	initCamera();
	cout << Module<const Camera>().isValid() << endl;
	std::cout << Module<const Camera>().getInstance().getMode() << std::endl;
	std::cout << Module<Camera>().getInstance().getMode() << std::endl;
	// Module<const Camera>{}.getInstance().configure(42);  // Error: const reference
	Module<Camera>{}.getInstance().configure(42);
	return 0;
}
