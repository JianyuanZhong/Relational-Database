#include <stdio.h>
#include <stdlib.h>
#include <string.h>






typedef struct attribute* ATTRIBUTE;
struct attribute{
    char* Attribute_Name;
    char* data;
    ATTRIBUTE prev;
    ATTRIBUTE next;
};

typedef struct Tuple* TUPLE;
struct Tuple{
    char* Relation_Name;
    ATTRIBUTE Key;
    ATTRIBUTE A_Head;
    ATTRIBUTE A_Tail;
    TUPLE next;
};

typedef struct TUPLE_hashTable* TUPLE_TABLE;
struct TUPLE_hashTable{
    TUPLE * Table;
    char* Relation_Name;
    int size;
};

typedef struct Schema* SCHEMA;
struct Schema{
    char* Schema_Name;
    char* Discription;
    char* Key;
    TUPLE_TABLE Table;
    SCHEMA next;
};

typedef struct SchemaList* SCHEMA_LIST;
struct SchemaList{
    SCHEMA head;
    SCHEMA tail;

};

extern ATTRIBUTE new_Attribute(char* name, char* data);
extern TUPLE new_TUPLE(char* R);
extern TUPLE init_TUPLE_From_String(char* input);
extern TUPLE Read_TUPLE_From_String(char* input, SCHEMA_LIST lst);
extern void Add_ATTRIBUTE(TUPLE tuple, char* name, char* data);
extern TUPLE_TABLE TUPLE_Table_new(char* R);
extern void TUPLE_Insert(TUPLE_TABLE t, TUPLE tuple);
extern TUPLE TUPLE_copy(TUPLE tuple);
extern void TUPLE_print(TUPLE tuple);
extern bool TUPLE_LOOKUP_Compare(TUPLE in, TUPLE tuple);
TUPLE_TABLE Search_Table(char* input, SCHEMA_LIST lst);
extern TUPLE TUPLE_lookup(char* input,SCHEMA_LIST lst);
extern void TUPLE_free_TUPLE(TUPLE tuple);
extern void TUPLE_delete(char* input, SCHEMA_LIST lst);
extern void TUPLE_print(TUPLE tuple);
extern void TUPLE_Print_Table(TUPLE_TABLE t);
extern void TUPLE_Set_Key(char* Key_Name, TUPLE tuple);
extern int TUPLE_hash(TUPLE tuple);
extern SCHEMA_LIST new_SCHEMA_LIST();
extern SCHEMA new_SCHEMA(char* input, char* Key);
extern void Add_SCHEMA(char* input, char* Key,SCHEMA_LIST lst );