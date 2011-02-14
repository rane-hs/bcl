#include "stdafx.h"
#pragma warning( disable : 4786 )   // C4786‚ÌWarning‚ð–³Ž‹
#include "CCastkind.h"
#include "castAccess.h"

void CCastKindInfo::sb_getSysKindInfo(const char *dpath, int officeID, int unitID)
{
	int i,ret;
	char fpath[_MAX_PATH];

	for (i = 0; i < ckind_data.size(); i++) {
		if (ckind_data[i].name.empty()) continue;

		// ‹ÇŽÀ‘•î•ñ‚Ì“Ç‚Ýž‚Ý
		ckind_data[i].local = new sys_local_t[ckind_data[i].max];
		memset(ckind_data[i].local, 0, sizeof(sys_local_t) * ckind_data[i].max);
		sprintf(fpath, "%ssys_table\\csv\\%s_local_%02d%02d%02d.csv", dpath, ckind_data[i].name.c_str(), officeID, unitID, 0);
		ret = castGetSysLocal(fpath, &ckind_data[i], ckind_data[i].local);

		// ‹Ç‰Ò“®î•ñ‚Ì“Ç‚Ýž‚Ý
		ckind_data[i].action = new sys_action_t[ckind_data[i].max];
		memset(ckind_data[i].action, 0, sizeof(sys_action_t) * ckind_data[i].max);
		sprintf(fpath, "%ssys_table\\csv\\%s_action.csv", dpath, ckind_data[i].name.c_str());
		ret = castGetSysAction(fpath, &ckind_data[i], ckind_data[i].action);
	}

};
