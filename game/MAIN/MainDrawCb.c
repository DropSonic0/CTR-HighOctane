#include <common.h>

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80034a80-0x80034aa4.
void MainDrawCb_DrawSync()
{
	struct GameTracker *gGT;
	gGT = sdata->gGT;

	if (gGT->bool_DrawOTag_InProgress == 1)
	{
		gGT->bool_DrawOTag_InProgress = 0;
	}

	return;
}

// NOTE(aalhendi): ASM-verified NTSC-U 926 0x80034aa4-0x80034b48 for the retail body.
// NOTE(aalhendi): CTR_NATIVE keeps native input/audio/VRAM bridge work.
void MainDrawCb_Vsync()
{
	struct GameTracker *gGT;

	Platform_Log("[CTR Native] MainDrawCb_Vsync: start...\n");
	Platform_LogFlush();

	gGT = sdata->gGT;
	gGT->frameTimer_VsyncCallback++;
	if ((gGT->gameMode1 & PAUSE_ALL) == 0)
	{
		gGT->frameTimer_Confetti++;
	}

	sdata->vsyncTillFlip--;
	gGT->vSync_between_drawSync++;

	// NOTE(aalhend): accumulate root-counter 1 units between VSync callbacks; Timer_GetTime_Total converts them through the retail 0x147e divisor.
	sdata->rcntTotalUnits += GetRCnt(0xf2000001);
	ResetRCnt(0xf2000001);

#ifdef CTR_NATIVE
	// NOTE(aalhendi): Retail calls HOWL unconditionally. Native skips only while
	// channel lists are mid-edit.
	if (sdata->criticalSectionCount == 0)
#endif
	{
		Platform_Log("[CTR Native] MainDrawCb_Vsync: howl_PlayAudio_Update start...\n");
		Platform_LogFlush();
		howl_PlayAudio_Update();
		Platform_Log("[CTR Native] MainDrawCb_Vsync: howl_PlayAudio_Update done.\n");
		Platform_LogFlush();
	}

#ifdef CTR_NATIVE
	// NOTE(aalhendi): Native owns host input and writes PSX-shaped pad
	// snapshots before retail GAMEPAD_PollVsync consumes them.
	Platform_Log("[CTR Native] MainDrawCb_Vsync: Platform_PollInput...\n");
	Platform_LogFlush();
	Platform_PollInput();
#endif

	Platform_Log("[CTR Native] MainDrawCb_Vsync: GAMEPAD_PollVsync...\n");
	Platform_LogFlush();
	GAMEPAD_PollVsync(sdata->gGamepads);

	Platform_Log("[CTR Native] MainDrawCb_Vsync: finished!\n");
	Platform_LogFlush();

	return;
}
