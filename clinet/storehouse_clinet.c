/*
*  仓库客户端 模拟m0
*/
#include"my.h"
int fd;
struct shm_addr shm_buf;
pthread_mutex_t mutex;
void init_uart(int fd)             // zigbee串口初始化函数   文件描述符 fd
{
	struct termios opt;
	tcgetattr(fd, &opt);
	opt.c_cflag |= (CLOCAL | CREAD);
	opt.c_cflag &= ~CSIZE;
	opt.c_cflag &= ~CRTSCTS;
	opt.c_cflag |= CS8;
	opt.c_iflag |= IGNPAR;
	opt.c_cflag &= ~CSTOPB;
	opt.c_oflag = 0;
	opt.c_iflag &= ~(INLCR | ICRNL | IGNCR);
	opt.c_oflag &= ~(ONLCR | OCRNL);
	opt.c_lflag = 0;
	cfsetispeed(&opt, B115200);        
	cfsetospeed(&opt, B115200);
	tcsetattr(fd, TCSANOW, &opt);
}
void imitate_data(struct shm_addr* shm_buf)
{
		s_r data;		
     		//打开设备节点		
		int usb_fd = open("/dev/ttyUSB0",O_RDWR|O_NOCTTY|O_NONBLOCK);
		if(usb_fd ==-1)
		{
			//perror("m0 error");
		}        
		//调用初始化函数
		init_uart(usb_fd);
		
		int ret =read(usb_fd,&data,sizeof(data));
	if(ret > 0 && data.ecc1 ==1 ){		
		printf("update str message\n");
 		
		int storage_num = 1; //仓库号
		int good_num = 1; // 货物号
		struct env_info_clien_addr cline_info;
		struct storage_info   stroage;
		struct storage_goods_info goods;

		stroage.storage_status = 1;	
		stroage.led_status = data.led;
		stroage.buzzer_status =data.spe;  
		stroage.fan_status=data.fan;   
		stroage.seg_status=data.seg7led;
		stroage.x=data.x;
		stroage.y=data.y;
		stroage.z=data.z;
		//stroage.samplingTime[20]="s";
		sprintf(stroage.samplingTime,"what?");
		stroage.temperature=atof(data.temp);
		stroage.temperatureMIN=-10;
		stroage.temperatureMAX=30;
		stroage.humidity=atof(data.hum);
		stroage.humidityMIN=10;
		stroage.humidityMAX=100;
		stroage.illumination=atoi(data.light);
		stroage.illuminationMIN=0;
		stroage.illuminationMAX=1500;
		stroage.battery=atof(data.vol);
		stroage.adc=data.card;
		stroage.adcMIN=10.5;
		goods.goods_type = 'c';
		goods.goods_count =100;
		stroage.goods_info[good_num] = goods;
		cline_info.storage_no[storage_num] = stroage;
		shm_buf->rt_status=cline_info;
	}
}

void control(struct Data datas)//  向zigbee 写入字符
{
	s_r data;
     	//打开设备节点
	int usb_fd = open("/dev/ttyUSB0",O_RDWR|O_NOCTTY|O_NONBLOCK);
	if(usb_fd ==-1)
	{
		perror("usb error");
	}        
	//调用初始化函数
	init_uart(usb_fd);
	
	if(datas.data_flag == 2)
	{
		char sign = datas.control_sign;
		int ret = write(usb_fd,&sign,sizeof(char));
		if(ret > 0)
		{
			printf("successed! %c ",sign);		
		}
	}
}
void goodsMoni(struct Data* data,int a)
{
	data->data_flag  = 1;
	data->good_id = 1;	
	data->good_num = a;
}
void *fun(void *p){
int ret;
   while(1){
	//接收控制信号
 	struct Data data;
 	ret= recv(fd,&data,sizeof(data),0);
	if(ret>0){
		pthread_mutex_lock(&mutex);
		control(data);
		pthread_mutex_unlock(&mutex);
   	}
   }
}
void *shmChange(void *p)  //
{
   struct Data data;
  
   while(1){
	sleep(4);
	//   4s/发一次m0状态信息
	
	pthread_mutex_lock(&mutex);
	imitate_data(&shm_buf);   // 从m0获取仓库状态
	pthread_mutex_unlock(&mutex);
	// read card
	if(shm_buf.rt_status.storage_no[1].adc != 0 )
	{
		printf("card read!!!\n");
		goodsMoni(&data,30); //模拟入库30
 		send(fd,&data,sizeof(data),0);
	}
	
	data.data_flag = 3;
	data.shm_buf = shm_buf; 	
	send(fd,&data,sizeof(data),0);
   }
}
int main(int argc, char const *argv[])
{
    srand(time(0));
    if(argc == 1)
    {
	printf("please input port and address\n");
	return 0;
    }
    struct Data data;
    int port =atoi(argv[1]);
    pthread_t pt_id;
    pthread_t shmChange_id;
    int account=0;
    fd =socket(AF_INET,SOCK_STREAM,0);
   
    struct sockaddr_in youaddr;
    youaddr.sin_family =AF_INET;
    youaddr.sin_port = htons(port);
    youaddr.sin_addr.s_addr=inet_addr(argv[2]);
    connect(fd,(struct sockaddr *)&youaddr,sizeof(youaddr));
    
    
    pthread_mutex_init(&mutex,NULL);
    
    pthread_create(&shmChange_id,NULL,shmChange,NULL);
    pthread_create(&pt_id,NULL,fun,NULL); // 接收控制信号
      // 
    
    while(1){
	scanf("%d",&account);
	getchar();
	
    }
	return 0;
}
