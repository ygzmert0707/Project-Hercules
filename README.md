# Project Hercules

**In development** — an Unreal Engine gameplay prototype and game engineering portfolio project for 2027 industrial placement applications. The focus is integrating C++ gameplay rules with Blueprints, animation and UMG interfaces.

## Current status

- Inventory, proximity-based pickups, equipment state and two quick slots with assignment, swapping and clearing.
- Ten-stat progression with upgrade currency, level increases and a rising upgrade cost.
- Player health and clamped damage handling; unarmed attacks, sphere-sweep hit detection and timed combo logic in C++.
- Inventory/menu widgets and a render-target character preview; the level-up screen remains a foundation.

Basic health reduction, punch animation/hits, weapon changes, slot refresh and transparent character preview have been reported working in manual playtests. The latest one-second combo behavior, montage-section configuration and complete menu/level-up flow still need validation. Full regression testing remains pending.

## Contribution scope and attribution

- **Project-specific extensions:** inventory and quick-slot rules, equipment state, stat/currency checks, and C++–Blueprint event connections, integrated into the player and UI.
- **Template and asset foundation:** movement/camera code, combat interfaces, animation notifications and sample variants build on Epic's Unreal templates. The melee sweep adapts the [Combat template implementation](Source/PrjctHerculesPrttype/Variant_Combat/CombatCharacter.cpp#L267), adding PhysicsBody filtering and per-query target deduplication. Models and animations use existing assets.
- **Development approach:** guided learning with AI-assisted examples, adaptations and some direct assistant edits. Contributions include implementation and integration across original and adapted code.

## Implementation highlights

- [Pickups](Source/PrjctHerculesPrttype/ItemPickup.cpp#L88) require the interacting player to be in range and remove the pickup only after the item is accepted.
- [Inventory and slots](Source/PrjctHerculesPrttype/PrjctHerculesPrttypeCharacter.cpp#L224): equipping/assigning rejects unset or unowned item IDs; slots are limited to 1 and 2. Assigning an item from the other slot swaps their contents.
- [Progression](Source/PrjctHerculesPrttype/PrjctHerculesPrttypeCharacter.cpp#L316) rejects nonpositive currency additions, insufficient funds and unknown stat selections. Successful upgrades spend currency and raise the selected stat and level. [Cost](Source/PrjctHerculesPrttype/PrjctHerculesPrttypeCharacter.cpp#L439) is `100 + 20L + 2L²`, where `L = CharacterLevel - 1`.
- [Blueprint events](Source/PrjctHerculesPrttype/PrjctHerculesPrttypeCharacter.h#L186), `OnEquippedItemChanged` and `OnQuickSlotsChanged`, notify presentation code after valid state changes.
- [Health](Source/PrjctHerculesPrttype/PrjctHerculesPrttypeCharacter.cpp#L452): `TakeDamage` rejects nonpositive damage or depleted HP and clamps applied damage to the remaining health.
- [Melee hits](Source/PrjctHerculesPrttype/PrjctHerculesPrttypeCharacter.cpp#L466) sweep a sphere forward from a mesh socket, ignore the attacker and pass damage/impulse to `ICombatDamageable`. A local `TSet` prevents multiple components of one actor causing repeated damage **within one query**; later trace calls can hit that actor again.
- [Attack input](Source/PrjctHerculesPrttype/PrjctHerculesPrttypeCharacter.cpp#L175) and [combo transitions](Source/PrjctHerculesPrttype/PrjctHerculesPrttypeCharacter.cpp#L532) buffer clicks and advance montage sections within a configurable window. Empty hands and an assigned montage are required; final timing and Blueprint setup remain to be tested.

## Controls

| Input | Action |
| --- | --- |
| WASD / mouse / Space | Move / look / jump |
| E | Pick up a nearby item |
| I | Open or close the inventory |
| Tab | Open the game menu |
| 1 / 2 | Equip the item assigned to quick slot 1 / 2 |
| Left mouse button | Unarmed attack with empty hands |

## Technologies

Unreal Engine 5.8, C++, Blueprints, Enhanced Input, UMG/Slate, Git and GitHub. Current development target: Windows / Win64.

## Run locally

Install Unreal Engine 5.8 and Visual Studio with an Unreal-compatible C++ toolchain.

1. Clone the repository:

   ```sh
   git clone https://github.com/ygzmert0707/Project-Hercules.git
   ```

2. Generate Visual Studio project files from `PrjctHerculesPrttype.uproject`.
3. Build `PrjctHerculesPrttypeEditor` using **Development Editor / Win64** in the generated solution.
4. Open `PrjctHerculesPrttype.uproject`, load `/Game/ThirdPerson/Lvl_ThirdPerson` if needed, and select **Play**.

Generated `.sln` files, build outputs and local caches are excluded from version control. A packaged release is not currently provided.

## Next steps

- Validate combo timing, interruptions, section bindings and equipped-hand restrictions.
- Implement charged attacks: `CheckChargedAttack` is currently empty.
- Complete incoming combat-interface integration, health UI and death handling.
- Finish the level-up UI, currency rewards and gameplay effects for stats.
- Refine weapon-specific transforms, menu navigation and character-preview performance.
- Explore a Java/Spring Boot/MySQL telemetry backend for selected gameplay events; this backend is planned and is not implemented in the repository.
