# Skyrim VR port notes

Based on upstream commit 95304ca (= Nexus 2.1.1). Built against alandtse CommonLibSSE-NG v11.0.0 as a **VR-only**
DLL, so CommonLib compiles VR's class layouts and vtable order.

Build: `plugin/build-vr.cmd vr` (VS 2022 Build Tools, vcpkg). `DELIRIUM_RUNTIMES` in CMake = `flat` | `vr` | `all`.
Output: `plugin/build/vr-Release`. `delirium_physics.dll` and the data files are unchanged from the 2.1.1 release.

## VR 1.4.15 addresses (RVAs)

| SE id / what | VR | how found |
|---|---|---|
| 40436 animation job, +0x74 -> UpdateAnimationInternal | 0x7087B0 (+0x74 -> 0x2B5840) | Character vtable 0x7D -> callers |
| 62933 bone copy | 0xB3C260 | post-physics caller's +0x287 call |
| 62627 graph update +0x7BF | no inlined copy on VR: sites 0xB26D00+0xAD, 0xB2A9E0+0x110, 0xB27670+0x2F | callers of the bone copy |
| 62622 post-physics +0x287 | 0xB26750+0x287 | same function as PLANCK's postPhysics site |
| 62621 +0x25B -> hkbRagdollDriver::driveToPose | 0xB26450+0x25B (-> 0xA25B60) | PLANCK driveToPoseHookLoc |
| 35565 frame sync | 0x5BAB10 +0x611 | FSMP (hdtSMP64) VR offset |
| 38859 / 36681 get-up check | 0x686D80+0x3E4 -> 0x600DF0 | PLANCK Actor_IsRagdollMovingSlowEnoughToGetUp |
| 36415 Actor::GetBumped | 0x5E4B70 | PLANCK |
| 37633 hit, 36700 stagger | VR Address Library CSV | |
| Character KillImpl / UpdateActor3DPosition | slots 0x110 / 0x121 (SE 0x10E / 0x11F) | disassembly, PLANCK |
| Projectile AddImpact | slot 0xBE (SE 0xBD) | CommonLibVR |

All checked against a decrypted copy of the 1.4.15 exe with capstone, and every patched vtable checked by RTTI.

## VR-specific changes

- Hooks chain after other mods' hooks at the same call (PLANCK patches driveToPose and the get-up check first).
- HIGGS (`GetGrabbedObject`): Delirium never drives an NPC held in a HIGGS hand, and waits 0.6 s after a drop.
- HIGGS saves a body's inertia on grab and writes it back on drop. If Delirium had scaled it, HIGGS wrote the scaled
  value back and every grab compounded it until the physics exploded. Delirium now keeps each body's original
  inertia and mass from first sight and restores them after a HIGGS drop.
- Safety net: if Delirium's simulated body blows up (head >15 m from origin, or bodies steered >150 m/s), it lets go
  and Skyrim's ragdoll takes over.
- PLANCK's hand shove starts a stagger straight on the animation graph (`staggerStart`), not through the stagger
  function: VR hooks Character's `NotifyAnimationGraph` (holder vtable slot 1) and plays it as a Delirium stagger.
- No Delirium death on essential / protected NPCs (Skyrim doesn't let them die; e.g. training dummies).
- Collision-filter restore only clears Delirium's own bits (keeps HIGGS/PLANCK changes).
- Present hook skipped on VR (the swap chain is the desktop mirror; it crashed with Community Shaders' Streamline proxy).
- `NiSkinInstance::lock` doesn't exist on VR: poses are published without it (possible one-frame tear, none seen).
- The flat floor is kept when the collision patch is re-gathered with 0 triangles (bodies fell through; also upstream).

## Open issues

- `Meet()` world part (`[VRDebug] iCoreMeetWorld`, standing NPCs' ragdoll bodies joining Delirium's world) crashes
  Skyrim VR ~1 s after save load with PLANCK active. Off by default on VR; cause not found.
- AI aims at the actor's logical position while Delirium drives the body (upstream design).
- No SKSE Menu Framework on VR: settings are ini only.

## Debugging

`[VRDebug]` ini section: each hook can be switched off with key=0 (host: iLoadCore, iAnimationHook, iPoseHook,
iRagdollHook, iGetUpHook, iHitHook, iStaggerHook, iBumpHook, iKillHook, iFrameSyncHook, iGraphStaggerHook; core:
iCoreAfterAnimation, iCoreProjectiles, iCoreNoKillMoves, iCoreMeet, iCoreMeetOthers, iCoreMeetStrikes, iCoreMeetWorld,
iCoreTakeOver, iCoreFall).

Live commands: write lines to `Documents\My Games\Skyrim VR\SKSE\SkyrimDelirium.cmd` (`near`, `push <id> [N s]`,
`fus <id> [force]`, `higgs`, `status`, `release`, `reload`). Logs: `SkyrimDelirium.log`, `SkyrimDeliriumCore.log`.
