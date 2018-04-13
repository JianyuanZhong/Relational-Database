#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "General_Database.h"


TUPLE SELECTION(char* Schema_Name, char* Attribute_Name, char* component, SCHEMA_LIST lst){

    TUPLE_TABLE t = Search_Table(Schema_Name,lst);

    SCHEMA s = lst->head;
    TUPLE tuple = NULL;
    while(s != NULL){
        if(strcmp(s->Schema_Name, Schema_Name) == 0){
            tuple = init_TUPLE_From_String(s->Discription);
            TUPLE_Set_Key(s->Key,tuple);
            break;
        }
        s = s->next;
    }

    ATTRIBUTE a = tuple->A_Head;

    while( a!=NULL){

        if(strcmp(a->Attribute_Name, Attribute_Name) == 0)
            a->data = component;
        else
            a->data = "*";

        a = a->next;
    }

    TUPLE curr;
    TUPLE head = NULL;
    TUPLE tail = head;

    int HashCode;
    bool isKey = false;
    if(tuple->Key != NULL) {
        if (strcmp(tuple->Key->data, "*") != 0) {
            HashCode = TUPLE_hash(tuple);
            isKey = true;
        }
    }

    int i;
    for (i = 0; i < 1009; i++) {

        if(isKey) i = HashCode;
        curr = t->Table[i];
        while (curr != NULL) {

            if (TUPLE_LOOKUP_Compare(tuple,curr)) {


                if (head == NULL) {
                    head = TUPLE_copy(curr);
                    tail = head;

                } else {

                    tail->next = TUPLE_copy(curr);
                    tail = tail->next;
                }

            }
            curr = curr->next;
        }

        if(isKey) break;
    }


    return head;

}

ATTRIBUTE FIND_ATTRIBUTE(char* Attribute_Name,TUPLE tuple){
    ATTRIBUTE attribute = tuple->A_Head;
    while(attribute != NULL){
        if(strcmp(attribute->Attribute_Name,Attribute_Name) == 0)
            return attribute;
        attribute = attribute->next;
    }
    return NULL;
}

TUPLE PROJECTION( char* Attribute_Name, TUPLE T_List){

    TUPLE node = T_List;

    TUPLE head = NULL;
    TUPLE curr ;
    ATTRIBUTE attribute;
    while(node != NULL){

        attribute = FIND_ATTRIBUTE(Attribute_Name,node);

        if(head == NULL){
            head = new_TUPLE(Attribute_Name);
            Add_ATTRIBUTE(head,Attribute_Name,attribute->data);

            curr = head;

        } else{
            curr->next = new_TUPLE(Attribute_Name);
            Add_ATTRIBUTE(curr->next,Attribute_Name,attribute->data);
            curr = curr->next;
        }



        node = node->next;
    }

    return head;
}

TUPLE_TABLE JOIN(char* Schema_A, char* Schema_B, char* Attribute_A, char* Attribute_B, char* ResultName, SCHEMA_LIST list){
    TUPLE_TABLE t_A = Search_Table(Schema_A, list);
    TUPLE_TABLE t_B = Search_Table(Schema_B, list);
    TUPLE_TABLE result = TUPLE_Table_new(ResultName);

    TUPLE curr_A;


    TUPLE curr_B;


    //TUPLE_Print_Table(t_A);
    //TUPLE_Print_Table(t_B);

    int i;
    for(i = 0; i< 1009; i++){
        curr_A = t_A ->Table[i];



        while(curr_A != NULL){

            ATTRIBUTE a = FIND_ATTRIBUTE(Attribute_A,curr_A);

            int j;
            for(j = 0; j<1009; j++) {
                curr_B = t_B->Table[i];
                while (curr_B != NULL) {

                    ATTRIBUTE b = FIND_ATTRIBUTE(Attribute_B,curr_B);




                    if (strcmp(a->data, b->data) == 0) {

                        TUPLE tuple = new_TUPLE(ResultName);

                        ATTRIBUTE a = curr_A->A_Head;
                        while(a != NULL){
                            Add_ATTRIBUTE(tuple,a->Attribute_Name,a->data);
                            a= a->next;
                        }
                        ATTRIBUTE b = curr_B->A_Head;
                        while(b != NULL){
                            if(strcmp(b->Attribute_Name, Attribute_B) != 0 )
                                Add_ATTRIBUTE(tuple,b->Attribute_Name,b->data);
                            b= b->next;
                        }
                        TUPLE_Set_Key(curr_A->Key->Attribute_Name,tuple);
                        TUPLE_Insert(result,tuple);

                    }

                    curr_B = curr_B->next;
                }
            }
            curr_A = curr_A ->next;
        }
    }

    return result;

}

