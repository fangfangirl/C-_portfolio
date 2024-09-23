#include<iostream>
#include <fstream>
#include <string.h>
using namespace std;
int count_numtrade_1=0,count_numtrade_2=0,count_numtrade_3=0,count_numtrade_4=0,count_numtrade_5=0,count_numtrade_6=0,count_numtrade_7=0;
int count_slp_2=0,count_slp_5=0,count_slp_3=0,count_slp_7=0;
int count_tpp_5=0,count_tpp_3=0,count_tpp_7=0;
int count_BreakBuy_4/*��}*/=0,count_BreakSell_4/*�^�}*/=0;
int count_BreakBuy_5/*��}*/=0,count_BreakSell_5/*�^�}*/=0;
int count_BreakBuy_6/*��}*/=0,count_BreakSell_6/*�^�}*/=0,count_FaBreakBuy_6/*���^�}*/=0,count_FaBreakSell_6/*����}*/=0,count_callback_6/*�伵*/=0,count_bouncesell_6/*�ϼu*/=0;
int count_BreakBuy_7/*��}*/=0,count_BreakSell_7/*�^�}*/=0,count_FaBreakBuy_7/*���^�}*/=0,count_FaBreakSell_7/*����}*/=0,count_callback_7/*�伵*/=0,count_bouncesell_7/*�ϼu*/=0;
struct TX{
	int year ,month,day,date ,exdate;
	int price[285][3];
}; 

int strategy1(TX*ptx)//
{
	int r=(ptx->price[284][2]-ptx->price[0][2])*200;
//	cout<<r<<endl;//�C�ѳ̲���Q 
	count_numtrade_1++;
	return r;
}

int strategy2(TX*ptx,double stoploss)//�����@+���l�I 
{
	int cost=ptx->price[0][2];
	int slp=(int)(cost*(1-stoploss)+0.5);//�|�ˤ��J 
	int n,final_price;
	for(n=1;n<285;n++)
	{
		final_price=ptx->price[n][2];
		if(ptx->price[n][2]<=slp)
		{
			count_slp_2++;
//			cout<<"���l��:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;
			break; 
		}
	}
	count_numtrade_2++;
	return (final_price-cost)*200;
}

int strategy3(TX*ptx,double stoploss,double takeprofit)//�����@+���l�M���Q 
{
	int cost=ptx->price[0][2];
	int slp=(int)(cost*(1-stoploss)+0.5);
	int tpp=(int)(cost*(1+takeprofit)+0.5);
	int n,final_price;
	int count_slp=0,count_tpp=0;
	for(n=1;n<285;n++)
	{
		final_price=ptx->price[n][2];
		if(ptx->price[n][2]<=slp)
		{
			count_slp_3++;
//			cout<<"���l��:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;//�i�D���X�Ѱ��l 
			break; 
		}
		if(ptx->price[n][2]>=tpp)
		{
			count_tpp_3++;
//			cout<<"���Q��:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;//�i�D���X�Ѱ��Q  
			break; 
		}
	}
	count_numtrade_3++;
	return (final_price-cost)*200;
}

int strategy4(TX*ptx)
{
	double MA5[285];
	double sum;
	int cost,position=0,r=0;
	int n,i;
	for(n=4;n<285;n++)
	{
		sum=0;
		for(i=n-4;i<=n;i++)
			sum+=ptx->price[i][2];
		MA5[n]=sum/5;
	}
	for(n=6;n<285;n++)
	{
		if(ptx->price[n-1][2]<=MA5[n-1]&&ptx->price[n][2]>MA5[n]&&MA5[n-2]>MA5[n-1]&&MA5[n]>MA5[n-1])//��} 
		{
			if(position==0)
			{
				position=1;
				cost=ptx->price[n][2];
				count_BreakBuy_4++; 
				count_numtrade_4++;
			}
			else if(position==-1)
			{
				position=0;
				r+=(cost-ptx->price[n][2])*200;
				count_BreakBuy_4++;
				count_numtrade_4++;
			}
		}
		else if(ptx->price[n-1][2]>=MA5[n-1]&&ptx->price[n][2]<MA5[n]&&MA5[n-2]<MA5[n-1]&&MA5[n]<MA5[n-1])//�^�} 
		{
			if(position==1)
			{
				position=0;
				r+=(ptx->price[n][2]-cost)*200;
				count_BreakSell_4++;
				count_numtrade_4++;
			}
			else if(position==0)
			{
				position=-1;
				cost=ptx->price[n][2];
				count_BreakSell_4++;
				count_numtrade_4++;
				
			}
		}
	}
	if(position==1)//��Ѧp�G���ѱN�L��X��ū�^ 
	{
		r+=(ptx->price[284][2]-cost)*200;
		count_numtrade_4++;
	} 
	else if(position==-1)
	{
		r+=(cost-ptx->price[284][2])*200;
		count_numtrade_4++;
	}
//	cout<<r<<endl;
	return r;
}

