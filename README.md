# Project Hercules

Project Hercules is an Unreal Engine gameplay prototype and game engineering portfolio project, developed in preparation for 2027 industrial placement applications. It combines C++ gameplay systems with Blueprints and UMG interfaces.

## Current status

An actively developed prototype with inventory, equipment, character progression and menu systems. Combat integration, gameplay effects for stats and the complete level-up interface are still in development.

## Features in the prototype

- Third-person movement, jumping and camera controls using Enhanced Input.
- Item pickups with overlap-based interaction range checks and an interaction prompt widget.
- Inventory and item widgets, with two quick slots that support assignment, swapping and clearing.
- Equipment state with a Blueprint event for updating weapon visuals when the equipped item changes.
- Character level, upgrade currency and ten upgradeable stats, with a cost that increases as the character levels up.
- Game-menu and level-up widget foundations, plus a character preview using a render target and a UI material.

## Controls

| Input | Action |
| --- | --- |
| WASD | Move |
| Mouse | Look |
| Space | Jump |
| E | Pick up a nearby item |
| I | Open or close the inventory |
| Tab | Open the game menu |
| 1 / 2 | Equip the item assigned to quick slot 1 / 2 |

## Implementation highlights

- [Character gameplay code](Source/PrjctHerculesPrttype/PrjctHerculesPrttypeCharacter.cpp) handles inventory validation, quick slots, equipment changes and stat upgrades.
- [Item pickup code](Source/PrjctHerculesPrttype/ItemPickup.cpp) checks that the interacting player is in range and removes a pickup after it is added to the inventory.
- Gameplay functions and data are exposed to Blueprints through `UFUNCTION` and `UPROPERTY`; UMG widgets provide the interface.
- `UpgradeStat` spends currency and raises both the selected stat and character level. The upgrade cost is `100 + 20L + 2L²`, where `L = CharacterLevel - 1`.
- The ten stats are Strength, Dexterity, Agility, Thickness, Vigor, Endurance, Pace, Magical Capability, Intelligence and Faith.

## Technologies

- Unreal Engine 5.8
- C++ and Blueprints
- Enhanced Input
- UMG and Slate
- Git and GitHub

## Run locally

The current development target is Windows / Win64. Install Unreal Engine 5.8 and Visual Studio with an Unreal-compatible C++ toolchain.

1. Clone the repository:

   ```sh
   git clone https://github.com/ygzmert0707/Project-Hercules.git
   ```

2. Generate Visual Studio project files from `PrjctHerculesPrttype.uproject`.
3. Open the generated solution and build `PrjctHerculesPrttypeEditor` with the **Development Editor** configuration and **Win64** platform.
4. Open `PrjctHerculesPrttype.uproject` in Unreal Editor.
5. Open `/Game/ThirdPerson/Lvl_ThirdPerson` if it is not already loaded, then select **Play**.

Generated .sln files, build outputs and local caches are excluded from version control. This is a development prototype; a packaged release is not currently provided.

## Repository layout

```text
Source/PrjctHerculesPrttype/   C++ gameplay module and template variants
Content/ThirdPerson/          Main map, character Blueprints and UI assets
Content/Input/                Enhanced Input actions and mapping contexts
Config/                       Unreal project configuration
Plugins/VisualStudioTools/    Visual Studio editor integration
PrjctHerculesPrttype.uproject  Unreal project entry point
```

## Next steps

- Add health, damage and combat behavior to the main player character.
- Connect the level-up interface, add gameplay currency rewards and define how stats affect gameplay.
- Refine weapon visuals, individual weapon transforms and character-preview performance.
- Add a telemetry backend using Java, Spring Boot and MySQL to record selected gameplay events.

The telemetry backend is planned and has not been implemented in this repository. Features and scope will continue to evolve as the prototype develops.
