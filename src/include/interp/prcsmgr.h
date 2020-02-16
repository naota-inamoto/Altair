/*
 * Copyright (C) 1995-2020 Naota Inamoto,
 * All rights reserved.
 *
 * For conditions of redistribution and reuse,
 * see the accompanying License-Altair.txt file
 */

#ifndef _AIP_PRCS_MGR_H
#define _AIP_PRCS_MGR_H

class AL_EXT_CLASS AIpProcessMgr : public AGm {
public:
	AIpProcessMgr();
	virtual ~AIpProcessMgr();
	static	AIpProcessMgr* Instance();

	virtual AIpProcess* CreateProcess(AGmNode *root_class, AGmNode *arg, AGmNode *prog, AGmNode *this_p=NULL);
	virtual int NextProcess(AGmLink* lnk=NULL);

	AGmNode*	Processes() { return _processes; }
	AIpProcess* Process();
protected:
	AGmNode*	_processes;
	AGmLink*	_curr_process_link;

// for garbage collection
public:
	void GCmark();

// processes in local loop status
public:
	void AddLocalLoopProcess(AIpProcess*, int wait_notify_flag=0);
	void RemoveLocalLoopProcess(AIpProcess*, int wait_notify_flag=0);
};

#endif	/* _PRCS_MGR_H */