int strategy5(TX*ptx,double stoploss,double takeprofit)// ²�檩�����ѥ���k�h+�����T 
{
	double MA5[285];
	double sum;
	int cost,position=0,r=0;
	int n,i;
	int slp;
	int tpp;
	for(n=4;n<285;n++)//�p�Ⲿ�ʥ����u 
	{
		sum=0;
		for(i=n-4;i<=n;i++)
			sum+=ptx->price[i][2];
		MA5[n]=sum/5;
	}
	for(n=6;n<285;n++)//���ݸ��źѦb�ݰ��l���Q 
	{
		if(ptx->price[n-1][2]<=MA5[n-1]&&ptx->price[n][2]>MA5[n]&&MA5[n-2]>MA5[n-1]&&MA5[n]>MA5[n-1])// ��} 
		{
			if(position==0)
			{
				position=1;
				cost=ptx->price[n][2];
				count_BreakBuy_5++;
				count_numtrade_5++;
			}
			else if(position==-1)
			{
				position=0;
				r+=(cost-ptx->price[n][2])*200;
				count_BreakBuy_5++;
				count_numtrade_5++;
			}
		}
		else if(ptx->price[n-1][2]>=MA5[n-1]&&ptx->price[n][2]<MA5[n]&&MA5[n-2]<MA5[n-1]&&MA5[n]<MA5[n-1])//�^�} 
		{
			if(position==1)
			{
				position=0;
				r+=(ptx->price[n][2]-cost)*200;
				count_BreakSell_5++;
				count_numtrade_5++;
			}
			else if(position==0)
			{
				position=-1;
				cost=ptx->price[n][2];
				count_BreakSell_5++;
				count_numtrade_5++;
			}
		}
		else if(position==-1)//�Ť@�Ѯɪ����l���Q 
		{
			slp=(int)(cost*(1+stoploss)+0.5);
			tpp=(int)(cost*(1-takeprofit)+0.5);
			if(ptx->price[n][2]>=slp)
			{
				count_slp_5++;
				count_numtrade_5++;
				position=0;
				r+=(cost-ptx->price[n][2])*200;
			//	cout<<"���l��:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;
				continue;
			}	
			else if(ptx->price[n][2]<=tpp)
			{
				count_tpp_5++;
				count_numtrade_5++;
				position=0;
				r+=(cost-ptx->price[n][2])*200;
			//	cout<<"���Q��:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;
				continue;
			}
		}
		else if(position==1)//�����@�Ѫ����l���Q 
		{
			slp=(int)(cost*(1-stoploss)+0.5);
			tpp=(int)(cost*(1+takeprofit)+0.5);
			if(ptx->price[n][2]<=slp)
			{
				count_slp_5++;
				count_numtrade_5++;
				r+=(ptx->price[n][2]-cost)*200;
				position=0;
			//	cout<<"���l��:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;
				continue;
			}	
			else if(ptx->price[n][2]>=tpp)
			{
				count_tpp_5++;
				count_numtrade_5++;
				r+=(ptx->price[n][2]-cost)*200;
				position=0;
			//	cout<<"���Q��:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;
				continue;
			}
		}
//		cout<<"n=="<<n<<",,"<<cost<<endl;
	}
	if(position==1)
	{
		r+=(ptx->price[284][2]-cost)*200;
		count_numtrade_5++;
	} 
	else if(position==-1)
	{
		r+=(cost-ptx->price[284][2])*200;
		count_numtrade_5++;
	} 
//	cout<<r<<endl;
	return r;
} 

int strategy6(TX*ptx)// (�����ѥ���k�h)(��}+���^�}+�伵+�^�}+����}+����)
{
	double MA5[285];
	double sum;
	int cost,position=0,r=0;
	int n,i;
	for(n=4;n<285;n++)
	{
		sum=0;
		for(i=n-4;i<=n;i++)
			sum+=ptx->price[i][2];
		MA5[n]=sum/5;
	}
	for(n=6;n<285;n++)
	{
		if(ptx->price[n-1][2]<=MA5[n-1]&&ptx->price[n][2]>MA5[n]&&MA5[n-2]>MA5[n-1]&&MA5[n]>MA5[n-1])//��} 
		{
			if(position==0)
			{
				position=1;
				cost=ptx->price[n][2];
				count_BreakBuy_6++;
				count_numtrade_6++;
			}
			else if(position==-1)
			{
				position=0;
				r+=(cost-ptx->price[n][2])*200;
				count_BreakBuy_6++;
				count_numtrade_6++;
			}
		}
		else if(ptx->price[n-1][2]<=MA5[n-1]&&ptx->price[n][2]>MA5[n]&&MA5[n-2]<MA5[n-1]&&MA5[n]>MA5[n-1])//���^�} 
		{
			if(position==0)
			{
				position=1;
				cost=ptx->price[n][2];
				count_BreakSell_6++;
				count_numtrade_6++;
			}
			else if(position==-1)
			{
				position=0;
				r+=(cost-ptx->price[n][2])*200;
				count_BreakSell_6++;
				count_numtrade_6++;
			}
		}
		else if(ptx->price[n-1][2]>MA5[n-1]&&ptx->price[n][2]>MA5[n]&&ptx->price[n+1][2]>MA5[n+1]&&ptx->price[n][2]<ptx->price[n-1][2]&&ptx->price[n+1][2]>ptx->price[n][2]) //�伵 
		{
			if(position==0)
			{
				position=1;
				cost=ptx->price[n][2];
				count_FaBreakBuy_6++;
				count_numtrade_6++;
			}
			else if(position==-1)
			{
				position=0;
				r+=(cost-ptx->price[n][2])*200;
				count_FaBreakBuy_6++;
				count_numtrade_6++;
			}
		}
		else if(ptx->price[n-1][2]>=MA5[n-1]&&ptx->price[n][2]<MA5[n]&&MA5[n-2]<MA5[n-1]&&MA5[n]<MA5[n-1])//�^�} 
		{
			if(position==1)
			{
				position=0;
				r+=(ptx->price[n][2]-cost)*200;
				count_FaBreakSell_6++;
				count_numtrade_6++;
			}
			else if(position==0)
			{
				position=-1;
				cost=ptx->price[n][2];
				count_FaBreakSell_6++;
				count_numtrade_6++;
			}
		}
		else if(ptx->price[n-1][2]>=MA5[n-1]&&ptx->price[n][2]<MA5[n]&&MA5[n-2]>MA5[n-1]&&MA5[n]<MA5[n-1])//����} 
		{
			if(position==1) 
			{
				position=0;
				r+=(ptx->price[n][2]-cost)*200;
				count_callback_6++;
				count_numtrade_6++;
			}
			else if(position==0)
			{
				position=-1;
				cost=ptx->price[n][2];
				count_callback_6++;
				count_numtrade_6++;
			}
		}
		else if(ptx->price[n-1][2]<MA5[n-1]&&ptx->price[n][2]<MA5[n]&&ptx->price[n+1][2]<MA5[n+1]&&ptx->price[n][2]>ptx->price[n-1][2]&&ptx->price[n+1][2]<ptx->price[n][2])//�ϼu 
		{
			if(position==1) 
			{
				position=0;
				r+=(ptx->price[n][2]-cost)*200;
				count_bouncesell_6++;
				count_numtrade_6++;
			}
			else if(position==0)
			{
				position=-1;
				cost=ptx->price[n][2];
				count_bouncesell_6++;
				count_numtrade_6++;
			}
		}
		
	}
	if(position==1)
	{
		r+=(ptx->price[284][2]-cost)*200;
		count_numtrade_6++;
	} 
	else if(position==-1)
	{
		r+=(cost-ptx->price[284][2])*200;
		count_numtrade_6++;
	} 
//	cout<<r<<endl;
	return r;
}

