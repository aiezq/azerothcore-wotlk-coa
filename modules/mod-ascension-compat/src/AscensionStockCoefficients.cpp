/* Copyright (C) 2016+ AzerothCore, GNU AGPL v3. */

#include "AscensionStockCoefficientData.h"
#include "ScriptMgr.h"
#include "SpellInfo.h"

#include <algorithm>

namespace
{
// Spell.dbc f229-231 (EffectBonusMultiplier) is the stock 3.3.5a spell-power coefficient column. CoA
// never wired it up: it authors coefficients in tooltip formula text instead, and the shipped column
// agrees with that text on 6.2% of the CoA effect slots carrying both. AzerothCore still reads it as
// the default coefficient whenever spell_bonus_data holds no row (Unit::SpellDamageBonusDone and
// Unit::SpellHealingBonusDone), so a 2008 value scales CoA content that never asked for it. Of the
// live CoA damaging and healing spells carrying one, none agreed with its own tooltip within 5%:
// some run several times high, some low, and many scale off a stat their tooltip never mentions.
//
// Clearing the field makes spell_bonus_data the only coefficient channel for CoA content. Where a row
// exists the damage and healing paths already overwrite the field's value with the row's, so this is a
// no-op there and carries no dependency on the load order between the two.
//
// Scope is the generated list, not an id range: the CoA bands also hold copies of stock records whose
// coefficients are genuine. Stock spells, Reborn clone bands and offset copies are excluded by the
// generator, so this pass is a membership test, never a heuristic.
class ascension_stock_coefficients : public GlobalScript
{
public:
    ascension_stock_coefficients() : GlobalScript("ascension_stock_coefficients",
        {GLOBALHOOK_ON_LOAD_SPELL_CUSTOM_ATTR}) { }

