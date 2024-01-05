#pragma once
template<typename type>
class Var
{
public:
	type m_Var;
	const char* m_sDisplayName;
};

namespace player
{
	inline Var<bool> m_bGodMode{ false, "God Mode" };
	inline Var<bool> m_bNoWanted{ false, "No Wanted" };
}

namespace recovery
{
	inline Var<bool> m_bGiveOnlineMoney{ false, "Give Money" };
	inline Var<bool> m_bGiveOnlineRank{ false, "Give Rank" };

	inline Var<bool> m_bGiveStoryModeMoney{ false, "Story Mode Money" };
}
namespace horse
{

}

namespace misc
{
	inline Var<bool> m_bEnableFpsCounter{ false, "Fps Window" };
}