#!/bin/sh

#Inicializa CAN BUS
sudo modprobe vcan
sudo ip link add dev can0 type vcan
sudo ip link set up can0

#Cria nós virtuais
gnome-terminal -- /bin/bash /home/mayara-cruz/embarcados2021/Projeto-braco/master.sh

gnome-terminal -- /bin/bash /home/mayara-cruz/embarcados2021/Projeto-braco/1slave.sh

gnome-terminal -- /bin/bash /home/mayara-cruz/embarcados2021/Projeto-braco/2slave.sh

gnome-terminal -- /bin/bash /home/mayara-cruz/embarcados2021/Projeto-braco/candumpLog.sh
