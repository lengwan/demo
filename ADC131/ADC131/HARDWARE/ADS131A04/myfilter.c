#include "myfilter.h"

/*//////////////////////////////////////////////////////////////////////////
方法二：中位值滤波法
方法： 连续采样N次（N取奇数），把N次采样值按大小排列，取中间值为本次有效值。
优点：克服偶然因素（对温度、液位的变化缓慢的被测参数有良好的滤波效果）
缺点：对流量、速度等快速变化的参数不宜
//////////////////////////////////////////////////////////////////////////*/
//#define N 3

//u16 value_buf[N]; 
//u16 filter2()
//{  
//  u16 count,i,j,temp;
//  for(count=0;count<N;count++)
//  {
//    value_buf[count] =  ftable[a];
//	  a++;
//	  if(a==255) a=0;
//  }
//	for (j=0;j<N-1;j++)
//	{
//		 for (i=0;i<N-j;i++)
//		 {
//			if ( value_buf[i] >  value_buf[i+1] )
//			{
//			 temp = value_buf[i];
//			 value_buf [i]= value_buf[i+1]; 
//			 value_buf[i+1] = temp;
//			}
//		 }
//	}
////	printf("%d\n",value_buf[(N-1)/2]);
//	return value_buf[(N-1)/2];
//}
//void pros2()
//{
//   print_host(4,filter2());
//}

/*//////////////////////////////////////////////////////////////////////////
方法四：递推平均滤波法（又称滑动平均滤波法）
方法： 把连续取得的N个采样值看成一个队列，队列的长度固定为N，
       每次采样到一个新数据放入队尾，并扔掉原来队首的一次数据（先进先出原则），
       把队列中的N个数据进行算术平均运算，获得新的滤波结果。
       N值的选取：流量，N=12；压力，N=4；液面，N=4-12；温度，N=1-4。
优点：对周期性干扰有良好的抑制作用，平滑度高；
      适用于高频振荡的系统。
缺点：灵敏度低，对偶然出现的脉冲性干扰的抑制作用较差；
      不易消除由于脉冲干扰所引起的采样值偏差；
      不适用于脉冲干扰比较严重的场合；
      比较浪费RAM。
//////////////////////////////////////////////////////////////////////////*/

#define FILTER4_N 3
//u16 filter_buf[FILTER4_N + 1];
//u16 filter4() 
//{
//  int i;
//  int filter_sum = 0;
//  filter_buf[FILTER4_N] = ftable[a];		
//	a++;
//	if(a==255) a=0;
//  for(i = 0; i < FILTER4_N; i++) 
//	{
//    filter_buf[i] = filter_buf[i + 1]; // 所有数据左移，低位仍掉
//    filter_sum += filter_buf[i];
//  }
//  return (int)(filter_sum / FILTER4_N);
//}


//void pros4(void)
//{
//	u16 i=0;
//  print_host(4,filter4());
//}
/*//////////////////////////////////////////////////////////////////////////
方法五：中位值平均滤波法（又称防脉冲干扰平均滤波法）
方法： 采一组队列去掉最大值和最小值后取平均值，     （N值的选取：3-14）。 
      相当于“中位值滤波法”+“算术平均滤波法”。
      连续采样N个数据，去掉一个最大值和一个最小值，
      然后计算N-2个数据的算术平均值。    
优点： 融合了“中位值滤波法”+“算术平均滤波法”两种滤波法的优点。
       对于偶然出现的脉冲性干扰，可消除由其所引起的采样值偏差。
       对周期干扰有良好的抑制作用。
       平滑度高，适于高频振荡的系统。
缺点：对于测量速度较慢或要求数据计算速度较快的实时控制不适用，比较浪费RAM。
//////////////////////////////////////////////////////////////////////////*/

