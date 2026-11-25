// Skyrim VR's physics mods: HIGGS (grabbing) and PLANCK (physical melee, active ragdolls). The host fetches their mod
// APIs (SKSE messages) at kPostPostLoad and hands the pointers to the core; both are null outside VR or without the mod.
// Only the calls Delirium needs are declared; the slot order follows each mod's public interface header
// (adamhynek/higgs include/higgsinterface001.h, adamhynek/activeragdoll include/planckinterface001.h).
#pragma once

namespace delirium::vrmods
{
	void Set(void* a_higgs, void* a_planck);  // any thread, once per core load
	[[nodiscard]] bool Higgs();
	[[nodiscard]] bool Planck();

	// either HIGGS hand holds this actor (a grabbed body part counts): Delirium keeps its hands off her
	[[nodiscard]] bool HiggsHolds(const RE::Actor* a_actor);
}
