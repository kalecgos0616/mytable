mytable
=======

compile:
gcc -o mytable $(mysql_config --cflags) mytable.c $(mysql_config --libs)

useage:
./mytable note add2 your_message
