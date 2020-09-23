#include <pspkernel.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <pspdebug.h>
#include <math.h>
#include <string.h>
#include <pspctrl.h>
#include <pspgu.h>
#include "FrontEnd.h"
#include <psprtc.h>
#include <pspdisplay.h>
#include "video.h"
//#include "Version.h"

configP configparms[30];
int totalconfig=0;


void InitConfigParms(configured_features * params){
	int c=0;

    strcpy(configparms[c].name,"Screen <--> SWAP");
	params->swap = configparms[c].var;
	c++;
	strcpy(configparms[c].name,"Show Touch Cursor");
	params->cur = configparms[c].var;
	c++;
	strcpy(configparms[c].name, "Backdrops Redering");
	params->RenderBack = configparms[c].var;
	c++;
	strcpy(configparms[c].name,"Show FPS");
	params->showfps = configparms[c].var;
	c++;
	strcpy(configparms[c].name,"Render 3D");
	params->Render3D = configparms[c].var;
	c++;
	strcpy(configparms[c].name,"Enable Audio");
	params->enable_sound = configparms[c].var;
	c++;
	strcpy(configparms[c].name,"Frameskip");
	params->frameskip = configparms[c].var;
	c++;
    strcpy(configparms[c].name,"Language");
	params->firmware_language = configparms[c].var;
	c++;
	strcpy(configparms[c].name,"FPS Cap");
	params->fps_cap = configparms[c].var;
	c++;
	strcpy(configparms[c].name,"Max FPS");
	params->fps_cap_num = configparms[c].var;
	c++;
	strcpy(configparms[c].name,"Render 3D Core");
	params->Render3DCore = configparms[c].var;
	c++;

	//
	totalconfig =c;
}

int selposconfig=0;
int frameposconfig=0;
int langposconfig=0;
int render3dconfig=0;
int fpscap=0;

void DoConfig(configured_features * params)
{
	int done =0;
	SceCtrlData pad,oldPad;

	int cnt;
	//for (cnt=0;cnt<100;cnt++)////pspDebugScreenPrintf("\n");
	while(!done){
		sceDisplayWaitVblankStart();
		InitConfigParms(params);
		DrawSettingsMenu(configparms, totalconfig, selposconfig);
		if(sceCtrlPeekBufferPositive(&pad, 1))
		{
			if (pad.Buttons != oldPad.Buttons)
			{
                if(pad.Buttons & PSP_CTRL_LEFT)
				{ 
					if(strcmp(configparms[selposconfig].name,"Language")== 0)
					{
					configparms[selposconfig].var = langposconfig--;
					if(langposconfig == -1 )langposconfig = 0;
					}else			
					if(strcmp(configparms[selposconfig].name,"Frameskip")== 0)
					{
					configparms[selposconfig].var = frameposconfig--;
					if(frameposconfig == -1 )frameposconfig = 0;
					}else			
					if(strcmp(configparms[selposconfig].name,"Render 3D Core")== 0)
					{
					configparms[selposconfig].var = render3dconfig--;
					if(render3dconfig == -1 )render3dconfig = 0;
					}else			
					if(strcmp(configparms[selposconfig].name,"Max FPS")== 0)
					{
					configparms[selposconfig].var = (fpscap--)*5;
					if(fpscap == -1 )fpscap = 1;
					}
					else
					{
						configparms[selposconfig].var = 0;
					}
				}
				if(pad.Buttons & PSP_CTRL_RIGHT){
				if(strcmp(configparms[selposconfig].name,"Language")== 0)
				{
				configparms[selposconfig].var = langposconfig++;
				if(langposconfig == 8 )langposconfig = 7;
				}else
				if(strcmp(configparms[selposconfig].name,"Frameskip")== 0)
				{
				configparms[selposconfig].var = frameposconfig++;
                if(frameposconfig == 10)frameposconfig = 9;  
				}else
				if(strcmp(configparms[selposconfig].name,"Render 3D Core")== 0)
				{
				configparms[selposconfig].var = render3dconfig++;
                if(render3dconfig == 3)render3dconfig = 2;  
				}else
				if(strcmp(configparms[selposconfig].name,"Max FPS")== 0)
				{
				configparms[selposconfig].var = (fpscap++)*5;
                if(fpscap == 13)fpscap = 12;  
				}else{
                configparms[selposconfig].var = 1;
				}
				}
				if(pad.Buttons & PSP_CTRL_START){
					done = 1;//delay
					break;
				}
				if(pad.Buttons & PSP_CTRL_UP){
					selposconfig--;
					if(selposconfig < 0)selposconfig=0;
				}
				if(pad.Buttons & PSP_CTRL_DOWN){
					selposconfig++;
					if(selposconfig >= totalconfig -1)selposconfig=totalconfig-1;
				}

			}
			oldPad = pad;
		}

	}

}


