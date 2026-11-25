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
	// she was in a HIGGS hand less than a_seconds ago: HIGGS/PLANCK are still putting her bodies back (taking her then
	// blew Delirium's body up: limbs at 500 m/s, the head 40 m up)
	[[nodiscard]] bool HiggsHeldRecently(const RE::Actor* a_actor, float a_seconds);
	void LogHiggs();  // what each HIGGS hand holds (debug command "higgs")
}
