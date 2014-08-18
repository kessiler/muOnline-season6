#pragma once
// ----------------------------------------------------------------------------------------------

#pragma pack(push, 1)
struct MU3Float
{
	float x;
	float y;
	float z;
};
#pragma pack(pop)
// ----------------------------------------------------------------------------------------------

struct Preview
{
		BYTE	Shift1[14];

/*4*/	//BYTE	Unknown4;	// -> 100%
/*8*/	//DWORD	Unknown8;	// -> 100%

/*14*/	BYTE	IsSafeZone;
/*15*/	BYTE	Visible;
/*16*/	BYTE	Unknown16;
/*17*/	BYTE	IsLiveObject;
/*18*/	BYTE	Unknown18;
/*19*/	BYTE	Class;
/*20*/	BYTE	Unknown20;
/*21*/	BYTE	CtrlCode;
/*22*/	BYTE	AncientEffect;
/*23*/	BYTE	PetStatus;
/*24*/	BYTE	Unknown24;
/*25*/	BYTE	Unknown25;
/*26*/	BYTE	Unknown26;
/*27*/	BYTE	Unknown27;
/*28*/	BYTE	Unknown28;
/*29*/	BYTE	Unknown29;
/*30*/	BYTE	Unknown30;
/*31*/	BYTE	Unknown31;
/*32*/	BYTE	PKLevel;
/*33*/	BYTE	Unknown33;
/*34*/	BYTE	Unknown34;
/*35*/	BYTE	Unknown35;
/*36*/	BYTE	Unknown36;
/*37*/	BYTE	Unknown37;
/*38*/	WORD	Unknown38;
/*40*/	BYTE	Unknown40;
/*41*/	BYTE	Unknown41;
/*42*/	BYTE	Unknown42;
/*43*/	BYTE	Unknown43;
/*44*/	BYTE	Unknown44;
/*45*/	BYTE	MapPosX;
/*46*/	BYTE	MapPosY;
		BYTE	Shift47[9];
/*56*/	char	Name[25];	//-> Need check size

/*50*/	//BYTE	Unknown50;	// -> 100%
/*88*/	//BYTE	Unknown88;	// -> 100%
/*90*/	//BYTE	Unknown90;	// -> 100%
/*91*/	//BYTE	Unknown91;	// -> 100%
/*92*/	//WORD	InGuild;	// -> 100%
/*94*/	//WORD	ObjIndex;	// -> 100%
/*96*/	//WORD	Unknown96;	// -> 100%
/*98*/	//WORD	Unknown98;	// -> 100%
/*100*/	//WORD	Unknown100;	// -> 100%
/*106*/	//WORD	Unknown106;	// -> 100%
/*112*/	//DWORD	Unknown112;	// -> 100%
/*116*/	//DWORD	Unknown116;	// -> 100%
/*124*/	//DWORD	Unknown124;	// -> 100%
/*140*/	//DWORD MapPosX;	// -> 100%
/*144*/	//DWORD MapPosY;	// -> 100%
/*156*/	//DWORD	Unknown156;	// -> 100%
/*172*/	//DWORD	Unknown172;	// -> 100%
/*188*/	//DWORD	Unknown188;	// -> 100%
/*192*/	//DWORD	Unknown192;	// -> 100%
/*196*/	//DWORD	Unknown196;	// -> 100%

