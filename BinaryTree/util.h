#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace GameEngine2D
{
	struct AtlasInfo
	{
		AtlasInfo() = default;
		~AtlasInfo() = default;
		AtlasInfo(const AtlasInfo& other) = default;
		AtlasInfo& operator=(const AtlasInfo& other) = default;
		AtlasInfo(AtlasInfo&& other) noexcept
			: name(std::move(other.name)), files(std::move(other.files))
		{
		}
		AtlasInfo& operator=(AtlasInfo&& other) noexcept
		{
			name = std::move(other.name);
			files = std::move(other.files);
			return *this;
		}

		std::string name;
		std::vector<std::string> files;
	};

	struct otherSrcInfo
	{
		otherSrcInfo() = default;
		~otherSrcInfo() = default;
		otherSrcInfo(const otherSrcInfo& other) = default;
		otherSrcInfo& operator=(const otherSrcInfo& other) = default;
		otherSrcInfo(otherSrcInfo&& other) noexcept
			: name(std::move(other.name)), files(std::move(other.files))
		{
		}
		otherSrcInfo& operator=(otherSrcInfo&& other) noexcept
		{
			name = std::move(other.name);
			files = std::move(other.files);
			return *this;
		}

		std::string name;
		std::string files;
	};

	typedef struct vecsInfo
	{
		vecsInfo() = default;
		~vecsInfo() = default;
		//vecsInfo(const vecsInfo& other) = default;
		//vecsInfo& operator=(const vecsInfo& other) = default;
		vecsInfo(vecsInfo&& other) noexcept
			: otherInfos(std::move(other.otherInfos)), atlas(std::move(other.atlas))
		{
		}
		vecsInfo& operator=(vecsInfo&& other) noexcept
		{
			otherInfos = std::move(other.otherInfos);
			atlas = std::move(other.atlas);
			return *this;
		}
		std::unordered_map<std::string, std::vector<otherSrcInfo>> otherInfos;
		std::vector<AtlasInfo> atlas;
	}vecsInfo;

	typedef struct 
	{
		int hp = 0;
		int phy_atk = 0;		//物理攻击
		int mag_atk = 0;		//魔法攻击
		int phy_def = 0;		//物理防御
		int mag_def = 0;		//魔法防御
	}NodeStatus;
}
