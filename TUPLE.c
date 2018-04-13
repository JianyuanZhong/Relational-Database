#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

#include "TUPLE.h"

ATTRIBUTE new_Attribute(char* name, char* data){
    ATTRIBUTE a = (ATTRIBUTE) malloc(sizeof(struct attribute));
    if(data != NULL) {
        a->data = (char *) malloc(50);
        a->data = strcpy(a->data, data);
    }
    a->Attribute_Name = name;
    return a;
}


void Add_ATTRIBUTE(TUPLE tuple, char* name, char* data){
    ATTRIBUTE a = new_Attribute(name, data);
    a ->prev = tuple->A_Tail;

    if(tuple->A_Tail != NULL){
        tuple->A_Tail->next = a;
    }

    tuple->A_Tail = a;

    if(tuple ->A_Head == NULL)
        tuple ->A_Head =a;

}



TUPLE new_TUPLE(char* R){
    TUPLE tuple = (TUPLE)malloc(sizeof(struct Tuple));
    tuple ->Relation_Name = R;
    tuple->A_Head = NULL;
    tuple->A_Tail =tuple->A_Head;
    return tuple;
}

void TUPLE_Set_Key(char* Key_Name, TUPLE tuple){
    ATTRIBUTE a = tuple->A_Head;
    while(a != NULL){
        if (strcmp(Key_Name, a->Attribute_Name) == 0) {
            tuple->Key = a;
            break;
        }
        a = a->next;
    }
}

SCHEMA_LIST new_SCHEMA_LIST(){
    SCHEMA_LIST sl = (SCHEMA_LIST)malloc(sizeof(struct SchemaList));
    sl->head=NULL;
    sl->tail=sl->head;
    return sl;
}

SCHEMA new_SCHEMA(char* input, char* Key){
    SCHEMA shema = (SCHEMA)malloc(sizeof(struct Schema));

    char* temp = (char*)malloc(sizeof(char) * strlen(input));
    temp =strcpy(temp,input);


    shema->Key = (char*)malloc(sizeof(char) *strlen(Key));
    shema->Key = strcpy(shema->Key,Key);

    char* str = strtok(input,",");
    shema->Schema_Name = (char*)malloc(sizeof(char) *strlen(str));
    shema->Schema_Name = strcpy(shema->Schema_Name,str);


    shema->Table = TUPLE_Table_new(shema->Schema_Name);

    shema->Discription = (char*)malloc(sizeof(char) * strlen(temp));
    shema->Discription = strcpy(shema->Discription,temp);
    return shema;
}

void Add_SCHEMA(char* input, char* Key,SCHEMA_LIST lst ){
    SCHEMA schema= new_SCHEMA(input, Key);

    if(lst->tail != NULL)
        lst->tail->next = schema;

    lst->tail = schema;

    if(lst->head == NULL)
        lst ->head = schema;
}

TUPLE init_TUPLE_From_String(char* input){

    char* tp = (char*)malloc(sizeof(char) * strlen(input));
    tp = strcpy(tp, input);

    char* str = strtok(tp,",");
    TUPLE tuple = new_TUPLE(str);
    while(str != NULL){
        str = strtok(NULL,", ");
        if(str != NULL)
            Add_ATTRIBUTE(tuple, str, NULL);
    }

    return tuple;
}

TUPLE Read_TUPLE_From_String(char* input, SCHEMA_LIST lst){
    char* tp = (char*)malloc(sizeof(char) * strlen(input));
    tp = strcpy(tp, input);

    char* str = strtok(input,",");
    SCHEMA s = lst->head;
    TUPLE tuple = NULL;
    while(s != NULL){
        if(strcmp(s->Schema_Name, str) == 0){
            tuple = init_TUPLE_From_String(s->Discription);
            TUPLE_Set_Key(s->Key,tuple);
            break;
        }
        s = s->next;
    }

    ATTRIBUTE a = tuple->A_Head;
    str = strtok(tp,", ");
    while(str != NULL && a!=NULL){
        str = strtok(NULL,", ");
        if(str != NULL)
            a->data = str;
        a = a->next;
    }

    return tuple;
}




TUPLE_TABLE TUPLE_Table_new(char* R) {
    TUPLE_TABLE hashTable = (TUPLE_TABLE) malloc(sizeof(struct TUPLE_hashTable));
    hashTable->Table = (TUPLE *) malloc(sizeof(struct Tuple) * 1009);
    hashTable->Relation_Name =R;

//    int i;
//    for (i = 0; i < 1009; i++)
//        hashTable->Table[i] = new_TUPLE(R);

    hashTable->size = 0;
    return hashTable;
}

int TUPLE_hash(TUPLE tuple){
    long HashKey = 0;
    char* Key = tuple ->Key ->data;
    int len = strlen(Key);
    for(int i =0; i< len; i++) {
        HashKey += (long) 1543 * (len-i-1) * Key[i];
        HashKey= HashKey % 1009;
    }
    return (int) HashKey;
}

void TUPLE_Insert(TUPLE_TABLE t, TUPLE tuple){

    if(strcmp(t->Relation_Name, t->Relation_Name) != 0)
        return;

    if(tuple == NULL)
        return;;

    int index = TUPLE_hash(tuple);


    if(t ->Table[index] == NULL){
        t ->Table[index] = tuple;
//        printf("%d ",index);
//        printf("%s_Table",t->Relation_Name);
//        TUPLE_print(tuple);
//        printf("\n");
        t->size++;
    } else {
        TUPLE bucket = t->Table[index];
        while (bucket->next != NULL){
            if(TUPLE_LOOKUP_Compare(bucket,tuple))
                return;
            bucket = bucket ->next;

        }

        if(TUPLE_LOOKUP_Compare(bucket,tuple))
            return;

        bucket->next = tuple;

//        printf("%d ",index);
//        printf("%s_Table",t->Relation_Name);
//        TUPLE_print(tuple);
//        printf("\n");

        t->size++;
    }
}

