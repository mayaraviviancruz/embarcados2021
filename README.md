# Projeto de acionamento de motor e comunicação com encoder para a movimentação ponto a ponto de um eixo de robo de reabilitação de um grau de liberdade

<hr>

# Configurações iniciais:

Para iniciar o projeto é necessário baixar e configurar o u-boot e o kernel linux.
Na primeira etapa configuramos a toolchain. Pois assim será possívem fazer o cross-compile para buildar o u-boot ou linux
Para baixar instalar siga os seguintes comandos em seu terminal:

```sh
$ cd ~
$ wget -O gcc-linaro-7.3.1-2018.05-x86_64_arm-linux-gnueabihf.tar.xz "https://releases.linaro.org/components/toolchain/binaries/7.3-2018.05/arm-linux-gnueabihf/gcc-linaro-7.3.1-2018.05-x86_64_arm-linux-gnueabihf.tar.xz"
$ tar xvf gcc-linaro-7.3.1-2018.05-x86_64_arm-linux-gnueabihf.tar.xz
$ ln -s gcc-linaro-7.3.1-2018.05-x86_64_arm-linux-gnueabihf gcc-linaro
```
Após isso será possível ver a pasta da toolchain em seu computador. 
Em seguida é necessário exportar as variáveis que irão executar os comandos e configurações necessárias. 
Use os comandos a seguir am seu terminal:

```sh
$ export ARCH=arm
$ export PATH=~/gcc-linaro/bin/:$PATH
$ export CROSS_COMPILE=arm-linux-gnueabihf-
```
```sh
$ echo "export ARCH=arm" >> ~/export_compiler
$ echo "export PATH=~/gcc-linaro/bin/:$PATH" >> ~/export_compiler
$ echo "export CROSS_COMPILE=arm-linux-gnueabihf-" >> ~/export_compiler
$ source ~/export_compiler
```
Além disso, antes de iniciar as configutações precisamos baixar e instalar as ferramenteas o u-boot, para fazer isso, siga os seguintes passos no seu terminal.

```sh
$ sudo apt-get install u-boot-tools
$ sudo apt-get install bc build-essential git libncurses5-dev lzop perl libssl-dev
```
Agora iniciamos com o u-boot. Criamos uma pasta para ele chamada workdir e fizemos o git clone do repositório como segue:

```sh
$ mkdir -p ~/workdir
$ cd ~/workdir
$ git clone -b 2016.11-toradex git://git.toradex.com/u-boot-toradex.git
```
Observe que o branch utilizado é o 2016.11-toradex, pois é a branch para a nossa placa VF50
Em seguida executamos a configuração:

```sh
$ cd ~/workdir/u-boot-toradex
$ make colibri_vf_defconfig
```
Observe que a configuração colibri_vf_defconfig específica para a nossa placa, para miores informações é possível acessar o tutorial da toradex com mais detalhes. O link está no final do Readme.
Agora compilamos o boot loader:

```sh
$ make -j$(nproc) 2>&1 | tee build.log
$ ls u-boot.bin
```
Agora nós vamos configurar o Linux Kernel.
Primeiro, é necessário criar um diretório e fazer o git clone:

```sh
$ mkdir -p ~/workdir
$ cd ~/workdir
$ git clone -b 	toradex_vf_4.4 git://git.toradex.com/linux-toradex.git
```
Atenção para a branch toradex_vf_4.4 que precisa ser esta especificamente para a nossa placa.
Para configurar o Kernel executamos:

```sh
$ cd ~/workdir/linux-toradex
$ make colibri_vf_defconfig
```
Sendo a configuração esclusiva para a nossa placa, assim como no u-boot
Em seguida utilizamos os seguintes comandos:

```sh
$ make -j$(nproc) zImage | tee build.log
$ make vf500-colibri-eval-v3.dtb
$ make -j$(nproc) modules
```


# Habilitação do CAN 

No caso do nosso projeto decidimos habilitar a can0
Para a devida habilitação da mesma é necessário seguir os seguintes passos no seu terminal:

