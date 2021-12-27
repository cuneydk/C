#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct plane{
    int planeID;
    int priorityID;
	int landingTime;
	int delayTime;
	int departureTime;
    struct plane *next;
};

struct plane *landingFirst = NULL, *landingLast = NULL;
struct plane *departureFirst = NULL, *departureLast = NULL;
int capacity = 10;

void PrintQueue(struct plane *first){
    printf("ONCELIKLI KUYRUK:\n");
        	printf("Oncelik - Ucak - Inis - Erteleme\n");
    while(first != NULL){
        printf("| %4d - %4d - %4d - %4d |\n",first->priorityID,first->planeID,first->landingTime,first->delayTime);
        first = first->next;
    }
    printf("\n\n");
}

//inis istegini alan fonksiyon
void GetRequest(int priorityID, int planeID, int landingTime, int departureTime, int delayTime){

    if((capacity-FreePlace()) == 0){ //kapasite kontrolu
        printf("\n\n***UCAK ID: %d, SAAT: %d, ONCELIK: %d \n Havalimani Kapasistesi Doldugu Icin Talep Reddedildi!+++\n",planeID, landingTime, priorityID);
        return;
	}

	if(delayTime == 4){ //dorduncu kez ertelemeye  dustuyse talep reddedilir
        printf("\n\n*** UCAK ID: %d, SAAT: %d, ONCELIK: %d \n Max. Erteleme Saat Araliginda, Pistte Daha Yuksek Oncelikli Ucaklar Var. Baska Bir Havalimanina Yonlendiriliyorsunuz!\n\n",planeID, landingTime-delayTime, priorityID);
        return;
	}

	//yer ayir ve inis istegini al
    struct plane *newplane = (struct plane*) malloc(sizeof(struct plane));
    newplane->planeID = planeID;
    newplane->priorityID = priorityID;
    newplane->delayTime = delayTime;
    newplane->landingTime = landingTime;
    if(departureTime == 25) //kalkis saat 24 ten sonra saat 1 e dusmesi icin kontrol
        departureTime = 1;
    newplane->departureTime = departureTime;
    newplane->next = NULL;

    if(landingFirst == NULL){ //kuyruk bos ise
        landingFirst = newplane;
        landingLast = newplane;
        return;
    }else if(landingFirst == landingLast){ //kuyrukta 1 eleman var ise
        if(landingTime == landingFirst->landingTime){ //yyeni talep edilen ucagin inis saati ilk ucagin inis saatine esit ise
            if(priorityID < landingFirst->priorityID){ //onceliklerini karsilastir
                landingFirst->landingTime = landingFirst->landingTime+1; 
                landingFirst->departureTime = landingFirst->departureTime+1;
                landingFirst->delayTime = landingFirst->departureTime - landingFirst->landingTime + 1;
                landingLast = landingFirst;

                newplane->next = landingLast;
                landingFirst = newplane;
                printf("-----------------------------------------------------------------------\n");
                printf("|%d ID'li Ucak! Havalimani Kullaniminiza Oncelikli Olarak Izin Verildi|\n",newplane->planeID);
                printf("------------------------------------------------------------------------\n");
                return;
            }else{ //yyeni elemanin onceligi yuksekse kuyrugun sagina ekle
                newplane->landingTime = landingTime + 1;
                newplane->departureTime = departureTime + 1;
                newplane->delayTime = departureTime - landingTime + 1;

                landingLast = newplane;
                landingFirst->next = landingLast;
                printf("-----------------------------------------------------------------------\n");
                printf("|%d ID'li Ucak! Havalimani Kullaniminiza Ertelemeli Olarak Izin Verildi|\n",newplane->planeID);
                printf("------------------------------------------------------------------------\n");
                return;
            }
        }else if(landingTime < landingFirst->landingTime){ //yyeni talebin inisi ilk ucagin inisinden once ise listenin basi yap ve cik
            newplane->next = landingLast;
            landingFirst = newplane;
            printf("------------------------------------------------------------------\n");
            printf("|%d ID'li Ucak! Talep Saatinizde Pist Bos Oldugundan Izin Verildi|\n",newplane->planeID);
            printf("------------------------------------------------------------------\n");
            return;
        }else{ //degilse son eleman yap, zaten tek elemanli kuyruk kontrolundeyiz
            landingLast = newplane;
            landingFirst->next = landingLast;
            printf("------------------------------------------------------------------------\n");
            printf("|%d ID'li Ucak! Talep Saatinizde Dolu Oldugundan Ertelemeli Izin Verildi|\n",newplane->planeID);
            printf("------------------------------------------------------------------------\n");
            return;
        }
    }else{ //kuyrukta birden fazla elaman var ise
        struct plane *temp = landingFirst;
        if(landingTime > landingLast->landingTime){ //yyeni ucagin inisi son ucagin inisinden buyuk ise sona ekle
            landingLast->next = newplane;
            landingLast = newplane;
            printf("------------------------------------------------------------------\n");
            printf("|%d ID'li Ucak! Talep Saatinizde Pist Bos Oldugundan Izin Verildi|\n",newplane->planeID);
            printf("------------------------------------------------------------------\n");
            return;
        }else if(landingTime == landingLast->landingTime){ //sona esit mi
            if(landingTime == 24){ //saat 24 ise erteleyemeyecegi icin talebi reddet
                printf("\n\n*** UCAK ID: %d, SAAT: %d, ONCELIK: %d \n Havalimani Pistleri Tum Saatler Icin Dolu, Talebiniz Reddedildi!\n\n",planeID, landingTime, priorityID);
                return;
            }else{ //sona esit ve talep 24 degilse ertelenme mumkun
                if(priorityID < landingLast->priorityID){ //yyeni talebin onceligi son elemanin onceliginden kucukse
                    if(landingLast->delayTime < 3){ //ve 3 ten az ertelenmisse sonu saga kaydir yerine yyeniyi ekle
                        while(temp->next != landingLast) 
                            temp = temp->next;
                        if(landingLast->departureTime == 24)
                            landingLast->departureTime = 1;
                        landingLast->landingTime += 1;
                        landingLast->delayTime += 1;
                        newplane->next = landingLast;
                        temp->next = newplane;
                		printf("-----------------------------------------------------------------------\n");
               		 	printf("|%d ID'li Ucak! Havalimani Kullaniminiza Oncelikli Olarak Izin Verildi|\n",newplane->planeID);
                		printf("------------------------------------------------------------------------\n");
                        return;
                    }else{ //eleman 3 kez ertelenmis ise yyeni geleni son yap
                        if(newplane->departureTime == 24)
                            newplane->departureTime = 1;
                        newplane->delayTime += 1;
                        newplane->landingTime += 1;
                        landingLast->next = newplane;
                        landingLast = newplane;
                		printf("-----------------------------------------------------------------------\n");
                		printf("|%d ID'li Ucak! Havalimani Kullaniminiza Izin Verildi| (=>3 erteleme)\n",newplane->planeID);
                		printf("------------------------------------------------------------------------\n");
                        return;
                    }
                }else{ //yyeni talebin onceligi son elemanin onceligine esit veya buyukse
                    if(newplane->departureTime == 24)
                        newplane->departureTime = 1;
                    newplane->delayTime += 1;
                    newplane->landingTime += 1;
                    landingLast->next = newplane;
                    landingLast = newplane;
                    printf("-----------------------------------------------------------------------\n");
               		printf("|%d ID'li Ucak! Havalimani Kullaniminiza Ertelemeli Olarak Izin Verildi|\n",newplane->planeID);
                	printf("------------------------------------------------------------------------\n");
                    return;
                }
            }
        }else if(landingTime < landingFirst->landingTime){ //yyeni talebin inis saat ilk elemanin inis saatinden kucukse
            newplane->next = landingFirst; 
            landingFirst = newplane; //talebi ilk yap
            printf("------------------------------------------------------------------\n");
            printf("|%d ID'li Ucak! Talep Saatinizde Pist Bos Oldugundan Izin Verildi|\n",newplane->planeID);
            printf("------------------------------------------------------------------\n");
            return;
        }else if(landingTime == landingFirst->landingTime){ // yyeni talebin inis saati ile ilk elemanin inis saati esitse
            if(priorityID < landingFirst->priorityID){ //yyeni talebin onceligi ilkin onceliginden dusuk ise
            	//burada recursive mantigi ile fonksiyonu yeniden cagiriyoruz ancak ilk eleman talebini 1 saat ilave ederek cagiriyoruz
                GetRequest(landingFirst->priorityID, landingFirst->planeID, landingFirst->landingTime+1, landingFirst->departureTime+1, landingFirst->delayTime+1);
                //yyeni elemani ilk yap
				landingFirst->priorityID = priorityID;
                landingFirst->planeID = planeID;
                landingFirst->landingTime = landingTime;
                landingFirst->departureTime = departureTime;
                landingFirst->delayTime = delayTime;
                printf("-----------------------------------------------------------------------\n");
               	printf("|%d ID'li Ucak! Havalimani Kullaniminiza Oncelikli Olarak Izin Verildi|\n",newplane->planeID);
                printf("------------------------------------------------------------------------\n");
                return;
            }else{
            	//yyeni talebin onceligi ilkin onceliginden yuksekse yyeni elamani recursive olarak 1 saat ilave ederek cagir
                GetRequest(priorityID, planeID, landingTime+1, departureTime+1, delayTime+1);
                return;
            }
        }else{ //gelen talep ne ilk ne de son elamanla aynidir arada bir yere inis talebinde bulunmustur
            struct plane *temp = landingFirst->next, *tempOnceki = landingFirst; //ilk inis ve ilk inisin sonrakini gecicilerde tut
            while(temp != NULL){ //tum listeyi gez
                if(landingTime > temp->landingTime){ //ucagin kuyruk icinde inecegi yeri bulana kadar gez
                    temp = temp->next;
                    tempOnceki = tempOnceki->next;
                    continue;
                }else if(landingTime == temp->landingTime){ //araya eklemede ayni inis saatine sahip bir ucak varsa
                    if(priorityID < temp->priorityID){ //yyeni gelenin onceligi dusukse
                    	//recursive olarak onceligi yuksek olani sanki 1 saat sonrasini talep etmis gibi yyeni inis iste ve yyeniyi ardindan araya ekle
                        GetRequest(temp->priorityID, temp->planeID, temp->landingTime+1, temp->departureTime+1, temp->delayTime+1);
                        temp->priorityID = priorityID;
                        temp->planeID = planeID;
                        temp->landingTime = landingTime;
                        temp->departureTime = departureTime;
                        temp->delayTime = delayTime;
                        printf("-----------------------------------------------------------------------\n");
               			printf("|%d ID'li Ucak! Havalimani Kullaniminiza Oncelikli Olarak Izin Verildi|\n",newplane->planeID);
                		printf("------------------------------------------------------------------------\n");
                        return;
                    }else{ //yyeni gelenin onceligi kuyrukta bulunanin onceligi ile esit ise yyeni geleni recursive 1 saat sonrasi ile cagir
                        GetRequest(priorityID, planeID, landingTime+1, departureTime+1, delayTime+1);
                        return;
                    }
                }else{ //gezerken (while) bulunan yyeni talebin saati kucukse araya ekle
                    newplane->next = temp;
                    tempOnceki->next = newplane;
                    printf("------------------------------------------------------------------\n");
            		printf("|%d ID'li Ucak! Talep Saatinizde Pist Bos Oldugundan Izin Verildi|\n",newplane->planeID);
            		printf("------------------------------------------------------------------\n");
                    return;
                }
            }
        }
    }
}

