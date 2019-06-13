#include <iostream>
#include <pthread.h>
#include <time.h>
#include <thread> //hardware_concurrency

struct data{
	int **mtx1;
	int **mtx2;
	int **mtx3;
	int start;
	int r1;
	int c1;
	int c2;
	int t;
};

void* multiply(void* e){
struct data *pack = (struct data *) e;
int **mtx1 = pack->mtx1;
int **mtx2 = pack->mtx2;
int **mtx3 = pack->mtx3;
int start = pack->start;
int r1 = pack->r1;
int c1 = pack->c1;
int c2 = pack->c2;
int t = pack->t;

	for(int i = start; i < r1*c2; i +=t){
		mtx3[i/c2][i%c2] = 0;
		for(int j = 0;j<c1; j++){
			mtx3[i/c2][i%c2] += mtx1[i/c2][j] * mtx2[j][i%c2];
		}
	}
}

int main(){

int rows1;
int col1;
int rows2;
int col2;
int rc;

unsigned threadNum = std::thread::hardware_concurrency();
pthread_t threads[threadNum];
struct data pack[threadNum];

std::cout<<"Ingrese numero de filas de la primera matriz:\n";
std::cin>>rows1;
std::cout<<"Ingrese numero de columnas de la primera matriz:\n";
std::cin>>col1;
std::cout<<"Ingrese numero de filas de la segunda matriz:\n";
std::cin>>rows2;
std::cout<<"Ingrese numero de columnas de la segunda matriz:\n";
std::cin>>col2;

if(col1 != rows2){
	std::cout<<"\nNo se puede realizar la multiplicacion de matrices :c\n";
}else{

	//Allocate space
	int** M1 = new int*[rows1];
	for(int i =0;i < rows1;i++){
		M1[i] = new int[col1];
	}
	int** M2 = new int*[rows2];
	for(int i =0;i < rows2;i++){
		M2[i] = new int[col2];
	}
	int** M3 = new int*[rows1];
	for(int i =0;i < rows1;i++){
		M3[i] = new int[col2];
	}
	//Fills the values
	for(int i=0;i<rows1;i++){
		for(int o=0;o<col1;o++)
			M1[i][o]= rand() %51;
	}
	for(int i=0;i<rows2;i++){
		for(int o=0;o<col2;o++)
			M2[i][o]= rand() %51;
	}

	//Takes time
    auto start = std::chrono::high_resolution_clock::now();

	//Creates data pack
	for(int i =0;i<threadNum;i++){
		pack[i].mtx1 = M1;
		pack[i].mtx2 = M2;
		pack[i].mtx3 = M3;
		pack[i].start = i;
		pack[i].r1 = rows1;
		pack[i].c1 = col1;
		pack[i].c2 = col2;
		pack[i].t = threadNum;

		rc = pthread_create(threads + i, NULL, multiply,(void *)&pack[i]);

		if (rc){
			printf("ERROR, return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}


	for(int i = 0; i< threadNum;i++){
		rc = pthread_join(threads[i],NULL);
		if (rc){
			printf("ERROR, return code from pthread_create() is %d\n", rc);
			exit(-1);
		}	
	}

	//Takes time
	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();


	std::cout<<"\n\n";
	for(int i=0;i<rows1;i++){
		for(int o=0;o<col1;o++){
			std::cout<<M1[i][o]<<" ";
		}
		std::cout<<"\n";
	}
	std::cout<<"\n\n";
	for(int i=0;i<rows2;i++){
		for(int o=0;o<col2;o++){
			std::cout<<M2[i][o]<<" ";
		}
		std::cout<<"\n";
	}
	std::cout<<"\n\nMatriz resultante: \n";
	for(int i=0;i<rows1;i++){
		for(int o=0;o<col2;o++){
			std::cout<<M3[i][o]<<" ";
		}
		std::cout<<"\n";
	}
	std::cout<<"\nSe calculo en "<<microseconds<<" microsegundos\n";


	return 0;
	}
}