```sh
$ cd linux-toradex/arch/arm/boot/dts
$ nano vf-colibri-eval-v3.dtsi
```
Esses passos servem para abrirmos o código em um editor de texto no terminal do linux, dessa maneira poderemos efetuar as alterações necessárias para a habilitação da can0.
Nesse caso, é necessário adicionar o nó do can0 com um status de okay no código, além de alterar o status do i2c0 para desabilitado.
Segue uma imagem com esse trecho do código, para que seja possível ver como deverá ficar o código:

![image](https://user-images.githubusercontent.com/78976475/131508689-35d5a711-0ddf-470f-be49-b28c01fc1960.png)

Em seguida, basta fazer a configuração e compilação como segue:

```sh
$ cd linux-toradex
$ make colibri_vf_defconfig
$ make vf500-colibri-eval-v3.dtb
```
Agora, basta trocar a dtb no módulo para que que a CAN possa ser acessível no módulo.

# Testando o CAN

Para a comunicação CAN é necessária a instalação do CANopen, sendo necessário rodar em pelo menos 4 terminais diferentes. cada terminal com uma função. O terminal 1 para a função de comuniação, o terminal 2 para a criação do nó master, o terminal 3 para o nó servo e o terminal 4 para a transferência de comandos via cocomm.

Primeiro, testamos a CAN.
Para testar a CAN é necessário primeiro a instalação dos pacotes binários, de desenvolvimento e ferramentas do CANopen.

Para a instalação dos pacotes binários no Linux, basta usar as linhas de comando que seguem:

```sh
$ sudo add-apt-repository ppa:lely/ppa
$ sudo apt-get update
```
```sh
$ sudo apt-get install liblely-coapp-dev liblely-co-tools python3-dcf-tools
```
```sh
$ pkg-config --cflags liblely-coapp
$ pkg-config --libs liblely-coapp
```
```sh
$ sudo apt-get install can-utils
```
Por fim, instalaremos outras ferramentas adicionais que foram sugeridas no tutorial da Lely CANopen (link no final do readme):

```sh
$ sudo apt-get install \
    git build-essential automake libtool \
    python3-setuptools python3-wheel \
    python3-empy python3-yaml \
    libbluetooth-dev \
    valgrind \
    doxygen graphviz
    
```
 
 Essas últimas ferramentas irão demorar um pouco para baixar. 
 Em seguida poderemos clonar o repositório do git da Lely, conforme segue:
 
 ```sh
$ git clone https://gitlab.com/lely_industries/lely-core.git
$ cd lely-core
$ autoreconf -i
$ mkdir -p build
$ cd build
$ ../configure --disable-cython
$ make
$ make check
```

Nessa etapa criamos o script de configuração para depois buildar. 

Agora, com as ferramentas em mãos, vamos fazer os testes a partir da ferramenta CANopen control. E então criaremos o master e o slave para a comunicação do CAN bus virtual.

```sh
$ sudo modprobe vcan
$ sudo ip link add dev vcan0 type vcan
$ sudo ip link set up vcan0
$ ip link show vcan0
```
Aqui acabamos de criar uma interface de rede CAN e checamos ela.
Agora para monitorar as informações utilizamos o seguinte comando em um terminal que ficará aberto:

```sh
$ candump vcan0
```
![image](https://user-images.githubusercontent.com/78976475/131523754-79163f3b-c023-4481-805d-e3630c5efc7a.png)


Em um novo terminal iremos criar um dispositivo virtual CANopen e inicializar o dispositivo conforme segue:

```sh
$ coctl vcan0 /etc/coctl.dcf
```

![image](https://user-images.githubusercontent.com/78976475/131524110-b241950d-f031-4395-8dca-40404b172e6a.png)

É possível ver que o terminal que roda o candump já rodas as linhas:

![image](https://user-images.githubusercontent.com/78976475/131524350-736adc6f-5190-49e2-9640-f450bad81cd1.png)

Para adicionar o slave, nó baixamos o arquivo cmd-slave.dcf do seguinte link:

https://opensource.lely.com/canopen/assets/files/cmd-slave.dcf

E então abrimos um novo terminal para o slave que terá o node id 2 conforme segue:

```sh
$ coctl vcan0 ./cmd-slave.dcf
```

Veja que o candump retorna uma nova linha:

![image](https://user-images.githubusercontent.com/78976475/131525086-359ad504-3cb3-44cf-89ba-5c4963ce863b.png)

A partir disso configuramos a produção de batimentos, conforme o tutorial da Ley sugere, que por meio da requisição de SDO, a qual em seguida aparece no candump:

![image](https://user-images.githubusercontent.com/78976475/131526474-1f38cf60-c21e-4fd5-b8cb-1b83c9544be4.png)

Neste caso, no primeiro frame temos a solicitação de download do SDO. O segundo frame é a resposta do slave.
Para a devida configuração é necessário identificar e e alterar o id do nó. 
Por meio do LSS é possível fazer isso, conforme mostra a imagem:

![image](https://user-images.githubusercontent.com/78976475/131527779-4f8880a9-185c-47ea-9e81-eaf0e3c82e44.png)

Para que a transmissão do PDO seja sincrona, precisamos escrever o período entre mensagens para o objeto. Para que os PDOs sejam enviados após o SYNC, precisamos ir no terminal do slave e configurar o TPDO. Após isso, o slave poderá criar o primeiro TPDO. Conforme segue na imagem:

![image](https://user-images.githubusercontent.com/78976475/131528622-3783c3ba-211c-41b6-af87-5bd6fc11f2ff.png)

A partir de então o PDO é enviado e para ser recebido é necessário o RPDO, então basta ir no terminal do master e digitar o comando:

```sh
> [17] set rpdo 1 0x183 sync1 1 0x2007 0 u32
< [17] OK
```

Por fim, basta apertar enter para conferir que o mestre está recebendo os PDOs

# Conexão:

A partir de agora utilizaremos a comunicação com base no CANopenNode tutorial, o qual se encontra na pasta do nosso repositório.Assume-se aqui que o tutorial acima foi realizado, existindo portanto uma rede CAN virtual

Criaremos mestre e escravo: em um novo terminal
```
~$ cd <caminho até a pasta>/embarcados2021/CANopenDemo/CANopenLinux/

~$ make
# Compila de acordo com o arquivo Makefile, não é necessário mexer nele 
~$ ./canopend can0 -i 1 -c "local-/tmp/CO_command_socket"
#gera um mestre na rede can0 com id 1 
```
No processo de embarcar os códigos, haverá a necessidade de mudar o caminho "local-/tmp/CO_command_socket". Para que o cocomm saiba onde enviar suas instruções, é necessário que o caminho para o socket seja informado ao programa.Isso pode ser feito com o seguinte comando:
```
export cocomm_socket= <caminho para o local do socket>
```
com isso será possível remanejar o socket para outro caminho ao embarcar o código.

Para criar o escravo:
```
~$ cd <caminho até a pasta>/embarcados2021/CANopenDemo/demo/

~/CANopenDemo/demo/$ make
# Compila de acordo com o arquivo Makefile, não é necessário mexer nele 

~/CANopenDemo/demo/$ ./demoLinuxDevice can0
#Cria um escravo na mesma rede 
```
Em mais um outro terminal, utilizaremos o programa cocomm para enviar commandos á rede :
```
~$<caminho até a pasta>/CANopenDemo/CANopenLinux/cocomm/
~$ sudo make
~$ sudo make install
#compilação e instalação do programa
~$ cd 
#Volta ao diretório home.Pode ser ignorada

~$ cocomm "4 read 0x1017 0 u16"
#Neste ponto, este comando retorará 0, nos informando que o nó 4, o escravo, está com a produção de heartbeat desabilitada. 
```
![image](https://user-images.githubusercontent.com/62487469/131557730-8a6a6271-78b4-4592-b603-34785f49adc9.png)


Este comando é um comando SDO. Em seguida podemos testar comandos de controle de rede 

```
~$ cocomm "4 reset communication"
~$ cocomm "0 reset node"
```

Os nós são iniciados em um estado de não-volatilidade, gerando mensagens de emergência e desabilitando a função PDO.É necessário então realizar a configuração dos nós através dos comandos abaixo:
```
cocomm "1 w 0x1011 1 vs load"
cocomm "4 w 0x1011 1 vs load"
#carregando configurações pré definidas nos object dictionaries

#se necessário, alterações nas configuraçoes podem ser feitas utilizando diretamente o terminal
cocomm "1 write 0x1017 0 u16 1000"
cocomm "4 write 0x1017 0 u16 1000"
#alterações da função de heartbeat de cada nó

cocomm "1 w 0x1010 1 vs save"
cocomm "4 w 0x1010 1 vs save"
#guardando alterações 

```
Note que mudanças de parâmetros PDO so podem ser executadas com a função PDO desabilitada

É possível tambem criar um arquivo com intruções pré programadas e utiliza-lo como fonte de comandos:
```
~$ touch comandos.txt
~$printf ” [1] r 0x1017 0 u16 ;[2] 1 start” >> comandos.txt
```
Em seguida utilizar a função de leitura da própria ferramenta:
```
~$ cocomm -f comandos.txt
```
Para o nosso projeto os Index e SubIndex a serem utilizados deverão seguir os padrões da Maxxon, conforme segue:

![image](https://user-images.githubusercontent.com/78976475/131531150-df3fa1d4-0bf9-47b5-9189-49a8015e2d05.png)

Ao criar uma rede CAN, as configurações dos nós como timer de eventos, COB-ID, tipo de transmissão, sub-índices e outros parâmetros são retirados do Object dictionary, que neste projeto se encontra em CANopenNode/example. Ele pode ser alterado usando o editor presente no CANopen Node ou utilizando o próprio terminal como descrito acima. Para alterar estes valores utiliza-se os índices 1400 e 1800 para comunicação RPDO e TPDO respectivamente, e índices 1600 e 1A00 para mapeamento RPDO e TPDO respectivamente.O mapeamento dependerá de cada a aplicação

Para maiores informações acesse o link sobre aplicação de CANopen no EPOS2, que se encontra no final deste readme

<hr>
# Arquivo do projeto final:

A ideia dos códigos do projeto final é fazer com que programa inicialize os nós, depois inicialize as threads de comunicação e chamada de função, segurar a comunicação e depois encerrar os scripts dos nós.
Os códigos contidos, portanto no projeto final são combinações de funções prontas sem integração com o projeto, possívelmente servindo como bibliotecas para futuras alterações e implementaçoes
Os trechos dos códigos de de erro e conversão de ddp, constidos em motor.c, encoder.c e projetofinal.c são de autoria de outro grupo. O link do projeto do gripo está no final do readme.
O próximo passo do projeto seria utilizar o arquivo bufsize para ler o log criado pelo script candumplog e extrair as informações transmitidas entre mestre e escravo para processá-las e poder mais tarde enviar comandos atraves do cocomm para alterar o comportamento do motor. O arquivo nomeado motor contem código para o estabelecimento de threads na rede, passo importante a ser feito antes de embarcar o sistema
No final das contas não foi possível executar com sucesso todas os recursos encontrados, portanto não é possível saber se os mesmos estão funcionando corretamente para uma aplicação prática do módulo.

# Referências e outros links:

> Git com os arquivos do CANopenDemo, de onde fizemos o git clone e de onde baseamos este projeto:
> https://github.com/CANopenNode/CANopenDemo

> Tutorial Lely:
> https://opensource.lely.com/canopen/docs/cmd-tutorial/

>Onde vimos como usar o CAN no Colibri:
>https://developer.toradex.com/knowledge-base/can-controller-area-network-on-colibri-module#CAN_on_Colibri_VFxx

>Notas de aplicação do CANopen no EPOS2 da Maxxon:
>https://www.maxongroup.com/medias/sys_master/root/8834322759710/EPOS2-Application-Notes-Collection-En.pdf

> Habilitação do CAN na toradex Colibri VFxx <br>
> https://developer.toradex.com/knowledge-base/can-linux#Kernel_Support_Colibri_VFxx

> Criação da Build do kernel e onde conseguimos os arquivos do git toradex <br>
> https://developer.toradex.com/knowledge-base/build-u-boot-and-linux-kernel-from-source-code

> Este projeto foi feito em parceria com o de outro grupo de colegas da turma =D
> https://github.com/KingCoolgate/projetoEmbarcados2021


*OBS: não foi possível deletar as pastas u-boot-toradex, tcColibri e linux-toradex do repositório, conforme solicitado. Elas estão dando erro ao tentar deletar.
