カーネルモジュールのサンプル
=================

# 動作環境
| Item   | Ver. |備考|
|--------|--------|--------|
| OS     | Ubuntu 18.04.5 LTS | |
| カーネル | 4.19.104-microsoft-standard||
| GCC    | 7.5.0||


# build
```
git clone https://github.com/hidekuno/counter.git
cd src
KERN_SRC=${where} make
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
