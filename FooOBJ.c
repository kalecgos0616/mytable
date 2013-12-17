#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <mysql/mysql.h>
#include <stdlib.h>
#include "FooOBJ.h"

struct fooobj {
	int privateint;
	char *privateString;
	char *server;
	//char *database = "mytable";
	MYSQL * conn;
	MYSQL_RES * res;
	MYSQL_ROW row;
	/* Depending on your preferences, you
	 * may prefer privateString to be a char buffer[],
	 * OR malloc it and free on delete.
	 */
};

/* Any "class static" vars simply get declared as
static int staticvar;
 * or whatever you like here.
 * Whereas instance variables go in the struct above.
 */

FooOBJ newFooOBJ(){
	FooOBJ foo=(FooOBJ)malloc(sizeof(struct fooobj));
	bzero(foo, sizeof(struct fooobj));
	foo->server = "localhost";
	printf("server:%s\n", foo->server);
	return foo;
}

void setFooNumber(FooOBJ foo,int num){
	if(foo==NULL) return;  /* you may chose to debugprint something
				*instead
				*/
	foo->privateint=num;
}

void setFooString(FooOBJ foo,char *string){
	/* If you malloced the string, you'll want to free() it here!!*/
	if(foo==NULL) return;
	foo->privateString=string;
}
	

void dumpFooState(FooOBJ foo){
	if(foo==NULL) return;
	//printf("value of private int==%d\n", foo->privateint);
	printf("value of server string==%s\n", foo->server);
	//printf("value of private string==");
	if(foo->privateString == NULL){
		//puts("(NULL)");
	} else {
		//printf("%s\n", foo->privateString);
	}

}

void deleteFooOBJ(FooOBJ foo){
	/* dont forget to free privateString if you malloced it here!!*/
	free(foo);
}

void listData2(FooOBJ foo){
	printf("listData2:%s\n", foo->privateString);
}

void connetDatabase(FooOBJ foo, MYSQL * conn, char * username, char * password){
	/* Change me */
	//char * server = "localhost";
	char * database = "mytable";
	printf("connetDatabase0 TEST database:%s, string:%d\n", foo->server, foo->privateint);
	dumpFooState(foo);

	foo->conn = conn;
	dumpFooState(foo);
	printf("connetDatabase1 TEST database:%s, string:%d\n", foo->server, foo->privateint);

}