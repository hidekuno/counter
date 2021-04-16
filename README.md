Linux kernel module demo program(like incremental generator)
=================

## Development
| Item   | Ver. |Remarks|
|--------|--------|--------|
| OS     | Ubuntu 18.04.5 LTS |WSL2|
| Kernel | 4.19.104-microsoft-standard||
| GCC    | 7.5.0||

| Item   | Ver. |Remarks|
|--------|--------|--------|
| OS     | Ubuntu 20.04 LTS |WSL2|
| Kernel | 5.4.72-microsoft-standard||
| GCC    | 9.3.0||

## build(ex. 4.19.104-microsoft-standard)
```
sudo apt-get install libelf-dev bison flex

cd /usr/src
sudo git clone https://github.com/microsoft/WSL2-Linux-Kernel.git
sudo bash -c "cat /proc/config.gz  | gzip -dc > /usr/src/WSL2-Linux-Kernel/.config"

cd /usr/src/WSL2-Linux-Kernel
sudo make prepare
sudo make scripts

cd $HOME
git clone https://github.com/hidekuno/counter.git
cd counter/src
KERN_SRC=WSL2-Linux-Kernel make
```

## build(ex. 5.4.72-microsoft-standard)
```
sudo apt-get install libelf-dev bison flex

cd /usr/src
sudo git clone https://github.com/microsoft/WSL2-Linux-Kernel.git

cd /usr/src/WSL2-Linux-Kernel
git checkout linux-msft-5.4.72
sudo bash -c "cat /proc/config.gz  | gzip -dc > /usr/src/WSL2-Linux-Kernel/.config"
make modules_prepare
make scripts

cd $HOME
git clone https://github.com/hidekuno/counter.git
cd counter/src
KERN_SRC=WSL2-Linux-Kernel make
```

## Test & Run
```
$ sudo mknod /dev/counter c 90 1
$ sudo insmod counter_core.ko
$ sudo insmod counter.ko
$ cat /dev/counter
0
$ cat /dev/counter
1
$ cat /dev/counter
2
$ sudo bash -c "echo -n 99999998 > /dev/counter"
$ cat /dev/counter
99999998
$ cat /dev/counter
99999999
$ cat /dev/counter
0
$ sudo rmmod counter counter_core
$
```
