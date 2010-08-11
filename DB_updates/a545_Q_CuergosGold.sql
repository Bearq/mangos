-- Support for quest Cuergo's Gold (2882)

-- Random movement for those mobs
UPDATE creature_template SET
MovementType=1
WHERE entry IN(7899,7901,7902);

-- Treasure Hunting Pirate
DELETE FROM creature WHERE id = 7899;
INSERT INTO creature VALUES (12041050,7899,1,1,1,0,0,-10234.2,-3816.17,1.18257,3.162,300,5,0,2200,0,0,1),
(12041051,7899,1,1,1,0,0,-10155.3,-3864.39,1.69264,5.66193,300,5,0,2200,0,0,1),
(12041052,7899,1,1,1,0,0,-10155,-3922.26,5.91616,5.23153,300,5,0,2200,0,0,1),
(12041053,7899,1,1,1,0,0,-10253.9,-3942.08,2.09698,3.34264,300,5,0,2300,0,0,1),
(12041054,7899,1,1,1,0,0,-10197.7,-4006.53,4.98456,5.73575,300,5,0,2200,0,0,1),
(12041055,7899,1,1,1,0,0,-10245.1,-4078.53,0.571595,4.07699,300,5,0,2200,0,0,1),
(12041056,7899,1,1,1,0,0,-10290.7,-3977.35,0.569303,0.532487,300,5,0,2300,0,0,1);

-- Treasure Hunting Swashbuckler
DELETE FROM creature WHERE id = 7901;
INSERT INTO creature VALUES 
(12041057,7901,1,1,1,0,0,-10307.5,-3923.14,0.627066,2.38917,300,5,0,2200,0,0,1),
(12041058,7901,1,1,1,0,0,-10283.2,-3867.99,1.94563,1.1341,300,5,0,2200,0,0,1),
(12041059,7901,1,1,1,0,0,-10252.7,-3882.24,2.07302,5.81115,300,5,0,2200,0,0,1),
(12041060,7901,1,1,1,0,0,-10210.4,-3831.51,2.41043,0.668364,300,5,0,2200,0,0,1),
(12041061,7901,1,1,1,0,0,-10169.3,-3789.82,0.974424,0.889847,300,5,0,2200,0,0,1),
(12041062,7901,1,1,1,0,0,-10128.6,-3854.9,4.38555,4.43827,300,5,0,2200,0,0,1),
(12041063,7901,1,1,1,0,0,-10190.3,-3940.68,3.63953,5.91325,300,5,0,2200,0,0,1),
(12041064,7901,1,1,1,0,0,-10174.1,-3956.01,6.01275,5.52134,300,5,0,2200,0,0,1),
(12041065,7901,1,1,1,0,0,-10087.9,-4187.35,0.658203,5.35013,300,5,0,2200,0,0,1),
(12041066,7901,1,1,1,0,0,-10198.3,-4040.38,1.48576,3.94584,300,5,0,2200,0,0,1);

-- Treasure Hunting Buccaner
DELETE FROM creature WHERE id = 7902;
INSERT INTO creature VALUES (12041067,7902,1,1,1,0,0,-10204.5,-4046.77,1.01988,3.94584,300,5,0,2200,0,0,1),
(12041068,7902,1,1,1,0,0,-10232.4,-3928.09,0.581448,2.96409,300,5,0,2200,0,0,1),
(12041069,7902,1,1,1,0,0,-10207.1,-3874.61,1.14092,1.20951,300,5,0,2200,0,0,1),
(12041070,7902,1,1,1,0,0,-10151.2,-3823.26,3.8683,0.247395,300,5,0,2200,0,0,1),
(12041071,7902,1,1,1,0,0,-10101.3,-3846.19,5.80378,5.76167,300,5,0,2200,0,0,1),
(12041072,7902,1,1,1,0,0,-10122.1,-3881.27,5.0473,4.07621,300,5,0,2200,0,0,1),
(12041073,7902,1,1,1,0,0,-10170.1,-3889.2,2.92981,2.86984,300,5,0,2200,0,0,1),
(12041074,7902,1,1,1,0,0,-10198.4,-3791.15,1.19671,2.30435,300,5,0,2200,0,0,1);