int strategy7(TX*ptx,double stoploss,double takeprofit)//������+���l���Q 
{
	double MA5[285];
	double sum;
	int cost,position=0,r=0;
	int n,i;
	int slp;
	int tpp;
	for(n=4;n<285;n++)
	{
		sum=0;
		for(i=n-4;i<=n;i++)
			sum+=ptx->price[i][2];
		MA5[n]=sum/5;
	}
	for(n=6;n<285;n++)
	{
		 if(ptx->price[n-1][2]<=MA5[n-1]&&ptx->price[n][2]>MA5[n]&&MA5[n-2]>MA5[n-1]&&MA5[n]>MA5[n-1])//��} 
		{
			if(position==0)
			{
				position=1;
				cost=ptx->price[n][2];
				count_BreakBuy_7++;
				count_numtrade_7++;
			}
			else if(position==-1)
			{
				position=0;
				r+=(cost-ptx->price[n][2])*200;
				count_BreakBuy_7++;
				count_numtrade_7++;
			}
		}
		else if(ptx->price[n-1][2]<=MA5[n-1]&&ptx->price[n][2]>MA5[n]&&MA5[n-2]<MA5[n-1]&&MA5[n]>MA5[n-1])//���^�} 
		{
			if(position==0)
			{
				position=1;
				cost=ptx->price[n][2];
				count_BreakSell_7++;
				count_numtrade_7++;
			}
			else if(position==-1)
			{
				position=0;
				r+=(cost-ptx->price[n][2])*200;
				count_BreakSell_7++;
				count_numtrade_7++;
			}
		}
		else if(ptx->price[n-1][2]>MA5[n-1]&&ptx->price[n][2]>MA5[n]&&ptx->price[n+1][2]>MA5[n+1]&&ptx->price[n][2]<ptx->price[n-1][2]&&ptx->price[n+1][2]>ptx->price[n][2]) //�伵 
		{
			if(position==0)
			{
				position=1;
				cost=ptx->price[n][2];
				count_FaBreakBuy_7++;
				count_numtrade_7++;
			}
			else if(position==-1)
			{
				position=0;
				r+=(cost-ptx->price[n][2])*200;
				count_FaBreakBuy_7++;
				count_numtrade_7++;
			}
		}
		else if(ptx->price[n-1][2]>=MA5[n-1]&&ptx->price[n][2]<MA5[n]&&MA5[n-2]<MA5[n-1]&&MA5[n]<MA5[n-1])//�^�} 
		{
			if(position==1)
			{
				position=0;
				r+=(ptx->price[n][2]-cost)*200;
				count_numtrade_7++;
				count_FaBreakSell_7++;
			}
			else if(position==0)
			{
				position=-1;
				cost=ptx->price[n][2];
				count_FaBreakSell_7++;
				count_numtrade_7++;
			}
		}
		else if(ptx->price[n-1][2]>=MA5[n-1]&&ptx->price[n][2]<MA5[n]&&MA5[n-2]>MA5[n-1]&&MA5[n]<MA5[n-1])//����} 
		{
			if(position==1) 
			{
				position=0;
				r+=(ptx->price[n][2]-cost)*200;
				count_callback_7++;
				count_numtrade_7++;
			}
			else if(position==0)
			{
				position=-1;
				cost=ptx->price[n][2];
				count_callback_7++;
				count_numtrade_7++;
			}
		}
		else if(ptx->price[n-1][2]<MA5[n-1]&&ptx->price[n][2]<MA5[n]&&ptx->price[n+1][2]<MA5[n+1]&&ptx->price[n][2]>ptx->price[n-1][2]&&ptx->price[n+1][2]<ptx->price[n][2])//�ϼu 
		{
			if(position==1) 
			{
				position=0;
				r+=(ptx->price[n][2]-cost)*200;
				count_bouncesell_7++;
				count_numtrade_7++;
			}
			else if(position==0)
			{
				position=-1;
				cost=ptx->price[n][2];
				count_bouncesell_7++;
				count_numtrade_7++;
			}
		}
		else if(position==1)
		{
			slp=(int)(cost*(1-stoploss)+0.5);
			tpp=(int)(cost*(1+takeprofit)+0.5);
			if(ptx->price[n][2]<=slp)
			{
				
				count_slp_7++;
				count_numtrade_7++;
				r+=(ptx->price[n][2]-cost)*200;
				position=0;
			//	cout<<"���l��:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;
				continue;
			}	
			else if(ptx->price[n][2]>=tpp)
			{
				count_tpp_7++;
				count_numtrade_7++;
				r+=(ptx->price[n][2]-cost)*200;
				position=0;
			//	cout<<"���Q��:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;
				continue;
			}
		}
		else if(position==-1)
		{
			slp=(int)(cost*(1+stoploss)+0.5);
			tpp=(int)(cost*(1-takeprofit)+0.5);
			if(ptx->price[n][2]>=slp)
			{
				count_slp_7++;
				count_numtrade_7++;
				position=0;
				r+=(cost-ptx->price[n][2])*200;
			//	cout<<"���l��:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;
				continue;
			}	
			else if(ptx->price[n][2]<=tpp)
			{
				count_tpp_7++;
				position=0;
				count_numtrade_7++;
				r+=(cost-ptx->price[n][2])*200;
			//	cout<<"���Q��:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;
				continue;
			}
		}
//		cout<<"n=="<<n<<",,"<<cost<<endl;
	}
	if(position==1)
	{
		r+=(ptx->price[284][2]-cost)*200;
		count_numtrade_7++;
	} 
	else if(position==-1)
	{
		r+=(cost-ptx->price[284][2])*200;
		count_numtrade_7++;
	} 
//	cout<<r<<endl;
	return r;
} 

