//Inclusões de bibliotecas

//Funçoes básicas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Sockets, threads e mutex
#include <pthread.h>
#include <semaphore.h>

//Funçoes gerais
#include <unistd.h>
#include <math.h>
#include <fcntl.h>

//Redes
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>


/*Variáveis*/

pthread_t id1, id2, id3;
double posicaoFinal, ddp, erro;
double coef, correcao;

/*Declarações de Funções*/

/*Threads*/

//1
void * thread_1 (void *apelido) {
	float k;
	while (1) {
		k = sin(k+ (3.14/2));
		printf("%f\n", k);
		sleep(2);
	}
	pthread_exit(NULL);
}

//2
void * thread_2(void *apelido) {
	sleep(1);
	while (1) {
		printf(" 2\n");
		sleep(2);
	}
	pthread_exit(NULL);
}

//3
void * thread_3(void *apelido) {
	sleep(1);
	while (1) {
		printf(" 3\n");
		sleep(2);
	}
	pthread_exit(NULL);
}

//Configuração das portas LED
int portConfig(int argc, char *argv[]){
	int fd;

  // Escrita de GPIO
  fd = open("/sys/class/gpio/export", O_WRONLY);
  write(fd, "50", 2);
  close(fd);

  // Configuração
  fd = open("/sys/class/gpio/gpio50/direction", O_WRONLY);
  write(fd, "out", 3);
  close(fd);


  return EXIT_SUCCESS;
}

int calculoTensao(posicaoFinal, posicaoInicial){
	erro = math.abs(posicaoFinal - posicaoInicial);
	ddp = erro*coef + correcao;
}

/*MAIN*/

int main(int argc, char *argv[]) {

	pthread_create (&id1, NULL , (void *) thread_1, NULL);

	pthread_create (&id2, NULL , (void *) thread_2, NULL);

	pthread_create (&id3, NULL , (void *) calculoTensao, NULL);

	while (erro < 10^-6);
	return 0;
}


int saidaProMotor(posicaoAtual, portaComunicacao, portaSaida, ....){
	s = posicao;
	ptc = porta;
	pts = portaS;

	.
	.
	.

	return(dpp)
}


int envio(dauibfailsdf){
	pegar o valor de tensao
	manda para a porta X
}

int thread2(){
	while(true){
		envio(tensao, porta);
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
Sensor
Controlador
Atuador

Interface


thread 1 ( sensor->controlador)
	Entrada constante de dados do sensor
	Transformação de dados decimal<->numerico
	Processamento dos dados
	Envio de posicao

	Modelagem do controlador
	Encontrar o valor do erro do sistema
	Calcular a tensao a ser enviada baseada no erro 

thread 2 (controlador->atuador)
	Entrada de dados do controlador
	Processamento e transformacao dos dados 
	Envio do sinal 
	
	No calculo do valor de saida ocorre atualizacao do sensor,gerando um feedback 

protocolo de comunicao em cam, a especificar as portas dos arquivos etc
Se o motor tiver ponte H so e necessario enviar a tensao,mas se houver um controlador e necesario um txt para determinar a tarefa