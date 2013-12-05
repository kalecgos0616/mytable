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

#define CONFIG_PATH "test.conf"
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

int main(int argc, char * *argv) {
	//parameter
	char * table = argv[1];
	char * action = argv[2];
	char * message = argv[3];

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

	printf("username = %s\n", username);
	printf("password = %s\n", password);

	// DB
	MYSQL * conn;
	MYSQL_RES * res;
	MYSQL_ROW row;
	/* Change me */
	char * server = "localhost";
	char * user = username;
	//char * password = password;
	char * database = "mytable";

	printf("user = %s\n", user);
	printf("password = %s\n", password);

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
		printf("sql_buf:%s\n", sql_buf);

		if (mysql_query(conn, sql_buf)) {
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}
	}

	/* close connection */
	mysql_free_result(res);
	mysql_close(conn);

	return 0;
}


