#include <stdio.h>
#include <stdlib.h>

struct node{
    int data;
    struct node *next;
};

struct node *firstQueue=NULL, *lastQueue=NULL;
struct node *firstStack=NULL, *lastStack=NULL;
int stackCount = 0; 

void PushQueue(int data){ 
    struct node *newNode=(struct node*)malloc(sizeof(struct node)); 
    newNode->data=data; 

    if(firstQueue != NULL){ 
        lastQueue->next=newNode;
        lastQueue=newNode; 
        lastQueue->next=NULL; 
    }else{ 
        firstQueue=newNode; 
        lastQueue=newNode;
        lastQueue->next=NULL; 
    }
}

void PopQueue(int arr[], int index){
    struct node *deleteNode;
    if(firstQueue != NULL){
        deleteNode=firstQueue;
        firstQueue=firstQueue->next;
        free(deleteNode);
    }
}

void ListQueue(){ 
    struct node *tmpList=firstQueue; 

    printf("Queue:\n");
    while(tmpList != NULL){ 
        printf("%d\t",tmpList->data); 
        tmpList=tmpList->next; 
    }
}

void PushStack(int data){ 
    struct node *newNode=(struct node*)malloc(sizeof(struct node)); 
    newNode->data=data; 

    if(firstStack == NULL){ 
        firstStack=newNode; 
        lastStack=newNode; 
        firstStack->next=NULL;
    }else{  
        lastStack->next=newNode;
        lastStack=newNode; 
        lastStack->next=NULL; 
    }
}

int PopStack(){ 
    struct node *deleteNode=lastStack, *prevNode=firstStack;
    int deleteNodeData = 0; 

    if(firstStack != NULL){ 
        while(prevNode->next != deleteNode && prevNode->next != NULL) 
            prevNode=prevNode->next; 
        prevNode->next = NULL; 
        lastStack =  prevNode; 
        if(firstStack == deleteNode) 
            firstStack = NULL; 
        deleteNodeData = deleteNode->data; 
        free(deleteNode); 
        return deleteNodeData; 
    }
}

void ListStack(){ 
    struct node *tmpList=firstStack;

    printf("Stack:\n");
    while(tmpList != NULL){ 
        printf("%d\t",tmpList->data);
        tmpList=tmpList->next;
    }
}

int FindQueueAVG(){ 
    int avg = 0, count = 0; 
    struct node *tmpList = firstQueue;

    while(tmpList != NULL){ 
        avg += tmpList->data; 
        count++; 
        tmpList = tmpList->next; 
    }
    if(avg > 0){ 
        avg = avg / count; 
    }
    return avg; 
}

void AddStackFromQueue(){ 
    struct node *tmpList = firstQueue; 
    int avg = FindQueueAVG(); 

    while(tmpList != NULL){ 
        if(tmpList->data > avg){ 
            PushStack(tmpList->data); 
            stackCount++; 
        }
        tmpList = tmpList->next; 
    }
}

void AddArrayFromStack(int arr[]){ 
    struct node *tmpList = firstStack; 

    for(int i=0;i<stackCount;i++){ 
        arr[i] = PopStack();
    }
}

void ListArray(int arr[]){ 
    printf("Array:\n");
    for(int i=0;i<stackCount;i++){ 
        printf("%d\t",arr[i]); 
    }
}

int main()
{

    for(int i=0;i<10;i++){
        int data;
        printf("Enter %d.number  :",i+1);
        scanf("%d",&data);
        PushQueue(data); 
    }
    
printf("\n________________________________________________________________________________\n");
    ListQueue();
    printf("(Average of the Queue: %d)",FindQueueAVG());
printf("\n________________________________________________________________________________\n");    
    AddStackFromQueue(); 
    ListStack();
printf("\n________________________________________________________________________________\n");   

    int arr[stackCount];
    AddArrayFromStack(arr); 
    ListArray(arr);
printf("\n________________________________________________________________________________\n");
}