TUPLE TUPLE_copy(TUPLE tuple){
    TUPLE cpy = new_TUPLE(tuple->Relation_Name);

    ATTRIBUTE node = tuple->A_Head;
    while(node != NULL){
        Add_ATTRIBUTE(cpy,node->Attribute_Name,node->data);
        node = node->next;
    }

    return cpy;

}

bool TUPLE_LOOKUP_Compare(TUPLE in, TUPLE tuple){

    if(strcmp(in->Relation_Name, tuple ->Relation_Name) != 0)
        return false;

    ATTRIBUTE a_in = in->A_Head;
    ATTRIBUTE a_tuple = tuple->A_Head;
    while(a_in != NULL && a_tuple!= NULL){
        if( strcmp(a_in->data,a_tuple->data) != 0 && strcmp(a_in->data,"*") !=0 )
            return false;
        //if(a_in->next != NULL)
            a_in = a_in->next;
        //if(a_tuple->next != NULL)
            a_tuple = a_tuple->next;
    }

    return true;
}

TUPLE_TABLE Search_Table(char* input, SCHEMA_LIST lst){
    char* str = strtok(input,",");
    SCHEMA s = lst->head;

    while(s != NULL){
        if(strcmp(s->Schema_Name, str) == 0){
            return s->Table;
        }
        s = s->next;
    }

    return NULL;
}



TUPLE TUPLE_lookup(char* input, SCHEMA_LIST lst){
    TUPLE tuple = Read_TUPLE_From_String(input,lst);
    TUPLE_TABLE t = Search_Table(input,lst);
    TUPLE curr;
    TUPLE head = NULL;
    TUPLE tail = head;

    int HashCode;
    bool isKey = false;
    if(tuple->Key!=NULL) {
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


void TUPLE_free_TUPLE(TUPLE tuple){

    free(tuple->Relation_Name);
    ATTRIBUTE a = tuple->A_Head;
    while(a != NULL){
        ATTRIBUTE next = a->next;
        if(a->data != NULL)
            a->data=NULL;
        if(a->Attribute_Name != NULL)
            a->Attribute_Name=NULL;
        free(a);
        a = next;
    }
    free(tuple);
    tuple =NULL;

}


void TUPLE_delete(char* input, SCHEMA_LIST lst){
    TUPLE tuple = Read_TUPLE_From_String(input,lst);
    TUPLE_TABLE t = Search_Table(input,lst);
    TUPLE head;
    TUPLE curr;
    TUPLE prev;

    int HashCode;
    bool isKey = false;
    if(tuple ->Key != NULL) {
        if (strcmp(tuple->Key->data, "*") != 0) {
            HashCode = TUPLE_hash(tuple);
            isKey = true;
        }
    }

    int i;
    for (i = 0; i < 1009; i++) {

        if (isKey) i = HashCode;
        if (t->Table[i] != NULL) {
            head = t->Table[i];
            curr = head;
            prev = curr;
            while (curr != NULL) {
                if (TUPLE_LOOKUP_Compare(tuple, curr)) {


                    if (curr == prev) {
                        if (curr->next == NULL) {
                            TUPLE_free_TUPLE(t->Table[i]);
                            t->Table[i] = NULL;

                        } else {
                            TUPLE temp = t->Table[i]->next;
                            TUPLE_free_TUPLE(t->Table[i]);
                            t->Table[i] = temp;
                            t->Table[i] = NULL;

                            curr = t->Table[i];
                            prev = curr;
                        }
                    } else {


                        if (curr->next != NULL) {
                            prev->next = curr->next;
                            TUPLE_free_TUPLE(curr);
                            t->Table[i] = NULL;
                            curr = prev->next;
                        } else {
                            prev->next = NULL;
                            TUPLE_free_TUPLE(curr);
                            t->Table[i] = NULL;
                        };
                    }


                } else {
                    if (curr == prev) {
                        curr = curr->next;
                    } else {
                        prev = prev->next;
                        curr = curr->next;
                    }
                }
            }

            if (isKey) break;

        }
    }

}


void TUPLE_print(TUPLE tuple){
    printf("%s :<",tuple->Relation_Name);
    ATTRIBUTE a = tuple ->A_Head;
    while(a != NULL){
        if(a == tuple->A_Tail) {
            printf("%s=%s", a->Attribute_Name, a->data);
            break;
        } else{
            printf("%s=%s, ", a->Attribute_Name,a->data);
        }
        a = a->next;
    }
    printf(">");
}


void TUPLE_Print_Table(TUPLE_TABLE t) {
    int i;
    TUPLE bucket = NULL;
    for (i = 0; i < 1009; i++) {
        if (t->Table[i] != NULL) {
            printf("Index: %d ", i);
            if (t->Table[i]->next == NULL)
                TUPLE_print(t->Table[i]);

            else {
                bucket = t->Table[i];
                while (bucket != NULL) {

                    TUPLE_print(bucket);
                    bucket = bucket->next;

                }
            }
            printf("\n");
        }
    }

}

