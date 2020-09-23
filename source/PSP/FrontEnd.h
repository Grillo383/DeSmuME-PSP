#ifndef FRONTEND
#define FRONTEND
//const char *nds_file;

class configured_features 
{
public:
  int auto_pause;
  int frameskip;

  //int engine_3d;
  int savetype;

  int fps_cap_num;

  int firmware_language;

  int Render3DCore;

  bool enable_sound;

  bool showfps;

  bool swap;

  bool cur;

  bool Render3D;

  bool fps_cap;

  bool UseExtFirmware;

  bool RenderBack;
};

typedef struct configparm {
	char name[32];
	int var;
}configP;

typedef struct fname {
	char name[256];
}f_name;

typedef struct flist {
	f_name fname[256];
	int cnt;
}f_list;


extern configured_features my_config;

extern void ChangeRom(bool reset);
extern void ResetRom();
extern void EMU_Conf();

void DoConfig(configured_features * params);

void DSEmuGui(char *path,char *out);

/*
inline UDMAC_MEMCPY(u32* src, u32* dst, u16 size) {
	union {
		u32 val[size];
		u64 val[size/2];
	};
   
}*/

#endif
