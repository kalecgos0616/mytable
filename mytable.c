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

void listData(MYSQL * conn, MYSQL_RES * res, MYSQL_ROW row){
	/* send SQL query */
	char * sql_buf = "SELECT * FROM note";
	if (mysql_query(conn, sql_buf)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	printf("sql_buf:%s\n", sql_buf);

	res = mysql_use_result(conn);

	/* output table name */
	printf("function MySQL Tables in mysql database:\n");
	while ((row = mysql_fetch_row(res)) != NULL)
		printf("id:%s, message:%s, new_time:%s, update_time:%s \n", row[0], row[1], row[2], row[3]);
}

void addData(char * message, MYSQL * conn){
	/* send SQL query */
	char * sql = "INSERT INTO `mytable`.`note` (\
			  `id` ,\
			  `message` ,\
			  `new_time` ,\
			  `update_time`\
			  )\
			  VALUES (\
					  NULL , '%s', NOW( ), NOW( )\
				 );";
	char sql_buf[256];
	snprintf(sql_buf, sizeof sql_buf, sql, message);			
	printf("function addData sql_buf:%s\n", sql_buf);

	if (mysql_query(conn, sql_buf)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
}

void delData(int note_id, MYSQL * conn){
	//char * note_id = argv[3];
	//int note_id = atoi(argv[3]);
	printf("note_id = %d\n", note_id);
	/* send SQL query */
	char * sql = "DELETE FROM `note` WHERE `id` = '%d';";
	char sql_buf[256];
	snprintf(sql_buf, sizeof sql_buf, sql, note_id);			
	printf("function delData sql_buf:%s\n", sql_buf);

	if (mysql_query(conn, sql_buf)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
}

void updateData(int note_id, char * message, MYSQL * conn){
	//char * note_id = argv[3];
	//int    note_id = atoi(argv[3]);
	//char * message = argv[4];
	printf("note_id = %d, message = %s\n", note_id, message);
	/* send SQL query */
	char * sql = "UPDATE  `note` SET  `message` =  '%s' WHERE  `id` ='%d';";
	char sql_buf[256];
	snprintf(sql_buf, sizeof sql_buf, sql, message, note_id);			
	printf("function updateData sql_buf:%s\n", sql_buf);

	if (mysql_query(conn, sql_buf)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
}

int main(int argc, char * *argv) {
	//parameter
	char * table = argv[1];
	char * action = argv[2];
	//char * message = argv[3];

	//	printf("table = %s\n", table);
	printf("action = %s\n", action);
	//	printf("message = %s\n", message);
	memset(username,0,SIZE);
	memset(password,0,SIZE);

	/*read config*/
	//	readConfig();
	if(!readConfig()){
		fprintf(stderr,"read config fail!");
		return 1;
	}

	//printf("username = %s\n", username);
	//printf("password = %s\n", password);

	// DB
	MYSQL * conn;
	MYSQL_RES * res;
	MYSQL_ROW row;
	/* Change me */
	char * server = "localhost";
	char * user = username;
	//char * password = password;
	char * database = "mytable";

	//printf("user = %s\n", user);
	//printf("password = %s\n", password);

	//check
	conn = mysql_init(NULL);

	/* Connect to database */
	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

	/* send SQL query */
	if (mysql_query(conn, "show tables")) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

	res = mysql_use_result(conn);

	/* output table name */
	printf("MySQL Tables in mysql database:\n");
	while ((row = mysql_fetch_row(res)) != NULL) printf("%s \n", row[0]);

	// insert
	if( strcmp(action,"add") == 0 ){
		addData(argv[3], conn);
	}
	
	//list
	if( strcmp(action,"list") == 0 ){
		listData(conn, res, row);
	}
	
	// del
	if( strcmp(action,"del") == 0 ){
		delData(atoi(argv[3]), conn);		
	}
	
	// update
	if( strcmp(action,"update") == 0 ){
		updateData(atoi(argv[3]), argv[4], conn);
	}

	/* close connection */
	mysql_free_result(res);
	mysql_close(conn);

	return 0;
}