// Kuyruga eleman ekleme
void enqueue(int priorityID, int planeID, int landingTime, int departureTime, int delayTime){
        
		struct plane *temp = (struct plane*) malloc(sizeof(struct plane));
        temp->planeID = planeID;
        temp->priorityID = priorityID;
        temp->delayTime = delayTime;
        temp->landingTime = landingTime;
        temp->departureTime = departureTime;
		temp->next = NULL;
		
    if(departureFirst == NULL){
        departureFirst = temp;
        departureLast = temp;
    }else{

        departureLast->next = temp;
        departureLast = temp;
    }
}

//Inis Kuyrugundan eleman cikar
void dequeueLanding(){
    if(landingFirst != NULL){ //kuyruk bos degilse
        if((landingFirst)->next == NULL){ //kuyrugun tek elemani varsa
            free(landingFirst); // bellekten ucur
            landingFirst = NULL;
            landingLast = NULL;
        }else{ //kuyrukta birden fazla eleman varsa
            struct plane *temp = landingFirst; // ilk elemani gecicide tut
            landingFirst = temp->next; //ilk eleman, gecicinin next elemani olsun
            free(temp); //geciciyi bellekten ucur
        }
    }
}

//Kalkis Kuyrugundan eleman cikar
void dequeueDeparture(){
    if(departureFirst != NULL){ //kuyruk bos degilse
        if((departureFirst)->next == NULL){ //kuyrugun tek elemani varsa
            free(departureFirst); // bellekten ucur
            departureFirst = NULL;
            departureLast = NULL;
        }else{ //kuyrukta birden fazla eleman varsa
            struct plane *temp = departureFirst; // ilk elemani gecicide tut
            departureFirst = temp->next; //ilk eleman, gecicinin next elemani olsun
            free(temp); //geciciyi bellekten ucur
        }
    }
}

