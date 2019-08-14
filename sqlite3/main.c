#include <stdio.h>
#include <time.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>

//查询的回调函数声明
int select_callback(void * data, int col_count, char ** col_values, char ** col_Name);
char c[500] = "";
int main(int argc, char * argv[])
{ 
	const char * sSQL1 = "create table dic(id integer PRIMARY KEY AUTOINCREMENT, name varchar(20) NOT NULL, remarks varchar(255));";
	const char * sSQL2 = "create table user(uid integer PRIMARY KEY AUTOINCREMENT, uname varchar(20) NOT NULL, password varchar(255),state integer);";
	const char * sSQL3 = "create table history(id integer PRIMARY KEY AUTOINCREMENT,uname varchar(20) NOT NULL,timestamp varchar(20) NOT NULL, word varchar(10))";
	char * pErrMsg = NULL;
	int result = 0;
	char * sql;
	
	// 连接数据库
	sqlite3 * db = 0;
	int ret = sqlite3_open("./dict.db", &db);
	if( ret != SQLITE_OK ){
		fprintf(stderr, "无法打开数据库: %s", sqlite3_errmsg(db));
		return(1);
	}
	printf("数据库连接成功!\n");

	// 执行建表SQL
	sqlite3_exec( db, sSQL1, 0, 0, &pErrMsg );
	sqlite3_exec( db, sSQL2, 0, 0, &pErrMsg );
	sqlite3_exec( db, sSQL3, 0, 0, &pErrMsg );
	if( ret!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", pErrMsg);
		sqlite3_free(pErrMsg);
		return 1;
	}
	printf("建表成功！\n");


	char *a;
	char *a1;
	char b[200];
	const char* split = " ";
	int flag;

	FILE* fr = fopen("./dict.txt", "r");
	if (NULL == fr)
	{
		perror("open fail:");
		exit(-1);
	}
	
	while (fscanf(fr, "%[^\n]%*c\n", b) != EOF)
	{
		flag = 0;
		a = strtok(b, split);
		while (a != NULL)
		{
			if (flag == 0)
				a1 = a;
			a = strtok(NULL, split);
			flag++;
			if (flag >= 1) {
				if (a == NULL) break;
				//sprintf(c, "%s %s", c, a);
				strcpy(c, strcat(strcat(c, " "), a));
			}
		}
		//printf("%s\n", c);
		//sqlite3_exec( db, ("insert into dic([id],[name],[remarks]) values(null,%q,%q)",a,c), 0, 0, &pErrMsg);
		sql=sqlite3_mprintf("INSERT INTO dic VALUES(NULL , '%q' , '%q')", a1,c);
		sqlite3_exec( db , sql , 0 , 0 , &pErrMsg);
		memset(c,0,sizeof(c));
	}

	int nrow = 0;
	int ncolumn = 0;
	char** azResult = NULL;
	int i;
	sql="select *from dic";
	sqlite3_get_table( db , sql , &azResult , &nrow , &ncolumn , &pErrMsg );
	printf("nrow=%d ncolumn=%d\n",nrow,ncolumn);
	printf("the result is:\n");
	for(i=0;i<(nrow+1)*ncolumn;i++)
	{
		printf("azResult[%d]=%s\n",i,azResult[i]);
	}
	// 关闭数据库
	sqlite3_close(db);
	db = 0;
	printf("数据库关闭成功!\n");
	return 0;
}
