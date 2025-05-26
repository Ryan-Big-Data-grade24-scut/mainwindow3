# mainwindow
## 使用方法：
- 克隆项目，打开克隆后的项目文件地址
- 创建新的qt_widdget项目，名字必须为mainwindow，配置好opencv和network
- 先直接把下面的东西复制替换到新的项目文件夹的对应位置
	- .vcxproj.filters
	- ./mainwindow/mainwindow.ui 与 ./mainwindow/x64/Debug/qt/uic/ui_mainwindow.h
		- 关闭.sln文件，直接复制替换后再打开
	- Opencv部分：
 		- 包括各类opts.h和processor.h/.cpp以及Opt_types.h
		- 所有都可以直接复制到./mainwindow下
		- 复制后，打开.sln，添加到对应的筛选器下
- main.cpp, mainwindow.h, mainwindow.cpp
	- 不可以替换，只能够把代码粘贴到新的包
