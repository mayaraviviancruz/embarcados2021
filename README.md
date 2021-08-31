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

Em um novo terminal iremos criar um dispositivo virtual CANopen com o seguinte comando:

```sh
$ coctl vcan0 /etc/coctl.dcf
```

# Conexão:

Por meio do Script config.sh teremos o CAN Bus virtual e poderemos inicializar os 3 nós para o uso do comando cocomm. As threads foram utilizadas para permitir a comunicação e é possível integrar os scripts que usam essas threads aos códigos em C.


# Configuração pata a habilitação das conexões:
Para efetuar a conexão basta realizar o download da toolchain , assim será possível compilar no formato do colibri. Em seguida é necessário baixar e editar os arquivos a serem compilados e definir o Kernel.



<hr>


# Configuração de arquivos do CANopenNode

Para fazer o CANopenNode funcionar é preciso a função cocomm no terminal  como segue abaixo:
Para criar um nó para a rede CAN utilize o OD, isso aumenta a liberdade para as capacidades dos nós. Depois disso, altere os valores dos parâmetros de comunicação PDO e os de mapeamento PDO.  valores 1400(4) e 1800(4) seguido de 1600(4) e 1A00(4) respectivamente. 

<hr>

# Referências e outros links:

> Git com os arquivos do CANopenDemo, de onde fizemos o git clone e de onde baseamos este projeto:
https://github.com/CANopenNode/CANopenDemo

>Onde vimos como usar o CAN no Colibri:
>https://developer.toradex.com/knowledge-base/can-controller-area-network-on-colibri-module#CAN_on_Colibri_VFxx

> Habilitação do CAN na toradex Colibri VFxx <br>
https://developer.toradex.com/knowledge-base/can-linux#Kernel_Support_Colibri_VFxx

> Criação da Build do kernel e onde conseguimos os arquivos do git toradex <br>
https://developer.toradex.com/knowledge-base/build-u-boot-and-linux-kernel-from-source-code

> Este projeto foi feito em parceria com o de outro grupo de colegas da turma =D
https://github.com/KingCoolgate/projetoEmbarcados2021