		BYTE	Shift2[150];

/*232*/	WORD	HelmSlot;
/*234*/	BYTE	HelmLevel;
/*235*/	BYTE	HelmExcellent;
/*236*/	BYTE	HelmAncient;
		BYTE	Shift5[27];
/*264*/	WORD	ArmorSlot;
/*266*/	BYTE	ArmorLevel;
/*267*/	BYTE	ArmorExcellent;
/*268*/	BYTE	ArmorAncient;
		BYTE	Shift6[27];
/*296*/	WORD	PantsSlot;
/*298*/	BYTE	PantsLevel;
/*299*/	BYTE	PantsExcellent;
/*300*/	BYTE	PantsAncient;
		BYTE	Shift7[27];
/*328*/	WORD	GlovesSlot;
/*330*/	BYTE	GlovesLevel;
/*331*/	BYTE	GlovesExcellent;
/*332*/	BYTE	GlovesAncient;
		BYTE	Shift8[27];
/*360*/	WORD	BootsSlot;
/*362*/	BYTE	BootsLevel;
/*363*/	BYTE	BootsExcellent;
/*364*/	BYTE	BootsAncient;
		BYTE	Shift9[27];
/*392*/	WORD	WeaponFirstSlot;
/*394*/	BYTE	WeaponFirstLevel;
/*395*/	BYTE	WeaponFirstExcellent;
/*396*/	BYTE	WeaponFirstAncient;
/*397*/	BYTE	Unknown397;
		BYTE	Shift10[26];
/*424*/	WORD	WeaponSecondSlot;
/*426*/	BYTE	WeaponSecondLevel;
/*427*/	BYTE	WeaponSecondExcellent;
/*428*/	BYTE	WeaponSecondAncient;
/*429*/	BYTE	Unknown429;
		BYTE	Shift11[26];
/*456*/	WORD	WingsSlot;
/*458*/ BYTE	Unknown458;
/*459*/ BYTE	Unknown459;
/*460*/ BYTE	Unknown460;
		BYTE	Shift12[27];
/*488*/	WORD	PetSlot;
/*490*/ BYTE	Unknown490;
/*491*/ BYTE	FenrirCharSet;
/*492*/ BYTE	ExpectPet;

/*520*/	//WORD	Unknown520;	// -> 100%
/*596*/	//DWORD	Unknown596;	// -> 100%
/*688*/	//DWORD	Unknown688;	//
		BYTE	Shift13[207];

/*700*/	DWORD	Unknown700;	// -> # Need check
/*704*/	BYTE	Unknown704;

		BYTE	Shift14[2];

/*707*/	BYTE	Unknown707;
/*708*/	BYTE	Unknown708;
/*709*/	BYTE	Unknown709;
/*710*/	BYTE	Unknown710;	// -> 100%
/*711*/	BYTE	Unknown711;
/*712*/	BYTE	Unknown712;
		BYTE	Shift16[5];
/*718*/	BYTE	Action;
/*719*/	BYTE	Unknown719;
/*720*/	BYTE	Unknown720; //-> 100%
/*721*/	BYTE	Unknown721;	//-> 100%
/*722*/ BYTE	ObjectType; //-> Object Type (1 = Player, 2 = Monster, 4 = NPC) 100%
		BYTE	Shift17[25];
/*748*/	DWORD	Unknown748;
/*752*/	DWORD	Unknown752;
/*756*/	DWORD	Unknown756;
/*760*/	DWORD	Unknown760;

/*768*/	//DWORD	Unknown768;	// -> 100%

		BYTE	Shift18[32];
/*796*/	DWORD	ModelScale;
		BYTE	Shift19[16];

/*800*/	//DWORD	Unknown800;	// -> 100%

/*816*/	DWORD	Unknown816;

/*824*/	//DWORD	Unknown824;	// -> 100%

		BYTE	Shift20[103];

/*856*/	//DWORD	Unknown856;	// -> 100%
/*860*/	//DWORD	Unknown860;	// -> 100%
/*864*/	//DWORD	Unknown864;	// -> 100%
/*972*/	//DWORD	Unknown972;	// -> 100%
/*904*/	//DWORD	Unknown904;	// -> 100%
/*908*/	//DWORD	Unknown908;	// -> 100%
/*912*/	//DWORD	Unknown912;	// -> 100%
/*916*/	//DWORD	Unknown916;	// -> 100% / Maybe float
/*920*/	//DWORD	Unknown920;	// -> 100% / Maybe float

/*924*/	float	SelectZone;	// -> Maybe DWORD
		BYTE	Shift21[24];

/*952*/	float	Unknown952;
/*956*/	float	Unknown956;
/*960*/ float	Unknown960;

/*964*/	//DWORD	Unknown964;	// -> 100%
/*968*/	//DWORD	Unknown968;	// -> 100%
/*972*/	//DWORD	Unknown972;	// -> 100%
/*1120*/	//DWORD	Unknown1120;	// -> 100%
/*1184*/	//void?
};
//---------------------------------------------------------------------------