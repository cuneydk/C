#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void addNew();
static void createIndex();
static void findInIndex();
static void deleteLine();
static void updateLine();
static void listData ();
static void listIndex ();
static void destroyIndex ();
static void mainMenu ();

FILE *DF,*IF;
int counter;

struct kayit{
    int ogrNo;
    int dersKodu;
    int puan;
};

void addNew (int ogrNo, int dersKodu, int puan) {
    struct kayit newStudent;
    newStudent.ogrNo = ogrNo; newStudent.dersKodu = dersKodu; newStudent.puan = puan;

    DF = fopen("data.bin","a+b");

    fwrite(&newStudent,sizeof(struct kayit),1,DF);
    fclose(DF);
    createIndex();
}

void createIndex () {
    DF = fopen("data.bin","rb");
    counter = 0;
    struct kayit reader, writer;

    while(fread(&reader,sizeof(struct kayit),1,DF))
    counter++;

    fclose(DF);

    int arrNums[counter], arrLoc[counter], count=0;

    DF = fopen("data.bin","rb");
    while(fread(&writer,sizeof(struct kayit),1,DF)) {
            arrNums[count] = writer.ogrNo;
            arrLoc[count] = ftell(DF) -  sizeof(struct kayit);
            count++;
    }
    fclose(DF);
    int i, j, k, min, temp;
    for (i=0; i<counter; i++) {
    min = i;
        for (j=i+1; j<counter; j++) {
            if (arrNums[j]<arrNums[min])
                min = j;
            else if (arrNums[j] == arrNums[min] && arrLoc[j] < arrLoc[min])
                min = j;
        }

    temp = arrNums[min];
    arrNums[min] = arrNums[i];
    arrNums[i] = temp;
    temp = arrLoc[min];
    arrLoc[min] = arrLoc[i];
    arrLoc[i] = temp;
    }

    FILE *index = fopen("index.txt","w");
    for (k=0; k<counter; k++)
        fprintf(index,"%d=%d\n",arrNums[k],arrLoc[k]);

    fclose(index);
    printf("________________________________\n\n");
    printf("Index Dosyasi Yaratildi!\n");
    printf("________________________________\n");
}

void findInIndex(int ogrNo) {
    IF = fopen("index.txt","rb");
    int arrIndex[counter][2];

    int count=0;
    char line[11];
        while(fgets(line,11,IF)){
        arrIndex[count][0] = atoi(strtok(line,"="));
        arrIndex[count][1] = atoi(strtok(NULL,"="));
        count++;
    }
    fclose(IF);
    struct kayit reader;
    int first=-1, last=counter, flag;
    printf("______________________________________________________________________________\n\n");
    while(last-first>1) {
        int state = (first+last)/2;
        if(arrIndex[state][0] == ogrNo){
            printf("Bulunan Anahtar: %3d = Offset: %d. Arama islemi devam ediyor... \n\n",arrIndex[state][0],arrIndex[state][1]);
            DF = fopen("data.bin","rb");
            while(fread(&reader,sizeof(struct kayit),1,DF))
                if(arrIndex[state][0] == reader.ogrNo) {
                    printf("INDEX Dosyasinda Aranan %d Anahtar Nolu Ogrencinin, DATA Dosyasindaki Ilk Kaydi \n\n \t\t Ogrenci No: %3d | Ders Kodu: %3d | Puan: %3d\n",ogrNo,reader.ogrNo,reader.dersKodu,reader.puan);
                    printf("______________________________________________________________________________\n\n");
                    fclose(DF);
                    flag = 1;
                    //break;
                }
            fclose(DF);
            break;
        }else if(arrIndex[state][0] < ogrNo){
            first = state;
        }else{
            last = state;
        }
    }
        if(flag == 0)
            printf("\n KAYIT BULUNAMADI!");

}

void deleteLine (int ogrNo) {
    DF = fopen("data.bin","a+b");
    FILE *TEMP = fopen("temp.bin","wb");
    struct kayit reader;
    int count=0;
        while(fread(&reader,sizeof(struct kayit),1,DF)){
        if(ogrNo != reader.ogrNo){
            fwrite(&reader,sizeof(struct kayit),1,TEMP);
        }else{
            count++;
        }
    }
    printf("\n_______________________________________________________________________\n\n");
    printf("%3d Numarali Ogrenciye Ait Tum Veriler (%d Adet) Silindi.",ogrNo, count);
    printf("\n_______________________________________________________________________\n\n");

    fclose(DF); fclose(TEMP);
    remove("data.bin");
    rename("temp.bin","data.bin");
    createIndex();
}

