#ifndef __ITEMDEF_H__
#define __ITEMDEF_H__

#define MAX_ITEM_TYPE      16
#define MAX_ITEM_INDEX     512
#define MAX_ITEM           (MAX_ITEM_TYPE*MAX_ITEM_INDEX)

#define MAX_ITEM_SPECIAL   8

#define ITEM_LEVEL_NORMAL  8
#define GOLD_TYPE_INDEX			((14<<9)+15)	// 금의 타입과 인덱스

//equipment index
#define EQUIPMENT_WEAPON_RIGHT 0
#define EQUIPMENT_WEAPON_LEFT  1
#define EQUIPMENT_HELM         2
#define EQUIPMENT_ARMOR        3
#define EQUIPMENT_PANTS        4
#define EQUIPMENT_GLOVES       5
#define EQUIPMENT_BOOTS        6
#define EQUIPMENT_WING         7
#define EQUIPMENT_HELPER       8
#define EQUIPMENT_AMULET       9
#define EQUIPMENT_RING_RIGHT   10
#define EQUIPMENT_RING_LEFT    11
#define EQUIPMENT_GOLD         12
//#define MAX_EQUIPMENT          EQUIPMENT_GOLD+1

//item index
#define ITEM_SWORD		 0   //pal,etc
#define ITEM_AXE		 (MAX_ITEM_INDEX*1)  //ran,dru
#define ITEM_MACE		 (MAX_ITEM_INDEX*2)  //ran,nec
#define ITEM_SPEAR		 (MAX_ITEM_INDEX*3)  //ama
#define ITEM_BOW		 (MAX_ITEM_INDEX*4) //ama
#define ITEM_STAFF	 	 (MAX_ITEM_INDEX*5) //soc
#define ITEM_SHIELD		 (MAX_ITEM_INDEX*6)
#define ITEM_HELM		 (MAX_ITEM_INDEX*7)
#define ITEM_ARMOR		 (MAX_ITEM_INDEX*8)
#define ITEM_PANTS		 (MAX_ITEM_INDEX*9)
#define ITEM_GLOVE		 (MAX_ITEM_INDEX*10)
#define ITEM_BOOT		 (MAX_ITEM_INDEX*11)
#define ITEM_WING		 (MAX_ITEM_INDEX*12)
#define ITEM_HELPER		 (MAX_ITEM_INDEX*13)
//#define ITEM_RING		 224
#define ITEM_POTION		 (MAX_ITEM_INDEX*14)
#define ITEM_ETC		 (MAX_ITEM_INDEX*15)

//weapon special
#define PLUS_POISON                1
#define PLUS_COLD                  2
#define PLUS_FIRE                  4
#define PLUS_ATTACK_SPEED          8  //race
#define PLUS_ATTACK_RATING         16 //race
#define PLUS_DAMAGE                32 //race
#define PLUS_MAGIC_DAMAGE          64 //race

//armor special
#define PLUS_DEXTERIRY             1
#define PLUS_DEFENSE               2
#define PLUS_MAGIC_DEFENSE         4
#define PLUS_CHANCE_TO_BLOCK       8  //shield
#define PLUS_WALK_SPEED            16 //boot

//helper special
#define PLUS_STRENGTH              1
#define PLUS_LIFE                  2
#define PLUS_LIFE_STOLEN           4
#define PLUS_MANA                  8
#define PLUS_MANA_STOLEN           16
#define PLUS_LIFE_REGENERATION     32 //helper
#define PLUS_SKILL_LEVEL           64 //helper


// 스킬
/*#define SKILL_POISON               1
#define SKILL_METEORITE            2
#define SKILL_THUNDER              3
#define SKILL_BLOCKING             4
#define SKILL_SWORD1               5
#define SKILL_SWORD2               6
#define SKILL_SWORD3               7
#define SKILL_SWORD4               8
*/

#define MAX_RESISTANCE	7
#endif

