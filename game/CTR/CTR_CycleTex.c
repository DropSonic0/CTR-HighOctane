#include <common.h>

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80021984-0x80021a20.
void CTR_CycleTex_LEV(struct AnimTex *animtex, int timer)
{
	int frameCurr;
	struct AnimTex *curAnimTex = animtex;

	while (*(int *)curAnimTex != (int)animtex)
	{
		s16 numFrames = (s16)CTR_ReadU16LE(&curAnimTex->numFrames);
		s16 frameOffset = (s16)CTR_ReadU16LE(&curAnimTex->frameOffset);
		s16 frameSkip = (s16)CTR_ReadU16LE(&curAnimTex->frameSkip);

		if (numFrames <= 0) { break; }

		frameCurr = FPS_HALF(timer) + frameOffset;
		frameCurr = frameCurr >> frameSkip;
		frameCurr = frameCurr % numFrames;

		CTR_WriteU16LE(&curAnimTex->frameCurr, (u16)frameCurr);

		struct IconGroup4 **ptrArray = ANIMTEX_GETARRAY(curAnimTex);
		curAnimTex->ptrActiveTex = (int *)ptrArray[frameCurr];

		curAnimTex = (struct AnimTex *)&ptrArray[numFrames];
	}
}

void CTR_CycleTex_Model(struct AnimTex *animtex, int timer)
{
	int frameCurr;
	struct AnimTex *curAnimTex = animtex;

	while (*(int *)curAnimTex != (int)animtex)
	{
		s16 numFrames = (s16)CTR_ReadU16LE(&curAnimTex->numFrames);
		s16 frameOffset = (s16)CTR_ReadU16LE(&curAnimTex->frameOffset);
		s16 frameSkip = (s16)CTR_ReadU16LE(&curAnimTex->frameSkip);

		if (numFrames <= 0) { break; }

		frameCurr = FPS_HALF(timer) + frameOffset;
		frameCurr = frameCurr >> frameSkip;
		frameCurr = frameCurr % numFrames;

		CTR_WriteU16LE(&curAnimTex->frameCurr, (u16)frameCurr);

		struct IconGroup4 **ptrArray = ANIMTEX_GETARRAY(curAnimTex);
		*curAnimTex->ptrActiveTex = (int)ptrArray[frameCurr];

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

		// iterate over all model headers
		for (int j = 0; j < pModel->numHeaders; j++)
		{
			pHeader = &pModel->headers[j];

			if ((pHeader->animtex != NULL) && ((pHeader->flags & 2) == 0))
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
