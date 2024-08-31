Linux kernel module demo program
=================

## Development(WSL1)
| Item   | Ver. |Remarks|
|--------|--------|--------|
| OS     | Ubuntu 18.04.5 LTS |WSL1|
| Kernel | 4.19.104-microsoft-standard||
| GCC    | 7.5.0||

## Build(ex. 4.19.104-microsoft-standard)
```
sudo apt-get install build-essential flex bison libssl-dev libelf-dev bc

cd /usr/src
sudo git clone https://github.com/microsoft/WSL2-Linux-Kernel.git
sudo bash -c "cat /proc/config.gz  | gzip -dc > /usr/src/WSL2-Linux-Kernel/.config"

cd /usr/src/WSL2-Linux-Kernel
sudo make prepare
sudo make scripts

cd $HOME
git clone https://github.com/hidekuno/counter.git
cd counter/linux/src
KERN_SRC_DIR=/usr/src/WSL2-Linux-Kernel make
```
## Development(WSL2)
| Item   | Ver. |Remarks|
|--------|--------|--------|
| OS     | Ubuntu 20.04 LTS |WSL2|
| Kernel | 5.x.x-microsoft-standard||
| GCC    | 9.3.x,9.4.x||

## Build(ex. 5.4.72-microsoft-standard)
```
sudo apt-get install build-essential flex bison libssl-dev libelf-dev bc

cd /usr/src
sudo git clone https://github.com/microsoft/WSL2-Linux-Kernel.git

cd /usr/src/WSL2-Linux-Kernel
sudo git checkout linux-msft-5.4.72
sudo bash -c "cat /proc/config.gz | gzip -dc > /usr/src/WSL2-Linux-Kernel/.config"
sudo make modules_prepare scripts

cd $HOME
git clone https://github.com/hidekuno/counter.git
cd counter/linux/src
KERN_SRC_DIR=/usr/src/WSL2-Linux-Kernel make
```
## Build(ex. 5.10.60.1-microsoft-standard-WSL2)
```
sudo apt-get install build-essential flex bison libssl-dev libelf-dev bc

cd /usr/src
sudo git clone https://github.com/microsoft/WSL2-Linux-Kernel.git

cd /usr/src/WSL2-Linux-Kernel
sudo git checkout linux-msft-wsl-5.10.y
sudo bash -c "cat /proc/config.gz | gzip -dc > /usr/src/WSL2-Linux-Kernel/.config"
sudo make modules_prepare scripts

cd $HOME
git clone https://github.com/hidekuno/counter.git
cd counter/linux/src
KERN_SRC_DIR=/usr/src/WSL2-Linux-Kernel make
```
## Build(ex. 5.15.90.1-microsoft-standard-WSL2)
```
sudo apt-get install build-essential flex bison libssl-dev libelf-dev bc
sudo apt-get install dwarves

cd /usr/src
sudo git clone https://github.com/microsoft/WSL2-Linux-Kernel.git

cd /usr/src/WSL2-Linux-Kernel
sudo git checkout linux-msft-wsl-5.15.y
sudo bash -c "cat /proc/config.gz | gzip -dc > /usr/src/WSL2-Linux-Kernel/.config"
sudo make olddefconfig modules_prepare scripts

cd $HOME
git clone https://github.com/hidekuno/counter.git
cd counter/linux/src
KERN_SRC_DIR=/usr/src/WSL2-Linux-Kernel make CFLAGS=-DKERNEL_51X
```

## Development(Amazon Linux)
| Item   | Ver. |Remarks|
|--------|--------|--------|
| OS     | Amazon Linux release 2023 (Amazon Linux)|AWS EC2|
| Kernel | 6.1.21-1.45.amzn2023.x86_64||
| GCC    | 11.3.1 20221121|Red Hat 11.3.1-4|

## Build(ex. 6.1.21-1.45.amzn2023.x86_64)
```
cd $HOME
sudo dnf install flex bison openssl-devel libelf
sudo dnf install dwarves
sudo dnf install python3-devel rpm-build

dnf download --source kernel
rpm -ivh kernel-6.1.21-1.45.amzn2023.src.rpm
rpmbuild -bp rpmbuild/SPECS/kernel.spec
cd rpmbuild/BUILD/kernel-6.1.21.amzn2023/linux-6.1.21-1.45.amzn2023.x86_64
make olddefconfig modules_prepare scripts

cd $HOME
git clone https://github.com/hidekuno/counter.git
cd counter/linux/src
export KERN_SRC_DIR=${HOME}/rpmbuild/BUILD/kernel-6.1.21.amzn2023/linux-6.1.21-1.45.amzn2023.x86_64
make CFLAGS=-DKERNEL_51X
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
