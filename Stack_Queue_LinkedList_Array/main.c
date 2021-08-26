/* S141210344 - M.Cüneyd KURTBAŞ*/

#include <stdio.h>
#include <stdlib.h>

struct node{
    int data;
    struct node *next;
};

struct node *firstQueue=NULL, *lastQueue=NULL;
struct node *firstStack=NULL, *lastStack=NULL;
int stackCount = 0;//Yığından diziye kaç eleman aktarilacağini saydiracağım değişkeni global olarak tanımladım 

void PushQueue(int data){ //Kuyruğa eleman ekleme fonksiyonu
    struct node *newNode=(struct node*)malloc(sizeof(struct node)); //Bellekte yer ayır
    newNode->data=data; // yeninin datasına parametre olarak gelen veriyi aktar

    if(firstQueue != NULL){ //Eğer Kuyruğun ilki NULL değilse
        lastQueue->next=newNode; // sonun nexti yeniyi göstersin
        lastQueue=newNode; // son yeni olsun
        lastQueue->next=NULL; // sonun nexti null olsun
    }else{ //Eğer kuyrukta eleman yoksa
        firstQueue=newNode; //ilki yeni yap
        lastQueue=newNode; //sonu yeni yap
        lastQueue->next=NULL; //sonun nextini null yap
    }
}

void ListQueue(){ //Kuyruktaki verileri listeleme fonk.
    struct node *tmpList=firstQueue; //bir templistte kuyruğun ilkini tut

    printf("Queue:\n");
    while(tmpList != NULL){ //temp list null değilse
        printf("%d\t",tmpList->data); //datasını yazdır
        tmpList=tmpList->next; //bir sonrakine geç
    }
}

void PushStack(int data){ //Yığına veri ekleme fonksiyonu, veriyi int parametre olarak aldım
    struct node *newNode=(struct node*)malloc(sizeof(struct node)); //Bellekte yer ayırarak yeni node tanımı
    newNode->data=data; // Parametre olarak gelen veriyi yeninin datasına 

    if(firstStack == NULL){ //Yığının ilk elemanı NULL ise boştur o halde
        firstStack=newNode; // Yığının ilk elemanını yeni yap
        lastStack=newNode; // Son elemanını da yeni yap
        firstStack->next=NULL;// First Stack yani İlk elemanı yeni olan yığının nextini NULL yap
    }else{   //Yığının ilk elemanı null değilse
        lastStack->next=newNode; //yığının son elemanının nextini yeni yap
        lastStack=newNode;  //son yeni olsun
        lastStack->next=NULL; //sonun nexti null olsun
    }
}

int PopStack(){ //Yığından eleman çıkarma fonksiyonu
    struct node *deleteNode=lastStack, *prevNode=firstStack;
    int deleteNodeData = 0; //Çıkarılacak yığın verisini tutacak değişken.

    if(firstStack != NULL){ //yığının ilki NULL değilse
        while(prevNode->next != deleteNode && prevNode->next != NULL) //yığının ilkinin nexti çıkarılacak node değilse ve null değilse
            prevNode=prevNode->next; //bir sonraki elemana geç bu işlemi şart false olana kadar yapacak
        prevNode->next = NULL; // false olup çıktığında prevNode nextini null yap
        lastStack =  prevNode; //prev node u yığının sonu yap
        if(firstStack == deleteNode) //Çıkarılacak eleman hala deletenode olarak tutuluyor. çıkarılacak olan  yığının ilk node u ise
            firstStack = NULL; //ilki NULL yap
        deleteNodeData = deleteNode->data; // delete nodedaki veriyi diziye aktarmak için al
        free(deleteNode); //deleteNodeu free yap
        return deleteNodeData; //Diziye aktarılacak değeri fonk. dönüş değeri yap
    }
}

void ListStack(){ //yığındaki elemanları yazdır
    struct node *tmpList=firstStack;

    printf("Stack:\n");
    while(tmpList != NULL){ //boş değilse
        printf("%d\t",tmpList->data);//temp list datasını yazdır
        tmpList=tmpList->next; //sonrakine geç
    }
}

int FindQueueAVG(){ //Kuyruğun ortalamasını bulan fonk.
    int avg = 0, count = 0; 
    struct node *tmpList = firstQueue;

    while(tmpList != NULL){ //boş değilse
        avg += tmpList->data; //ortalamaya listedeki datayı ekle
        count++; //sayacı bir arttır
        tmpList = tmpList->next; //bir sonrakine geç
    }
    if(avg > 0){ //eğer ortalama dediğimiz toplam sıfırdan büyükse
        avg = avg / count; //ortalamayı toplam bölü eleman sayısı olarak revize et at
    }
    return avg; //ve ortalamayı geri döndür
}

void AddStackFromQueue(){ //Kuyruktan yığına veri ekleme fonk.
    struct node *tmpList = firstQueue; 
    int avg = FindQueueAVG(); //avg değişkenine biraz önce yazdığımız ort. hesaplayan fonk. dönüş değerini ata

    while(tmpList != NULL){ //boş değilse
        if(tmpList->data > avg){ // eğer temp list datası ortalamadan büyükse
            PushStack(tmpList->data); //yığına bu elemanı ekle
            stackCount++; //stackteki eleman sayısını bir arttır. Bu aktaracağımız dizi için gerekli
        }
        tmpList = tmpList->next; //temp listin sonraki elemanına geç while döngüsüne devam et
    }
}

void AddArrayFromStack(int arr[]){ //Yığına ektarılan ortalamadan büyük elemanları diziye aktarma fonk.
    struct node *tmpList = firstStack; 

    for(int i=0;i<stackCount;i++){ //yığındaki eleman adedince çalış
        arr[i] = PopStack(); //dizinin i. indisine yığından çıkarılan elemanı aktar devam et
    }
}

void ListArray(int arr[]){ //Diziyi yazdıralım
    printf("Array:\n");
    for(int i=0;i<stackCount;i++){ //yine global olarak tuttuğumuz yığındaki veri sayısı kadar çalış
        printf("%d\t",arr[i]); //dizinin elemanlarını yazdır
    }
}

int main()
{
    //Kullanıcının Kuyruğa eleman eklemesi için döngü 
    for(int i=0;i<10;i++){
        int data;
        printf("Enter %d.number  :",i+1);
        scanf("%d",&data);
        PushQueue(data); // Kuyruğa veriyi push et
    }
    
printf("\n________________________________________________________________________________\n");
    ListQueue(); //Kuyruğu listele
    printf("(Average of the Queue: %d)",FindQueueAVG()); //Ortalamayı görelim
printf("\n________________________________________________________________________________\n");    
    AddStackFromQueue(); //Kuyruktan yığına eleman ekleyelim
    ListStack(); //Yığınu listele
printf("\n________________________________________________________________________________\n");   

    int arr[stackCount];//Diziye aktarmak için bir dizi tanımlayalım ve dizi boyutunu yığındaki eleman sayısı kadar yapalım
    AddArrayFromStack(arr); //diziyi, yığından diziye eleman ekleme fonk.na gönderelim
    ListArray(arr); //Ve diziyide yazdıralım.
printf("\n________________________________________________________________________________\n");
}
