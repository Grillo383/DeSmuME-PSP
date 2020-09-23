/** PSP helper library ***************************************/
/**                                                         **/
/**                          video.h                        **/
/**                                                         **/
/** This file contains declarations for the video rendering **/
/** library                                                 **/
/**                                                         **/
/** Copyright (C) Akop Karapetyan 2007                      **/
/**     You are not allowed to distribute this software     **/
/**     commercially. Please, notify me, if you make any    **/
/**     changes to this file.                               **/
/*************************************************************/
#ifndef _PSP_VIDEO_H
#define _PSP_VIDEO_H

#include "../types.h"
#include <oslib/oslib.h>
#include "FrontEnd.h"

void Init_PSP_DISPLAY_FRAMEBUFF();

extern OSL_COLOR backColor;

extern void (*UpdateScreen)();

void SetupEmuDisplay(bool direct);
void UpdateSingleScreen();
void DrawRom(char* file, f_list* list, int pos,bool reload);
void DrawSettingsMenu(configP* params,int size,int currPos);
void DrawStartUpMenu();

OSL_IMAGE* GetUpperFrameBuffer();
OSL_IMAGE* GetLowerFrameBuffer();

void* GetPointerFrameBuffer(bool upper);
void* GetPointerUpperFrameBuffer();
void* GetPointerLowerFrameBuffer();

#endif  // _PSP_VIDEO_H