#pragma once
#include <cstdint>
#include <concepts>
#include <libhat.hpp>
#include "../../../Memory/Memory.h"
#include <vector>
#include <map>
#include <unordered_map>
#include "../../../Values/Vtables.h"
#include "../../../Values/Offsets.h"

class Actor;
class mcUUID {
public:
	uint64_t mostSig, leastSig;
};
class PlayerListEntry {
public:
	uint64_t id;
	mcUUID UUID;
	std::string name, XUID, platformOnlineId;
	int buildPlatform;
	char filler[0x164];

	bool isTeacher, isHost;
};

class Level {
public:
	static std::vector<std::pair<mcUUID, PlayerListEntry>>
		copyMapInAlphabeticalOrder(const std::unordered_map<mcUUID, PlayerListEntry>& sourceMap) {
		std::vector<std::pair<mcUUID, PlayerListEntry>> sortedPairs(sourceMap.begin(), sourceMap.end());


		std::sort(sortedPairs.begin(), sortedPairs.end(), [](const auto& a, const auto& b) {
			return a.second.name < b.second.name;
			});

		return sortedPairs;
	}
	std::unordered_map<mcUUID, PlayerListEntry>& getPlayerMap() {
		return hat::member_at<std::unordered_map<mcUUID, PlayerListEntry>>(this, Offsets::PlayerMap);
	}
	std::vector<Actor*> getRuntimeActorList() {
		std::vector<Actor*> listOut;
		Memory::CallVFunc<VTables::GetRuntimeActorList, decltype(&listOut)>(this, &listOut);
		return listOut;
	}
};
