Linuxカーネルモジュールのサンプル
=================

# 開発環境
| Item   | Ver. |備考|
|--------|--------|--------|
| OS     | Ubuntu 18.04.5 LTS | |
| カーネル | 4.19.104-microsoft-standard||
| GCC    | 7.5.0||


# build
```
sudo apt-get install libelf-dev bison flex

sudo git clone https://github.com/microsoft/WSL2-Linux-Kernel.git /usr/src/4.19.104-microsoft-standard
sudo bash -c "cat /proc/config.gz  | gzip -dc > /usr/src/4.19.104-microsoft-standard/.config"

cd /usr/src/4.19.104-microsoft-standard
sudo make prepare
sudo make scripts

cd $HOME
git clone https://github.com/hidekuno/counter.git
cd counter/src
KERN_SRC=/usr/src/4.19.104-microsoft-standard make
```

# counter(/dev版)
```
sudo mknod /dev/counter c 90 1
sudo insmod counter.ko
cat /dev/counter
sudo rmmod counter
```

# counter(/proc版)
```
sudo insmod sequence.ko
cat /proc/sequence
sudo rmmod sequence
```
