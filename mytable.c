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
#include <getopt.h>

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

/* Flag set by ‘--verbose’. */
static int verbose_flag;

int my_get_opt(int argc, char * *argv){
	int c;

	while (1)
	{
		static struct option long_options[] =
		{
			/* These options set a flag. */
			{"verbose", no_argument,       &verbose_flag, 1},
			{"brief",   no_argument,       &verbose_flag, 0},
			/* These options don't set a flag.
			   We distinguish them by their indices. */
			{"add",     no_argument,       0, 'a'},
			{"append",  no_argument,       0, 'b'},
			{"delete",  required_argument, 0, 'd'},
			{"create",  required_argument, 0, 'c'},
			{"file",    required_argument, 0, 'f'},
			{0, 0, 0, 0}
		};
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long (argc, argv, "abc:d:f:",
				long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c)
		{
			case 0:
				/* If this option set a flag, do nothing else now. */
				if (long_options[option_index].flag != 0)
					break;
				printf ("option %s", long_options[option_index].name);
				if (optarg)
					printf (" with arg %s", optarg);
				printf ("\n");
				break;

			case 'a':
				puts ("option A -a\n");
				break;

			case 'b':
				puts ("option B -b\n");
				break;

			case 'c':
				printf ("option -c with value `%s'\n", optarg);
				break;

			case 'd':
				printf ("option -d with value `%s'\n", optarg);
				break;

			case 'f':
				printf ("option -f with value `%s'\n", optarg);
				break;

			case '?':
				/* getopt_long already printed an error message. */
				break;

			default:
				abort ();
		}
	}

	/* Instead of reporting ‘--verbose’
	   and ‘--brief’ as they are encountered,
	   we report the final status resulting from them. */
	if (verbose_flag)
		puts ("verbose flag is set");

	/* Print any remaining command line arguments (not options). */
	/*if (optind < argc)
	{
		printf ("non-option ARGV-elements: ");
		while (optind < argc)
			printf ("%s, optind:%d ", argv[optind++], optind);
		putchar ('\n');
	}*/
	return optind;
}

int main(int argc, char * *argv) {
	int index = my_get_opt(argc, argv);
	

	//parameter
	char * table = argv[index];
	char * action = argv[index+1];
	
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
		addData(fobj, argv[index+2]);
	}
	if( strcmp(action,"del") == 0 ){
		delData(fobj, atoi(argv[index+2]));		
	}
	if( strcmp(action,"update") == 0 ){
		updateData(fobj, atoi(argv[index+2]), argv[index+3]);
	}
	
	closeConnection(fobj);
	
	deleteFooOBJ(fobj);

	return 0;
}


