// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------

#pragma once

class CDoppelgangerPosInfo
{
public:
	CDoppelgangerPosInfo(void);
	virtual ~CDoppelgangerPosInfo(void);

	enum { kMaxMonsterType = 23 };
    enum { kStartEventMapNumber = 65 };
    enum { kMaxMapTypes = 4 };

    void LoadDoppelgangerPosInfo();
    void GetCenterPosition(int map_number, int pos_index, BYTE& pos_x, BYTE& pos_y);
    int GetPosIndex(int map_number, BYTE pos_x, BYTE pos_y);
    void GetStartMapAttr(int map_number, BYTE &left_x, BYTE &top_y, BYTE &right_x, BYTE &bottom_y);
    bool CheckStartMapTile(int map_number, BYTE pos_x, BYTE pos_y);

private:
	bool loaded_;
	BYTE posinfo_[kMaxMapTypes][kMaxMonsterType][4];
	BYTE start_posinfo_[kMaxMapTypes][4];
};

