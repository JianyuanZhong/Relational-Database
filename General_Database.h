#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TUPLE.h"

extern TUPLE SELECTION(char* Schema_Name, char* Attribute_Name, char* component, SCHEMA_LIST lst);
extern TUPLE PROJECTION( char* Attribute_Name, TUPLE T_List);
extern TUPLE_TABLE JOIN(char* Schema_A, char* Schema_B, char* Attribute_A, char* Attribute_B, char* ResultName,SCHEMA_LIST list);
extern SCHEMA_LIST Build_Database_From_File( char* File_Name);
extern void Write_DATABASE_to_File(char* File_Name,SCHEMA_LIST list);
extern void Get_Student_Course_Grade(char* Name, char* Course, SCHEMA_LIST list);
//TUPLE Where_is_StudentName_at_Time_on_Day(char* Name, SCHEMA_LIST list);