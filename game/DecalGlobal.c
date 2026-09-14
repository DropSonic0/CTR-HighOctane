#include <common.h>

enum
{
	DECAL_NAME_BYTE_COUNT = 0x10,
	DECAL_NAME_WORD_COUNT = DECAL_NAME_BYTE_COUNT / (s32)sizeof(u32),
};

static u32 DecalGlobal_ReadNameWord(const char *name, s32 wordIndex)
{
	u32 word;
	memcpy(&word, &name[wordIndex * (s32)sizeof(word)], sizeof(word));
	return word;
}

static b32 DecalGlobal_NameEquals(const char *lhs, const char *rhs)
{
	return (DecalGlobal_ReadNameWord(lhs, 0) == DecalGlobal_ReadNameWord(rhs, 0)) && (DecalGlobal_ReadNameWord(lhs, 1) == DecalGlobal_ReadNameWord(rhs, 1)) &&
	       (DecalGlobal_ReadNameWord(lhs, 2) == DecalGlobal_ReadNameWord(rhs, 2)) && (DecalGlobal_ReadNameWord(lhs, 3) == DecalGlobal_ReadNameWord(rhs, 3));
}

CTR_STATIC_ASSERT(DECAL_NAME_BYTE_COUNT == 0x10);
CTR_STATIC_ASSERT(DECAL_NAME_WORD_COUNT == 4);
CTR_STATIC_ASSERT(sizeof(((struct Icon *)0)->name) == DECAL_NAME_BYTE_COUNT);
CTR_STATIC_ASSERT(sizeof(((struct IconGroup *)0)->name) == DECAL_NAME_BYTE_COUNT);


// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80022b94-0x80022b9c.
void DecalGlobal_EmptyFunc_MainFrame_ResetDB(void)
{
}


// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80022b9c-0x80022bdc.
void DecalGlobal_Clear(struct GameTracker *gGT)
{
	memset(&gGT->ptrIcons, 0, sizeof(gGT->ptrIcons));
	memset(&gGT->iconGroup, 0, sizeof(gGT->iconGroup));
}


// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80022bdc-0x80022c88.
void DecalGlobal_Store(struct GameTracker *gGT, struct LevTexLookup *LTL)
{
	struct Icon *currIcon;
	struct IconGroup **currGroup;
	u32 numIcon;
	u32 numIconGroup;

	if (LTL == 0)
	{
		return;
	}

	numIcon = CTR_ReadU32LE(&LTL->numIcon);
	numIconGroup = CTR_ReadU32LE(&LTL->numIconGroup);

	for (
	    // array of Icon
	    currIcon = &LTL->firstIcon[0]; currIcon < &LTL->firstIcon[numIcon]; currIcon++)
	{
		u32 globalIdx = CTR_ReadU32LE(&currIcon->global_IconArray_Index);
		// uint, in case of negatives
		if (globalIdx < 0x88)
		{
			gGT->ptrIcons[globalIdx] = currIcon;
		}
	}

	for (
	    // array of POINTER to iconGroup
	    currGroup = &LTL->firstIconGroupPtr[0]; currGroup < &LTL->firstIconGroupPtr[numIconGroup]; currGroup++)
	{
		u16 groupID = CTR_ReadU16LE(&currGroup[0]->groupID);
		// use '[0]' to dereference pointer
		if ((u32)groupID < 0x11)
		{
			gGT->iconGroup[groupID] = currGroup[0];
		}
	}
}


// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80022c88-0x80022d2c.
int *DecalGlobal_FindInLEV(struct Level *level, char *str)
{
	u32 numIconGroup;
	struct IconGroup **curr;
	struct IconGroup **end;
	struct LevTexLookup *ltl = level->levTexLookup;

	if (ltl == NULL)
	{
		return NULL;
	}

	numIconGroup = CTR_ReadU32LE(&ltl->numIconGroup);
	curr = ltl->firstIconGroupPtr;
	end = &ltl->firstIconGroupPtr[numIconGroup];

	for (; curr < end; curr++)
	{
		struct IconGroup *group = *curr;

		if (DecalGlobal_NameEquals(group->name, str))
		{
			return (int *)group;
		}
	}

	return NULL;
}


// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80022d2c-0x80022db0.
int *DecalGlobal_FindInMPK(u32 *icons, char *str)
{
	struct Icon *icon = (struct Icon *)icons;

	for (; icon->name[0] != '\0'; icon++)
	{
		if (DecalGlobal_NameEquals(icon->name, str))
		{
			return (int *)icon;
		}
	}

	return NULL;
}
