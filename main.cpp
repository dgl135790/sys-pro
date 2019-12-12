#include <QApplication>
#include "mainwindow.h"

#include<sys/types.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/stat.h>
int main(int argc, char *argv[])
{
    //创建无名管道
    //int fd[2];
    //pipe(fd);
    //创建有名管道
    int ret = mkfifo("myfifo",0777);
    if (ret < 0)
    {
        perror("mkfifo");
    }
    //创建父子进程
    pid_t pid=fork();
    if(pid < 0)
    {
        perror("fork");
    }
    else if(pid == 0)//子进程启动播放器
    {
        int fd1=open("myfifo",O_WRONLY);
        close(1);
        dup2(fd1,1);
        execlp("mplayer","mplayer","-slaver","-quiet","-idle","-input","file=./fifo_cmd","coffee.mp3",NULL);
        write(fd1,"cc",sizeof("cc"));
        while(1);

    }
    else//父进程显示界面
    {
    //界面显示
    QApplication a(argc, argv);
    //创建按钮
    //按钮事件（写命名管道）
    char buf[128]="";
    int fd2=open("myfifo",O_RDONLY);
    read(fd2,buf,sizeof(buf));

    MainWindow w;
    w.show();
    return a.exec();
    }
}
