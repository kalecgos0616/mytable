/* 
 * See url for more info:
 * http://www.cyberciti.biz/tips/linux-unix-connect-mysql-c-api-program.html
 *
 * */
#include <mysql/mysql.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define CONFIG_PATH "my.conf"
#define SIZE         256

/*config vars*/
char username[SIZE];
char password[SIZE];

bool readConfig(){
	char name[SIZE];
	char value[SIZE];

	FILE *fp = fopen(CONFIG_PATH, "r");
	if (fp == NULL) { return false; }
	while(!feof(fp)){
		memset(name,0,SIZE); memset(value,0,SIZE);

		/*Read Data*/
		fscanf(fp,"%s = %s\n", name, value);

		if (!strcmp(name, "username")){
			strcpy(username, value);
		}else if (!strcmp(name, "password")){
			strcpy(password, value);
		}
	}
	fclose(fp);

	return true;
}

#include "FooOBJ.h"

void diddle(FooOBJ obj){
	/* Perform member functions on FooOBJ.
	 * If you try these functions on a different type of object,
	 * you will get a compile-time error
	 */
	setFooNumber(obj, 1);
	setFooString(obj, "somestring");

}

int main(int argc, char * *argv) {

	//parameter
	char * table = argv[1];
	char * action = argv[2];
	
	struct DatabaseConfig config;
	config.server = "localhost";
	config.database = "mytable";
	config.table = table;

	FooOBJ fobj;
	fobj=newFooOBJ(config); /* create a new object of type "FooOBJ" */
	
	memset(username,0,SIZE);
	memset(password,0,SIZE);

	/*read config*/
	if(!readConfig()){
		fprintf(stderr,"read config fail!");
		return 1;
	}
	
	
	
	/* Change me */
	char * server = "localhost";
	char * database = "mytable";

	//use oop db connection
	connetDatabase(fobj, username, password);
	if( strcmp(action,"list") == 0 ){
		listData(fobj);
	}
	if( strcmp(action,"add") == 0 ){
		addData(fobj, argv[3]);
	}
	if( strcmp(action,"del") == 0 ){
		delData(fobj, atoi(argv[3]));		
	}
	if( strcmp(action,"update") == 0 ){
		updateData(fobj, atoi(argv[3]), argv[4]);
	}
	
	closeConnection(fobj);
	
	deleteFooOBJ(fobj);

	return 0;
}