char* remove_newline_sing(char* line){
    int i;
    for(i=0;line[i]!='\0';i++)
    {
        if(line[i]=='\n')
        {
            line[i]='\0';
        }
    }
    return line;
}


SCHEMA_LIST Build_Database_From_File( char* File_Name){
    FILE *stream;
    char *line = NULL;
    char *Key = NULL;
    size_t len = 0;
    ssize_t read ;

    stream = fopen(File_Name, "r");
    if (stream == NULL)
        exit(EXIT_FAILURE);

    SCHEMA_LIST lst = new_SCHEMA_LIST();

    read = getline(&line,&len,stream);
    while (read  != -1) {
//        printf("Retrieved line of length %zu :\n", read);
//        printf("%s", line);

        if(strcmp(line, "Schemas\n") == 0){
            while(strcmp(line, "End\n") != 0){

                getline(&line, &len, stream);
                getline(&Key, &len,stream);

                if(strcmp(line,"End\n")!= 0) {
                    printf("!%s", line);
                    printf("!%s", Key);

                    Add_SCHEMA(remove_newline_sing(line), remove_newline_sing(Key), lst);
                }
            }
        } else if ( strcmp(line, "Insert\n") == 0){
            while(strcmp(line, "End\n") != 0){

                getline(&line, &len, stream);


                if(strcmp(line,"End\n")!= 0 && strcmp(line, "\n") != 0 && strcmp(line, " ") != 0 && strcmp(line, "") != 0) {
                    printf("!Insert: %s", line);
                    line = remove_newline_sing(line);
                    TUPLE tuple = Read_TUPLE_From_String(line,lst);
                    TUPLE_TABLE t = Search_Table(line,lst);

                    TUPLE_Insert(t,tuple);
                }

                //TUPLE_Print_Table(Search_Table(line,lst));
                //printf("\n");

            }
        }



        read = getline(&line,&len,stream);

    }

    return lst;

//    free(line);
//    fclose(stream);



}


void Get_Student_Course_Grade(char* Name, char* Course, SCHEMA_LIST list){


    Add_SCHEMA("SNAPCG","Name",list);
    list->tail->Table = JOIN("SNAP","CSG","StudentId","StudentId","SNAPCG",list);
    TUPLE head = SELECTION("SNAPCG","Name",Name,list);

    TUPLE node = head;

    while(node != NULL){

        ATTRIBUTE a = FIND_ATTRIBUTE("Course",node);
        ATTRIBUTE name = FIND_ATTRIBUTE("Name",node);



        if(strcmp(a->data,Course) == 0  && strcmp(name->data,Name) ==0 ){
            ATTRIBUTE grade = FIND_ATTRIBUTE("Grade",node);
            printf("%s's Grade on %s is: %s.\n",Name,Course,grade->data);
        }

        node= node->next;

    }

    SCHEMA schema = list->head;
    while(schema->next == list->tail){
        list->tail =NULL;
        list->tail = schema;
    }




}