//#define N 3
//int filter5() 
//{
//  int i, j;
//  int filter_temp, filter_sum = 0;
//  int filter_buf[N];
//  for(i = 0; i < N; i++) 
//	{
//    filter_buf[i] = ftable[a];
//		a++;
//		if(a==255)   a=0;
//    delay_us(10);
//  }
//  // 采样值从小到大排列（冒泡法）
//  for(j = 0; j < N - 1; j++) 
//	{
//    for(i = 0; i < N - 1 - j; i++) 
//		{
//      if(filter_buf[i] > filter_buf[i + 1]) 
//			{
//        filter_temp = filter_buf[i];
//        filter_buf[i] = filter_buf[i + 1];
//        filter_buf[i + 1] = filter_temp;
//      }
//    }
//  }
//  // 去除最大最小极值后求平均
//  for(i = 1; i < N - 1; i++) filter_sum += filter_buf[i];
////	printf("%d\n",filter_sum / ( N - 2));
//  return filter_sum / (N - 2);
//}

//void pros5(void)
//{
//	u16 i=0;
//	for(i=0;i<255;i++)
//	{
//     print_host(ftable[i],filter5());
//	}
//}

/*//////////////////////////////////////////////////////////////////////////
方法六：限幅平均滤波法
方法： 相当于“限幅滤波法”+“递推平均滤波法”；
       每次采样到的新数据先进行限幅处理，
       再送入队列进行递推平均滤波处理。
优点： 融合了两种滤波法的优点；
      对于偶然出现的脉冲性干扰，可消除由于脉冲干扰所引起的采样值偏差。
缺点：比较浪费RAM。
//////////////////////////////////////////////////////////////////////////*/

//#define FILTER6_N 3
//#define FILTER6_A 51
//int filter_buf[FILTER6_N];

//int filter6() 
//{
//  int i;
//  int filter_sum = 0;
//  filter_buf[FILTER6_N - 1] = ftable[a];		
//	a++;
//	if(a==255)   a=0;
//  if(((filter_buf[FILTER6_N - 1] - filter_buf[FILTER6_N - 2]) > FILTER6_A) || ((filter_buf[FILTER6_N - 2] - filter_buf[FILTER6_N - 1]) > FILTER6_A))
//    filter_buf[FILTER6_N - 1] = filter_buf[FILTER6_N - 2];
//  for(i = 0; i < FILTER6_N - 1; i++) 
//	{
//    filter_buf[i] = filter_buf[i + 1];
//    filter_sum += filter_buf[i];
//  }
////	printf("%d\n",filter_sum / ( FILTER6_N - 1));
//  return filter_sum / (FILTER6_N - 1);
//}

//void pros6(void)
//{
//  print_host(4,filter6());
//}
/*//////////////////////////////////////////////////////////////////////////
方法七：一阶滞后滤波法
方法： 取a=0-1，本次滤波结果=(1-a)*本次采样值+a*上次滤波结果。
优点：  对周期性干扰具有良好的抑制作用；
        适用于波动频率较高的场合。
       平滑度高，适于高频振荡的系统。
缺点： 相位滞后，灵敏度低；
      滞后程度取决于a值大小；
      不能消除滤波频率高于采样频率1/2的干扰信号。
//////////////////////////////////////////////////////////////////////////*/

//#define FILTER7_A 0.01
//u16 Value;
//u16 filter7() 
//{
//  int NewValue;
//	Value = ftable[b-1];		
//  NewValue = ftable[b];		
//	b++;
//	if(b==255)   b=1;
//  Value = (int)((float)NewValue * FILTER7_A + (1.0 - FILTER7_A) * (float)Value);
////	printf("%d\n",Value);
//  return Value;
//}
//void pros7(void)
//{
//	u16 i=0;
//	for(i=0;i<255;i++)
//	{
//     print_host(ftable[i],filter7());
//	}
//}

