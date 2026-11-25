#include "shared.h"

#include "vrmods.h"

namespace delirium::vrmods
{
	namespace
	{
		// IHiggsInterface001, up to the one call used (virtual order must match HIGGS's header exactly)
		struct IHiggs001
		{
			virtual unsigned int         GetBuildNumber() = 0;
			virtual void                 AddPulledCallback(void*) = 0;
			virtual void                 AddGrabbedCallback(void*) = 0;
			virtual void                 AddDroppedCallback(void*) = 0;
			virtual void                 AddStashedCallback(void*) = 0;
			virtual void                 AddConsumedCallback(void*) = 0;
			virtual void                 AddCollisionCallback(void*) = 0;
			virtual void                 GrabObject(RE::TESObjectREFR*, bool) = 0;
			virtual RE::TESObjectREFR*   GetGrabbedObject(bool a_isLeft) = 0;
		};
		// IPlanckInterface001, up to GetBuildNumber (only used to log that PLANCK is there)
		struct IPlanck001
		{
			virtual unsigned int GetBuildNumber() = 0;
		};

		std::mutex                                                              g_heldLock;
		std::unordered_map<const RE::Actor*, std::chrono::steady_clock::time_point> g_heldAt;  // last seen in a hand
		std::atomic<IHiggs001*>  g_higgs{ nullptr };
		std::atomic<IPlanck001*> g_planck{ nullptr };
	}

	void Set(void* a_higgs, void* a_planck)
	{
		auto* h = static_cast<IHiggs001*>(a_higgs);
		auto* p = static_cast<IPlanck001*>(a_planck);
		g_higgs = h;
		g_planck = p;
		logger::info("VR mods: HIGGS {}, PLANCK {}", h ? std::format("build {}", h->GetBuildNumber()) : "not found",
			p ? std::format("build {}", p->GetBuildNumber()) : "not found");
	}

	bool Higgs() { return g_higgs.load(std::memory_order_relaxed) != nullptr; }
	bool Planck() { return g_planck.load(std::memory_order_relaxed) != nullptr; }

	void LogHiggs()
	{
		auto* h = g_higgs.load(std::memory_order_relaxed);
		if (!h) {
			logger::info("higgs: no HIGGS API");
			return;
		}
		for (const bool left : { true, false }) {
			auto* r = h->GetGrabbedObject(left);
			logger::info("higgs: {} hand holds {}", left ? "left" : "right",
				r ? std::format("{:08X} '{}' (form type {})", r->GetFormID(), r->GetName(), static_cast<int>(r->GetFormType())) : std::string("nothing"));
		}
	}

	bool HiggsHolds(const RE::Actor* a_actor)
	{
		auto* h = g_higgs.load(std::memory_order_relaxed);
		if (!h || !a_actor) {
			return false;
		}
		const bool held = h->GetGrabbedObject(true) == a_actor || h->GetGrabbedObject(false) == a_actor;
		if (held) {
			const auto       now = std::chrono::steady_clock::now();
			std::scoped_lock l(g_heldLock);
			g_heldAt[a_actor] = now;
			if (g_heldAt.size() > 64) {
				std::erase_if(g_heldAt, [&](const auto& p) { return now - p.second > std::chrono::seconds(10); });
			}
		}
		return held;
	}

	bool HiggsHeldRecently(const RE::Actor* a_actor, float a_seconds)
	{
		std::scoped_lock l(g_heldLock);
		const auto       it = g_heldAt.find(a_actor);
		return it != g_heldAt.end() && std::chrono::steady_clock::now() - it->second < std::chrono::duration<float>(a_seconds);
	}
}
