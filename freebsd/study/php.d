dtrace:::BEGIN
{
        printf("start\n");
}
pid$target::mmap:entry
{
        printf("entry %d %d\n",timestamp,arg2);
}
pid$target::mmap:return
{
        printf("return %d %d\n", timestamp, arg1);
}
pid$target::_php_stream_mmap_range:entry
{
        printf("entry %d %d\n",timestamp,arg2);
}
pid$target::_php_stream_mmap_range:return
{
        printf("return %d %d\n", timestamp, arg1);
}
pid$target::php_stdiop_read:entry
{
        printf("entry %d %d\n",timestamp,arg2);
}
pid$target::php_stdiop_read:return
{
        printf("return %d %d\n", timestamp, arg1);
}
pid$target::php_stdiop_write:entry
{
        printf("entry %d %d\n",timestamp,arg2);
}
pid$target::php_stdiop_write:return
{
        printf("return %d %d\n", timestamp, arg1);
}
syscall::write:entry
/pid != $pid && (execname == "php" || execname == "php-8.2")/
{
        printf("entry %d 0x%p %d\n",timestamp,arg1, arg2);
}
syscall::write:return
/pid != $pid && (execname == "php" || execname == "php-8.2")/
{
        printf("return %d %d\n", timestamp, arg1);
}
