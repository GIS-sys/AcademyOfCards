To add new Fighting card:

- add ID to ABActorFightingDeck (BActorFightingDeck.h)

- add condition parsing in FightingAbilityCondition::Check if new condition (FightingAbility.cpp)

- add ability type parsing in FightingAbility::Build if new ability type (FightingAbility.cpp) and add new class, derived from FightingAbility

- add when parsing in FightingAbility::Build if new When type (FightingAbility.cpp)

- add target type FightingAbilityTarget::With if new target (FightingAbility.cpp)

- implement new Ability class