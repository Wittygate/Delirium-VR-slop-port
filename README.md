# Delirium VR slop port

Unofficial Skyrim VR port of **[Skyrim Delirium](https://www.nexusmods.com/skyrimspecialedition/mods/196333)** 2.1.1 by
YoucefNabil/Mentally0Unstable ([source](https://github.com/YoucefNabil/skyrim-delirium)): physics-driven, Euphoria-style hit reactions. NPCs
stumble and try to keep their footing, writhe in fire, get blown over by explosions and shouts, and die on their feet
instead of playing canned animations.

## Before you use this, a few things to note

**WARNING: this is a slop port. Made with AI.** I'm not a real modder and I don't claim to be. This was created over a
few days of testing and a lot of telling the AI "fix that". I can't help you debug it. Use at your own risk.

The main loss compared to the flatscreen version is **body-on-body collision**: if you grab NPC 1 with PLANCK and throw
him at NPC 2, NPC 2 won't be knocked over. It crashed the game, I don't know why. If I get back to it, it's first on my
fixing list.

I tested it on a fresh save in the realm of Lorkhan, on a heavy modlist (modified Tahrovin). Not much in-game.

**Please don't report bugs of this port to YoucefNabil/Mentally0Unstable.** He made the flatscreen mod and has
nothing to do with this port.

## Requirements

- Skyrim VR 1.4.15 (it does not work on SE/AE: use the original there)
- SKSEVR
- Address Library for SKSE Plugins, VR version
- A CPU with AVX2 (Intel from 2013 / Haswell, AMD from 2017 / Ryzen)
- A standard humanoid skeleton (XP32 Maximum Skeleton recommended, as for the original)

## Works with

- **HIGGS** (tested 1.10.10): grab an NPC and Delirium lets go, so the body follows your hand (limp while held, that's
  intended). Drop or throw it and Delirium takes over again.
- **PLANCK** (tested 0.8.2): PLANCK melee hits reach Delirium, PLANCK's hand shove plays as a Delirium stagger.
- Community Shaders.

## What was tested

Melee, fire/frost/shock spells, explosions, crossbow, Fus Ro Dah, staggers and pushes, PLANCK shoves, kills and dying
on the feet, HIGGS grab/drop/throw. Frame rate felt fine, but I'm playing on a relatively beefy setup.

Not tested: creatures (wolves, draugr, trolls...), long falls, big fights, followers, mounted NPCs.

My specs: CPU AMD Ryzen 9 9950X3D, RAM 32 GB, GPU Radeon RX 9070 XT (16 GB).

## Install

Download the zip from [Releases](../../releases) and install it with MO2 or Vortex. No .esp. Don't install the
original (SE/AE) Skyrim Delirium next to it: it's not needed and it won't work.

## Known issues

- No body-on-body collision (see above). `[VRDebug] iCoreMeetWorld=1` in the ini turns it back on, and probably
  crashes your game.
- No in-game settings menu (SKSE Menu Framework doesn't exist for VR): edit `SKSE\Plugins\SkyrimDelirium.ini`.
- Enemies aim at where a ragdolled NPC was standing until it gets back up.
- The mod turns killmoves off through a global the save keeps. In VR that only concerns the killmoves NPCs do on each
  other. After uninstalling, `set killmove to 1` in the console brings them back.

## Credits

- **YoucefNabil/Mentally0Unstable**: Skyrim Delirium, the whole mod. Big thank you to him.
- **PVCPC**: the Delirium physics engine (`delirium_physics.dll`, unchanged from 2.1.1).
- **adamhynek**: HIGGS and PLANCK, whose open source made the VR addresses and compatibility possible.
- **alandtse** and the CommonLibSSE-NG contributors, the SKSEVR team, meh321 (Address Library).

Third-party code and licences: [CREDITS.md](CREDITS.md). VR port notes: [PORT-NOTES.md](PORT-NOTES.md).

## Licence

GPL-3.0-only, just like the original ([LICENSE](LICENSE)). This repository is the full source of the port.

*Wittygate*
