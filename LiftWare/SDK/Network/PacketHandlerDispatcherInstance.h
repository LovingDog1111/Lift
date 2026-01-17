#pragma once

class PacketHandlerDispatcherInstance {
   public:  
   BUILD_ACCESS(uintptr_t**, vtable, 0x0);
    virtual ~PacketHandlerDispatcherInstance();
    virtual void handle(void* networkIdentifier, void* netEventCallback,
                        std::shared_ptr<class Packet>) = 0;
   public:
    uintptr_t getPacketHandler() {
        return Memory::GetVTableFunction(this, 1);
    }
};