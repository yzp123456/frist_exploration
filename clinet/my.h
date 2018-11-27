#ifndef _MY_H
#define _MY_H
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<time.h>
#include<pthread.h>
#include<stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <termios.h>
#include <netinet/in.h>
#define W 32
#define STORAGE_NUM         5
#define GOODS_NUM           10
void* control_fun(void *p);
void *shm_fun(void* p);
void* goodsChange(void *p);
int Dao_update(int storage_id ,int goods_id,int account);

struct cl_msg
{
    long type;
    long msgtype;
    unsigned char text[W];
};
struct storage_goods_info
{
	unsigned char goods_type;
	unsigned int goods_count;
};

struct storage_info
{
	unsigned char storage_status;				// 0:open 1:close
	unsigned char led_status;
	unsigned char buzzer_status;  
	unsigned char fan_status;   
	unsigned char seg_status;
	signed char x;
	signed char y;
	signed char z;
	char samplingTime[20];
	float temperature;
	float temperatureMIN;
	float temperatureMAX;
	float humidity;
	float humidityMIN;
	float humidityMAX;
	float illumination;
	float illuminationMIN;
	float illuminationMAX;
	float battery;
	float adc;
	float adcMIN;
	struct storage_goods_info goods_info[GOODS_NUM];
};
struct env_info_clien_addr
{
    struct storage_info storage_no[STORAGE_NUM];    
};
struct shm_addr
{
	char cgi_status;
	char qt_status;
	struct env_info_clien_addr rt_status;
};
struct Data
{
	int data_flag; // 3 仓库 1 货物 2 控制
	int storeHouse_id;
	//仓库信息
	float temp;	  
	float humidity;
	float light;
	char speed[3]; 
	// 货物信息
	int good_id;
	//char* good_name;	
	int good_num;
	// 控制信息
	char control_sign;
	struct shm_addr shm_buf;		 
};
typedef struct sendAndRecv{

char ecc1;    //校验位
char card;    //读卡信号
char led;      //灯
char spe;     //蜂鸣器
char fan;      //风扇
char x;         //三轴
char y;
char z;
char seg7led;  //显像管
char light[4];    //光照
char vol[7];     //电压
char temp[7];  //温度
char hum[7];   //湿度
}s_r;
#endif