void CreateOutputTxt(){
    CreateDepartureQueue(); //kalkis listesini olustur
    FILE *txtFile = fopen("output.txt", "w"); //yazma modunda ac
    struct plane *temp = departureFirst; //kalkisi al
    while(temp != NULL){
    												//oncelik_id, ucak_id, talep_edilen_inis_saati, inis_saati, gecikme_suresi, kalkis_saati
        fprintf(txtFile, "%d %3d %3d %3d %2d %3d\n", temp->priorityID, temp->planeID, temp->landingTime-temp->delayTime, temp->landingTime, temp->delayTime, temp->departureTime);
        temp = temp->next;
    }
    fclose(txtFile);
}

void CreateDepartureQueue(){
    clearDeparture(&departureFirst, &departureLast); //bazen araya eklenme oldugu icin kontrol yerine direkt kalkis listesini temizle
    struct plane *temp = landingFirst; //inis listesini al
    while(temp != NULL){ //tek tek kalkis listesine ekle
        enqueue(temp->priorityID, temp->planeID, temp->landingTime, temp->departureTime, temp->delayTime);
        temp = temp->next;
    }
}

//Kuyruktaki son elemani dondur
struct plane *PeekLanding(){
    return landingFirst;
}

struct plane *PeekDeparture(){
    return departureFirst;
}

