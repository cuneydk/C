/*
	200201132 - Muhammet Cuneyd KURTBAS
	Bilgisayar Muhendisligi (I.Ogretim)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <iostream>
#include <math.h>
#include <sstream>
#define MAX_CHAR 256

int yaprak[7];
int yapraksay=17;
int array[8];
int arrayindex=0 ;
int arraygez=0;
int arraykontrol=23 ;
int dizisay=0;
int global_i;
int global_j;
char textDizisi[32];
int siraliYaprakDizisi[40];
int siralamaSayaci=0 ;
typedef struct SuffixTreeNode Node;
char yapraktakiVeri[50][50];
int parcalananDiziSayisi=0;
char text[100]; 
Node *root = NULL;
int okunanTextUzunlugu = 0;
									// KAYNAKCA
										struct SuffixTreeNode {
											struct SuffixTreeNode *children[MAX_CHAR];
											struct SuffixTreeNode *suffixLink;
											int start;
											int *end;

											int suffixIndex;
										};
										
										Node *lastNewNode = NULL;
										Node *activeNode = NULL;
										int count=0;
										int activeEdge = -1;
										int activeLength = 0;
										int remainingSuffixCount = 0;
										int leafEnd = -1;
										int *rootEnd = NULL;
										int *splitEnd = NULL;
										int size = -1; 
										
										Node *newNode(int start, int *end)
										{
											count++;
											Node *node =(Node*) malloc(sizeof(Node));
											int i;
											for (i = 0; i < MAX_CHAR; i++)
												node->children[i] = NULL;

											node->suffixLink = root;
											node->start = start;
											node->end = end;
											node->suffixIndex = -1;
											return node;
										}
										
										int edgeLength(Node *n) {
											return *(n->end) - (n->start) + 1;
										}
										
										int walkDown(Node *currNode)
										{
											if (activeLength >= edgeLength(currNode))
											{
												activeEdge =
												(int)text[activeEdge+edgeLength(currNode)]-(int)' ';
												activeLength -= edgeLength(currNode);
												activeNode = currNode;
												return 1;
											}
											return 0;
										}
										
										void extendSuffixTree(int pos)
										{
											leafEnd = pos;
											remainingSuffixCount++;
											lastNewNode = NULL;
											while(remainingSuffixCount > 0) {
												if (activeLength == 0) {	
													activeEdge = (int)text[pos]-(int)' ';
												}
												if (activeNode->children[activeEdge] == NULL)
												{
													activeNode->children[activeEdge] =
																		newNode(pos, &leafEnd);
													if (lastNewNode != NULL)
													{
														lastNewNode->suffixLink = activeNode;
														lastNewNode = NULL;
													}
												}
										
												else
												{
													Node *next = activeNode->children[activeEdge];
													if (walkDown(next))
													{
														continue;
													}
													if (text[next->start + activeLength] == text[pos])
													{
														if(lastNewNode != NULL && activeNode != root)
														{
															lastNewNode->suffixLink = activeNode;
															lastNewNode = NULL;
														}
														activeLength++;
														break;
													}
													splitEnd = (int*) malloc(sizeof(int));
													*splitEnd = next->start + activeLength - 1;
													Node *split = newNode(next->start, splitEnd);
													activeNode->children[activeEdge] = split;
													split->children[(int)text[pos]-(int)' '] = newNode(pos, &leafEnd);
													next->start += activeLength;
													split->children[activeEdge] = next;
										
													if (lastNewNode != NULL)
													{
														lastNewNode->suffixLink = split;
													}
													lastNewNode = split;
												}
												remainingSuffixCount--;
												if (activeNode == root && activeLength > 0) 
												{
													activeLength--;
													activeEdge = (int)text[pos - remainingSuffixCount + 1]-(int)' ';
												}
												
												else if (activeNode != root)
												{
													activeNode = activeNode->suffixLink;
												}
											}
										}
										
										void print(int nodestart, int nodeend)
										{
											int i;
											int printIndex=0;
											char arrayPrint[nodeend];
											for (i=nodestart; i<=nodeend; i++){
											
												arrayPrint[printIndex]=text[i] ;
												printIndex++;
												printf("%c", text[i]);
											}
											arrayPrint[printIndex]='\0';
											strcpy(yapraktakiVeri[parcalananDiziSayisi],arrayPrint);
											parcalananDiziSayisi++;	
												
										}
										
										void setSuffixIndexByDFS(Node *n, int labelHeight)
										{
											if (n == NULL) return;
										
											if (n->start != -1) 
											{
												print(n->start, *(n->end));
											}
											int leaf = 1;
											int i;
											for (i = 0; i < MAX_CHAR; i++)
											{
												if (n->children[i] != NULL)
												{
													if (leaf == 1 && n->start != -1){
													
														printf(" [%d]\n", n->suffixIndex);
														siraliYaprakDizisi[siralamaSayaci]=n->suffixIndex ;
												        siralamaSayaci++;
												        }
													leaf = 0;
													setSuffixIndexByDFS(n->children[i],labelHeight + edgeLength(n->children[i]));
												}
											}
											if (leaf == 1)
											{
												n->suffixIndex = size - labelHeight;
												printf(" [%d]\n", n->suffixIndex);
												siraliYaprakDizisi[siralamaSayaci]=n->suffixIndex ;
												siralamaSayaci++;
											}
										}
										
										void freeSuffixTreeByPostOrder(Node *n)
										{
											if (n == NULL)
												return;
											int i;
											for (i = 0; i < MAX_CHAR; i++)
											{
												if (n->children[i] != NULL)
												{
													freeSuffixTreeByPostOrder(n->children[i]);
												}
											}
											if (n->suffixIndex == -1)
												free(n->end);
											free(n);
										}
										
										void buildSuffixTree()
										{
											size = strlen(text);
											int i;
											rootEnd = (int*) malloc(sizeof(int));
											*rootEnd = - 1;

											root = newNode(-1, rootEnd);
										
											activeNode = root; 
											for (i=0; i<size; i++)
												extendSuffixTree(i);
											int labelHeight = 0;
											setSuffixIndexByDFS(root, labelHeight);
										
											freeSuffixTreeByPostOrder(root);
										}
										
void yeniYaprakOlustur(int x,int y,int dizi,char *stringarray)
{
	if(yapraksay==dizi && yapraksay!=17)
	{
		setcolor(GREEN);
		circle(x, y, 15);
		outtextxy(x +14, y - 13, stringarray);
		dizisay++;
	}
	else if(arraykontrol==dizi && arraykontrol!=23)
	{
		setcolor(GREEN);
		circle(x, y, 15);
		outtextxy(x +14, y - 13, stringarray);
		dizisay++;
	}
	else if(arrayindex!=0 && array[arraygez]==dizi && (dizisay==global_i || dizisay==global_j))
	{
		setcolor(GREEN);
		circle(x, y, 15);
		outtextxy(x +14, y - 13, stringarray);
		arraygez++;
		dizisay++;
		
	}
	else 
	{
		circle(x, y, 15);
		outtextxy(x +14, y - 13, stringarray);
		dizisay++;
	}
}

int sonekYarat()
{
	int x=300, y=100, j, i;
	int index=0,n=0,m=0;
	int gez=0;
	char arr[12][12];
	for(i=0;i<parcalananDiziSayisi;i++)
	{
		m=0;
		if(siraliYaprakDizisi[i]==-1)
		{
			strcpy(arr[gez],yapraktakiVeri[i+1]);
			gez++;
			strcpy(arr[gez],yapraktakiVeri[i+2]);
			gez++;
			if(i+2<=6)
			{
				for(j=i+3;j<i+5;j++)
				{
					for(int p=0;p<2;p++)
					{
						if(strcmp(arr[p],yapraktakiVeri[j])==0)
						{
							yaprak[index]=gez ;
							m=1 ;
						}
						else 
						{
							n=1 ;
						}
					
					}
					if(m==0)
					{
						gez++ ;
						yaprak[index]=gez ;
					}
				}
				index++;
				gez=0;
				
			}
			else {
				yaprak[index]=gez ;
			}
		}
	}	
	return gez ;
}


void search(char *acilanDosya){
	FILE *file ;
	char dosyadanOkunanMetin[40] ;
	if((file=fopen(acilanDosya,"r"))==NULL){
		printf("Okuma Yapilacak Dosya Konumunda Yok!\n");
	}
	else{
		fscanf(file, "%s",dosyadanOkunanMetin);
		}
	fclose(file);
	strcpy(textDizisi,dosyadanOkunanMetin);
	int i,j=0,sayac=0 ;
	strcpy(text, dosyadanOkunanMetin); 
	buildSuffixTree();
	char arananString[50];
	printf("Aranacak String Giriniz:");
	scanf("%s",arananString);
	
	int x=300 ;
	int y=100 ;
	int n=0,m=0;
	//int ex=0;
	int degisken[10];
	for(i=0;i<7;i++)
	{
		yaprak[i]=0;
	}
	for(i=0;i<5;i++)
	{
		array[i]=50;
	}
	int index=0;
	char arrKopyaYaprak[10][10];
	for(i=0;i<parcalananDiziSayisi;i++)
	{
		m=0;
		if(siraliYaprakDizisi[i]==-1)
		{
			strcpy(arrKopyaYaprak[index],yapraktakiVeri[i+1]);
			
			index++;
			strcpy(arrKopyaYaprak[index],yapraktakiVeri[i+2]);
			
			index++;
			if(i+2<=6)
			{
				
				for(j=i+3;j<i+5;j++)
				{
					
					for(int p=0;p<2;p++)
					{
						if(strcmp(arrKopyaYaprak[p],yapraktakiVeri[j])==0)
						{
							yaprak[i]=index ;
							m=1 ;
						}
						else 
						{
							n=1 ;
						}
					
					}
					if(m==0)
					{
						index++ ;
						yaprak[i]=index ;
					}
				}
				
				//ex++;
				index=0;
				
			}
			else {
				yaprak[i]=index ;
			}
			
		}
	
	}
	for(i=0;i<10;i++)
	{
		printf("%d\n",yaprak[i]);
	}
	int q=0,arananinUzunlugu=strlen(arananString),kontrol=0,sutun=0;
	for(i=0;i<parcalananDiziSayisi;i++)
	{
		
		if(siraliYaprakDizisi[i]==-1) 
		{
			if(yapraktakiVeri[i][q]==arananString[q])
			{
				q++;
				i--;
				kontrol++;
			}
			else
			{
				if(kontrol!=0)
				{
					for(j=0;j<yaprak[i];j++)
					{
						if(yapraktakiVeri[i+j+1][sutun]==arananString[q] && q<arananinUzunlugu)
						{
							sutun++;
							q++;
							j--;
							kontrol++;
						}
						else if(arananinUzunlugu==kontrol)
						{
							array[0]=siraliYaprakDizisi[i] ;
							array[1]=siraliYaprakDizisi[i+j+1];
							global_i=i ;
							global_j=i+j+1 ;
							
							break;
						}
						
						
					}
					if (arananinUzunlugu==kontrol)
						{
							break ;
						}
						else
						{
							kontrol=0;
							q=0 ;
						}
				}
				else
				{
					q=0;
					i=i+yaprak[i]; //i degerini alt yaprak sayisi kadar (yaprak) arttir
				}
				
				
			}
		}
		else
		{
			
			if(yapraktakiVeri[i][q]==arananString[q])
			{
				if(arananinUzunlugu==kontrol)
					{
				arraykontrol=siraliYaprakDizisi[i];
				break ;
					}
				q++;
				i--;
				kontrol++;
					
			}
			 
			else
			{
				q=0 ;
				kontrol=0;
			}
		}
			
	}
	if(kontrol==arananinUzunlugu)
	{
	if(arraykontrol==23)
	{
		for(i=0;i<5;i++)
		{
			if(array[i]!=50)
			{
				arrayindex++ ;
			}
		}
	}		
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "None");
	
	int x=320 ;
	int y=130,j,q,m=0 ;
	int value=0,n=0;
	
	int sonek=sonekYarat();
	for(q=0;q<sonek;q++)
	{
		printf("%d\n",yaprak[q]);
	}
	value=0;
	int tvalue=1;
	int dd=1 ;
	for(q=0;q<parcalananDiziSayisi+1;q++)
	{
		
		char str2[20][20];
		if(q==0)
		{
		setcolor(WHITE);
		outtextxy(x - 2, y - 3, textDizisi);
		}
		else
		{
			if(siraliYaprakDizisi[q-1]==-1)
			{
				setcolor(YELLOW);
				
				yeniYaprakOlustur((x-y/(q)+(q*10)), y + 45,siraliYaprakDizisi[q-1],yapraktakiVeri[q-1]);
				line(x, y, (x-y/(q)+(q*10)), y + 45);
				int xvalue=(x-y/(q)+(q*10));
				int valuey=y+45 ;
				if(value!=-1)
				{
					for(j=0;j<yaprak[value];j++)
					{
						setcolor(YELLOW);
						if(j==0)
						{
							yeniYaprakOlustur(((xvalue-valuey)*tvalue/2)+(q*10),y+90+(tvalue*20),siraliYaprakDizisi[q+j],yapraktakiVeri[q+j]);
							line(xvalue, valuey, ((xvalue-valuey)*tvalue/2)+(q*10), y+90+(tvalue*20)) ;
							tvalue++;
						}
						else
						{
							yeniYaprakOlustur((((xvalue+valuey)*(tvalue))/4)+(q*10),y+90,siraliYaprakDizisi[q+j],yapraktakiVeri[q+j]);
							line(xvalue, valuey, (((xvalue+valuey)*(tvalue))/4)+(q*10), y+90) ;
							tvalue++;
						}

					}
					q=q+yaprak[value];
					value++;
				}
				
			}
			else
			{
				setcolor(WHITE);
				yeniYaprakOlustur(((x-y)*dd/2)+(q*12),y+180+(dd*20),siraliYaprakDizisi[q-1],yapraktakiVeri[q-1]);
				line(x, y, ((x-y)*dd/2)+(q*10), y+180+(dd*20)) ;
				dd++;
			}
		}
	}
	
	getch();

	closegraph();
	}	
	else
	{
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "None");
	
	int x=320 ;
	int y=130,j,q,m=0 ;
	int value=0,n=0;
	
	int sonek=sonekYarat();
	for(q=0;q<sonek;q++)
	{
		printf("%d\n",yaprak[q]);
	}
	value=0;
	int tvalue=1;
	int dd=1 ;
	for(q=0;q<parcalananDiziSayisi+1;q++)
	{
		
		char str2[20][20];
		if(q==0)
		{
		setcolor(WHITE);
		outtextxy(x - 2, y - 3, textDizisi);
		}
		else
		{
			if(siraliYaprakDizisi[q-1]==-1) // -1 ise o node da dallanma olacaktir
			{
				setcolor(YELLOW);
				//dallanan node ait grafilerin yazdirilmasi
				yeniYaprakOlustur((x-y/(q)+(q*10)), y + 45,siraliYaprakDizisi[q-1],yapraktakiVeri[q-1]);
				line(x, y, (x-y/(q)+(q*10)), y + 45);
				
				//node ait konumlari tutmamiz lazim cunku dallanacak nodelari ordan devam ettirecegiz
				int xvalue=(x-y/(q)+(q*10));
				int valuey=y+45 ;
				if(value!=-1)
				{
					for(j=0;j<yaprak[value];j++)
					{
						setcolor(YELLOW);
						if(j==0)
						{
							yeniYaprakOlustur(((xvalue-valuey)*tvalue/2)+(q*10),y+90+(tvalue*20),siraliYaprakDizisi[q+j],yapraktakiVeri[q+j]);
							line(xvalue, valuey, ((xvalue-valuey)*tvalue/2)+(q*10), y+90+(tvalue*20)) ;
							tvalue++;
						}
						else
						{
							yeniYaprakOlustur((((xvalue+valuey)*(tvalue))/4)+(q*10),y+90,siraliYaprakDizisi[q+j],yapraktakiVeri[q+j]);
							line(xvalue, valuey, (((xvalue+valuey)*(tvalue))/4)+(q*10), y+90) ;
							tvalue++;
						}
					}
					q=q+yaprak[value];
					value++;
				}
			}
			else
			{
				setcolor(WHITE);
				//Grafik ve ciktilari yazdiran fonksiyona degerleri gonder
				yeniYaprakOlustur(((x-y)*dd/2)+(q*12),y+180+(dd*20),siraliYaprakDizisi[q-1],yapraktakiVeri[q-1]);
				line(x, y, ((x-y)*dd/2)+(q*10), y+180+(dd*20)) ; //nodelari baglayan cizgileri ciz
				dd++;
			}
		}
	}
	outtextxy(320,370,"B U L U N A M A D I!");
	getch();
	closegraph();
	}
}
void longestsub(char *acilanDosya)
{
	FILE *dosya ;
	char dosyadanOkunanMetin[40] ;
	if((dosya=fopen(acilanDosya,"r"))==NULL){
		printf("Okuma Yapilacak Dosya Konumunda Yok!\n");
	}
	else{
		fscanf(dosya, "%s",dosyadanOkunanMetin);
		}
	fclose(dosya);
	strcpy(textDizisi,dosyadanOkunanMetin);
	int i,j=0,sayac=0 ;
	strcpy(text, dosyadanOkunanMetin);
	buildSuffixTree();
	int max[parcalananDiziSayisi] ;
	for(i=0;i<parcalananDiziSayisi;i++)
	{
		max[i]=strlen(yapraktakiVeri[i]);
	}
	int gecici ;
	for(i=0;i<parcalananDiziSayisi;i++)
	{
        for(j=0;j<parcalananDiziSayisi;j++)
		{
            if(max[j]<=max[i])
			{
                gecici=max[i];  
                max[i]=max[j];
                max[j]=gecici;
            }
        }
    }
	int sayacson=0,tutac ;
    for(i=0;i<parcalananDiziSayisi;i++)
	{
		if(strlen(yapraktakiVeri[i])==max[0])
		{
			tutac=siraliYaprakDizisi[i];
			for(j=0;j<parcalananDiziSayisi;j++)
			{
				if(strcmp(yapraktakiVeri[i],yapraktakiVeri[j])==0)
				{
					sayacson++ ;
				}
			}
			break;
		}
	}
	
	for(i=0;i<parcalananDiziSayisi;i++){
		printf("\t%s : [%d]\n",yapraktakiVeri[i],siraliYaprakDizisi[i]);
	}
	yapraksay=tutac ;
	
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "None");
	
	int x=320 ;
	int y=130,q,m=0 ;
	int value=0,n=0;
	
	int sonek=sonekYarat();
	for(q=0;q<sonek;q++)
	{
		printf("%d\n",yaprak[q]);
	}
	value=0;
	int tvalue=1;
	int dd=1 ;
	for(q=0;q<parcalananDiziSayisi+1;q++)
	{
		
		char str2[20][20];
		if(q==0)
		{
		setcolor(WHITE);
		outtextxy(x - 2, y - 3, textDizisi);
		}
		else
		{
			if(siraliYaprakDizisi[q-1]==-1)
			{
				setcolor(YELLOW);
				
				yeniYaprakOlustur((x-y/(q)+(q*10)), y + 45,siraliYaprakDizisi[q-1],yapraktakiVeri[q-1]);
				line(x, y, (x-y/(q)+(q*10)), y + 45);
				int xvalue=(x-y/(q)+(q*10));
				int valuey=y+45 ;
				if(value!=-1)
				{
					for(j=0;j<yaprak[value];j++)
					{
						setcolor(YELLOW);
						if(j==0)
						{
							yeniYaprakOlustur(((xvalue-valuey)*tvalue/2)+(q*10),y+90+(tvalue*20),siraliYaprakDizisi[q+j],yapraktakiVeri[q+j]);
							line(xvalue, valuey, ((xvalue-valuey)*tvalue/2)+(q*10), y+90+(tvalue*20)) ;
							tvalue++;
						}
						else
						{
							yeniYaprakOlustur((((xvalue+valuey)*(tvalue))/4)+(q*10),y+90,siraliYaprakDizisi[q+j],yapraktakiVeri[q+j]);
							line(xvalue, valuey, (((xvalue+valuey)*(tvalue))/4)+(q*10), y+90) ;
							tvalue++;
						}
					}
					q=q+yaprak[value];
					value++;
				}
			}
			else
			{
				setcolor(WHITE);
				yeniYaprakOlustur(((x-y)*dd/2)+(q*12),y+180+(dd*20),siraliYaprakDizisi[q-1],yapraktakiVeri[q-1]);
				line(x, y, ((x-y)*dd/2)+(q*10), y+180+(dd*20)) ;
				dd++;
			}
		}
	}
	char son[4]=" ";
	son[1]=' ';
	son[2]=sayacson+'0';
	son[3]='\0';
	outtextxy(400, 450, son);
	getch();
	closegraph();
}
void tekrar(char *acilanDosya)
{
	FILE *dosya ;
	char dosyadanOkunanMetin[40] ;
	if((dosya=fopen(acilanDosya,"r"))==NULL){
		printf("Okuma Yapilacak Dosya Konumunda Yok!\n");
	}
	else{
		fscanf(dosya, "%s",dosyadanOkunanMetin);
		}
	fclose(dosya);
	strcpy(textDizisi,dosyadanOkunanMetin);
	int i,j=0,sayac=0 ;
	strcpy(text, dosyadanOkunanMetin); buildSuffixTree();
	int max1[parcalananDiziSayisi];
	for(i=0;i<parcalananDiziSayisi;i++)
	{
		max1[i]=0;
	}
	
	for(i=0;i<parcalananDiziSayisi;i++)
	{
		for(j=0;j<parcalananDiziSayisi;j++)
		{
			if(strcmp(yapraktakiVeri[i],yapraktakiVeri[j])==0)
			{
				max1[i]=max1[i]+1 ;
			}
		}
	}
	int max2[parcalananDiziSayisi];
	for (i=0;i<parcalananDiziSayisi;i++){
		max2[i]=max1[i];
	}
	
	for(i=0;i<parcalananDiziSayisi;i++)
	{
		printf("%d\n",max2[i]);
	}
	int gecici ;
	for(i=0;i<parcalananDiziSayisi;i++)
	{
        for(j=0;j<parcalananDiziSayisi;j++)
		{
            if(max2[j]<=max2[i])
			{
                gecici=max2[i];  
                max2[i]=max2[j];
                max2[j]=gecici;
            }
        }
    }
    
    int sayacson=0,tutac ;
    for(i=0;i<parcalananDiziSayisi;i++)
	{
		printf("max1 %d",max1[i]);
		if(max1[i]==max2[0])
		{
			tutac=siraliYaprakDizisi[i];
			sayacson=max2[0];
			break;
		}
	}
	yapraksay=tutac ;
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "None");
	
	int x=320 ;
	int y=130,q,m=0 ;
	int value=0,n=0;
	
	int sonek=sonekYarat();
	for(q=0;q<sonek;q++)
	{
		printf("%d\n",yaprak[q]);
	}
	value=0;
	int tvalue=1;
	int dd=1 ;
	for(q=0;q<parcalananDiziSayisi+1;q++)
	{
		
		char str2[20][20];
		if(q==0)
		{
		setcolor(WHITE);
		outtextxy(x - 2, y - 3, textDizisi);
		}
		else
		{
			if(siraliYaprakDizisi[q-1]==-1)
			{
				setcolor(YELLOW);
				
				yeniYaprakOlustur((x-y/(q)+(q*10)), y + 45,siraliYaprakDizisi[q-1],yapraktakiVeri[q-1]);
				line(x, y, (x-y/(q)+(q*10)), y + 45);
				int xvalue=(x-y/(q)+(q*10));
				int valuey=y+45 ;
				if(value!=-1)
				{
					for(j=0;j<yaprak[value];j++)
					{
						setcolor(YELLOW);
						if(j==0)
						{
							yeniYaprakOlustur(((xvalue-valuey)*tvalue/2)+(q*10),y+90+(tvalue*20),siraliYaprakDizisi[q+j],yapraktakiVeri[q+j]);
							line(xvalue, valuey, ((xvalue-valuey)*tvalue/2)+(q*10), y+90+(tvalue*20)) ;
							tvalue++;
						}
						else
						{
							yeniYaprakOlustur((((xvalue+valuey)*(tvalue))/4)+(q*10),y+90,siraliYaprakDizisi[q+j],yapraktakiVeri[q+j]);
							line(xvalue, valuey, (((xvalue+valuey)*(tvalue))/4)+(q*10), y+90) ;
							tvalue++;
						}
						
						
					}
					q=q+yaprak[value];
					value++;
				}
				
			}
			else
			{
				setcolor(WHITE);
				yeniYaprakOlustur(((x-y)*dd/2)+(q*12),y+180+(dd*20),siraliYaprakDizisi[q-1],yapraktakiVeri[q-1]);
				line(x, y, ((x-y)*dd/2)+(q*10), y+180+(dd*20)) ;
				dd++;
			}
		}
	}
	char son[4]=" " ;
	son[1]=' ';
	son[2]=sayacson+'0';
	son[3]='\0';
	outtextxy(400, 450, son);
	
		getch();

	closegraph();
		
}


void AgacOlustur(char *acilanDosya){
	FILE *dosya ;
	char dosyadanOkunanMetin[40] ; //Okunacak karakterleri tutan dizi
	if((dosya=fopen(acilanDosya,"r"))==NULL){
		printf("Okuma Yapilacak Dosya Konumunda Yok!\n");
	}
	else{
		fscanf(dosya, "%s",dosyadanOkunanMetin); //Dosyadan okunani diziye at
		}
	fclose(dosya);
	strcpy(textDizisi,dosyadanOkunanMetin);
	int w,j=0,sayac=0 ;
	strcpy(text, dosyadanOkunanMetin);  
	buildSuffixTree();
	
	int okunanTextUzunlugu=strlen(dosyadanOkunanMetin); //textin uzunlugunu al
	char on_ek[okunanTextUzunlugu][okunanTextUzunlugu]; //on ek icin iki boyutlu dizi olustur
	
	for(w=0;w<okunanTextUzunlugu;w++){ 
		if(w==okunanTextUzunlugu-1){ //textin sonuna geldiysen
			strcpy(on_ek[w],dosyadanOkunanMetin); //okunan tum texti diziye aktar
		}
		else{ //dongu textin sonunda degilse
		
		//memset: C parametresi ile gösterilen bellek bölgesindeki karakteri (unsigned char), dest parametresindeki bellek bölgesinin ilk n parametre deðeri kadar byte'ýna kopyalar
		memset(on_ek[w], '\0', sizeof(on_ek[w]));
		strncpy(on_ek[w],dosyadanOkunanMetin,w+1); // dongu dondukce part part on ek dizilerini olustur
		}
	}
	//benzer mantikla son ekleri olustur
	char sonek[okunanTextUzunlugu+1][okunanTextUzunlugu+1] ;
	char sonek2[okunanTextUzunlugu+1][okunanTextUzunlugu+1] ;
	int l=0 ;
	
	for(w=0;w<okunanTextUzunlugu;w++){ //0 dan text uzunlugu kadar donguyu baslat
		for(j=l;j<okunanTextUzunlugu;j++){ //1 den text uzunlugu kadar ic donguye gir
			
			sonek[w][j-w]=dosyadanOkunanMetin[j] ; // son en dizisinin 0,1 - 0,2 - 0,3 .... 1,1 - 1,2, 1,3 ... seklinde textin dongudeki degeri kadar sonek dizilerini olustur 
			
			if(w==0){ // bu kontrol sadece 1 kez calisacak, yani dis dongu 0 iken ve ic dongu 1 iken
				if(j==okunanTextUzunlugu-1){ //textin tamami ise, yani orjinal hali ise
				sonek[w][okunanTextUzunlugu]='\0'; // iki boyutlu [0][text uzunlugu] dizisine bitis karakteri ekle
					  }
			}
			else if(w!=0){ //dis dongu dondugu surece
				if(j==okunanTextUzunlugu-1){ 
				sonek[w][okunanTextUzunlugu-w]='\0'; //kuculen son ek dizilerini bitis karakteri ile sonlandir
					  }
			}
			
		}
		strcpy(sonek2[w],sonek[w]); //diziyi kopyala
	}
	
	
	sayac=0 ;
	for(w=0;w<okunanTextUzunlugu;w++){
		for(j=0;j<okunanTextUzunlugu;j++){
			
		if(strcmp(sonek2[w],on_ek[j])==0){ //Str1 parametresi ile gösterilen karakter dizisini str2 parametresi ile gösterilen karakter dizisi ile karþýlaþtýrýr. Eðer her iki karakter dizisindeki tüm karakterler ayný ise sýfýr deðeri, aksi takdirde sýfýrdan farklý bir deðer geri döndürür.
			sayac++ ;
			}	
		}
	}
	if(sayac>=3){ // son ek ve on ek sayisi textin kendisi haric daha fazla esitlik varsa son ek agaci olusturulamaz
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "None");
	
	int x=320 ;
	int y=130,j,q,m=0 ;
	int value=0,n=0;
	
	
	getch();
	closegraph();
	
	}
	else // sayaca gore esitlik yoksa agac olusturulabilir
	{
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "None");
	
	//grafigin baslangic konumu
	int x=320 ;
	int y=130,j,q,m=0 ;
	int value=0,n=0;
	
	int sonek=sonekYarat(); //suffix degerlerini bastir
	for(q=0;q<sonek;q++)
	{
		printf("%d\n",yaprak[q]);
	}
	value=0;
	int tvalue=1;
	int dd=1 ;
	for(q=0;q<parcalananDiziSayisi+1;q++) //parcalanan text sonucunda olusan dizi sayisi adendince
	{
		
		char str2[20][20]; /////
		if(q==0)
		{
		setcolor(WHITE);
		outtextxy(x - 2, y - 3, textDizisi); //okunan string tamamini kok olarak en basa yazdir
		}
		else
		{
			if(siraliYaprakDizisi[q-1]==-1) // dallanma olan bir node'da isek 
			{
				setcolor(YELLOW);
				
				yeniYaprakOlustur((x-y/(q)+(q*10)), y + 45,siraliYaprakDizisi[q-1],yapraktakiVeri[q-1]);
				line(x, y, (x-y/(q)+(q*10)), y + 45);
				int xvalue=(x-y/(q)+(q*10));
				int valuey=y+45 ;
				if(value!=-1)
				{
					for(j=0;j<yaprak[value];j++)
					{
						setcolor(YELLOW);
						if(j==0)
						{
							yeniYaprakOlustur(((xvalue-valuey)*tvalue/2)+(q*10),y+90+(tvalue*20),siraliYaprakDizisi[q+j],yapraktakiVeri[q+j]);
							line(xvalue, valuey, ((xvalue-valuey)*tvalue/2)+(q*10), y+90+(tvalue*20)) ;
							tvalue++;
						}
						else
						{
							yeniYaprakOlustur((((xvalue+valuey)*(tvalue))/4)+(q*10),y+90,siraliYaprakDizisi[q+j],yapraktakiVeri[q+j]);
							line(xvalue, valuey, (((xvalue+valuey)*(tvalue))/4)+(q*10), y+90) ;
							tvalue++;
						}
						
						
					}
					q=q+yaprak[value];
					value++;
				}
				
			}
			else
			{
				setcolor(WHITE);
				yeniYaprakOlustur(((x-y)*dd/2)+(q*12),y+180+(dd*20),siraliYaprakDizisi[q-1],yapraktakiVeri[q-1]);
				line(x, y, ((x-y)*dd/2)+(q*10), y+180+(dd*20)) ;
				dd++;
			}
		}
	}
		getch();

		closegraph();
	}
	
}

void mainMenu () {
    printf("\n........................................................\n");
    printf("    		SUFFIX TREE ISLEM MENUSU    				\n");
    printf("........................................................\n");
    printf("* S KATARI ICIN SONEK AGACI OLUSTURULABILIR MI?	<< 1 >> \n\n");
    printf("* S KATARI ICINDE P KATARI VAR MI?              << 2 >> \n\n");
    printf("* S KATARI ICINDE TEKRAR EDEN EN UZUN ALT KATAR?<< 3 >> \n\n");
    printf("* S KATARI ICINDE EN COK TEKRAR EDEN AL KATAR?  << 4 >> \n\n");
    printf("........................................................\n");

}

int main(int argc, char *argv[]) {
	
	
	printf("DOSYA ADINI GIRINIZ (s1.txt)	:\n");
	char acilanDosya[30] ;
	scanf("%s",&acilanDosya);
	char * file ;
	file=acilanDosya ;
	
int selection=-1;
while (1) {
        mainMenu();
        printf("\tISLEM NUMARASINI GIRINIZ: "); scanf("%d",&selection);

            switch(selection) {
                case 1:
					AgacOlustur(file);
                    break;
                case 2:
                    search(file);
                    break;
                case 3:
                    longestsub(file);
                    break;
                case 4:
                    tekrar(file);
                    break;
                default:
                    printf("<<HATALI ISLEM NUMARASI GIRDINIZ!>>>\n");
        }
    }	
	return 0;
}

