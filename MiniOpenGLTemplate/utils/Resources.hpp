#pragma once

#include <string>


class Resources {
	// This class is used to manage resources like shaders, textures, etc.
	// is a singleton class, so it can be accessed from anywhere in the code.

public:
	static Resources& getInstence() {
		if (!instance) {
			instance = new Resources();
		}
		return *instance;
	}

	static void destroyInstance() {
		if (instance) {
			delete instance;
			instance = nullptr;
		}
	}

private:
	Resources() = default; // private constructor to prevent instantiation
	~Resources() = default; // private destructor to prevent deletion
	Resources* instance = nullptr;
	Resources(const Resources&) = delete; // prevent copy construction
	Resources& operator=(const Resources&) = delete; // prevent assignment

}