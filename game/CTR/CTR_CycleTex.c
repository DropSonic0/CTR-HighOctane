#include <common.h>

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80021984-0x80021a20.
void CTR_CycleTex_LEV(struct AnimTex *animtex, int timer)
{
	int frameCurr;
	struct AnimTex *curAnimTex = animtex;

	// Termination is determined by pointer to First AnimTex
	while (*(int *)curAnimTex != (int)animtex)
	{
		s16 numFrames = (s16)CTR_ReadU16LE(&curAnimTex->numFrames);
		s16 frameOffset = (s16)CTR_ReadU16LE(&curAnimTex->frameOffset);
		s16 frameSkip = (s16)CTR_ReadU16LE(&curAnimTex->frameSkip);

		// which texture to draw this frame
		frameCurr = FPS_HALF(timer) + frameOffset;

		// allow frames to skip updating (like 60fps hacks)
		frameCurr = frameCurr >> frameSkip;

		// loop back to index[0] after finished cycle
		if (numFrames > 0)
		{
			frameCurr = frameCurr % numFrames;
		}

		// save result
		curAnimTex->frameCurr = (s16)frameCurr;

		struct IconGroup4 **ptrArray = ANIMTEX_GETARRAY(curAnimTex);

		// Save new frame
		// For levels, this is just a pointer
		curAnimTex->ptrActiveTex = (int *)ptrArray[frameCurr];

		// Go to next AnimTex, which comes after this AnimTex's ptrarray
		curAnimTex = (struct AnimTex *)&ptrArray[numFrames];
	}
}

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80021a20-0x80021ac0.
void CTR_CycleTex_Model(struct AnimTex *animtex, int timer)
{
	int frameCurr;
	struct AnimTex *curAnimTex = animtex;

	// Termination is determined by pointer to First AnimTex
	while (*(int *)curAnimTex != (int)animtex)
	{
		s16 numFrames = (s16)CTR_ReadU16LE(&curAnimTex->numFrames);
		s16 frameOffset = (s16)CTR_ReadU16LE(&curAnimTex->frameOffset);
		s16 frameSkip = (s16)CTR_ReadU16LE(&curAnimTex->frameSkip);

		// which texture to draw this frame
		frameCurr = FPS_HALF(timer) + frameOffset;

		// allow frames to skip updating (like 60fps hacks)
		frameCurr = frameCurr >> frameSkip;

		// loop back to index[0] after finished cycle
		if (numFrames > 0)
		{
			frameCurr = frameCurr % numFrames;
		}

		// save result
		curAnimTex->frameCurr = (s16)frameCurr;

		struct IconGroup4 **ptrArray = ANIMTEX_GETARRAY(curAnimTex);

		// Save new frame
		// For Model, this is a pointer to a pointer
		*curAnimTex->ptrActiveTex = (int)ptrArray[frameCurr];

		// Go to next AnimTex, which comes after this AnimTex's ptrarray
		curAnimTex = (struct AnimTex *)&ptrArray[numFrames];
	}
}

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80021ac0-0x80021b94.
void CTR_CycleTex_AllModels(u32 numModels, struct Model **pModelArray, int timer)
{
	struct Model *pModel;
	struct ModelHeader *pHeader;

	if (pModelArray == NULL)
	{
		return;
	}

	if (numModels == 0)
	{
		return;
	}

	while (true)
	{
		pModel = *pModelArray;
		if (pModel == NULL)
		{
			return;
		}

		s16 numHeaders = (s16)CTR_ReadU16LE(&pModel->numHeaders);
		// iterate over all model headers
		for (int j = 0; j < numHeaders; j++)
		{
			pHeader = &pModel->headers[j];

			u16 flags = CTR_ReadU16LE(&pHeader->flags);
			if ((pHeader->animtex != NULL) && ((flags & 2) == 0))
			{
				CTR_CycleTex_Model(pHeader->animtex, timer);
			}
		}

		numModels--;
		if (numModels == 0)
		{
			return;
		}

		pModelArray++;
	}
}

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80021b94-0x80021bbc.
void CTR_CycleTex_2p3p4pWumpaHUD(u32 *ptrActiveTex, u32 *ptrArray, int numFrames)
{
	ptrArray[0] = ptrActiveTex[0];
	ptrActiveTex[0] = CtrGpu_PrimToOTLink24(&ptrArray[numFrames - 1]);
}
