-- Several Bloodmage proc talents carry trigger spells in Spell.dbc but
-- ProcFlags 0, so the engine never fires them (verified against Spell.dbc).
-- Lingering Blood (704639 -> 800988) and Hunger (704168 -> 504189) proc on
-- direct damage dealt; Insatiable Appetite (705739 -> 520494) on direct
-- damaging crits. Cooldown is milliseconds: Hunger's authored 0.5s ICD is 500.
-- Cursed Blood, Petrified Legions and Fang Over Fang already ride upstream
-- proc rows and are deliberately not repeated here. Any earlier local-only
-- rows for Cursed Blood (573250) and Petrified Legions (500012) are retired
-- so only the upstream definitions survive.
DELETE FROM `spell_proc` WHERE `SpellId` IN (573250, 500012);
DELETE FROM `spell_proc` WHERE `SpellId` IN (704639, 704168, 705739);
INSERT INTO `spell_proc` (`SpellId`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `ProcFlags`, `SpellTypeMask`, `SpellPhaseMask`, `HitMask`, `AttributesMask`, `DisableEffectsMask`, `ProcsPerMinute`, `Chance`, `Cooldown`, `Charges`) VALUES
(704639, 0, 0, 0, 0, 0, 65556, 1, 2, 0, 0, 0, 0, 0, 0, 0),
(704168, 0, 0, 0, 0, 0, 65556, 1, 2, 0, 0, 0, 0, 0, 500, 0),
(705739, 0, 0, 0, 0, 0, 65556, 1, 2, 2, 0, 0, 0, 0, 0, 0);