//Inis Kuyruk elemanlarini sil
void clearLanding(){
    if((landingFirst) != NULL){ //kuyruk bos degilse
        if((landingFirst)->next == NULL){ //tek eleman varsa
            free(landingFirst); //ucur
            landingFirst = NULL;
            landingLast = NULL;
        }else{ //birden fazla eleman varsa
            while(landingFirst != NULL){ //kuyrugu gez
                struct plane *temp = landingFirst;
                landingFirst = landingFirst->next; //bir ileri git
                temp->next = NULL; //sil
                free(temp); //ucur
            }
        }
    }
}

//Kalkis Kuyruk elemanlarini sil
void clearDeparture(){
    if(departureFirst != NULL){ //kuyruk bos degilse
        if(departureFirst->next == NULL){ //tek eleman varsa
            free(departureFirst); //ucur
            departureFirst = NULL;
            departureLast = NULL;
        }else{ //birden fazla eleman varsa
            while(departureFirst != NULL){ //kuyrugu gez
                struct plane *temp = departureFirst;
                departureFirst = departureFirst->next; //bir ileri git
                temp->next = NULL; //sil
                free(temp); //ucur
            }
        }
    }
}

//Inis Kuyrugunda son elemani dondurur ve kuyruktan siler
int pollLanding(){
    struct plane *temp = landingFirst;
    int planeID = landingLast-> planeID;

    if(landingFirst == NULL) //kuyruk bos ise
        return -1;
    else if(landingFirst == landingLast){ //kuyrukta tek eleman varsa
        free(landingFirst); //ucur
        landingFirst = NULL;
        landingLast = NULL;
        return(planeID); //elemani dondur
    }else{ //kuyrukta birden fazla eleman varsa 
        while(temp->next != landingLast){ //son elemani bulmak uzere dongu baslat
            temp = temp->next; //elemanlari gez
        }
        struct plane *destroy = landingLast; 
        landingLast = temp; //dongude belirlenen son elemani al
        landingLast->next = NULL;
        free(destroy); //ucur
    }
    return planeID; //elemani dondur
}