int trading(TX* data,int day_num,int strategy_no)//�M�w�O���إ�������åB�p��ֿn��Q 
{
	TX *ptx;
	int day_count;
	int temp_small=0,temp_big=0;
	int R=0;
	ptx=data;
	for(day_count=0;day_count<day_num;day_count++)
	{
		switch(strategy_no)
		{
			case 1:
				R+=strategy1(ptx);
				break;
			case 2:
				R+=strategy2(ptx,0.01);
				break;
			case 3:
				R+=strategy3(ptx,0.01,0.02);
				break;
			case 4:
				R+=strategy4(ptx);
				break;
			case 5:
				R+=strategy5(ptx,0.01,0.02);
				break;
			case 6:
				R+=strategy6(ptx);
				break;
			case 7:
				R+=strategy7(ptx,0.01,0.02);
				break;
		}	
		ptx++;
	} 
	return R;
}

int small(TX* data,int day_num,int strategy_no)//�M�w�O���إ�������åB�p���~���Ѫ����l�̦h 
{
	TX *ptx;
	int day_count;
	int temp_small=0;
	int Q;
	ptx=data;
	for(day_count=0;day_count<day_num;day_count++)
	{
		if(strategy_no==1)
		{
			Q=strategy1(ptx);
//			cout<<temp_small<<"==="<<endl; 
//			cout<<Q<<endl;
			if(temp_small>Q)
			{
				temp_small=Q;
			}
		}
		else if(strategy_no==2)
		{
			Q=strategy2(ptx,0.01);
//			cout<<temp_small<<"**="<<endl; 
//			cout<<Q<<endl;
			if(temp_small>Q)
			{
				temp_small=Q;
			}
				
		}
		else if(strategy_no==3)
		{
			Q=strategy3(ptx,0.01,0.02);
//			cout<<temp_small<<"==="<<endl; 
//			cout<<Q<<endl;
			if(temp_small>Q)
			{
				temp_small=Q;
			}
		
		}
		else if(strategy_no==4)
		{
			Q=strategy4(ptx);
//			cout<<temp_small<<"==="<<endl; 
//			cout<<Q<<endl;
			if(temp_small>Q)
			{
				temp_small=Q;
			}
		
		}
		else if(strategy_no==5)
		{
			Q=strategy5(ptx,0.01,0.02);
//			cout<<temp_small<<"==="<<endl; 
//			cout<<Q<<endl;
			if(temp_small>Q)
			{
				temp_small=Q;
			}
		
		}
		else if(strategy_no==6)
		{
			Q=strategy6(ptx);
//			cout<<temp_small<<"==="<<endl; 
//			cout<<Q<<endl;
			if(temp_small>Q)
			{
				temp_small=Q;
			}
		
		}
		else if(strategy_no==7)
		{
			Q=strategy7(ptx,0.01,0.02);
//			cout<<temp_small<<"==="<<endl; 
//			cout<<Q<<endl;
			if(temp_small>Q)
			{
				temp_small=Q;
			}
		
		}
		ptx++;
	} 
	return temp_small ;
}

