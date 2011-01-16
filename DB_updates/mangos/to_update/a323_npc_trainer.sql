-- Corrected fix for alchemy recipe Ethereal oil (62409) by Koksneo
DELETE FROM `npc_trainer` WHERE spell = 62409;
INSERT INTO `npc_trainer` (`entry`,`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel`) VALUES
(26975,62409,6000,171,375,0),
(33588,62409,6000,171,375,0),
(26987,62409,6000,171,375,0),
(26903,62409,6000,171,375,0),
(28703,62409,6000,171,375,0),
(26951,62409,6000,171,375,0); 

####################################################################################

-- Add missing spell Enchanted Thorium into trainers by Grandres
DELETE FROM npc_trainer WHERE spell=70524;
INSERT INTO npc_trainer VALUES
(16663,70524,8889,186,250,0),
(3001,70524,8889,186,250,0),
(1681,70524,8889,186,250,0),
(4598,70524,8889,186,250,0),
(26976,70524,8889,186,250,0),
(1701,70524,8889,186,250,0),
(17488,70524,8889,186,250,0),
(26999,70524,8889,186,250,0),
(33682,70524,8889,186,250,0),
(5513,70524,8889,186,250,0),
(4254,70524,8889,186,250,0),
(26912,70524,8889,186,250,0),
(33640,70524,8889,186,250,0),
(18779,70524,8889,186,250,0),
(28698,70524,8889,186,250,0),
(3555,70524,8889,186,250,0),
(26962,70524,8889,186,250,0),
(18747,70524,8889,186,250,0),
(3175,70524,8889,186,250,0),
(6297,70524,8889,186,250,0),
(3357,70524,8889,186,250,0),
(3137,70524,8889,186,250,0),
(33617,70524,8889,186,250,0),
(16752,70524,8889,186,250,0),
(8128,70524,8889,186,250,0),
(18804,70524,8889,186,250,0),
(5392,70524,8889,186,250,0);

####################################################################################

-- Add Turbo-Charged Flying Machine to trainers (by Bastek)
DELETE FROM npc_trainer WHERE entry IN
(33586, 8736, 25277, 16667, 33611, 11031, 17634, 5518, 11025, 11017, 26991, 5174, 28697, 3494, 26907, 19576) AND spell = 44157;
INSERT INTO npc_trainer VALUES
(33586, 44157, 200000, 202, 375, 70), -- Icecrown (A,H)
(8736, 44157, 200000, 202, 375, 70), -- Tanaris (A, H)
(25277, 44157, 200000, 202, 375, 70), -- Borean Tundra (H)
(16667, 44157, 200000, 202, 375, 70), -- Silvermoon City (H)
(33611, 44157, 200000, 202, 375, 70), -- Shattrath City (A, H)
(11031, 44157, 200000, 202, 375, 70), -- Udercity (H)
(17634, 44157, 200000, 202, 375, 70), -- Zangarmarsh (A)
(5518, 44157, 200000, 202, 375, 70), -- Stormwind City (A)
(11025, 44157, 200000, 202, 375, 70), -- Durotar (H)
(11017, 44157, 200000, 202, 375, 70), -- Orgrimmar (H)
(26991, 44157, 200000, 202, 375, 70), -- Borean Tundra (A)
(5174, 44157, 200000, 202, 375, 70), --  Ironforge (A)
(28697, 44157, 200000, 202, 375, 70), -- Dalaran (A, H)
(3494, 44157, 200000, 202, 375, 70), -- The Barrens (A, H)
(26907, 44157, 200000, 202, 375, 70), -- Howling Fjord (A)
(19576, 44157, 200000, 202, 375, 70); -- Netherstorm (A, H)