    void OnLoadSpellCustomAttr(SpellInfo* info) override
    {
        if (!info)
            return;

        if (std::binary_search(AscensionCompatData::StockCoefficientSpells.begin(),
                AscensionCompatData::StockCoefficientSpells.end(), info->Id))
        {
            for (SpellEffectInfo& effect : info->Effects)
                effect.BonusMultiplier = 0.0f;
        }

        // --- CoA Custom Class Balancing Overrides ---
        if (info->Id >= 500000)
        {
            // 1. Barbarian Crush Rank 1 (Level 17) & Ranks 2-10: normalize early spike weapon damage
            if (info->Id == 500915 && !info->Effects.empty())
            {
                info->Effects[0].BasePoints = 114; // 115% normalized weapon damage (was 274 = 275%)
            }
            else if (info->Id >= 501003 && info->Id <= 501011 && !info->Effects.empty())
            {
                static const int32 crushBps[] = { 124, 134, 144, 154, 164, 174, 184, 194, 204 };
                info->Effects[0].BasePoints = crushBps[info->Id - 501003];
            }

            // Barbarian Decapitate (804414): 220% -> 160% weapon damage, add 6s cooldown
            if (info->Id == 804414 && info->Effects.size() > 1)
            {
                info->Effects[1].BasePoints = 159; // 160% weapon damage (was 219 = 220%)
                info->RecoveryTime = 6000;
            }

            // 2. Primalist Skewer (Level 4): normalize early spike weapon damage
            if (info->Id == 800093 && !info->Effects.empty())
            {
                info->Effects[0].BasePoints = 109; // 110% weapon damage (was 199 = 200%)
            }

            // 3. Starcaller Arrow of the Goddess (563725): 400% -> 180% weapon damage
            if (info->Id == 563725 && info->Effects.size() > 2)
            {
                info->Effects[2].BasePoints = 179; // 180% weapon damage (was 399 = 400%)
            }

            // Starcaller Coalesced Resilience (680786): 1.5% max mana flat absorb -> 0.3%
            if (info->Id == 680786 && !info->Effects.empty())
            {
                info->Effects[0].BasePoints = 299; // 0.3% (was 1499 = 1.5%), prevents zero-damage immunity to DoTs
            }

            // 4. Ranger Brutal Shot (570014): 450% -> 210% weapon damage
            if (info->Id == 570014 && info->Effects.size() > 1)
            {
                info->Effects[1].BasePoints = 209; // 210% weapon damage (was 449 = 450%)
            }

            // Ranger Deadshot (520227): 250% -> 140% weapon damage, add 6s cooldown
            if (info->Id == 520227 && info->Effects.size() > 1)
            {
                info->Effects[1].BasePoints = 139; // 140% weapon damage (was 249 = 250%)
                info->RecoveryTime = 6000;
            }

            // Ranger Flank (804940): 150% -> 110% weapon damage on 0s CD spam
            if (info->Id == 804940 && info->Effects.size() > 1)
            {
                info->Effects[1].BasePoints = 109; // 110% weapon damage (was 149 = 150%)
            }

            // 5. Witch Hunter Heartseeking Bolt (805754): 200% -> 150% weapon damage
            if (info->Id == 805754 && info->Effects.size() > 1)
            {
                info->Effects[1].BasePoints = 149; // 150% weapon damage (was 199 = 200%)
            }

            // Witch Hunter Gaze of the Black Knight (802138): 3255 base absorb -> 750
            if (info->Id == 802138 && info->Effects.size() > 2)
            {
                info->Effects[2].BasePoints = 749; // 750 absorb (was 3254 = 3255)
            }

            // 6. Templar Argent Blade (804929): 215% -> 135% weapon damage
            if (info->Id == 804929 && info->Effects.size() > 1)
            {
                info->Effects[1].BasePoints = 134; // 135% weapon damage (was 214 = 215%)
            }

            // Templar Sacred Swing (705293): 140% -> 105% weapon damage on 0s CD AoE
            if (info->Id == 705293 && info->Effects.size() > 1)
            {
                info->Effects[1].BasePoints = 104; // 105% weapon damage (was 139 = 140%)
            }

            // 7. Runemaster Fracture (803018): 921 base frost dmg -> 380, add 6s cooldown
            if (info->Id == 803018 && !info->Effects.empty())
            {
                info->Effects[0].BasePoints = 379; // 380 base dmg (was 920 = 921)
                info->RecoveryTime = 6000;         // 6.0s CD (was 0s instant spam)
            }

            // 8. Guardian Final Verdict (500258): 1195 base dmg -> 450
            if (info->Id == 500258 && !info->Effects.empty())
            {
                info->Effects[0].BasePoints = 449; // 450 base dmg (was 1194 = 1195)
            }

            // 9. Stormbringer Ride the Lightning (800099): 540 base dmg -> 320, add 6s cooldown
            if (info->Id == 800099 && info->Effects.size() > 2)
            {
                info->Effects[2].BasePoints = 319; // 320 base dmg (was 539 = 540)
                info->RecoveryTime = 6000;         // 6.0s CD (was 0s instant spam)
            }

            // 10. Cultist Ancient Curse (500712): 976 base dmg -> 420
            if (info->Id == 500712 && !info->Effects.empty())
            {
                info->Effects[0].BasePoints = 419; // 420 base dmg (was 975 = 976)
            }

            // 11. Knight of Xoroth Call: Hellfire Abyssal (805074): 1500 impact dmg -> 450
            if (info->Id == 805074 && info->Effects.size() > 1)
            {
                info->Effects[1].BasePoints = 449; // 450 base impact dmg (was 1499 = 1500)
            }

            // 12. Bloodmage Hemoburst (572855): 1480 base dmg -> 520
            if (info->Id == 572855 && !info->Effects.empty())
            {
                info->Effects[0].BasePoints = 519; // 520 base dmg (was 1479 = 1480)
            }

            // Bloodmage Blood Veil (504263): 511 base absorb -> 180
            if (info->Id == 504263 && info->Effects.size() > 1)
            {
                info->Effects[1].BasePoints = 179; // 180 absorb (was 510 = 511)
            }

            // 13. Pyromancer Volcanic Shell (805477): 625 base absorb -> 280
            if (info->Id == 805477 && !info->Effects.empty())
            {
                info->Effects[0].BasePoints = 279; // 280 absorb (was 624 = 625)
            }

            // 14. Tinker Hyperblast Barrage (500249): 831 base dmg -> 190 (level 10 spike fix)
            if (info->Id == 500249 && !info->Effects.empty())
            {
                info->Effects[0].BasePoints = 189; // 190 base fire dmg (was 830 = 831)
            }

            // 15. Reaper Doomrend (800172): 140% -> 110% weapon damage on 0s CD spam
            if (info->Id == 800172 && info->Effects.size() > 1)
            {
                info->Effects[1].BasePoints = 109; // 110% weapon damage (was 139 = 140%)
            }

            // Reaper Slaughter (500373): 150% -> 115% weapon damage on 0s CD spam
            if (info->Id == 500373 && info->Effects.size() > 1)
            {
                info->Effects[1].BasePoints = 114; // 115% weapon damage (was 149 = 150%)
            }

            // 16. Custom Caster Mana Economy: reduce excessive base mana percentage
            // High costs (15% to 35% base mana) cause custom casters to go OOM after 4-5 casts.
            if (info->PowerType == POWER_MANA && info->ManaCostPercentage > 14)
            {
                if (info->ManaCostPercentage <= 25)
                    info->ManaCostPercentage = 9;  // rotational core nuke / dot / buff
                else if (info->ManaCostPercentage <= 45)
                    info->ManaCostPercentage = 14; // major heal / direct burst / utility
                else if (info->ManaCostPercentage <= 100)
                    info->ManaCostPercentage = 22; // raid cooldown
            }
        }
    }
};
}

void AddAscensionStockCoefficientScripts()
{
    new ascension_stock_coefficients();
}
