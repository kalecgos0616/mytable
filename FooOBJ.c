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
	char *database;
	char *table;
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

FooOBJ newFooOBJ(struct DatabaseConfig DatabaseConfigOBJ){
	FooOBJ foo=(FooOBJ)malloc(sizeof(struct fooobj));
	bzero(foo, sizeof(struct fooobj));
	foo->server = DatabaseConfigOBJ.server;
	foo->database = DatabaseConfigOBJ.database;
	foo->table = DatabaseConfigOBJ.table;
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
	printf("value of private foo->privateint==%d\n", foo->privateint);
	printf("value of server foo->server==%s\n", foo->server);
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

void listData(FooOBJ foo){

	char * sql = "SELECT * FROM %s";
	char sql_buf[256];
	snprintf(sql_buf, sizeof sql_buf, sql, foo->table);			
		
	printf("sql_buf:%s\n", sql_buf);
	if (mysql_query(foo->conn, sql_buf)) {
		fprintf(stderr, "%s\n", mysql_error(foo->conn));
		exit(1);
	}

	foo->res = mysql_use_result(foo->conn);

	/* output table name */
	printf("OOP MySQL Tables in mysql database:\n");
	while ((foo->row = mysql_fetch_row(foo->res)) != NULL){
		printf("id:%s, message:%s, new_time:%s, update_time:%s \n", foo->row[0], foo->row[1], foo->row[2], foo->row[3]);
	}
		
}

void connetDatabase(FooOBJ foo, char * username, char * password){
	/* Change me */
	printf("connetDatabase0 TEST server:%s, database:%s\n", foo->server, foo->database);
	
	foo->conn = mysql_init(NULL);
	
	/* Connect to database */
	if (!mysql_real_connect(foo->conn, "localhost", username, password, "mytable", 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(foo->conn));
		exit(1);
	}

}

void addData(FooOBJ foo, char * message){
	char * sql = "INSERT INTO `mytable`.`%s` (\
			  `id` ,\
			  `message` ,\
			  `new_time` ,\
			  `update_time`\
			  )\
			  VALUES (\
					  NULL , '%s', NOW( ), NOW( )\
				 );";
	char sql_buf[256];
	snprintf(sql_buf, sizeof sql_buf, sql, foo->table, message);			
	printf("OOP addData() sql_buf:%s\n", sql_buf);

	if (mysql_query(foo->conn, sql_buf)) {
		fprintf(stderr, "%s\n", mysql_error(foo->conn));
		exit(1);
	}
}

void delData(FooOBJ foo, int note_id){
	//char * note_id = argv[3];
	//int note_id = atoi(argv[3]);
	printf("note_id = %d\n", note_id);
	/* send SQL query */
	char * sql = "DELETE FROM `%s` WHERE `id` = '%d';";
	char sql_buf[256];
	snprintf(sql_buf, sizeof sql_buf, sql, foo->table, note_id);			
	printf("OOP delData() sql_buf:%s\n", sql_buf);

	if (mysql_query(foo->conn, sql_buf)) {
		fprintf(stderr, "%s\n", mysql_error(foo->conn));
		exit(1);
	}
}

void updateData(FooOBJ foo, int note_id, char * message){
	printf("note_id = %d, message = %s\n", note_id, message);
	/* send SQL query */
	char * sql = "UPDATE  `%s` SET  `message` =  '%s' WHERE  `id` ='%d';";
	char sql_buf[256];
	snprintf(sql_buf, sizeof sql_buf, sql, foo->table, message, note_id);			
	printf("OOP updateData() sql_buf:%s\n", sql_buf);

	if (mysql_query(foo->conn, sql_buf)) {
		fprintf(stderr, "%s\n", mysql_error(foo->conn));
		exit(1);
	}
}
void closeConnection(FooOBJ foo){
	mysql_free_result(foo->res);
	mysql_close(foo->conn);
}