-- Warrior - Damage Shield proc
/* spell chain loaded from DBC
DELETE FROM `spell_chain` WHERE `spell_id` IN (58872, 58874);
INSERT INTO `spell_chain` VALUES
(58872, 0, 58872, 1, 0),
(58874, 58872, 58872, 2, 0);
*/

DELETE FROM `spell_proc_event` WHERE `entry`=58872;
INSERT INTO `spell_proc_event` VALUES
(58872,0x01,0,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00100000,0x0000043,0.000000,0.000000,0);