int big(TX* data,int day_num,int strategy_no)//�M�w�O���إ�������åB�p���~���Ѫ����q�̦h 
{
	TX *ptx;
	int day_count;
	int temp_big=0;
	int Q;
	ptx=data;
//	cout<<"===="<<temp_big<<"===="<<endl;
	for(day_count=0;day_count<day_num;day_count++)
	{
		if(strategy_no==1)
		{
			Q=strategy1(ptx);
//			cout<<temp_big<<"==="<<endl; 
//			cout<<Q<<endl;
			if(temp_big<Q)
			{
				temp_big=Q;
			}
		}
		else if(strategy_no==2)
		{
			Q=strategy2(ptx,0.01);
//			cout<<temp_big<<"==="<<endl; 
//			cout<<Q<<endl;
			if(temp_big<Q)
			{
				temp_big=Q;
			}				
		}
		else if(strategy_no==3)
		{
			Q=strategy3(ptx,0.01,0.02);
//			cout<<temp_big<<"==="<<endl; 
//			cout<<Q<<endl;
			if(temp_big<Q)
			{
				temp_big=Q;
			}		
		}
		else if(strategy_no==4)
		{
			Q=strategy4(ptx);
//			cout<<temp_big<<"==="<<endl; 
//			cout<<Q<<endl;
			if(temp_big<Q)
			{
				temp_big=Q;
			}		
		}
		else if(strategy_no==5)
		{
			Q=strategy5(ptx,0.01,0.02);
//			cout<<temp_big<<"==="<<endl; 
//			cout<<Q<<endl;
			if(temp_big<Q)
			{
				temp_big=Q;
			}		
		}
		else if(strategy_no==6)
		{
			Q=strategy6(ptx);
//			cout<<temp_big<<"==="<<endl; 
//			cout<<Q<<endl;
			if(temp_big<Q)
			{
				temp_big=Q;
			}		
		}
		else if(strategy_no==7)
		{
			Q=strategy7(ptx,0.01,0.02);
//			cout<<temp_big<<"==="<<endl; 
//			cout<<Q<<endl;
			if(temp_big<Q)
			{
				temp_big=Q;
			}		
		}
		ptx++;
	} 
	return temp_big ;
}

int decide(int sum[])//�M�w���@�ӬO�ֿn��Q�̰� 
{
	int final_big=0;
	int Q=0;
	for(int p=1;p<=7;p++)
	{
		if(final_big<=sum[p])
		{
			final_big=sum[p];
			Q=p;
		}
	}
	return Q;
}