//Kalkis Kuyrugunda son elemani dondurur ve kuyruktan siler
int pollDeparture(){
    struct plane *temp = departureFirst;
    int planeID = departureLast->planeID;

    if(departureFirst == NULL) //kuyruk bos ise
        return -1;
    else if(departureFirst == departureLast){ //kuyrukta tek eleman varsa
        free(departureFirst); //ucur
        departureFirst = NULL;
        departureLast = NULL;
        return(planeID); //elemani dondur
    }else{ //kuyrukta birden fazla eleman varsa 
        while(temp->next != departureLast){ //son elemani bulmak uzere dongu baslat
            temp = temp->next; //elemanlari gez
        }
        struct plane *sil = departureLast; 
        departureLast = temp; //dongude belirlenen son elemani al
        departureLast->next = NULL;
        free(sil); //ucur
    }
    return planeID; //elemani dondur
}

// Inis kuyrugundan eleman cikar
void removeFromLanding(int planeID){
    if(landingFirst == NULL){
        return;
    }
    else if(planeID == landingFirst->planeID){
        if(landingFirst == landingLast){ //tek elemanli kuyruksa sil
            free(landingFirst);
            landingFirst = NULL;
            landingLast = NULL;
            return;
        }
        struct plane *temp = landingFirst;
        landingFirst = landingFirst->next; //birden fazla elaman var ve ilk eleman cikarilacaksa
        free(temp);
        return;
    }else if(planeID == landingLast->planeID){
        struct plane *temp = landingFirst;
        while(temp->next != landingLast){ //sonuncu elamani bul
            temp = temp->next;
        }
        struct plane *destroy = landingLast;
        landingLast = temp;
        landingLast->next = NULL; //sil
        free(destroy);
        return;
    }

	//buraya geldiyse arada bir elemandir
    struct plane *temp = landingFirst->next, *tempOnceki = landingFirst;
    while(temp != NULL){
        if(temp->planeID == planeID){
            tempOnceki->next = temp->next;
            free(temp);
            return;
        }
        tempOnceki = temp;
        temp = temp->next;
    }

    printf("Hata! %d ID'li Ucak Bulunamadi! \n\n",planeID);
}

// Kalkis kuyrugundan eleman cikar
void removeFromDeparture(int planeID){
    if(departureFirst == NULL){
        return;
    }
    else if(planeID == departureFirst->planeID){
        if(landingFirst == departureLast){ //tek elemanli kuyruksa sil
            free(departureFirst);
            departureFirst = NULL;
            departureLast = NULL;
            return;
        }
        struct plane *temp = departureFirst;
        departureFirst = departureFirst->next; //birden fazla elaman var ve ilk eleman cikarilacaksa
        free(temp);
        return;
    }else if(planeID == departureLast->planeID){
        struct plane *temp = departureFirst;
        while(temp->next != departureLast){ //sonuncu elamani bul
            temp = temp->next;
        }
        struct plane *destroy = departureLast;
        departureLast = temp;
        departureLast->next = NULL; //sil
        free(destroy);
        return;
    }

	//buraya geldiyse arada bir elemandir
    struct plane *temp = landingFirst->next, *tempOnceki = landingFirst;
    while(temp != NULL){
        if(temp->planeID == planeID){
            tempOnceki->next = temp->next;
            free(temp);
            return;
        }
        tempOnceki = temp;
        temp = temp->next;
    }

    printf("Hata! %d ID'li Ucak Bulunamadi! \n\n",planeID);
}