/*//////////////////////////////////////////////////////////////////////////
方法八：加权递推平均滤波法
方法： 是对递推平均滤波法的改进，即不同时刻的数据加以不同的权；
       通常是，越接近现时刻的数据，权取得越大。
       给予新采样值的权系数越大，则灵敏度越高，但信号平滑度越低。
优点： 适用于有较大纯滞后时间常数的对象，和采样周期较短的系统。
缺点：  对于纯滞后时间常数较小、采样周期较长、变化缓慢的信号；
       不能迅速反应系统当前所受干扰的严重程度，滤波效果差。
//////////////////////////////////////////////////////////////////////////*/

//#define FILTER8_N 12
//int coe[FILTER8_N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};    // 加权系数表
//int sum_coe = 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10 + 11 + 12; // 加权系数和
//int filter_buf[FILTER8_N + 1];
//int filter8() 
//{
//  int i;
//  int filter_sum = 0;
//  filter_buf[FILTER8_N] = ftable[a];		
//	a++;
//	if(a==255)   a=0;
//  for(i = 0; i < FILTER8_N; i++) 
// {
//    filter_buf[i] = filter_buf[i + 1]; // 所有数据左移，低位仍掉
//    filter_sum += filter_buf[i] * coe[i];
//  }
//  filter_sum /= sum_coe;
////	printf("%d\n",filter_sum);
//  return filter_sum;
//}

//void pros8(void)
//{
//	u16 i=0;
//	for(i=0;i<255;i++)
//	{
//     print_host(ftable[i],filter8());
//	}
//}
/*//////////////////////////////////////////////////////////////////////////
方法九： 消抖滤波法
方法：  设置一个滤波计数器，将每次采样值与当前有效值比较：
       如果采样值=当前有效值，则计数器清零；
       如果采样值<>当前有效值，则计数器+1，并判断计数器是否>=上限N（溢出）；
       如果计数器溢出，则将本次值替换当前有效值，并清计数器。
优点：  对于变化缓慢的被测参数有较好的滤波效果；
        可避免在临界值附近控制器的反复开/关跳动或显示器上数值抖动。
缺点：  对于快速变化的参数不宜；
       如果在计数器溢出的那一次采样到的值恰好是干扰值,则会将干扰值当作有效值导入系统。
//////////////////////////////////////////////////////////////////////////*/

//#define FILTER9_N 51
//u16 i = 0;
//u16 Value;
//u16 filter9() 
//{
//  int new_value;
//	Value = ftable[b-1];
//  new_value = ftable[b];		
//	b++;
//	if(b==255)   b=1;
//  if(Value != new_value) 
//	{
//    i++;
//    if(i > FILTER9_N) 
//		{
//      i = 0;
//      Value = new_value;
//    }
//  }
//  else   i = 0;
//  return Value;
//}

//void pros9(void)
//{
//	u16 i=0;
//	for(i=0;i<255;i++)
//	{
//     print_host(ftable[i],filter9());
//	}
//}

/*//////////////////////////////////////////////////////////////////////////
方法十：限幅消抖滤波法
方法： 相当于“限幅滤波法”+“消抖滤波法”；
       先限幅，后消抖。
优点：  继承了“限幅”和“消抖”的优点；
        改进了“消抖滤波法”中的某些缺陷，避免将干扰值导入系统。
缺点：   对于快速变化的参数不宜。
//////////////////////////////////////////////////////////////////////////*/

//#define FILTER10_A 51
//#define FILTER10_N 5
//u16 i = 0;
//u16 Value;

//u16 filter10() 
//{
//  u16 NewValue;
//  u16 new_value;
//	Value = ftable[b-1];
//  NewValue = ftable[b];		
//	b++;
//	if(b==255)   b=1;
//  if(((NewValue - Value) > FILTER10_A) || ((Value - NewValue) > FILTER10_A))
//    new_value = Value;
//  else
//    new_value = NewValue;
//  if(Value != new_value) 
//	{
//    i++;
//    if(i > FILTER10_N) 
//		{
//      i = 0;
//      Value = new_value;
//    }
//  }
//  else   i = 0;
//  return Value;
//}
