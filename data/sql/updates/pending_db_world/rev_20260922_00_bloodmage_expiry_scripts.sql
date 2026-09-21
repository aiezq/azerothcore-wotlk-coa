-- Bloodmage Vampiric Hunger (802316) and Aortic Aegis (704637) trigger when
-- Blood Veil (504263/572279) and Darkfallen Lament (680828) expire. Atherann's
-- Anguish (680680) and Infuse (681403) detonate their pooled damage when their
-- marks lapse, and lapsed Blood thirst (706613) takes Insatiable with it.
-- The listeners live in mod-ascension-compat.
DELETE FROM `spell_script_names` WHERE `spell_id` IN (504263, 572279, 680828)
    AND `ScriptName` = 'bloodmage_veil_lament_expiry';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(504263, 'bloodmage_veil_lament_expiry'),
(572279, 'bloodmage_veil_lament_expiry'),
(680828, 'bloodmage_veil_lament_expiry');
DELETE FROM `spell_script_names` WHERE `spell_id` IN (680680, 681403)
    AND `ScriptName` = 'bloodmage_plague_burst';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(680680, 'bloodmage_plague_burst'),
(681403, 'bloodmage_plague_burst');
DELETE FROM `spell_script_names` WHERE `spell_id` = 706613
    AND `ScriptName` = 'bloodmage_thirst_expiry';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(706613, 'bloodmage_thirst_expiry');
