#pragma once
#ifndef ROADINFO_H_
#define ROADINFO_H_
typedef struct tagRoad
{
	unsigned short usSize;
	unsigned int uiLinkId;
	unsigned short usNameSize;
	unsigned int uiFlag;
	unsigned int uiClass;
	unsigned uiBranch;
	char acName[50];
}RoadMsg_t, * pRoadMsg_t;;

#endif // !ROADINFO_H_
