#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <SDL.h>
namespace GameEngine2D
{
#define WORKER
#define BUNS

#if 1
#undef  BUNS
#endif

#ifdef BUNS
#define ABSOLUTEPATH "C:\\Users\\Zero4\\source\\repos\\learnSDL\\BinaryTree\\"
#elif defined(WORKER)
#define ABSOLUTEPATH "C:\\Users\\worker\\source\\repos\\LearnSDL\\BinaryTree\\"
#endif
	
	static std::string getAbsolutePath(const std::string& p)
	{
		return std::string(ABSOLUTEPATH) + p;
	}

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

	typedef struct NodeStatus
	{
		NodeStatus():hp(0),phy_atk(0), mag_atk(0), phy_def(0), mag_def(0), mana(0) {}
		NodeStatus(int h, int pa, int ma, int pd, int md, int mn,int vx) :hp(h),phy_atk(pa), mag_atk(ma), 
																   phy_def(pd), mag_def(md), mana(mn),vx_s(vx) {}
		NodeStatus(const NodeStatus& o):hp(o.hp),phy_atk(o.phy_atk),mag_atk(o.mag_atk),phy_def(o.phy_def),
										mag_def(o.mag_def),mana(o.mana),vx_s(o.vx_s){}
		NodeStatus& operator=(const NodeStatus&o) {
			hp = o.hp;
			phy_atk = o.phy_atk;
			phy_def = o.phy_def;
			mag_atk = o.mag_atk;
			mag_def = o.mag_def;
			mana = o.mana;
			vx_s = o.vx_s;
			return *this;
		}
		int hp = 0;
		int phy_atk = 0;		//物理攻击
		int mag_atk = 0;		//魔法攻击
		int phy_def = 0;		//物理防御
		int mag_def = 0;		//魔法防御
		int mana = 0;

		//节点属性
		int vx_s = 0; //每秒移动多少像素
	}NodeStatus;

	struct DamageSkill
	{
		std::string name;
		std::string desc;
		int damage = 0;
	};

	struct PlayerStatus
	{
		PlayerStatus() = default;
		PlayerStatus(const PlayerStatus& o) :exp(o.exp), need_exp(o.need_exp),
			playerName(o.playerName), status(o.status) {
		}
		int level = 0;
		int exp = 0;
		int need_exp = 0;
		std::string playerName;
		NodeStatus status;
	};

	void createGrayTexture(const SDL_Surface* source_surface, SDL_Surface** dst_surface);
	///线性插值
	float linear_interpolate_float(float start, float end, float t);
	static bool isZero(float x) {
		// 设定一个合理的阈值（例如 1e-6）
		const float epsilon = 1e-6f;
		return fabsf(x) < epsilon;
	}
}
