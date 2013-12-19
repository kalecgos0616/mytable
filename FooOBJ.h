typedef struct fooobj * FooOBJ;
FooOBJ newFooOBJ();
void setFooNumber(FooOBJ,int);
void setFooString(FooOBJ,char *); /* make comments about copy or not here */
void dumpFooState(FooOBJ);      /* dumps debug contents of FooOBJ to stdout */
void deleteFooOBJ(FooOBJ);
void listData(FooOBJ);
void connetDatabase(FooOBJ, char *, char *);
void addData(FooOBJ, char * message);
void delData(FooOBJ, int note_id);
void updateData(FooOBJ, int note_id, char * message);
void closeConnection(FooOBJ);