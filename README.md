# 專案部屬

------
專案環境:Visual studio 2022
使用框架:QT6.4.1
QT下載: https://www.qt.io/download-open-source
MySQL Drive:https://github.com/thecodemonkey86/qt_mysql_driver/releases

將檔案放入安裝路徑
example:C:\Qt\6.4.1\msvc2019_64\plugins
將動態連結庫dll放入
example:
C:\Qt\6.4.1\msvc2019_64\bin

可以透過Visual Studio 延伸模組擴充QT工具
並設定QT路徑

### 安裝MySQL CLI for Client
2022/3/29匯出SQL檔案需要安裝SQL CLI工具
#### windows
透過以下網址安裝，並選用MySQL Server 8.0
https://dev.mysql.com/downloads/mysql/

進入安裝程序請選用MySQL Server

#### Linux
使用指令
```bash
apt-get install mysql-client-*
```

# MySQL Server X Linux
Linux安裝指令
```bash
sudo apt-get install mysql-server
sudo apt install mysql-client
sudo apt install libmysqlclient-dev
```
透過netstat監測Mysql是否正在工作

```bash
sudo apt-get install net-tools
sudo netstat -tap | grep mysql

```
查看mysql root 預設帳戶密碼
```bash
sudo nano /etc/mysql/debian.cnf
```
進入MySQL設定使用者
```bash
mysql -u "user" -p "password"
```

建立使用者
```MySQL
CREATE USER 'user'@'%' IDENTIFIED BY 'password';
```
建立Database
```MySQL
CREATE DATABASE drive CHARACTER SET utf8 COLLATE utf8_general_ci;
```


給予權限
```MySQL
GRANT ALL PRIVILEGES ON 'DBName'.* TO 'UserName'@'%';
```
2023/03/29
SQL匯出需要執行權限
```MySQL
GRANT PROCESS ON *.* TO 'Username'@'HostName';

```