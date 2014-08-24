// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------

#pragma once
#include "ItemBagEx.h"

struct OBJECTSTRUCT;

struct DOPPELGANGER_DROP_ITEM_GROUP_INFO
{
    int number;
    int option1_rate;
    int option2_rate;
    int option3_rate;
    int excellent_option_rate;
    std::vector<CItemBagAttrEx> itembag_attributes;
};



class CDoppelgangerItemBag
{
public:
    CDoppelgangerItemBag(void);
    virtual ~CDoppelgangerItemBag(void);

    void DopplegangerItemBagInit();
    void LoadDoppelgangerItemBag();
    bool DoppelgangerItemDrop(OBJECTSTRUCT *lpObj, int itembag_index, int droprate, BYTE flag);

private:
    std::vector<DOPPELGANGER_DROP_ITEM_GROUP_INFO> drop_item_groups_;
    int option3_weights_[4];                // max additional option
    int excellent_option_count_weights_[2];
    int excellent_option_weights_[6];       // max excellent option types
    bool loaded_;
};

