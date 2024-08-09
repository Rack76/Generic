#ifndef ACTIONS_H
#define ACTIONS_H

#include <any>

namespace Action
{
	void moveCameraLeft(std::any data, int entityId);
	void stopCameraLeft(std::any data, int entityId);

	void moveCameraRight(std::any data, int entityId);
	void stopCameraRight(std::any data, int entityId);

	void moveCameraUp(std::any data, int entityId);
	void stopCameraUp(std::any data, int entityId);

	void moveCameraDown(std::any data, int entityId);
	void stopCameraDown(std::any data, int entityId);

	void moveCameraForward(std::any data, int entityId);
	void stopCameraForward(std::any data, int entityId);

	void moveCameraBackward(std::any data, int entityId);
	void stopCameraBackward(std::any data, int entityId);

	void rotateDebugCamera(std::any data, int entityId);
}

#endif 