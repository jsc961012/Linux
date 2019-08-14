#include <stdio.h>
#include <stdlib.h>
#include "../include/list.h"
#include "../include/Macro.h"

void replaceFile(pLinkNode_t head) {
	FILE* fp = fopen("./File/test.dat", "wb+");
	FILE* fp1 = fopen("./File/aaaa.dat", "wb+");
	data_t msg;
	unsigned int uiRoadInfo = 0;
	int count = head->count;
	pLinkNode_t p = head->next;

	/*for (int i = 0; i < 5000; i++)
	{
		msg.usSize = MC_GET_SHORT(&(p->data.usSize));
		fwrite(&msg.usSize, sizeof(msg.usSize), 1, fp1);

		msg.uiLinkId = MC_GET_LONG(&(p->data.uiLinkId));
		fwrite(&msg.uiLinkId, sizeof(msg.uiLinkId), 1, fp1);

		msg.usNameSize = MC_GET_SHORT(&(p->data.usNameSize));
		fwrite(&msg.usNameSize, sizeof(msg.usNameSize), 1, fp1);

		uiRoadInfo = (p->data.uiClass) | (p->data.uiBranch << 4) | (p->data.uiFlag << 7);
		uiRoadInfo = MC_GET_LONG(&uiRoadInfo);
		fwrite(&uiRoadInfo, sizeof(uiRoadInfo), 1, fp1);

		if (p->data.uiFlag != 0) {
			fwrite(&(p->data.acName), p->data.usSize - 12, 1, fp1);
		}
		p = p->next;
	}*/

	while (p != NULL) {
		msg.usSize = MC_GET_SHORT(&(p->data.usSize));
		fwrite(&msg.usSize, sizeof(msg.usSize),1,fp);

		msg.uiLinkId = MC_GET_LONG(&(p->data.uiLinkId));
		fwrite(&msg.uiLinkId, sizeof(msg.uiLinkId), 1, fp);

		msg.usNameSize = MC_GET_SHORT(&(p->data.usNameSize));
		fwrite(&msg.usNameSize, sizeof(msg.usNameSize), 1, fp);
		
		uiRoadInfo = (p->data.uiClass) | (p->data.uiBranch << 4) | (p->data.uiFlag << 7);
		uiRoadInfo = MC_GET_LONG(&uiRoadInfo);
		fwrite(&uiRoadInfo, sizeof(uiRoadInfo), 1, fp);

		if (p->data.uiFlag != 0) {
			fwrite(&(p->data.acName), p->data.usSize - 12, 1, fp);
		}
		p = p->next;
	}
	printf("已更新文件。。。。。。。。。。。。\n");
	fclose(fp);
	fclose(fp1);
}