//inis listesini saydir
int FreePlace(){
    if(landingFirst == NULL) //yoksa 
        return 0;
    else if(landingFirst == landingLast) //bir tane elaman varsa
        return 1;
    else{
        struct plane *temp = landingFirst;
        int counter = 0;
        while(temp != NULL){ //gez
            counter++;
            temp = temp->next;
        }
        return counter;
    }
}

//kalkis listesini saydir
int CountDeparture(){
    if(departureFirst == NULL)
        return 0;
    else if(departureFirst == departureLast)
        return 1;
    else{
        struct plane *temp = departureFirst;
        int counter = 0;
        while(temp != NULL){
            counter++;
            temp = temp->next;
        }
        return counter;
    }
}

void ShowCapacity(){
	printf("+++HAVALIMANI DURUMU+++\n");
    printf("KAPASITE: %d\n",capacity);
    printf("INIS PISTI DOLULUK: %d\n",FreePlace());
	printf("INISE ACIK KULLANIM: %d\n",capacity-FreePlace());
	printf("KALKIS PISTI DOLULUK: %d\n",CountDeparture());
    printf("KALKISA ACIK: %d\n\n",capacity-CountDeparture());
}


int main()
{
    FILE *txtFile = fopen("input.txt", "r");
    if(txtFile == NULL)
        printf("input.txt Okunamadi!");
    else
    {
        while(!feof(txtFile)){ //dosya sonuna kadar oku
            char satir[100]; 
            fgets(satir,100,txtFile); //satiri oku

            int priorityID, planeID, landingTime, departureTime;
            priorityID = atoi(strtok(satir, " ")); //bosluga gore bol
            planeID = atoi(strtok(NULL, " "));
            landingTime = atoi(strtok(NULL, " "));
            departureTime = landingTime+1; //inis kalkis arasi 1 saat olmali

            if(landingTime > 24 || landingTime < 1){
                printf("%d ID'li Ucak Icin Talep Alinamadi! Lutfen Gecerli Saat Giriniz:\n",planeID,landingTime);
                continue;
            }
            if(departureTime == 25)
            	departureTime = 1;
            GetRequest(priorityID, planeID, landingTime, departureTime, 0);
            
			//Adim adim ucak indir
			CreateOutputTxt();
            printf("> > > > > Kule! Izin Talebine Cevap Vermek Icin Bir Tusa Basiniz! < < < < < <\n");
            getch();
        }
        printf("\n");
    }
    fclose(txtFile);
    CreateOutputTxt();

    PrintQueue(landingFirst);
    ShowCapacity();
	
	/*
	enqueue(99, 99, 7, 7, 0); //Kuyruga eleman ekleme fonksiyonu
	removeFromLanding(18);	// Inis kuyrugundan eleman cikar
	removeFromDeparture(18); // Kalkis kuyrugundan eleman cikar

	dequeueLanding(); //Inis Kuyrugundan elaman cikar
	CreateOutputTxt();
	
	dequeueDeparture();//Kalkis Kuyrugundan eleman cikar
	CreateOutputTxt();
	
	clearLanding();//Inis Kuyruk elemanlarini sil
	CreateOutputTxt();
	
	clearDeparture();//Kalkis Kuyruk elemanlarini sil
	CreateOutputTxt();
	
	pollLanding(); //Inis Kuyrugunda son elemani dondurur ve kuyruktan siler
	CreateOutputTxt();

	pollDeparture();//Kalkis Kuyrugunda son elemani dondurur ve kuyruktan siler
	CreateOutputTxt();
	*/
	
    return 0;
}

