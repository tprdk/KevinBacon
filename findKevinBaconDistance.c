#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define nameLenght 100						//isimler için ayrýlan karakter sayýsý
#define indexLenght 2000					//film index ve movie index için ayrýlan karakter sayýsý 
#define queueLenght 1000					//queue uzunluðu
#define mapLenght 300000					//map uzunluðu
#define Prime 2								//string hashlerken kullanýlan asal sayý
#define printArrayLenght 1000
struct n{									//artistleri tutacak olan structure
	char name[nameLenght];					//artist adý
	int moviesIndex[indexLenght];			//baðlantýlý olduðu filmlerin indexleri
};
struct t{									//filmeleri tutacak olan structure
	char name[nameLenght];					//film adý
	int artistIndex[indexLenght];			//baðlantýlý olduðu artistlerin indexleri
};
struct m{
	int value;								//Queue structure
	int index;								
};
typedef struct n Artist;
typedef struct t Movies;
typedef struct m Queue;
FILE* openFile(char *nameOfInputFile,char *type){		//Dosya açma fonksiyonu
	FILE *file;
	file=fopen(nameOfInputFile,type);	
	if(file == NULL){
		printf("File can not be opened\n");
		return 0;
	}
	printf("File opened\n");
	return file;
}
void pushQueue(Queue *q,int value){
	q[(q->index)++].value=value;			//Queue'ya ekleme fonksiyonu
}
int popQueue(Queue *q){						//Queue'dan eleman çýkarttýðýmýz fonksiyon
	int i;
	int value=q[0].value;
	for(i=0;i<q->index;i++)
		q[i].value=q[i+1].value;
	(q->index)--;	
	return value;
}
void printArray(char *array){					//diziyi bastýrýyoruz
	int i=0;
	for(i;array[i] != '\0';i++)
		printf("%c",array[i]);
	printf("\n");	
}
void clearArray(char *array){					//ilk null görülene kadar dizinin karakterlerini null yapýyoruz
	int i=0;
	for(i;array[i] != '\0';i++)
		array[i]='\0';
}
void initArray(char *array,int size){			//ilk array oluþturulduðunda içini bütün diziyi null ile dolduruyoruz
	int i=0;
	for(i;i<size;i++)
		array[i]='\0';
}
void addToString(char *dest,char *src){
	int i=0,j=0;								//dest arrayi'nin son indexinden baþlayarak dest dizisini bu diziye ekleyen fonksiyon
	for(j=0;dest[j] !='\0';j++);
	for(i=0;src[i] != '\0';i++)
		dest[j++]=src[i];
	dest[j]='\0';	
}
Movies *initMovies(FILE *inputFile,Movies *movies){
	int i=0;
	movies=(Movies*)malloc(sizeof(Movies)*mapLenght);
	for(i;i<mapLenght;i++){								//movie structure inda mapteki bütün isimlerin ilk karakterini '\0' yaptýk
		movies[i].name[0]='\0';							//bu sayede boþ olmasýný anlýyoruz
		movies[i].artistIndex[0]=-1;					//artist indexin 0. indexini -1 yapýyoruz
	}													//bu sayede indexleri okurken durmamýz gereken yeri -1 den anlýyoruz
	return movies;
}
Artist *initArtist(Artist *artist){
	int i=0;
	artist=(Artist*)malloc(sizeof(Artist)*mapLenght);
	for(i;i<mapLenght;i++){								//artist structure inda mapteki bütün isimlerin ilk karakterini '\0' yaptýk
		artist[i].name[0]='\0';							//bu sayede boþ olmasýný anlýyoruz
		artist[i].moviesIndex[0]=-1;					//moviesindexin 0. indexini -1 yapýyoruz
	}													//bu sayede indexleri okurken durmamýz gereken yeri -1 den anlýyoruz
	return artist;
}
unsigned long int calculateKey(char *artistName){
	int i=0;
	unsigned long int key=0;						//hesaplanacak key deðeri
	int prime=1;									//her tur bir üssü alýnacak asal sayý	
	for(i;artistName[i] != '\0';i++){				//baþtan baþlayarak sýrasýyla üslü sayýyý 1 çarparak
		key+=artistName[i]*prime;
		prime*=Prime;
	}
	return key;									//hesaplanan key deðeri
}
int calculateDoubleHash(unsigned long int key,int iter){
	int lenght=mapLenght;													//double hashing formülü ile hash resultu buluyoruz
	return (((key % lenght)+(iter*(1+(key % (lenght-1))))) % lenght);
}
void insertArtist(Artist *artist,char *artistName,int movieIndex,int hashResult){
	int i=0;														//bu fonksiyon ile artist structure 'ýna nameini veriyoruz
	for(i;artistName[i] != '\0';i++)							
		artist[hashResult].name[i]=artistName[i];
	artist[hashResult].name[i]='\0';
	for(i=0;artist[hashResult].moviesIndex[i] != -1 ;i++);			//movie indexinde -1 görene kadar ilerliyoruz	
	artist[hashResult].moviesIndex[i]=movieIndex;					//-1 öncesinde önceden eklenmiþ film indeksleri var
	artist[hashResult].moviesIndex[i+1]=-1;							//ekleme sonrasýnda bir sonraki indexi -1 yapýyoruz
}
int compare(char *name,char *artistName){
	int i=0;
	int flag=1;													//name compare fonksiyonu
	while(name[i] != '\0' && flag){
		if(name[i] != artistName[i])
			flag=0;
		i++;	
	}
	return flag;
}
void addToMovies(Movies *movies,char *movieName,int movieIndex){   	//movie ye ismini ekliyoruz
	int i=0;
	for(i;movieName[i] != '\0';i++)
		movies[movieIndex].name[i]=movieName[i];
	movies[movieIndex].name[i]='\0';
}
int addToArtist(Artist *artist,char *artistName,int movieIndex){
	int iter=0;
	int flag=1;
	int hashResult=0;
	int i=0;
	unsigned long int key=calculateKey(artistName);						//key deðeri hesaplanýyor
	while(flag){	
		hashResult=calculateDoubleHash(key,iter);						//double hash deðerini hesaplýyoruz				
		if(artist[hashResult].name[0] == '\0'){							//Bu hashResulta önceden bir þey yazýlmamýþ olma durumu
			insertArtist(artist,artistName,movieIndex,hashResult);		//artisti bu indexe ekliyoruz
			flag=0;
		}else{															//Bu adrese önceden bir þey yazýlmýþ olma durumu
			if(compare(artist[hashResult].name,artistName)){			//Bu durumda öncelikle ayný kiþi mi onu kontrol ediyoruz
				for(i=0;artist[hashResult].moviesIndex[i] != -1 ;i++);	//Ayný kiþiyse filmin indexini ekliyoruz
				artist[hashResult].moviesIndex[i]=movieIndex;
				artist[hashResult].moviesIndex[i+1]=-1;	
				flag=0;
			}else{
				iter++;													//Ayný kiþi deðilse yeni bir index bulamýyýz
			}
		}
	}
	return hashResult;													//eklediðimiz indexi döndürüyoruz
}
void addArtistIndexToMovie(Movies *movies,int movieIndex,int artIndex){	//Bir film eklediðimizde o filmde oynayan oyunculurýn indexlerini filme ekliyoruz
	int i;
	for(i;movies[movieIndex].artistIndex[i] != -1 ;i++);
	movies[movieIndex].artistIndex[i]=artIndex;
	movies[movieIndex].artistIndex[i+1]=-1;
}
readFile(FILE *inputFile,Artist *artist,Movies *movies){
	char movieName[nameLenght];								//okuyacaðýmýz filmin adýný tutacaðýmýz deðiþken
	char artistName[nameLenght];							//okuyacaðýmýz oyuncularýn adýný tutacaðýmýz deðiþken
	char ch;												//karakter tutan deðiþken
	int index=0;											//dizinin hangi indexine yazdýðýmýzý tutan deðiþken
	int movieIndex=0;										//hangi movie indexindeyiz onu tutuyor
	int artistIndex=0;										//eklenen artistin hangi indexe yerlerþtiðini tutan deðiþken
	initArray(movieName,nameLenght);						
	initArray(artistName,nameLenght);						//dizilerin içini NULL ile doldurduk
	
	ch = getc (inputFile);
	while (ch != EOF){
		while(ch!='/'){										// ilk / karakterine kadar gelen karakterler film adýný oluþturur
			movieName[index++]=ch;
			ch = getc(inputFile);
    	}
    	addToMovies(movies,movieName,movieIndex);			//okudðumuz filmi movie structuruna ekledik
    	index=0;	
    	ch = getc(inputFile);
    	while(ch != '\n' && ch != EOF){						//buradan sonra /n veya EOF karakterine kadar ilerliyoruz burada hep oyuncu adý var
    		if(ch == '/'){									// '/' karaktesi oyuncu isimlerini ayýrýyor
    			artistIndex=addToArtist(artist,artistName,movieIndex);	//addToArtist fonksiyonu ile bu ismi hash fonksiyonuna gönderdik artistIndex hangi indise yerleþtiðini gösteriyor
    			addArtistIndexToMovie(movies,movieIndex,artistIndex);	//bu adrese filmin indexini de gönderiyoruz
    			clearArray(artistName);			
    			index=0;
			}else{
				if(ch != ',')
					artistName[index++]=ch;					// , karakterini almýyoruz
			}
			ch = getc(inputFile);
		}
		artistIndex=addToArtist(artist,artistName,movieIndex);	// Son oyuncu '/' iþareti ile ayrýlmadýðýndan son oyuncuyu burada ekliyoruz
		addArtistIndexToMovie(movies,movieIndex,artistIndex);
		clearArray(movieName);
		clearArray(artistName);
		ch = getc(inputFile);
		index=0;
		movieIndex++;
	}		
}
void reverse(char *name,char *surname){
	int i,j;
	for(i=0;surname[i] != '\0';i++);				//reverse fonksiyonu ile kullanýcýdan alýnan ad soyad formatýný
	surname[i++]=' ';								//soyad ad formatýna çevirdik
	for(j=0;name[j] != '\0' ; j++)
		surname[i++] = name[j];
	surname[i]='\0';	
}
int findArtistIndex(Artist *artist,char *name){		//Bu fonksiyon verien bir artist adýnýn hangi indexte olduðunu bulmak için		
	int flag=1;
	int iter=0;
	int hashResult=0;
	unsigned long int key = calculateKey(name);		//isim ile key hesaplanýyor
	while(flag){
		hashResult=calculateDoubleHash(key,iter);	
		if(compare(artist[hashResult].name,name))
			flag=0;									//bulunduðunda flag = 0 oluyor
		else
			iter++;
	}
	return hashResult;								//bu hashResultta oyuncunun indexi var
}
void pushElementsToArtistQueue(Movies *movies,int movieIndex,Queue *artistQueue,int *visitedArtist,int *pushArtist,int *pushMovie,int *artistPushMovie){
	int i=0;
	for(i;movies[movieIndex].artistIndex[i] != -1 ;i++){							//filmin baðlý olduðu her artist için 
		if(!visitedArtist[movies[movieIndex].artistIndex[i]]){						//daha önceden Q'ya eklenmemiþ ise
			artistPushMovie[movies[movieIndex].artistIndex[i]]=movieIndex;			//Þuan Q'ya eklenecek artisti hangi filmin eklediðini tutuyoruz
			pushArtist[movies[movieIndex].artistIndex[i]]=pushMovie[movieIndex];	//Þuan Q'ya eklenecek artisti hangi artistin eklediðini tutuyoruz
			pushQueue(artistQueue,movies[movieIndex].artistIndex[i]);				//filmin baðlý olduðu artistleri Q'ya ekliyoruz
			visitedArtist[movies[movieIndex].artistIndex[i]]=1;						//artist indeksini eklediklerimizi visited dizisinde 1 yaptýk
		}
	}
}
void pushElementsToMoviesQueue(Artist *artist,int artistIndex,Queue *moviesQueue,int *visitedMovies,int *pushMovie){
	int i=0;
	for(i;artist[artistIndex].moviesIndex[i] != -1 ;i++){					//artistin baðlý olduðu her film için 
		if(!visitedMovies[artist[artistIndex].moviesIndex[i]]){				//daha önceden Q'ya eklenmemiþ ise
			pushMovie[artist[artistIndex].moviesIndex[i]]=artistIndex;		//bu movieyi pushlayan kisiyi yazýyoruz
			pushQueue(moviesQueue,artist[artistIndex].moviesIndex[i]);		//artistin baðlý olduðu filmi Q'ya ekliyoruz
			visitedMovies[artist[artistIndex].moviesIndex[i]]=1;			//movie indeksini eklediklerimizi visited dizisinde 1 yaptýk
		}
	}
}
void findDistance(Artist *artist,Movies *movies,int *pushMovie,int *pushArtist,int srcArtistIndex,int destArtistIndex,int *artistPushMovie){
	int flag=1;
	int iter=1;
	int dest=destArtistIndex;
	char array[printArrayLenght];										//Ekrana bastiracak stringi tutacak dizi
	initArray(array,printArrayLenght);
	while(flag){														//sondan geriye doðru geliyoruz
		if(pushArtist[dest] == srcArtistIndex){							//bu artisti pushlayan eleman ilk artist ise yol oluþturulmuþ demektir
			flag=0;	
			addToString(array,artist[srcArtistIndex].name);				//ilk artist adýný ekledik
			addToString(array," - ");							
			addToString(array,artist[dest].name);						//2. artistin adýný ekledik
			addToString(array," ''");
			addToString(array,movies[artistPushMovie[dest]].name);		//artistin hangi film tarafýndan pushlandýðýný tutan diziden filmi aldýk
			addToString(array,"''\n");
			printf("Aralik = %d\n",iter);								//aralýðý yazdýrýyoruz
		}else{
			addToString(array,artist[dest].name);						//ilk artist adýný ekledik
			addToString(array," - ");
			addToString(array,artist[pushArtist[dest]].name);			//2. artistin adýný ekledik
			addToString(array," ''");
			addToString(array,movies[artistPushMovie[dest]].name);		//artistin hangi film tarafýndan pushlandýðýný tutan diziden filmi aldýk
			addToString(array,"''\n");
			iter++;
			dest=pushArtist[dest];
		}
	}
	if(iter < 6){
		printArray(array);
	}else{
		printf("Aradaginiz kisilerin uzakligi 6 dan fazla");
	}
	printf("Ana menuye Donmek Icin Bir Tusa Basiniz");
	getch();
}
void twoArtist(Movies *movies,Artist *artist,char *src,char *dest){
	Queue *moviesQueue = (Queue*) malloc(sizeof(Queue)*mapLenght);	//Filmelerin tutulduðu queue
	Queue *artistQueue= (Queue*) malloc(sizeof(Queue)*mapLenght);	//Artistlerin tutulduðu queue
	moviesQueue->index=0;
	artistQueue->index=0;
	int *visitedMovies= (int*) calloc(mapLenght,sizeof(int));
	int *visitedArtist= (int*) calloc(mapLenght,sizeof(int));
	int *artistPushMovie= (int*) calloc(mapLenght,sizeof(int)); 	//artistin ,Hangi film tarafýndan Queue'ya pushladýðýný tutyruz
	int *pushMovie= (int*) calloc(mapLenght,sizeof(int));			//filmi ,Hangi artistin  Queue'yapushladýðýný tutan dizi
	int *pushArtist= (int*) calloc(mapLenght,sizeof(int));			//artisti ,Hangi artistin Queue'ya pushladýðýný tutan dizi
	int flag=1;
	int movieIndex=0;												//poplanan filmin indexini tutacak deðiþken
	int artistIndex=0;												//poplanan artistin indexini tutacak deðiþken
	int srcArtistIndex=findArtistIndex(artist,src);					//ilk artistin indeksi
	int destArtistIndex=findArtistIndex(artist,dest);				//gidilecek artisti indexi
	pushElementsToMoviesQueue(artist,srcArtistIndex,moviesQueue,visitedMovies,pushMovie);	
	printf("\n%s -- ",artist[srcArtistIndex].name);
	printf("%s\n",artist[destArtistIndex].name);					//ilk artist -- son artist isimleri	
	visitedArtist[srcArtistIndex]=1;								//ilk artisti visited dizisinde 1 yaptýk
	
	while(flag && (moviesQueue->index >0  || artistQueue->index > 0)){	//2 queue'da boþ deðilse ve flag=1(son artiste ulaþýlmamýþsa) döngü devam eder
		if(moviesQueue->index >0){								//movie queue dolu is poplama yapýlýr
			movieIndex=popQueue(moviesQueue);					//buradan poplanan filmin baðlý olduðu oyuncular artistQ'ya pushlanýr
			pushElementsToArtistQueue(movies,movieIndex,artistQueue,visitedArtist,pushArtist,pushMovie,artistPushMovie);
		}
		if(artistQueue->index > 0){								//artistQ dolu ise poplama yapýlýr
			artistIndex=popQueue(artistQueue);		
			if(artistIndex == destArtistIndex)					//son oyuncunun indexkine eþit ise bulunmuþ demektir
				flag=0;	
			else												//buradan poplanan artistin baðlý olduðu filmler movieQ'ya pushlanýr
				pushElementsToMoviesQueue(artist,artistIndex,moviesQueue,visitedMovies,pushMovie);	
		}
	}
	if(!flag)													//Flag=0 olma durumunda oyuncuya ulaþýlmýtýr Aralýk bastýrýlýr
		findDistance(artist,movies,pushMovie,pushArtist,srcArtistIndex,destArtistIndex,artistPushMovie);
	else{
		printf("Bir baglanti yok.\n");							//flag=1 oyuncuya ulaþýlamamýþ
		printf("Ana menuye Donmek Icin Bir Tusa Basiniz");
		getch();
	}
		
	free(visitedMovies);
	free(visitedArtist);
	free(pushMovie);
	free(pushArtist);
	free(artistPushMovie);
}
void menu(Movies *movies,Artist *artist){
	char src_name[nameLenght];
	char src_surname[nameLenght];
	char dest_name[nameLenght];
	char dest_surname[nameLenght];			//kullanýcýdan alýnacak ad soyad dizileri
	initArray(src_name,nameLenght);
	initArray(src_surname,nameLenght);
	initArray(dest_name,nameLenght);
	initArray(dest_surname,nameLenght);
	int flag=1;
	do{
		system("cls");
		clearArray(src_name);
		clearArray(src_surname);
		clearArray(dest_name);
		clearArray(dest_surname);
		printf("**********************************************************************\n");
		printf("Herhangi Bir Oyuncunun Kevin Bacon Uzakligini Ogrenmek Icin 1'e");
		printf("\nHerhangi Iki Oyuncun Birbirine Olan Uzakligi Icin 2'ye\nCikmak icin 0'a basiniz\n");
		printf("**********************************************************************\n");
		scanf("%d",&flag);
		if(flag == 1){
			printf("Kevin Bacon'a uzakligini istediginiz oyuncunun adi soyadi:");
			scanf("%s %s",src_name,src_surname);
			reverse(src_name,src_surname);
			strcpy(dest_surname,"Bacon Kevin");
			if(strcmp(dest_surname,src_surname))						//girilen isim Kevin Bacon ise aralýk 0'dýr
				twoArtist(movies,artist,src_surname,dest_surname);	
			else{
				printf("Kevin Bacon'in Kevin Bacon'a uzakligi 0'dir.\n");
				printf("Ana menuye Donmek Icin Bir Tusa Basiniz");
				getch();	
			}	
		}
		if(flag == 2){
			printf("Oyuncu 1 ad soyad:");
			scanf("%s %s",src_name,src_surname);
			printf("\nOyuncu 2 ad soyad:");
			scanf("%s %s",dest_name,dest_surname);
			reverse(src_name,src_surname);
			reverse(dest_name,dest_surname);
			if(strcmp(dest_surname,src_surname))						//girilen iki isim ayný ise aralýk 0'dýr.
				twoArtist(movies,artist,src_surname,dest_surname);
			else{
				printf("%s'in %s'a uzakligi 0'dir.\n",src_name,src_surname);
				printf("Ana menuye Donmek Icin Bir Tusa Basiniz");
				getch();	
			}		
		}
	}while(flag);
}
int main(){
	Artist *artist;
	Movies *movies;
	FILE *inputFile;
	char nameOfInputFile[nameLenght];
	printf("Please give the name of input file\n");
	scanf("%s",nameOfInputFile);
	inputFile=openFile(nameOfInputFile,"r");
	movies=initMovies(inputFile,movies);
	artist=initArtist(artist);
	readFile(inputFile,artist,movies);
	menu(movies,artist);
	free(movies);
	free(artist);
	fclose(inputFile);
	return 0;
}
