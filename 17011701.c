#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define VERTEX 300000	//VERTEX DEGISKENI TEKRAR TEKRAR TANIMLANMAMASI ICIN PROGRAM BASLANGICINDA 30000 OLARAK MAKRO SEKLINDE TANIMLANIR
//GEREKLI STRUCT YAPILARIMIZIN TANIMLANMASI;
struct movies{
	char name[100];
};

struct actors{
	char name[100];
};

struct Qnode{
	int s;
	struct node* next;
};

struct queue{
	struct Qnode* front;
	struct Qnode* rear;
};

struct node{
	int vertex;
    struct node* next;
};

struct Graph {
    int numVertices;
    struct node** adjList;
};

FILE* fp;	//1DEN FAZLA FONKSIYON ICERISINDE DOSYA ISLEMLERI YAPILACAGINDAN DOLAYI GLOBAL OLARAK TANIMLANIR
//GEREKLI STRUCT YAPILARIMIZIN BASLANGICTA INITILIAZE EDILMESI GEREKTIGINDEN BU ISLEM GLOBAL OLARAK YAPILIR(ALTTAKI 4 SATIR ICIN);
struct Graph* graph;
struct queue* createQueue(struct queue*);
struct movies filmler[15000];
struct actors oyuncular[200000];
int movieNumber,actorSayisi,movieSayisi = 0,frequency[50] = {0};	//FARKLI FONKSIYONLARDAN DEGERLERININ AYRI AYRI MUDAHALE EDILMESI ICIN
//GEREKLI DEGISKENLERIN GLOBAL OLARAK TANIMLANMASI.

struct Qnode* createNodeQueue(int Index){	//KUYRUGA EKLENICEK DUGUMU OLUSTURUR;INDEX DEGERI ALIR VE YENI NODE U DONDURUR.
	struct Qnode* newNode = (struct Qnode*) malloc(sizeof(struct Qnode));
	newNode->s = Index;
	newNode->next = NULL;
	return newNode;
}

struct queue* createQueue(struct queue* kuyruk){	//YENI KUYRUK OLUSTURMA;KUYRUK TIPINDE PARAMETRE ALIR VE KUYRUK TIPINDE DEGISKEN RETURN EDER
	kuyruk = (struct queue*) malloc(sizeof(struct queue));
	kuyruk->front = NULL;
	kuyruk->rear = NULL;
	
	return kuyruk;
}

void enqueue(struct queue** kuyruk, int Index){	//KUYRUGA EKLEME ISLEMI;KUYRUK TIPINDE VE INT TIPINDE INDEX ALIR.RETURN DEGERI YOKTUR(VOID FONKSIYON)
	struct Qnode* newNode = createNodeQueue(Index);

	if((*kuyruk)->rear == NULL){
		(*kuyruk)->rear = newNode;
		(*kuyruk)->front = newNode;
	}
	else{
		(*kuyruk)->rear->next = newNode;
		(*kuyruk)->rear = newNode;
	}

}

int dequeue(struct queue** kuyruk){	//KUYRUKTAN CEKME ISLEMI;KUYRUK TIPINDE PARAMETRE ALIR,INT TIPINDE INDIS DONDURUR
	
	if((*kuyruk)->front == NULL){
		return -3;
	}
	struct Qnode* temp;
	int x = (*kuyruk)->front->s;
	temp = (*kuyruk)->front;
	(*kuyruk)->front = (*kuyruk)->front->next;
	if((*kuyruk)->front == NULL){
		(*kuyruk)->rear = NULL;
	}
	free(temp);
	return x;
}

int isQueueEmpty(struct queue* queue){	//GELEN KUYRUK TIPLI PARAMETRELI KUYRUK MEVCUTMU?INTEGER TIPINDE DEGER DONDURUR.
	if(queue->front == NULL)//BULUNMUYOR ISE 1 DONDURUR
		return 1;
	
	else	//KUYRUK MEVCUT ISE 0 DONDURUR
		return 0;
	
}