void updateLine (int ogrNo, int dersKodu, int puan) {
    DF = fopen("data.bin","r+b");
    struct kayit reader;
    int state = 0;

        while(fread(&reader,sizeof(struct kayit),1,DF) != NULL){
        if(reader.ogrNo == ogrNo && reader.dersKodu==dersKodu){
            printf("_______________________________________________________________________\n\n");
            printf("Bulunan Eski  Veri >>> Ogrenci No: %d | Ders Kodu: %d | Puan: %d \n",reader.ogrNo, reader.dersKodu, reader.puan);
            reader.puan = puan;
            state = ftell(DF);
            fseek(DF,state-sizeof(struct kayit),0);
            fwrite(&reader,sizeof(struct kayit),1,DF);
            fclose(DF);
            break;
        }
    }
    if (state ==0)
        printf("Veri Bulunamadi!\n");
    else
        printf("Guncelleme Sonrasi >>> Ogrenci No: %d | Ders Kodu: %d | Puan: %d \n",ogrNo, dersKodu, puan);
    printf("_______________________________________________________________________\n\n");
}

void listData () {
    DF = fopen("data.bin","rb");
    if (DF == NULL) {
        printf("Veri dosyasi okunamiyor!\n");
    } else {
        printf("\n\n\t\t############# VERI DOSYASI ICERIGI #############\n\n");
        printf(" OGRENCI NO  DERS KODU   PUAN \n");
        printf("______________________________\n");
        struct kayit reader;
        while(fread(&reader,sizeof(struct kayit),1,DF)) {
            printf("|%7d   | %5d    |   %3d |\n",reader.ogrNo, reader.dersKodu, reader.puan);

        }
        printf("------------------------------\n");
        printf("TOPLAM KAYIT: %d\n",counter);

    }
    fclose(DF);
}

void listIndex () {
    IF = fopen("index.txt","r");
    if (IF == NULL) {
        printf("Index dosyasi okunamiyor!\n");
    } else {
        printf("\n\n\t\t############# INDEX DOSYASI ICERIGI #############\n\n");
        printf(" OGRENCI NO   ADRES \n");
        printf("___________________\n");
        char line[11];
        while(fgets(line,11,IF)){
        printf("|%7d    |",atoi(strtok(line,"=")));
        printf("%5d |\n",atoi(strtok(NULL,"=")));
    }
    printf("-------------------\n");
    }
    fclose(IF);
}

void destroyIndex () {
    remove("index.txt");
    printf("________________________________\n\n");
    printf("Index Dosyasi Diskten Silindi!\n");
    printf("________________________________\n");
}

void mainMenu () {
    printf("\n..................................................................\n");
    printf("|    VERI DOSYASI ISLEMLERI    |      INDEX DOSYASI ISLEMLERI     |\n");
    printf("|.................................................................|\n");
    printf("| Kayit Ekle            [ 1 ]  |  Index Dosyasi Olustur   [ 2 ]   |\n");
    printf("| Kayit Sil             [ 3 ]  |  Kayit Bul/Binary Search [ 4 ]   |\n");
    printf("| Kayit Guncelle        [ 5 ]  |  Index Dosyasini Sil     [ 6 ]   |\n");
    printf("| Veri Dosyasini Goster [ 7 ]  |  Index Dosyasini Goster  [ 8 ]   |\n");
    printf("..................................................................\n");

}
void main()
{
    createIndex ();
    int ogrNo, dersKodu, puan;
    int selection=-1;

    while (1) {
        mainMenu();
        printf("\tISLEMINIZI SECINIZ [1]...[8], CIKIS ICIN [9]: "); scanf("%d",&selection);

            switch(selection) {
                case 1:
                    printf("\nYENI KAYIT EKLE:\n");
                    printf("Ogrencinin Numarasini Giriniz: "); scanf("%d",&ogrNo);
                    printf("Ders Kodu Bilgisini Giriniz  : "); scanf("%d",&dersKodu);
                    printf("Ders Puan Bilgisini Giriniz  : "); scanf("%d",&puan);
                    addNew(ogrNo, dersKodu, puan);
                    break;
                case 2:
                    createIndex();
                    break;
                case 3:
                    printf("\nSilmek Istediginiz Ogrenci Numarasini Giriniz: "); scanf("%d",&ogrNo);
                    deleteLine(ogrNo);
                    break;
                case 4:
                    printf("\nAramak Istediginiz Ogrenci Numarasini Giriniz: "); scanf("%d",&ogrNo);
                    findInIndex(ogrNo);
                    break;
                case 5:
                    printf("\nKAYIT GUNCELLE:\n");
                    printf("Ogrenci Numarasini  Giriniz  : "); scanf("%d",&ogrNo);
                    printf("Ders Kodu Bilgisini Giriniz  : "); scanf("%d",&dersKodu);
                    printf("Ders Puan Bilgisini Giriniz  : "); scanf("%d",&puan);
                    updateLine(ogrNo, dersKodu, puan);
                    break;
                case 6:
                    destroyIndex();
                    break;
                case 7:
                    listData();
                    break;
                case 8:
                    listIndex();
                    break;
                 case 9:
                    exit(1);
                    break;
                default:
                    printf("<<HATALI ISLEM NUMARASI GIRDINIZ!>>>\n");
        }
    }
}
