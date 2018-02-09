#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

//初始化互斥锁和条件变量
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond =PTHREAD_COND_INITIALIZER;

typedef struct listNode Node;
struct listNode
{
	int num;
	struct listNode * pNext;
};


Node *phead=NULL;//define the head point;

void * producter(void * arg)
{
	for(;;)
	{
		//只产生一个节点，即停止生产
		while(phead==NULL)
		{
			//产生一个新的节点
			Node *tmpNode=(Node*)malloc(sizeof(Node));
			tmpNode->num=rand()%1000+1;
			pthread_mutex_lock(&mutex);
			//把节点加入到链表
			tmpNode->pNext=phead;//头插法，即新节点的指针域指向链表的头指针
			phead=tmpNode;//新节点的指针变为链表的头指针。
			cout<<"producing the num = "<<tmpNode->num<<endl;
			pthread_mutex_unlock(&mutex);
			//添加结束，发送信号
			pthread_cond_signal(&cond);
		}
		//	sleep(rand()%1);//方便观察
	}
	return NULL;
}

void * consumer(void * arg)
{
	for (;;)
	{
		pthread_mutex_lock(&mutex);
		//判断链表是否为空，如果为空，则调用条件变量函数，阻塞线程
		while (phead==NULL)
		{
			//阻塞线程，释放锁（cpu控制权），收到signal后，释放线程,获得锁
			pthread_cond_wait(&cond,&mutex);
		}
		//如果不为空，则用头删法删除链表的头节点
		//定义一个临时节点保存被删除的头部节点
		Node *tmpNode=phead;
		phead=phead->pNext;//头节点的指针域存的地址作为新的头节点
		cout<<"---------------consumer--------"<<tmpNode->num<<endl;
		free(tmpNode);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}



int main(void)
{
	//定义并创建生产者和消费者线程
	pthread_t ptid,ctid;
	pthread_create(&ptid,NULL,producter,NULL);
	pthread_create(&ctid,NULL,consumer,NULL);

	//等待线程结束
	pthread_join(ptid,NULL);
	pthread_join(ctid,NULL);

	return 0;

}
