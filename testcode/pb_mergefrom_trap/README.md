pb好用, 性能虽然比不上二进制, 不过也比json在大多数情况下还是好很多. 尤其是字段的向前向后兼容, 这一点应该也是大多数同学
选用pb的原因. 然后终于**掉进坑**了.

整理测试程序的时候发现, python的官方指南压根就没提MergeFrom这个接口, 看起来是专门用来坑c++的兄弟的啊

参考博客链接:

[protocol buffer MergeFrom的坑](http://blog.csdn.net/wzzfeitian/article/details/69803166)