#pragma once
#include "../../Memory/Memory.h"
#include "RakNet/RaknetConnector.h"

class RemoteConnectorComposite {
public:
	BUILD_ACCESS(RaknetConnector*, rakNet, 0x70);
};

class NetworkSystem {
public:
	BUILD_ACCESS(RemoteConnectorComposite*, remoteConnector, 0x90);
};
