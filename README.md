mytable
=======

compile:
gcc -o mytable $(mysql_config --cflags) mytable.c $(mysql_config --libs)

useage:
	add
		./mytable note add your_message
	list
		./mytable note list