f_list filelist;

void ClearFileList(){
	filelist.cnt =0;
}


int HasExtension(char *filename){
	if(filename[strlen(filename)-4] == '.'){
		return 1;
	}
	return 0;
}


void GetExtension(const char *srcfile,char *outext){
	if(HasExtension((char *)srcfile)){
		strcpy(outext,srcfile + strlen(srcfile) - 3);
	}else{
		strcpy(outext,"");
	}
}

enum {
	EXT_NDS = 1,
	EXT_GZ = 2,
	EXT_ZIP = 4,
	EXT_UNKNOWN = 8,
};

const struct {
	char *szExt;
	int nExtId;
} stExtentions[] = {
	{"nds",EXT_NDS},
//	{"gz",EXT_GZ},
//	{"zip",EXT_ZIP},
	{NULL, EXT_UNKNOWN}
};

int getExtId(const char *szFilePath) {
	char *pszExt;

	if ((pszExt = strrchr(szFilePath, '.'))) {
		pszExt++;
		int i;
		for (i = 0; stExtentions[i].nExtId != EXT_UNKNOWN; i++) {
			if (!strcasecmp(stExtentions[i].szExt,pszExt)) {
				return stExtentions[i].nExtId;
			}
		}
	}

	return EXT_UNKNOWN;
}

void GetFileList(const char *root)
{
	int dfd;
	dfd = sceIoDopen(root);
	if(dfd > 0){
		SceIoDirent dir;
		while(sceIoDread(dfd, &dir) > 0)
		{
			if(dir.d_stat.st_attr & FIO_SO_IFDIR)
			{
				//directories
			}
			else
			{				
				if(getExtId(dir.d_name)!= EXT_UNKNOWN){
				strcpy(filelist.fname[filelist.cnt].name,dir.d_name);
				filelist.cnt++;
				}
			
			}
		}
		sceIoDclose(dfd);
	}
}

int selpos=0, old_sel = -1;
void DisplayFileList(char * rom_path)
{
	static const int LIST_SIZE = filelist.cnt;
		
	if (selpos >= LIST_SIZE) return;
	if (selpos == old_sel) return;


	DrawRom(rom_path, &filelist, selpos, !(selpos%3));
	
	old_sel = selpos;

}

void DSEmuGui(char *path,char *out)
{
	//int done =0;
	//char ext[4];
	char tmp[256];
	char app_path[128];
	char romname[256];
	//char *slash;
	//char appsave[1024];

	SceCtrlData pad,oldPad;
	
	ClearFileList();

	getcwd(app_path,128);

	sprintf(tmp,"%s/NDSROM/",app_path);

	GetFileList(tmp);

	int cnt;
	long tm;
	while(1){
	
		DisplayFileList(tmp);

		if(sceCtrlPeekBufferPositive(&pad, 1))
		{
			if (pad.Buttons != oldPad.Buttons)
			{
				if(pad.Buttons & PSP_CTRL_SQUARE){
			      sceKernelExitGame();
				}

				if(pad.Buttons & PSP_CTRL_CROSS)
				{
				 sprintf(out,"%s%s",tmp,filelist.fname[selpos].name);	
				 my_config.UseExtFirmware = false;		
						break;
				}

				if(pad.Buttons & PSP_CTRL_TRIANGLE)
				{
				 sprintf(out,"%s%s",tmp,filelist.fname[selpos].name);	
					my_config.UseExtFirmware = true;
				 break;
				}
			
				if(pad.Buttons & PSP_CTRL_UP){
					selpos--;
					if(selpos < 0)selpos=0;
				}
				if(pad.Buttons & PSP_CTRL_DOWN){
					selpos++;
					if(selpos >= filelist.cnt -1)selpos=filelist.cnt-1;
				}

			}
			oldPad = pad;
		}

	}
}

