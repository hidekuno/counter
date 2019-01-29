カーネルモジュールのサンプル
=================

# 動作環境
| Item   | Ver. |備考|
|--------|--------|--------|
| OS     | CentOS7 | |
| カーネル | 3.10.0-957.1.3||
| GCC    | 4.8.5||


# counter(/dev版)
```
cd special_file
make
sudo insmod counter.ko
sudo mknod /dev/counter c 90 1
cat /dev/counter
sudo rmmod counter

```

# counter(/proc版)
```
cd proc_file
make
sudo insmod counter.ko
cat /proc/counter
sudo rmmod counter

```

# page_cache
```
sudo insmod page_cache.ko
echo "1234567890" > test_file
./test.pl
sudo echo ${processID} > /proc/test_cache
dmesg
sudo rmmod page_cache
```
