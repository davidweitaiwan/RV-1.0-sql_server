#!/bin/bash

# 更新套件清單
sudo apt-get update

# 安裝必要的套件
sudo apt install -y build-essential
sudo apt-get install -y cmake
sudo apt-get install -y qtbase5-dev
sudo apt-get install -y libqt5sql5-mysql
sudo apt-get install -y mysql-server
sudo apt install -y mysql-client
sudo apt install -y libmysqlclient-dev
sudo apt-get install -y net-tools

# 下載並安裝 DBeaver
wget https://dbeaver.io/files/dbeaver-ce_latest_amd64.deb
sudo dpkg -i dbeaver-ce_latest_amd64.deb
