カーネルモジュールのサンプル
=================
# counter
```
sudo insmod counter.ko
sudo mknod /dev/counter c 90 1
cat /dev/counter
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