struct node* createNode(int v){	//NODE OLUSTURULUR;INT TIPINDE PARAMETRE ALIR VE NODE TIPINDE DEGER DONDURUR.
    struct node* newNode;
    newNode = (struct node*) malloc(sizeof(struct node)); 
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

struct Graph* createGraph(int nVertex){	//GRAPH OLUSTURULUR;INT TIPINDE PARAMETRE ALIR VE GRAPH TIPINDE DEGER DONDURUR.
    int i;
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->numVertices = nVertex;
    graph->adjList = (struct node**) malloc(nVertex*sizeof(struct node*));
    for (i = 0; i < nVertex ;i++) {
        graph->adjList[i] = NULL;
    }
    return graph;
}

void addEdge(struct Graph* graph, int source, int destination){	//EDGE EKLENILIR;GRAPH 1,INTEGER OLMAK UZERE 2(KAYNAK VE HEDEF) PARAMETRE ALIR.DONUS TIPI OLMAZ(VOID)
	struct node* newNode = createNode(destination);
	newNode->next = graph->adjList[source];
	graph->adjList[source] = newNode;
	newNode = createNode(source);
	newNode->next = graph->adjList[destination];
	graph->adjList[destination] = newNode;
}

void BFSforAll(struct queue** kuyruk, int startingIndex){	//TUM AKTORLERIN KEVIN BACON SAYISINI BULAN FONKSIYON;KUYRUK TIPINDE VE
//INTEGER(BASLANGIC INDISI) TIPINDE PARAMETRE ALIR.DONUS DEGERI YOKTUR.
	int i,v;
	struct node* temp;
	int visited[VERTEX];
	for(i = 0; i < VERTEX; i++)	//AKTORLERE BASLANGIC DEGERI ATANIR
		visited[i] = -1;
	
	enqueue(kuyruk,startingIndex);	//ILK GELEN DUGUM EKLENIR
	visited[startingIndex] = 0;
	
	while(!isQueueEmpty((*kuyruk))){
		v = dequeue(kuyruk);	//KUYRUKTAN CEKILME ISLEMI
		temp = graph->adjList[v];	//KOMSU DUGUMLERIN ISLEME ALINMASI
		while(temp != NULL){
			if(visited[temp->vertex] == -1){//DAHA ONCE GEZILMEYEN DUGUMLER(AKTORLER) BULUNUR
				enqueue(kuyruk,temp->vertex);	//KUYRUGA EKLENIR
				visited[temp->vertex] = visited[v] + 1;
			}
			temp = temp->next;	//ITERASYON ISLEMI
		}
	}
	
	for(i = movieNumber; i < actorSayisi; i++){	//FILM SAYISINDAN BASLAR->AKTOR SAYISI KADAR ILERLER
		if(visited[i] != -1)	//DAHA ONCE GEZILMEDIYSE
			frequency[visited[i]/2]++;	//SADECE AKTORLERLE ISLEM YAPMAK ICIN FILMLERI ES GECMEMIZ GEREK.BUNDAN DOLAYI MEVCUT INDISIN YARISINDAN ISLEM YAPILIR.
		
		else
			frequency[49]++;	//KEVIN BACON SAYISININ SONSUZ OLANLARIN HESAPLANMASI
	}
	
	for(i=0;i < 49; i++){//KEVIN BACON SAYILARININ EKRANA YAZDIRILMASI;
		if(frequency[i]>0)
			printf("%d Kisinin Kevin Bacon Sayisi = %d.\n", frequency[i], i);
				
	}
	// SONSUZ KEVIN BACON SAYISI OLANLARIN EKRANA YAZDIRILMASI.
	printf("%d Kisinin Kevin Bacon Sayisi = Sonsuz", frequency[49]);
}

void graphDoldur(struct Graph* graph, char* filename){	//GRAPHI DOLDURAN FONKSIYON;GRAPH TIPINDE VE CHAR STRING TIPINDE(DOSYA ADI ICIN) DEGISKEN ALIR DONUS DEGERI YOKTUR.
	char* token,*film,*aktor,string[5000];
	int keep;	//MEVCUT INDISDEKI DEGERIN KONTROLU ICIN KULLANILAN DEGISKEN

	fp = fopen(filename,"r");
	printf("%s Adli Dosyanin Acilma Islemi Baslatildi Lutfen Bekleyin...\n", filename);
	if(fp == NULL)	//DOSYA ACILAMADIYSA
		printf("Dosya Acilamadi!");
	
	else{	//DOSYA ACILDIYSA
		while(fgets(string,5000,fp)){	//ILK 5000 KELIMEYI OKUR STRING DIZISINE ATAR(5000 SAYISI STRING DIZISININ BOYUTUNDAN DOLAYIDIR)
			//ALTTAKI 3 SATIRDA KULLANILAN STRTOK FONKSIYONLARINDA ILK OPERANDA OKUMA YAPILIR,IKINCI OPERANDDAKI DEGER REFERANSI ILE PARCALAYARAK OKUYOR	
			token = strtok(string,"\n");
			film = strtok(token,"/");
			aktor = strtok(NULL,"/");
			strcpy(filmler[movieSayisi].name, film);	//IKINCI OPERAND,ILK OPERANDA KOPYALANIR
			while(aktor != NULL){
				keep = checkIndex(aktor);//AKTORUN MEVCUTLUK KONTROLU(MEVCUT ISE INDIS TUTAR,MEVCUT DEGIL ISE -1 TUTAR)
				if(keep == -1){	//AKTOR DAHA ONCEDEN OKUNMADIYSA YENI OLUSTURUYOR VE EDGE EKLENIYOR.
					strcpy(oyuncular[actorSayisi-movieNumber].name,aktor);
					addEdge(graph,movieSayisi,actorSayisi);
					actorSayisi++;
				}
				else{	//OKUNDUYSA EDGE EKLIYOR
					addEdge(graph,movieSayisi,keep);
				}	
				aktor = strtok(NULL,"/");
			}
				movieSayisi++;
		}
	}
	
	fclose(fp);
}

int checkIndex(char* string){	//GONDERILEN CHAR STRINGIN VARLIGININ KONTROLU
	int i=0;
	while(i<actorSayisi-movieNumber){
		if(!strcmp(oyuncular[i].name,string))	//AKTOR MEVCUT ISE INDISINI DONDURUYOR
			return movieNumber + i;
		i++;
	}
	return -1;	//MEVCUT DEGIL ISE
}

int BFS(struct queue** kuyruk, int startingIndex, int index, int type){	//SPESIFIK OLARAK PARAMETRESI GONDERILEN AKTORE BREADTH FIRST SEARCH YAPILMASI;
//KUYRUK TIPINDE 1 VE INTEGER TIPINDE 3 PAREMETRE ALIR.INTEGER TIPINDE DEGER DONDURUR.
	if(startingIndex == index)	//GIRILEN AKTOR,MEVCUT INDISDE ISE KENDISINE ESITTIR VE KEVIN BACON SAYISI 0DIR
		return 0;
	
	else{	//EGER DEGILSE
		int i,v;
		//FONKSIYON ICERISINDEKI DEGERLERINI KAYBETMEMELERI ICIN STATIC TANIMLANAN DEGISKENLERIMIZ;
		static counter = 0;	
		static int visited[VERTEX];
		static char* name;
		
		struct node* temp;
		for(i = 0; i < VERTEX; i++)	//AKTORLERE BASLANGIC DEGERI ATANIR(DAHA ONCEDEN GEZILIP GEZILMEDIGINI KONTROL ETMEK ICIN).
			visited[i] = -1;
		
		enqueue(kuyruk,startingIndex);	//KUYRUGA EKLENIR
		visited[startingIndex] = 0;
		
		while(!isQueueEmpty((*kuyruk))){	//KUYRUGUN BOS OLUP OLMAMASININ KONTROLU
			v = dequeue(kuyruk);
			temp = graph->adjList[v];
		
			while(temp != NULL){//BOS OLMADIKCA ITERASYONA DEVAM EDILIR;
				if(visited[temp->vertex] == -1){
					enqueue(kuyruk,temp->vertex);
					visited[temp->vertex] = visited[v] + 1;
	
					if(temp->vertex == index){
						int tempp = visited[temp->vertex]/2;
						if(type == 2)
							return tempp;
						
						//V DEGISKENINDEKI MEVCUT DEGER FILM SAYISINDAN BUYUK VEYA ESIT ISE;
						//OYUNCUYU YAZDIR,DEGIL ISE FILMI YAZDIR;
						(v>=movieNumber?printf("%s\n",oyuncular[v-movieNumber].name):printf("%s\n",filmler[v].name));
						
						//KUYRUGU YENIDEN OLUSTURMA;
						while(!isQueueEmpty(*kuyruk))
							dequeue(kuyruk);
						*kuyruk = createQueue(*kuyruk);
						
						counter++;
						if(v != startingIndex)	//MEVCUT INDISDE DEGIL ISE
							return BFS(kuyruk,startingIndex,v,1);	//BREADTH FIRST SEARCH ISLEMINE RECURSIVE OLARAK DEVAM EDILIR
						
						else
							return counter/2;	//MEVCUT INDISE VARMIS ISE AKTOR VE FILMLER 2 KERE SAYILDIGI ICIN YARISI RETURN EDILIR.
						
					}
				}
				temp = temp->next;	//ITERASYON ISLEMI
			}
		}
		return -1;
	}
}

int main(){
	int select, indexToStart, indexToFind, kevinBaconSayisi;
	char name[50],filename[15],str[5000];
	struct queue* kuyruk;
	kuyruk = createQueue(kuyruk);
	graph = createGraph(200000);
	

	printf("Dosya Adini Girin(.txt Formatinda) : ");
	gets(filename);

	fp = fopen(filename,"r");
	if(fp == NULL)//DOSYA ACILAMADI,PROGRAM KAPATILIR.
		printf("Dosya acilirken hata olustu!");
	
	else{//DOSYA ACILDI
		//FILM SAYISINI OGRENME;
		while(fgets(str,4999,fp)) //MEVCUT BOYUTUN -1 KADARINA KADAR GÝTMELÝ /0'i ALMAMASI ICIN
			movieNumber++;
		
		fclose(fp); 
		
		actorSayisi = movieNumber; 
    	graphDoldur(graph,filename); 
		indexToStart = checkIndex("Bacon, Kevin");
	
		
		printf("\nTUM AKTORLERIN FREKANS LISTESINI OGRENMEK ICIN = 1\nKENDINIZIN BELIRLEYECEGI AKTORUN KEVIN BACON SAYISINI OGRENMEK ICIN = 2\nGIRISLERINI YAPIN:");
		scanf("%d",&select);

		if(select == 1)
			BFSforAll(&kuyruk, indexToStart);		//(1)
		
		else if(select == 2){
			printf("\nAktor Ismi Girin(Ornek Format=Bacon, Kevin):");
    		scanf(" %[^\n]s",name);
    		
	   		indexToFind = checkIndex(name); 
	   		kevinBaconSayisi = BFS(&kuyruk, indexToStart, indexToFind,1);		//(2)
   			if(kevinBaconSayisi > 0)
   				printf("%s nin Kevin Bacon Sayisi=%d.\n",name,kevinBaconSayisi);
			
			else if(kevinBaconSayisi == 0)
				printf("Girilen Aktor Kevin Bacondur.Dolayisiyla Kevin Baconun,Kevin Bacon Sayisi 0dir.\n");
			
			else
				printf("\n%s nin Kevin Bacon Sayisi=Sonsuzdur.",name);
			
		}
		else
			printf("Lutfen Sadece 1 Veya 2 Girislerini Kullanin.");
	}	
	return 0;
}
	