int main()
{
	char start[9] ;
	int g=1;
	while(g==1)
	{
		cout<<"�p�G�n�}�l�^���п�Jstart�C" <<endl; 
		cin>>start;
		if(strcmp(start,"start")==0)
			g=0;
	}
	int day_num,count=0,m/*��ѲĴX����*/;
	char c,str[101];
	int year,month,hour,day,minute,exdate,price;
	TX *data,*ptx;
	ifstream fin;
	fin.open("TXF_minute_2021_285.csv");
	fin.getline(str,100);
	while(fin>>year)//�p��@�~���X�ѭn�}�X�ӫ��� 
	{
		fin>>c;
		fin>>month>>c;
		fin>>day>>c;
		fin>>hour>>c;
		fin>>minute>>c;
		fin>>exdate>>c;
		fin>>price;
		if(hour==13&&minute==29)
		{
			cout<<year<<"/"<<month<<"/"<<day<<endl;
			count++;
		}
	}
	fin.close();
	day_num=count;
	cout<<day_num<<endl;
	data=new TX[day_num];//�s�W�P�ѼƬۦP�ƶq������ 
	ptx=data;
	int sum[9],t;//t�N��̤j�O���@�ӵ��� 
	fin.open("TXF_minute_2021_285.csv");
	fin.getline(str,100);
	m=0;
	while(fin>>year)
	{
		fin>>c;
		fin>>month>>c;
		fin>>day>>c;
		fin>>hour>>c;
		fin>>minute>>c;
		fin>>exdate>>c;
		fin>>price;
		if(m==0)
		{
			ptx->year=year;
			ptx->month=month;
			ptx->day=day;
			ptx->exdate=exdate;
				}
		ptx->price[m][0]=hour;
		ptx->price[m][1]=minute;
		ptx->price[m][2]=price;
		m++;
		if(hour==13&&minute==29)//�@�ѵ�������U�@�ӫ���  
		{
			m=0;
			ptx++;
		}
	} 
	fin.close();//�N�Ҧ��ƭȿ�J������
	cout<<"�п�ܧA�Q�n�ϥΪ���������C"<<endl;
	cout<<"�����@(buy and hold)�A�п�J1"<<endl<<"�����G(�����@+���l)�A�п�J2"<<endl<<"�����T(�����@+���l���Q)�A�п�J3"<<endl<<"�����|(²�檩�����ѥ���k�h)�A�п�J4"<<endl;
	cout<<"������(�����|+���l���Q)�A�п�J5"<<endl<<"������(�[�j�������Ѫk�h)(��}+���^�}+�伵+�^�}+����}+����)�A�п�J6"<<endl<<"�����C(�[�j�������Ѫk�h+���l���Q)�A�п�J7"<<endl;
	cout<<"�p�G�S���Aı�o�A�X���A�N�|��������ֿn��Q�̦h���A�п�J8"<<endl<<"�p�G�Q���D�Ҧ����������סA�п�J9"<<endl; 
	cout<<"�Q�����^���A�п�J0"<<endl;
	
	for(int p=1;p<=7;p++)
	{
		sum[p]=trading (data,day_num,p);
	}
	int sum_numtrade[8]={0},sum_slp[8]={0},sum_tpp[8]={0},sum_BreakBuy[8]={0},sum_BreakSell[8]={0},sum_FaBreakBuy[8]={0},sum_FaBreakSell[8]={0},sum_callback[8]={0},sum_bouncesell[8]={0};//��p��X����פJ�}�C�H�K���ƭp�� 
	sum_numtrade[1]=count_numtrade_1,sum_numtrade[2]=count_numtrade_2,sum_numtrade[3]=count_numtrade_3,sum_numtrade[4]=count_numtrade_4,sum_numtrade[5]=count_numtrade_5,sum_numtrade[6]=count_numtrade_6,sum_numtrade[7]=count_numtrade_7;
	sum_slp[2]=count_slp_2,sum_slp[5]=count_slp_5,sum_slp[3]=count_slp_3,sum_slp[7]=count_slp_7;
	sum_tpp[5]=count_tpp_5,sum_tpp[3]=count_tpp_3,sum_tpp[7]=count_tpp_7;
	sum_BreakBuy[4]=count_BreakBuy_4/*��}*/,sum_BreakSell[4]=count_BreakSell_4/*�^�}*/;
	sum_BreakBuy[5]=count_BreakBuy_5/*��}*/,sum_BreakSell[5]=count_BreakSell_5/*�^�}*/;
	sum_BreakBuy[6]=count_BreakBuy_6/*��}*/,sum_BreakSell[6]=count_BreakSell_6/*�^�}*/,sum_FaBreakBuy[6]=count_FaBreakBuy_6/*���^�}*/,sum_FaBreakSell[6]=count_FaBreakSell_6/*����}*/,sum_callback[6]=count_callback_6/*�伵*/,sum_bouncesell[6]=count_bouncesell_6/*�ϼu*/;
	sum_BreakBuy[7]=count_BreakBuy_7/*��}*/,sum_BreakSell[7]=count_BreakSell_7/*�^�}*/,sum_FaBreakBuy[7]=count_FaBreakBuy_7/*���^�}*/,sum_FaBreakSell[7]=count_FaBreakSell_7/*����}*/,sum_callback[7]=count_callback_7/*�伵*/,sum_bouncesell[7]=count_bouncesell_7/*�ϼu*/;
////	cout<<data[1].price[0][2];
	int k=0;
	while(cin>>k)
	{
		switch(k)
		{
			case 1:
				cout<<"#�����@(buy and hold)�i��Q�G"<<trading (data,day_num,1)<<",���̤j��Q:"<<big(data,day_num,1)<<",���̤j�l��:"<<small(data,day_num,1)<<endl;
				cout<<"��~���@����X���G" <<sum_numtrade[1]<<endl; 
				break;
			case 2:
				cout<<"#�����G(�����@+���l)�i��Q�G"<<trading (data,day_num,2)<<",���̤j��Q:"<<big(data,day_num,2)<<",���̤j�l��:"<<small(data,day_num,2)<<",��~���l�X��:"<<sum_slp[2]<<endl;
				cout<<"��~���@����X���G" <<sum_numtrade[2]<<endl;
				break;
			case 3:
				cout<<"#�����T(�����@+���l���Q)�i��Q�G"<<trading (data,day_num,3)<<",���̤j��Q:"<<big(data,day_num,3)<<",���̤j�l��:"<<small(data,day_num,3)<<endl;
				cout<<"��~���l�X��:"<<sum_slp[3]<<",��~���Q�X��:"<<sum_tpp[3]<<endl;
				cout<<"��~���@����X���G" <<sum_numtrade[3]<<endl;
				break;
			case 4:
				cout<<"#�����|(²�檩�����ѥ���k�h)�i��Q�G"<<trading (data,day_num,4)<<",���̤j��Q:"<<big(data,day_num,4)<<",���̤j�l��:"<<small(data,day_num,4)<<endl;
				cout<<"��~��}�X��:"<<sum_BreakBuy[4]<<",��~�^�}�X��:"<<sum_BreakSell[4]<<endl;
				cout<<"��~���@����X���G" <<sum_numtrade[4]<<endl;
				break;
			case 5:
				cout<<"#������(�����|+���l���Q)�i��Q�G"<<trading (data,day_num,5)<<",���̤j��Q:"<<big(data,day_num,5)<<",���̤j�l��:"<<small(data,day_num,5)<<endl;
				cout<<"��~���l�X��:"<<sum_slp[5]<<",��~���Q�X��:"<<sum_tpp[5]<<endl;
				cout<<"��~��}�X��:"<<sum_BreakBuy[5]<<",��~�^�}�X��:"<<sum_BreakSell[5]<<endl;
				cout<<"��~���@����X���G" <<sum_numtrade[5]<<endl;
				break;
			case 6:
				cout<<"#������(�[�j�������Ѫk�h)�i��Q�G"/*(��}+���^�}+�伵+�^�}+����}+�ϼu)*/<<trading (data,day_num,6)<<",���̤j��Q:"<<big(data,day_num,6)<<",���̤j�l��:"<<small(data,day_num,6)<<endl;
				cout<<"��~��}�X��:"<<sum_BreakBuy[6]<<",��~�^�}�X��:"<<sum_BreakSell[6]<<endl<<"��~���^�}�X��:"<<sum_FaBreakBuy[6]<<",��~����}�X��:"<<sum_FaBreakSell[6]<<endl;
				cout<<"��~�伵�X��:"<<sum_callback[6]<<",��~�ϼu�X��:"<<sum_bouncesell[6]<<endl;
				cout<<"��~���@����X���G" <<sum_numtrade[6]<<endl;
				break;
			case 7:
				cout<<"#�����C(�[�j�������Ѫk�h+���l���Q)�i��Q�G"<<trading (data,day_num,7)<<",���̤j��Q:"<<big(data,day_num,7)<<",���̤j�l��:"<<small(data,day_num,7)<<endl;
				cout<<"��~���l�X��:"<<sum_slp[7]<<",��~���Q�X��:"<<sum_tpp[7]<<endl;
				cout<<"��~��}�X��:"<<sum_BreakBuy[7]<<",��~�^�}�X��:"<<sum_BreakSell[7]<<endl<<"��~���^�}�X��:"<<sum_FaBreakBuy[7]<<",��~����}�X��:"<<sum_FaBreakSell[7]<<endl;
				cout<<"��~�伵�X��:"<<sum_callback[7]<<",��~�ϼu�X��:"<<sum_bouncesell[7]<<endl;
				cout<<"��~���@����X���G" <<sum_numtrade[7]<<endl;
				break;
		}
		if(k==8)
		{
			cout<<"�ֿn��Q�̦h����������O:";
			t=decide(sum);
			switch(t)
			{
				case 1:
					cout<<"#�����@(buy and hold)�i��Q�G"<<trading (data,day_num,1)<<",���̤j��Q:"<<big(data,day_num,1)<<",���̤j�l��:"<<small(data,day_num,1)<<endl;
					cout<<"��~���@����X���G" <<sum_numtrade[1]<<endl;
					break;
				case 2:
					cout<<"#�����G(�����@+���l)�i��Q�G"<<trading (data,day_num,2)<<",���̤j��Q:"<<big(data,day_num,2)<<",���̤j�l��:"<<small(data,day_num,2)<<",��~���l�X��:"<<sum_slp[2]<<endl;
					cout<<"��~���@����X���G" <<sum_numtrade[2]<<endl;
					break;
				case 3:
					cout<<"#�����T(�����@+���l���Q)�i��Q�G"<<trading (data,day_num,3)<<",���̤j��Q:"<<big(data,day_num,3)<<",���̤j�l��:"<<small(data,day_num,3)<<endl;
					cout<<"��~���l�X��:"<<sum_slp[3]<<",��~���Q�X��:"<<sum_tpp[3]<<endl;
					cout<<"��~���@����X���G" <<sum_numtrade[3]<<endl;
					break;
				case 4:
					cout<<"#�����|(²�檩�����ѥ���k�h)�i��Q�G"<<trading (data,day_num,4)<<",���̤j��Q:"<<big(data,day_num,4)<<",���̤j�l��:"<<small(data,day_num,4)<<endl;
					cout<<"��~��}�X��:"<<sum_BreakBuy[4]<<",��~�^�}�X��:"<<sum_BreakSell[4]<<endl;
					cout<<"��~���@����X���G" <<sum_numtrade[4]<<endl;
					break;
				case 5:
					cout<<"#������(�����|+���l���Q)�i��Q�G"<<trading (data,day_num,5)<<",���̤j��Q:"<<big(data,day_num,5)<<",���̤j�l��:"<<small(data,day_num,5)<<endl;
					cout<<"��~���l�X��:"<<sum_slp[5]<<",��~���Q�X��:"<<sum_tpp[5]<<endl;
					cout<<"��~��}�X��:"<<sum_BreakBuy[5]<<",��~�^�}�X��:"<<sum_BreakSell[5]<<endl;
					cout<<"��~���@����X���G" <<sum_numtrade[5]<<endl;
					break;
				case 6:
					cout<<"#������(�[�j�������Ѫk�h)�i��Q�G"/*(��}+���^�}+�伵+�^�}+����}+�ϼu)*/<<trading (data,day_num,6)<<",���̤j��Q:"<<big(data,day_num,6)<<",���̤j�l��:"<<small(data,day_num,6)<<endl;
					cout<<"��~��}�X��:"<<sum_BreakBuy[6]<<",��~�^�}�X��:"<<sum_BreakSell[6]<<endl<<"��~���^�}�X��:"<<sum_FaBreakBuy[6]<<",��~����}�X��:"<<sum_FaBreakSell[6]<<endl;
					cout<<"��~�伵�X��:"<<sum_callback[6]<<",��~�ϼu�X��:"<<sum_bouncesell[6]<<endl;
					cout<<"��~���@����X���G" <<sum_numtrade[6]<<endl;
					break;
				case 7:
					cout<<"#�����C(�[�j�������Ѫk�h+���l���Q)�i��Q�G"<<trading (data,day_num,7)<<",���̤j��Q:"<<big(data,day_num,7)<<",���̤j�l��:"<<small(data,day_num,7)<<endl;
					cout<<"��~���l�X��:"<<sum_slp[7]<<",��~���Q�X��:"<<sum_tpp[7]<<endl;
					cout<<"��~��}�X��:"<<sum_BreakBuy[7]<<",��~�^�}�X��:"<<sum_BreakSell[7]<<endl<<"��~���^�}�X��:"<<sum_FaBreakBuy[7]<<",��~����}�X��:"<<sum_FaBreakSell[7]<<endl;
					cout<<"��~�伵�X��:"<<sum_callback[7]<<",��~�ϼu�X��:"<<sum_bouncesell[7]<<endl;
					cout<<"��~���@����X���G" <<sum_numtrade[7]<<endl;
					break;
			}
		}
		else if(k==9)
		{
			cout<<"#�����@(buy and hold)�i��Q�G"<<trading (data,day_num,1)<<",���̤j��Q:"<<big(data,day_num,1)<<",���̤j�l��:"<<small(data,day_num,1)<<endl;
			cout<<"��~���@����X���G" <<sum_numtrade[1]<<endl<<endl;
			cout<<"#�����G(�����@+���l)�i��Q�G"<<trading (data,day_num,2)<<",���̤j��Q:"<<big(data,day_num,2)<<",���̤j�l��:"<<small(data,day_num,2)<<",��~���l�X��:"<<sum_slp[2]<<endl;
			cout<<"��~���@����X���G" <<sum_numtrade[2]<<endl<<endl;
			cout<<"#�����T(�����@+���l���Q)�i��Q�G"<<trading (data,day_num,3)<<",���̤j��Q:"<<big(data,day_num,3)<<",���̤j�l��:"<<small(data,day_num,3)<<endl;
			cout<<"��~���l�X��:"<<sum_slp[3]<<",��~���Q�X��:"<<sum_tpp[3]<<endl;
			cout<<"��~���@����X���G" <<sum_numtrade[3]<<endl<<endl;
			cout<<"#�����|(²�檩�����ѥ���k�h)�i��Q�G"<<trading (data,day_num,4)<<",���̤j��Q:"<<big(data,day_num,4)<<",���̤j�l��:"<<small(data,day_num,4)<<endl;
			cout<<"��~��}�X��:"<<sum_BreakBuy[4]<<",��~�^�}�X��:"<<sum_BreakSell[4]<<endl;
			cout<<"��~���@����X���G" <<sum_numtrade[4]<<endl<<endl;
			cout<<"#������(�����|+���l���Q)�i��Q�G"<<trading (data,day_num,5)<<",���̤j��Q:"<<big(data,day_num,5)<<",���̤j�l��:"<<small(data,day_num,5)<<endl;
			cout<<"��~���l�X��:"<<sum_slp[5]<<",��~���Q�X��:"<<sum_tpp[5]<<endl;
			cout<<"��~��}�X��:"<<sum_BreakBuy[5]<<",��~�^�}�X��:"<<sum_BreakSell[5]<<endl;
			cout<<"��~���@����X���G" <<sum_numtrade[5]<<endl<<endl;
			cout<<"#������(�[�j�������Ѫk�h)�i��Q�G"/*(��}+���^�}+�伵+�^�}+����}+�ϼu)*/<<trading (data,day_num,6)<<",���̤j��Q:"<<big(data,day_num,6)<<",���̤j�l��:"<<small(data,day_num,6)<<endl;
			cout<<"��~��}�X��:"<<sum_BreakBuy[6]<<",��~�^�}�X��:"<<sum_BreakSell[6]<<endl<<"��~���^�}�X��:"<<sum_FaBreakBuy[6]<<",��~����}�X��:"<<sum_FaBreakSell[6]<<endl;
			cout<<"��~�伵�X��:"<<sum_callback[6]<<",��~�ϼu�X��:"<<sum_bouncesell[6]<<endl;
			cout<<"��~���@����X���G" <<sum_numtrade[6]<<endl<<endl;
			cout<<"#�����C(�[�j�������Ѫk�h+���l���Q)�i��Q�G"<<trading (data,day_num,7)<<",���̤j��Q:"<<big(data,day_num,7)<<",���̤j�l��:"<<small(data,day_num,7)<<endl;
			cout<<"��~���l�X��:"<<sum_slp[7]<<",��~���Q�X��:"<<sum_tpp[7]<<endl;
			cout<<"��~��}�X��:"<<sum_BreakBuy[7]<<",��~�^�}�X��:"<<sum_BreakSell[7]<<endl<<"��~���^�}�X��:"<<sum_FaBreakBuy[7]<<",��~����}�X��:"<<sum_FaBreakSell[7]<<endl;
			cout<<"��~�伵�X��:"<<sum_callback[7]<<",��~�ϼu�X��:"<<sum_bouncesell[7]<<endl;
			cout<<"��~���@����X���G" <<sum_numtrade[7]<<endl;
		}
		else if(k==0)
			break;
		else if(k>9)
		{
			cout<<"�z��J���Ʀr���b�A�Ƚd��A�бq�s��J�A����"<<endl; 
			cout<<"�����@(buy and hold)�A�п�J1"<<endl<<"�����G(�����@+���l)�A�п�J2"<<endl<<"�����T(�����@+���l���Q)�A�п�J3"<<endl<<"�����|(²�檩�����ѥ���k�h)�A�п�J4"<<endl;
			cout<<"������(�����|+���l���Q)�A�п�J5"<<endl<<"������(�[�j�������Ѫk�h)(��}+���^�}+�伵+�^�}+����}+����)�A�п�J6"<<endl<<"�����C(�[�j�������Ѫk�h+���l���Q)�A�п�J7"<<endl;
			cout<<"�p�G�S���Aı�o�A�X���A�N�|��������ֿn��Q�̦h���A�п�J8"<<endl<<"�p�G�Q���D�Ҧ����������סA�п�J9"<<endl; 
			cout<<"�Q�����^���A�п�J0"<<endl;	 
		}
		else 
		{
			cout<<"�бq�s��J�W�������Ʀr�A����"<<endl<<endl;
		}	
	}
	return 0;
}
