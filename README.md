# Projeto de acionamento de motor e comunicação com encoder para a movimentação ponto a ponto de um eixo de robo de reabilitação de um grau de liberdade

<hr>


# Conexão:

Por meio do Script config.sh teremos o CAN Bus virtual e poderemos inicializar os 3 nós para o uso do comando cocomm. As threads foram utilizadas para permitir a comunicação e é possível integrar os scripts que usam essas threads aos códigos em C.


# Configuração pata a habilitação das conexões:
Para efetuar a conexão basta realizar o download da toolchain , assim será possível compilar no formato do colibri. Em seguida é necessário baixar e editar os arquivos a serem compilados e definir o Kernel.

# Testando o CAN

Para a comunicação CAN é necessária a instalação do CANopen, sendo necessário rodar em pelo menos 4 terminais diferentes. cada terminal com uma função. O terminal 1 para a função de comuniação, o trminal 2 para a criação do nó master, o terminal 3 para o nó servo e o terminal 4 para a transferência de comandos via cocomm. Para instalar basta acessar o lin a seguir:


https://opensource.lely.com/canopen/docs/installation/

<hr>


# Configuração de arquivos do CANopenNode

Para fazer o CANopenNode funcionar é preciso a função cocomm no terminal  como segue abaixo:
```sh
~$ cd embarcados2021
~/embarcados2021/ git init
~/embarcados2021/ git pull
~/embarcados2021/ git submodule update --init --recursive
~/embarcados2021/ cd CANopenDemo/CANopenLinux/cocomm
~/embarcados2021/CANopenDemo/CANopenLinux/cocomm/ make
~/embarcados2021/CANopenDemo/CANopenLinux/cocomm/ sudo make install //Adicionando a função cocomm no Terminal
~/embarcados2021/CANopenDemo/CANopenLinux/ make
~/embarcados2021/CANopenDemo/CANopenLinux/ sudo make install
Função canopend: 
~/embarcados2021/CANopenDemo/CANopenLinux/cocomm/ cd ../demo
~/embarcados2021/CANopenDemo/demo/ nano Makefile
# Serão adicionadas as seguintes linhas no final:
'install:
	cp $(LINK_TARGET) /usr/bin/$(LINK_TARGET)'
~/embarcados2021/CANopenDemo/demo/ make
~/embarcados2021/CANopenDemo/demo/ sudo make install
Função demoLinuxDevice.
```



Para criar um nó para a rede CAN utilize o OD, isso aumenta a liberdade para as capacidades dos nós. Depois disso, altere os valores dos parâmetros de comunicação PDO e os de mapeamento PDO.  valores 1400(4) e 1800(4) seguido de 1600(4) e 1A00(4) respectivamente. 



<hr>

# Referências e outros links:

> Git com os arquivos do CANopenDemo, de onde fizemos o git clone e de onde baseamos este projeto:
https://github.com/CANopenNode/CANopenDemo

>Onde vimos como usar o CAN no Colibri:
https://developer.toradex.com/knowledge-base/can-controller-area-network-on-colibri-module#CAN_on_Colibri_VFxx

> Habilitação do CAN na toradex Colibri VFxx <br>
https://developer.toradex.com/knowledge-base/can-linux#Kernel_Support_Colibri_VFxx

> Criação da Build do kernel e onde conseguimos os arquivos do git toradex <br>
https://developer.toradex.com/knowledge-base/build-u-boot-and-linux-kernel-from-source-code

> Este projeto foi feito em parceria com o de outro grupo de colegas da turma =D
https://github.com/KingCoolgate/projetoEmbarcados2021
