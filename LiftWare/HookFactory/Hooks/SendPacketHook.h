#pragma once
#include "../FuncHook/FuncHook.h"
int lastSlot = 0;

class SendPacketHook : public FuncHook {
private:
	using func_t = void(__thiscall*)(PacketSender*, Packet*);
	static inline func_t oFunc;

	static void SendPacketCallback(PacketSender* _this, Packet* packet) {
		FeatureFactory::onSendPacket(packet);

		oFunc(_this, packet);
	}
public:
	SendPacketHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&SendPacketCallback;
	}
};