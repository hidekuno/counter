#pragma D option quiet
#pragma D option defaultargs
#pragma D option switchrate=10hz
#pragma D option bufsize=8m

fbt:kernel:nfs_strategy:entry,
fbt:kernel:ncl_vinvalbuf:entry,
fbt:kernel:ncl_write:entry,
fbt:kernel:bremfreef:entry,
fbt:kernel:newnfs_msleep:entry,
fbt:kernel:ncl_asyncio:entry,
fbt:kernel:newnfs_sigintr:entry,
fbt:kernel:nfs_directio_write:entry
/pid != $pid && execname == "dd"/
{
        printf("%s entry %d\n", probefunc, timestamp);
}

fbt:kernel:nfs_strategy:return,
fbt:kernel:ncl_vinvalbuf:return,
fbt:kernel:ncl_write:return,
fbt:kernel:bremfreef:return,
fbt:kernel:newnfs_msleep:return,
fbt:kernel:newnfs_sigintr:return,
fbt:kernel:ncl_asyncio:return,
fbt:kernel:nfs_directio_write:return
/pid != $pid && execname == "dd"/
{
        printf("%s return %d %d\n", probefunc, timestamp, arg1);
}
/* 
fbt:kernel:crhold:entry,
fbt:kernel:ncl_vinvalbuf:entry,
fbt:kernel:ncl_write:entry

fbt:kernel:crhold:return,
fbt:kernel:ncl_vinvalbuf:return,
fbt:kernel:ncl_write:return
 */
