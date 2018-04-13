#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "General_Database.h"


int main(){



    printf("\nPart1 INPUT: \n");
    SCHEMA_LIST  lst = Build_Database_From_File("Input.txt");
    printf("%s\n",lst->head->Schema_Name);
    TUPLE_Print_Table(lst->head->Table);
    SCHEMA schema = lst->head;
    while(schema != NULL){
        printf("%s_Table: \n",schema->Table->Relation_Name);
        TUPLE_Print_Table(schema->Table);
        schema = schema->next;
    }

    printf("\n\n\nPart1 OUTPUT: \n");
    Write_DATABASE_to_File("Output.txt",lst);
    printf("Successfully Write Database to Output.txt\n");

    printf("\nPart1 3a: \n");
    printf("lookup((“CS101”, 12345, ∗), Course-StudentId-Grade) \n");
    char input[] ="CSG, CS101, 12345, *";
    TUPLE tuple = TUPLE_lookup(input,lst);
    while(tuple != NULL){
        TUPLE_print(tuple);
        printf("\n");
        tuple = tuple->next;
    }

    printf("\nPart1 3b: \n");
    printf("lookup((“CS205”, CS120), Course-Prerequisite) \n");
    char input1[] ="CP, CS205, CS120";
    tuple = TUPLE_lookup(input1,lst);
    if(tuple == NULL)
        printf("No such Tuple in databse");
    while(tuple != NULL){
        TUPLE_print(tuple);
        printf("\n");
        tuple = tuple->next;
    }

    printf("\nPart1 3c \n");
    printf("Delete((“CS101”, *), Course-Room) \n");
    char input2[] ="CR, CS101, *";
    TUPLE_delete(input2,lst);
    SCHEMA schema1 = lst->head;

    while(schema1 != NULL){
        if(strcmp(schema1->Schema_Name,"CR") == 0){
            printf("%s_Table: \n",schema1->Table->Relation_Name);
            TUPLE_Print_Table(schema1->Table);
            break;
        }
        schema1= schema1 ->next;

    }


    printf("\nPart1 3d \n");

    printf("insert((“CS205”, \"CS120\"), Course-Room) \n");
    char input3[] ="CP, CS205, CS120";
    tuple = Read_TUPLE_From_String(input3,lst);
    TUPLE_TABLE t = Search_Table(input3,lst);
    TUPLE_Insert(t, tuple);
    SCHEMA schema2 = lst->head;

    while(schema2 != NULL){
        if(strcmp(schema2->Schema_Name,"CP") == 0){
            printf("%s_Table: \n",schema2->Table->Relation_Name);
            TUPLE_Print_Table(schema2->Table);
            break;
        }
        schema2= schema2 ->next;

    }

    printf("\nPart1 3e \n");

    printf("insert((“CS205”, \"CS120\"), Course-Room) \n");
    char input4[] ="CP, CS205, CS101";
    tuple = Read_TUPLE_From_String(input4,lst);
    t = Search_Table(input4,lst);
    TUPLE_Insert(t, tuple);
    SCHEMA schema3 = lst->head;

    while(schema3 != NULL){
        if(strcmp(schema3->Schema_Name,"CP") == 0){
            printf("%s_Table: \n",schema3->Table->Relation_Name);
            TUPLE_Print_Table(schema3->Table);
            break;
        }
        schema3= schema3 ->next;

    }

    printf("\n");
    printf("\n");
    printf("Part2\n");
    printf("1.Answer the query “What grade did StudentName get in CourseName?”\n");
    Get_Student_Course_Grade("P._Patty","EE200",lst);

//    printf("\n");
//    printf("\n");
//    printf("2.“Where is StudentName at Time on Day?”\n");
//    TUPLE n = Where_is_StudentName_at_Time_on_Day("L._Van_Pelt",lst);
//    while(n != NULL){
//        TUPLE_print(n);
//        printf("\n");
//        n=n->next;
//    }








    printf("\n\nPart3\n");
    printf("\n");
    printf("\n");
    printf("SELECTION\n");
    TUPLE list = SELECTION("CSG","Course","CS101",lst);
    TUPLE node = list;
    while(node != NULL){
        TUPLE_print(node);
        printf("\n");
        node=node->next;
    }


    printf("\n");
    printf("\n");
    printf("\n");
    printf("PROJECTION\n");
    TUPLE node1 = PROJECTION("StudentId",list);
    while(node1 != NULL){
        TUPLE_print(node1);
        printf("\n");
        node1=node1->next;
    }

    printf("\n");
    printf("\n");
    printf("\n");
    printf("JOIN\n");
    TUPLE_TABLE table = JOIN("CR","CDH","Course","Course","CRDH",lst);
    TUPLE_Print_Table(table);






}
