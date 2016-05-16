openwrt_barri_origin.tar.bz2 是openwrt barribreaker 官方上下载的源码，未作任何修改。

js9331_openwrt.patch 是将官方源码变成适应JS9331开发板的补丁。将其复制到解压后的源码目录openwrt_barri_origin下，输入命令“patch -p1 < js9331_openwrt.patch”。

js9331_config 是JS9331开发板openwrt系统源码配置文件。将其复制到解压后的openwrt源码目录openwrt_barri_origin下，输入命令“mv js9331_config .config”。

“dl”文件夹是存放openwrt编译时需要下载的安装包，将其覆盖掉openwrt_barri_origin/dl文件夹，可大大减小编译时间。