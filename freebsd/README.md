Linux kernel module demo program
=================
## Overview
- This Program is a like incremental generator.

## Development(FreeBSD)
| Item   | Ver. |Remarks|
|--------|--------|--------|
| OS     | FreeBSD 13.2| on VMware|
| Clang  | 7.5.0||

## Build
```
sudo pkg install git
sudo git clone -b releng/13.2 --depth 1 https://github.com/freebsd/freebsd-src /usr/src
cd $HOME
git clone https://github.com/hidekuno/counter.git
cd counter/freebsd/src
make
```

## Test & Run
```
$ sudo kldload ./counter.ko
$ cat /dev/counter
0
$ cat /dev/counter
1
$ cat /dev/counter
2
$ sudo bash -c "echo 100 > /dev/counter"
$ cat /dev/counter
99999998
$ cat /dev/counter
99999999
$ cat /dev/counter
0
$ cat /dev/counter
1
$ cat /dev/counter
2
$ cat /dev/counter
3
$ sudo kldunload ./counter.ko
$
```

## Debug
```
sudo dtrace -F -s counter.d -c "cat /dev/counter"
```