//TUPLE Where_is_StudentName_at_Time_on_Day(char* Name, SCHEMA_LIST list){
//
//    char* input = malloc(255);
//
//    strcpy(input,"SNAP, *, ");
//    strcat(input,Name);
//    strcat(input,", *, *");
//
//    printf("%s\n",input);
//
//
//
//    TUPLE head = TUPLE_lookup(input,list);
//
//
//    TUPLE node2=NULL;
//    while(head != NULL) {
////        TUPLE_print(head);
////        printf("\n");
//
//
//        input = malloc(100);
//        strcpy(input, "CSG, *, ");
//        ATTRIBUTE sid = FIND_ATTRIBUTE("StudentId",head);
//        strcat(input, sid->data);
//        strcat(input, ", *");
//
//
//        printf("%s\n", input);
//
//        TUPLE node1 = TUPLE_lookup(input,list);
//        free(input);
//
//
//        while(node1 != NULL){
//
//            char* input1 = malloc(100);
//            strcpy(input1, "CDH, ");
//            ATTRIBUTE course = FIND_ATTRIBUTE("Course",node1);
//            strcat(input1, course->data);
//            strcat(input1, ", *, *");
//
//            printf("%s\n", input1);
//            node2 = TUPLE_lookup(input1,list);
//
//            while(node2 != NULL){
//                TUPLE_print(node2);
//                printf("\n");
//            }
//
//
//            //free(input1);
//            node1= node1->next;
//
//        }
//
//
//        head = head->next;
//
//    }
//
//
//    return node2=NULL;
//
//
//}


void Write_TUPLE(FILE *File_Name, TUPLE tuple){
        //FILE* fptr = fopen(File_Name,"w");
        fprintf(File_Name,"%s, ",tuple->Relation_Name);
        ATTRIBUTE a = tuple ->A_Head;
        while(a != NULL){
            if(a == tuple->A_Tail) {
                fprintf(File_Name,"%s, ",a->data);
                break;
            } else{
                fprintf(File_Name, "%s, ",a->data);
            }
            a = a->next;
        }
        fprintf(File_Name,"\n");
    //fclose(fptr);

}

void Write_Table(FILE* File_Name, TUPLE_TABLE t){
    //FILE* fptr = fopen(File_Name,"w");
    int i;
    TUPLE bucket = NULL;
    for (i = 0; i < 1009; i++) {
        if (t->Table[i] != NULL) {
            //printf("Index: %d ", i);
            if (t->Table[i]->next == NULL){
                Write_TUPLE(File_Name,t->Table[i]);
            }
                //TUPLE_print(t->Table[i]);

            else {
                bucket = t->Table[i];
                while (bucket != NULL) {
                    Write_TUPLE(File_Name,bucket);
                    //TUPLE_print(bucket);
                    bucket = bucket->next;

                }
            }
            //printf("\n");
        }
    }
    //fclose(fptr);
}

void Write_DATABASE_to_File(char* File_Name,SCHEMA_LIST list){
    FILE* fptr = fopen(File_Name,"w");
    fprintf(fptr,"Schemas\n");
    SCHEMA schema = list->head;

    while(schema != NULL){
        fprintf(fptr,"%s\n",schema->Discription);
        schema = schema->next;
    }

    fprintf(fptr,"End\n\n");
    fprintf(fptr,"Insert\n");
    schema = list->head;
    TUPLE_TABLE t;
    while(schema != NULL){
        t = schema->Table;
        Write_Table(fptr,t);
        schema = schema->next;
    }
    fprintf(fptr,"End");


    fclose(fptr);
}


//int main(){
//
//    SCHEMA_LIST  lst = Build_Database_From_File("Input.txt");
//    printf("%s\n",lst->head->Schema_Name);
//    TUPLE_Print_Table(lst->head->Table);
//    SCHEMA schema = lst->head;
//    while(schema != NULL){
//        printf("%s_Table: \n",schema->Table->Relation_Name);
//        TUPLE_Print_Table(schema->Table);
//        schema = schema->next;
//    }
//    printf("\n");
//    printf("\n");
//    printf("\n");
//    printf("SELECTION\n");
//    TUPLE list = SELECTION("CSG","Course","CS101",lst);
//    TUPLE node = list;
//    while(node != NULL){
//        TUPLE_print(node);
//        printf("\n");
//        node=node->next;
//    }
//
//
//    printf("\n");
//    printf("\n");
//    printf("\n");
//    printf("PROJECTION\n");
//    TUPLE node1 = PROJECTION("StudentId",list);
//    while(node1 != NULL){
//        TUPLE_print(node1);
//        printf("\n");
//        node1=node1->next;
//    }
//
//    printf("\n");
//    printf("\n");
//    printf("\n");
//    printf("JOIN\n");
//    TUPLE_TABLE table = JOIN("CR","CDH","Course","Course",lst);
//    TUPLE_Print_Table(table);
//
//}
