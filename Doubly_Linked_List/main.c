#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    int data;
    struct Node *next;
    struct Node *prev;
};

//GLOBAL OLARAK;

struct Node *firstNodeL1=NULL, *lastNodeL1=NULL;
struct Node *firstNodeL2=NULL, *lastNodeL2=NULL;

void addNodeL1 (int number) {

    struct Node *new=(struct Node*)malloc(sizeof(struct Node));
    new -> data = number;
        
        if (firstNodeL1==NULL){
            firstNodeL1 = new;
            lastNodeL1 = new;
            lastNodeL1 -> next = NULL;
            firstNodeL1 ->prev = NULL; 
        } else { 
            lastNodeL1 -> next = new; 
            new -> prev = lastNodeL1;
            new -> next = NULL; 
            lastNodeL1 = new;
        }
}

void addNodeL2 (int number) {
    struct Node *new=(struct Node*)malloc(sizeof(struct Node));
    new -> data = number; 
        
        if (firstNodeL2==NULL){ 
            firstNodeL2 = new;
            lastNodeL2 = new;
            lastNodeL2 -> next = NULL; 
            firstNodeL2 ->prev = NULL; 
        } else { 
            lastNodeL2 -> next = new;
            new -> prev = lastNodeL2;
            new -> next = NULL;
            lastNodeL2 = new; 
        }
}


void printList (int l) { 
    struct Node *list;
    if (l==1) { 
        list = firstNodeL1; 
    }
    if (l==2) {
        list = firstNodeL2;
    }
    while (list != NULL)
    {
        printf("%d",list -> data);
        list = list -> next;
    }
    printf("\n");   
}


void reverse (int l) { 
    struct Node *list=NULL, *tempFirst = NULL, *tempLast = NULL;

    if (l == 1) {
        tempFirst = firstNodeL1;
        tempLast = lastNodeL1;
        list = lastNodeL1;
        firstNodeL1 = NULL;
        lastNodeL1 = NULL;

            while (list !=NULL)
            {
                addNodeL1(list -> data);
                list = list -> prev;
            }
    }
    if (l == 2) {
        tempFirst = firstNodeL2;
        tempLast = lastNodeL2;
        list = lastNodeL2;
        firstNodeL2 = NULL;
        lastNodeL2 = NULL;

            while (list !=NULL)
            {
                addNodeL2(list -> data);
                list = list -> prev;
            }        
    }
}


void changeNode (struct Node *x) {
    struct Node *temp, *tempL1, *tempL2;
    tempL1 = firstNodeL1;
    tempL2 = firstNodeL2;

    while (x !=tempL1)
    {
        tempL1 = tempL1 -> next;
        tempL2 = tempL2 -> next;
    }

    if (x -> data == firstNodeL1 -> data) {
        temp = firstNodeL1;
        firstNodeL1 = firstNodeL2;
        firstNodeL2 = temp;

        tempL1 -> next -> prev = tempL2;
        tempL2 -> next -> prev = tempL1;

        temp = tempL1 -> next;
        tempL1 -> next = tempL2 -> next; 
        tempL2 -> next = temp;
    } 
    
    else if (x -> data == lastNodeL1 ->data) {
        temp = lastNodeL1;
        lastNodeL1 = lastNodeL2;
        lastNodeL2 = temp;

        tempL1 -> prev -> next = tempL2;
        tempL2 -> prev -> next = tempL1;

        temp = tempL1 -> prev;
        tempL1 -> prev = tempL2 -> prev;
        tempL2 -> prev = temp;
    } 
    
    else {
        tempL1 -> prev -> next = tempL2;
        tempL2 -> prev -> next = tempL1;

        temp = tempL1 -> prev; 
        tempL1 -> prev = tempL2 -> prev;
        tempL2 -> prev = temp;

        tempL1 -> next -> prev = tempL2;
        tempL2 -> next -> prev = tempL1;

        temp = tempL1 -> next;
        tempL1 -> next = tempL2 -> next; 
        tempL2 -> next = temp;
    }
}

#define filePath "Sayilar.txt"
int main() {
    
    FILE *file;

    if ((file=fopen(filePath,"r"))==NULL)
        printf("File not found!\n");
    else {
        char ch[1000], *number1, *number2;

        fgets(ch,1000,file);

        number1 = strtok(ch,"#");
        number2 = strtok(NULL,"#");

        int arr1[170], arr2[170];
        char temp[3];

        for (int i=0; i<strlen(number1); i+=3){ 
            temp[0] = number1[i];
                if(temp[0] == '0'){
                temp[0] = '1';
                }
            temp[1] = number1[i+1];
            temp[2] = number1[i+2];
            arr1[i/3] = atoi(temp);
        }
    
        for (int i=0; i<strlen(number2); i+=3){ 
            temp[0] = number2[i]; 
                if(temp[0] == '0') 
                    temp[0] = '1'; 
            temp[1] = number2[i+1]; 
            temp[2] = number2[i+2]; 
            arr2[i/3] = atoi(temp);
        }

        for (int i=0; i<strlen(number1)/3; i++) {
            addNodeL1(arr1[i]);
            addNodeL2(arr2[i]);
        }

        for (int i=0; i<strlen(number1)/3; i++) {
            struct Node *tempL1 =firstNodeL1, *tempL2=firstNodeL2;

                for (int j=0; j<i; j++) {
                    tempL1 = tempL1 -> next;
                    tempL2 = tempL2 -> next;
                }
            if (tempL1 -> data > tempL2 -> data)
                reverse(1);
            else if (tempL2 -> data > tempL1 -> data)
                changeNode(tempL1);
            else 
                reverse(2);
        }
        
        printf("Sayi 1: "); printList(1);
        printf("Sayi 2: "); printList(2);
    }


   return 0;
}
