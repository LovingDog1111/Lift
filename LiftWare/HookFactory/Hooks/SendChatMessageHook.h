#pragma once
#include "../FuncHook/FuncHook.h"
#include "../../MainClient/Lift.h"
#include "../../MainClient/Commands/CommandFactory.h"

class SendChatMessageHook : public FuncHook {
private:
	using func_t = bool(__thiscall*)(__int64, const std::string&);
	static inline func_t oFunc;

	static bool SendChatMessageCallback(__int64 _this, const std::string& message) {
		if (message.size() > 0) {
			char firstChar = message[0];
			if (firstChar == CommandFactory::prefix) {
				CommandFactory::execute(message);
				return false;
			}
		}
		return oFunc(_this, message);
	}
public:
	SendChatMessageHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&SendChatMessageCallback;
	}
};