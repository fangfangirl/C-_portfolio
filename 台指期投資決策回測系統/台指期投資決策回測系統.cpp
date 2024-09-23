#include<iostream>
#include <fstream>
#include <string.h>
using namespace std;
int count_numtrade_1=0,count_numtrade_2=0,count_numtrade_3=0,count_numtrade_4=0,count_numtrade_5=0,count_numtrade_6=0,count_numtrade_7=0;
int count_slp_2=0,count_slp_5=0,count_slp_3=0,count_slp_7=0;
int count_tpp_5=0,count_tpp_3=0,count_tpp_7=0;
int count_BreakBuy_4/*突破*/=0,count_BreakSell_4/*跌破*/=0;
int count_BreakBuy_5/*突破*/=0,count_BreakSell_5/*跌破*/=0;
int count_BreakBuy_6/*突破*/=0,count_BreakSell_6/*跌破*/=0,count_FaBreakBuy_6/*假跌破*/=0,count_FaBreakSell_6/*假突破*/=0,count_callback_6/*支撐*/=0,count_bouncesell_6/*反彈*/=0;
int count_BreakBuy_7/*突破*/=0,count_BreakSell_7/*跌破*/=0,count_FaBreakBuy_7/*假跌破*/=0,count_FaBreakSell_7/*假突破*/=0,count_callback_7/*支撐*/=0,count_bouncesell_7/*反彈*/=0;
struct TX{
	int year ,month,day,date ,exdate;
	int price[285][3];
}; 

int strategy1(TX*ptx)//
{
	int r=(ptx->price[284][2]-ptx->price[0][2])*200;
//	cout<<r<<endl;//每天最終獲利 
	count_numtrade_1++;
	return r;
}

int strategy2(TX*ptx,double stoploss)//策略一+停損點 
{
	int cost=ptx->price[0][2];
	int slp=(int)(cost*(1-stoploss)+0.5);//四捨五入 
	int n,final_price;
	for(n=1;n<285;n++)
	{
		final_price=ptx->price[n][2];
		if(ptx->price[n][2]<=slp)
		{
			count_slp_2++;
//			cout<<"停損日:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;
			break; 
		}
	}
	count_numtrade_2++;
	return (final_price-cost)*200;
}

int strategy3(TX*ptx,double stoploss,double takeprofit)//策略一+停損和停利 
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
//			cout<<"停損日:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;//告訴哪幾天停損 
			break; 
		}
		if(ptx->price[n][2]>=tpp)
		{
			count_tpp_3++;
//			cout<<"停利日:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;//告訴哪幾天停利  
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
		if(ptx->price[n-1][2]<=MA5[n-1]&&ptx->price[n][2]>MA5[n]&&MA5[n-2]>MA5[n-1]&&MA5[n]>MA5[n-1])//突破 
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
		else if(ptx->price[n-1][2]>=MA5[n-1]&&ptx->price[n][2]<MA5[n]&&MA5[n-2]<MA5[n-1]&&MA5[n]<MA5[n-1])//跌破 
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
	if(position==1)//當天如果有剩將他賣出或贖回 
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

int strategy5(TX*ptx,double stoploss,double takeprofit)// 簡單版葛蘭碧交易法則+策略三 
{
	double MA5[285];
	double sum;
	int cost,position=0,r=0;
	int n,i;
	int slp;
	int tpp;
	for(n=4;n<285;n++)//計算移動平均線 
	{
		sum=0;
		for(i=n-4;i<=n;i++)
			sum+=ptx->price[i][2];
		MA5[n]=sum/5;
	}
	for(n=6;n<285;n++)//先看葛藍碧在看停損停利 
	{
		if(ptx->price[n-1][2]<=MA5[n-1]&&ptx->price[n][2]>MA5[n]&&MA5[n-2]>MA5[n-1]&&MA5[n]>MA5[n-1])// 突破 
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
		else if(ptx->price[n-1][2]>=MA5[n-1]&&ptx->price[n][2]<MA5[n]&&MA5[n-2]<MA5[n-1]&&MA5[n]<MA5[n-1])//跌破 
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
		else if(position==-1)//空一股時的停損停利 
		{
			slp=(int)(cost*(1+stoploss)+0.5);
			tpp=(int)(cost*(1-takeprofit)+0.5);
			if(ptx->price[n][2]>=slp)
			{
				count_slp_5++;
				count_numtrade_5++;
				position=0;
				r+=(cost-ptx->price[n][2])*200;
			//	cout<<"停損日:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;
				continue;
			}	
			else if(ptx->price[n][2]<=tpp)
			{
				count_tpp_5++;
				count_numtrade_5++;
				position=0;
				r+=(cost-ptx->price[n][2])*200;
			//	cout<<"停利日:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;
				continue;
			}
		}
		else if(position==1)//持有一股的停損停利 
		{
			slp=(int)(cost*(1-stoploss)+0.5);
			tpp=(int)(cost*(1+takeprofit)+0.5);
			if(ptx->price[n][2]<=slp)
			{
				count_slp_5++;
				count_numtrade_5++;
				r+=(ptx->price[n][2]-cost)*200;
				position=0;
			//	cout<<"停損日:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;
				continue;
			}	
			else if(ptx->price[n][2]>=tpp)
			{
				count_tpp_5++;
				count_numtrade_5++;
				r+=(ptx->price[n][2]-cost)*200;
				position=0;
			//	cout<<"停利日:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;
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

int strategy6(TX*ptx)// (葛蘭碧交易法則)(突破+假跌破+支撐+跌破+假突破+反壓)
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
		if(ptx->price[n-1][2]<=MA5[n-1]&&ptx->price[n][2]>MA5[n]&&MA5[n-2]>MA5[n-1]&&MA5[n]>MA5[n-1])//突破 
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
		else if(ptx->price[n-1][2]<=MA5[n-1]&&ptx->price[n][2]>MA5[n]&&MA5[n-2]<MA5[n-1]&&MA5[n]>MA5[n-1])//假跌破 
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
		else if(ptx->price[n-1][2]>MA5[n-1]&&ptx->price[n][2]>MA5[n]&&ptx->price[n+1][2]>MA5[n+1]&&ptx->price[n][2]<ptx->price[n-1][2]&&ptx->price[n+1][2]>ptx->price[n][2]) //支撐 
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
		else if(ptx->price[n-1][2]>=MA5[n-1]&&ptx->price[n][2]<MA5[n]&&MA5[n-2]<MA5[n-1]&&MA5[n]<MA5[n-1])//跌破 
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
		else if(ptx->price[n-1][2]>=MA5[n-1]&&ptx->price[n][2]<MA5[n]&&MA5[n-2]>MA5[n-1]&&MA5[n]<MA5[n-1])//假突破 
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
		else if(ptx->price[n-1][2]<MA5[n-1]&&ptx->price[n][2]<MA5[n]&&ptx->price[n+1][2]<MA5[n+1]&&ptx->price[n][2]>ptx->price[n-1][2]&&ptx->price[n+1][2]<ptx->price[n][2])//反彈 
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

int strategy7(TX*ptx,double stoploss,double takeprofit)//策略六+停損停利 
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
		 if(ptx->price[n-1][2]<=MA5[n-1]&&ptx->price[n][2]>MA5[n]&&MA5[n-2]>MA5[n-1]&&MA5[n]>MA5[n-1])//突破 
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
		else if(ptx->price[n-1][2]<=MA5[n-1]&&ptx->price[n][2]>MA5[n]&&MA5[n-2]<MA5[n-1]&&MA5[n]>MA5[n-1])//假跌破 
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
		else if(ptx->price[n-1][2]>MA5[n-1]&&ptx->price[n][2]>MA5[n]&&ptx->price[n+1][2]>MA5[n+1]&&ptx->price[n][2]<ptx->price[n-1][2]&&ptx->price[n+1][2]>ptx->price[n][2]) //支撐 
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
		else if(ptx->price[n-1][2]>=MA5[n-1]&&ptx->price[n][2]<MA5[n]&&MA5[n-2]<MA5[n-1]&&MA5[n]<MA5[n-1])//跌破 
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
		else if(ptx->price[n-1][2]>=MA5[n-1]&&ptx->price[n][2]<MA5[n]&&MA5[n-2]>MA5[n-1]&&MA5[n]<MA5[n-1])//假突破 
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
		else if(ptx->price[n-1][2]<MA5[n-1]&&ptx->price[n][2]<MA5[n]&&ptx->price[n+1][2]<MA5[n+1]&&ptx->price[n][2]>ptx->price[n-1][2]&&ptx->price[n+1][2]<ptx->price[n][2])//反彈 
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
			//	cout<<"停損日:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;
				continue;
			}	
			else if(ptx->price[n][2]>=tpp)
			{
				count_tpp_7++;
				count_numtrade_7++;
				r+=(ptx->price[n][2]-cost)*200;
				position=0;
			//	cout<<"停利日:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;
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
			//	cout<<"停損日:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;
				continue;
			}	
			else if(ptx->price[n][2]<=tpp)
			{
				count_tpp_7++;
				position=0;
				count_numtrade_7++;
				r+=(cost-ptx->price[n][2])*200;
			//	cout<<"停利日:"<<ptx->year<<"/"<<ptx->month<<"/"<<ptx->day<<endl;
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

int trading(TX* data,int day_num,int strategy_no)//決定是哪種交易策略並且計算累積獲利 
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

int small(TX* data,int day_num,int strategy_no)//決定是哪種交易策略並且計算整年哪天的虧損最多 
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

int big(TX* data,int day_num,int strategy_no)//決定是哪種交易策略並且計算整年哪天的收益最多 
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

int decide(int sum[])//決定哪一個是累積獲利最高 
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
		cout<<"如果要開始回測請輸入start。" <<endl; 
		cin>>start;
		if(strcmp(start,"start")==0)
			g=0;
	}
	int day_num,count=0,m/*當天第幾分鐘*/;
	char c,str[101];
	int year,month,hour,day,minute,exdate,price;
	TX *data,*ptx;
	ifstream fin;
	fin.open("TXF_minute_2021_285.csv");
	fin.getline(str,100);
	while(fin>>year)//計算一年有幾天要開幾個指標 
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
	data=new TX[day_num];//新增與天數相同數量的指標 
	ptx=data;
	int sum[9],t;//t代表最大是哪一個策略 
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
		if(hour==13&&minute==29)//一天結束換到下一個指標  
		{
			m=0;
			ptx++;
		}
	} 
	fin.close();//將所有數值輸入給指標
	cout<<"請選擇你想要使用的交易策略。"<<endl;
	cout<<"策略一(buy and hold)，請輸入1"<<endl<<"策略二(策略一+停損)，請輸入2"<<endl<<"策略三(策略一+停損停利)，請輸入3"<<endl<<"策略四(簡單版葛蘭碧交易法則)，請輸入4"<<endl;
	cout<<"策略五(策略四+停損停利)，請輸入5"<<endl<<"策略六(加強版葛蘭碧法則)(突破+假跌破+支撐+跌破+假突破+反壓)，請輸入6"<<endl<<"策略七(加強版葛蘭碧法則+停損停利)，請輸入7"<<endl;
	cout<<"如果沒有你覺得適合的，將會直接執行累積獲利最多的，請輸入8"<<endl<<"如果想知道所有策略的答案，請輸入9"<<endl; 
	cout<<"想結束回測，請輸入0"<<endl;
	
	for(int p=1;p<=7;p++)
	{
		sum[p]=trading (data,day_num,p);
	}
	int sum_numtrade[8]={0},sum_slp[8]={0},sum_tpp[8]={0},sum_BreakBuy[8]={0},sum_BreakSell[8]={0},sum_FaBreakBuy[8]={0},sum_FaBreakSell[8]={0},sum_callback[8]={0},sum_bouncesell[8]={0};//交計算出的質匯入陣列以免重複計算 
	sum_numtrade[1]=count_numtrade_1,sum_numtrade[2]=count_numtrade_2,sum_numtrade[3]=count_numtrade_3,sum_numtrade[4]=count_numtrade_4,sum_numtrade[5]=count_numtrade_5,sum_numtrade[6]=count_numtrade_6,sum_numtrade[7]=count_numtrade_7;
	sum_slp[2]=count_slp_2,sum_slp[5]=count_slp_5,sum_slp[3]=count_slp_3,sum_slp[7]=count_slp_7;
	sum_tpp[5]=count_tpp_5,sum_tpp[3]=count_tpp_3,sum_tpp[7]=count_tpp_7;
	sum_BreakBuy[4]=count_BreakBuy_4/*突破*/,sum_BreakSell[4]=count_BreakSell_4/*跌破*/;
	sum_BreakBuy[5]=count_BreakBuy_5/*突破*/,sum_BreakSell[5]=count_BreakSell_5/*跌破*/;
	sum_BreakBuy[6]=count_BreakBuy_6/*突破*/,sum_BreakSell[6]=count_BreakSell_6/*跌破*/,sum_FaBreakBuy[6]=count_FaBreakBuy_6/*假跌破*/,sum_FaBreakSell[6]=count_FaBreakSell_6/*假突破*/,sum_callback[6]=count_callback_6/*支撐*/,sum_bouncesell[6]=count_bouncesell_6/*反彈*/;
	sum_BreakBuy[7]=count_BreakBuy_7/*突破*/,sum_BreakSell[7]=count_BreakSell_7/*跌破*/,sum_FaBreakBuy[7]=count_FaBreakBuy_7/*假跌破*/,sum_FaBreakSell[7]=count_FaBreakSell_7/*假突破*/,sum_callback[7]=count_callback_7/*支撐*/,sum_bouncesell[7]=count_bouncesell_7/*反彈*/;
////	cout<<data[1].price[0][2];
	int k=0;
	while(cin>>k)
	{
		switch(k)
		{
			case 1:
				cout<<"#策略一(buy and hold)可獲利："<<trading (data,day_num,1)<<",單日最大獲利:"<<big(data,day_num,1)<<",單日最大損失:"<<small(data,day_num,1)<<endl;
				cout<<"整年中共交易幾次：" <<sum_numtrade[1]<<endl; 
				break;
			case 2:
				cout<<"#策略二(策略一+停損)可獲利："<<trading (data,day_num,2)<<",單日最大獲利:"<<big(data,day_num,2)<<",單日最大損失:"<<small(data,day_num,2)<<",整年停損幾次:"<<sum_slp[2]<<endl;
				cout<<"整年中共交易幾次：" <<sum_numtrade[2]<<endl;
				break;
			case 3:
				cout<<"#策略三(策略一+停損停利)可獲利："<<trading (data,day_num,3)<<",單日最大獲利:"<<big(data,day_num,3)<<",單日最大損失:"<<small(data,day_num,3)<<endl;
				cout<<"整年停損幾次:"<<sum_slp[3]<<",整年停利幾次:"<<sum_tpp[3]<<endl;
				cout<<"整年中共交易幾次：" <<sum_numtrade[3]<<endl;
				break;
			case 4:
				cout<<"#策略四(簡單版葛蘭碧交易法則)可獲利："<<trading (data,day_num,4)<<",單日最大獲利:"<<big(data,day_num,4)<<",單日最大損失:"<<small(data,day_num,4)<<endl;
				cout<<"整年突破幾次:"<<sum_BreakBuy[4]<<",整年跌破幾次:"<<sum_BreakSell[4]<<endl;
				cout<<"整年中共交易幾次：" <<sum_numtrade[4]<<endl;
				break;
			case 5:
				cout<<"#策略五(策略四+停損停利)可獲利："<<trading (data,day_num,5)<<",單日最大獲利:"<<big(data,day_num,5)<<",單日最大損失:"<<small(data,day_num,5)<<endl;
				cout<<"整年停損幾次:"<<sum_slp[5]<<",整年停利幾次:"<<sum_tpp[5]<<endl;
				cout<<"整年突破幾次:"<<sum_BreakBuy[5]<<",整年跌破幾次:"<<sum_BreakSell[5]<<endl;
				cout<<"整年中共交易幾次：" <<sum_numtrade[5]<<endl;
				break;
			case 6:
				cout<<"#策略六(加強版葛蘭碧法則)可獲利："/*(突破+假跌破+支撐+跌破+假突破+反彈)*/<<trading (data,day_num,6)<<",單日最大獲利:"<<big(data,day_num,6)<<",單日最大損失:"<<small(data,day_num,6)<<endl;
				cout<<"整年突破幾次:"<<sum_BreakBuy[6]<<",整年跌破幾次:"<<sum_BreakSell[6]<<endl<<"整年假跌破幾次:"<<sum_FaBreakBuy[6]<<",整年假突破幾次:"<<sum_FaBreakSell[6]<<endl;
				cout<<"整年支撐幾次:"<<sum_callback[6]<<",整年反彈幾次:"<<sum_bouncesell[6]<<endl;
				cout<<"整年中共交易幾次：" <<sum_numtrade[6]<<endl;
				break;
			case 7:
				cout<<"#策略七(加強版葛蘭碧法則+停損停利)可獲利："<<trading (data,day_num,7)<<",單日最大獲利:"<<big(data,day_num,7)<<",單日最大損失:"<<small(data,day_num,7)<<endl;
				cout<<"整年停損幾次:"<<sum_slp[7]<<",整年停利幾次:"<<sum_tpp[7]<<endl;
				cout<<"整年突破幾次:"<<sum_BreakBuy[7]<<",整年跌破幾次:"<<sum_BreakSell[7]<<endl<<"整年假跌破幾次:"<<sum_FaBreakBuy[7]<<",整年假突破幾次:"<<sum_FaBreakSell[7]<<endl;
				cout<<"整年支撐幾次:"<<sum_callback[7]<<",整年反彈幾次:"<<sum_bouncesell[7]<<endl;
				cout<<"整年中共交易幾次：" <<sum_numtrade[7]<<endl;
				break;
		}
		if(k==8)
		{
			cout<<"累積獲利最多的交易策略是:";
			t=decide(sum);
			switch(t)
			{
				case 1:
					cout<<"#策略一(buy and hold)可獲利："<<trading (data,day_num,1)<<",單日最大獲利:"<<big(data,day_num,1)<<",單日最大損失:"<<small(data,day_num,1)<<endl;
					cout<<"整年中共交易幾次：" <<sum_numtrade[1]<<endl;
					break;
				case 2:
					cout<<"#策略二(策略一+停損)可獲利："<<trading (data,day_num,2)<<",單日最大獲利:"<<big(data,day_num,2)<<",單日最大損失:"<<small(data,day_num,2)<<",整年停損幾次:"<<sum_slp[2]<<endl;
					cout<<"整年中共交易幾次：" <<sum_numtrade[2]<<endl;
					break;
				case 3:
					cout<<"#策略三(策略一+停損停利)可獲利："<<trading (data,day_num,3)<<",單日最大獲利:"<<big(data,day_num,3)<<",單日最大損失:"<<small(data,day_num,3)<<endl;
					cout<<"整年停損幾次:"<<sum_slp[3]<<",整年停利幾次:"<<sum_tpp[3]<<endl;
					cout<<"整年中共交易幾次：" <<sum_numtrade[3]<<endl;
					break;
				case 4:
					cout<<"#策略四(簡單版葛蘭碧交易法則)可獲利："<<trading (data,day_num,4)<<",單日最大獲利:"<<big(data,day_num,4)<<",單日最大損失:"<<small(data,day_num,4)<<endl;
					cout<<"整年突破幾次:"<<sum_BreakBuy[4]<<",整年跌破幾次:"<<sum_BreakSell[4]<<endl;
					cout<<"整年中共交易幾次：" <<sum_numtrade[4]<<endl;
					break;
				case 5:
					cout<<"#策略五(策略四+停損停利)可獲利："<<trading (data,day_num,5)<<",單日最大獲利:"<<big(data,day_num,5)<<",單日最大損失:"<<small(data,day_num,5)<<endl;
					cout<<"整年停損幾次:"<<sum_slp[5]<<",整年停利幾次:"<<sum_tpp[5]<<endl;
					cout<<"整年突破幾次:"<<sum_BreakBuy[5]<<",整年跌破幾次:"<<sum_BreakSell[5]<<endl;
					cout<<"整年中共交易幾次：" <<sum_numtrade[5]<<endl;
					break;
				case 6:
					cout<<"#策略六(加強版葛蘭碧法則)可獲利："/*(突破+假跌破+支撐+跌破+假突破+反彈)*/<<trading (data,day_num,6)<<",單日最大獲利:"<<big(data,day_num,6)<<",單日最大損失:"<<small(data,day_num,6)<<endl;
					cout<<"整年突破幾次:"<<sum_BreakBuy[6]<<",整年跌破幾次:"<<sum_BreakSell[6]<<endl<<"整年假跌破幾次:"<<sum_FaBreakBuy[6]<<",整年假突破幾次:"<<sum_FaBreakSell[6]<<endl;
					cout<<"整年支撐幾次:"<<sum_callback[6]<<",整年反彈幾次:"<<sum_bouncesell[6]<<endl;
					cout<<"整年中共交易幾次：" <<sum_numtrade[6]<<endl;
					break;
				case 7:
					cout<<"#策略七(加強版葛蘭碧法則+停損停利)可獲利："<<trading (data,day_num,7)<<",單日最大獲利:"<<big(data,day_num,7)<<",單日最大損失:"<<small(data,day_num,7)<<endl;
					cout<<"整年停損幾次:"<<sum_slp[7]<<",整年停利幾次:"<<sum_tpp[7]<<endl;
					cout<<"整年突破幾次:"<<sum_BreakBuy[7]<<",整年跌破幾次:"<<sum_BreakSell[7]<<endl<<"整年假跌破幾次:"<<sum_FaBreakBuy[7]<<",整年假突破幾次:"<<sum_FaBreakSell[7]<<endl;
					cout<<"整年支撐幾次:"<<sum_callback[7]<<",整年反彈幾次:"<<sum_bouncesell[7]<<endl;
					cout<<"整年中共交易幾次：" <<sum_numtrade[7]<<endl;
					break;
			}
		}
		else if(k==9)
		{
			cout<<"#策略一(buy and hold)可獲利："<<trading (data,day_num,1)<<",單日最大獲利:"<<big(data,day_num,1)<<",單日最大損失:"<<small(data,day_num,1)<<endl;
			cout<<"整年中共交易幾次：" <<sum_numtrade[1]<<endl<<endl;
			cout<<"#策略二(策略一+停損)可獲利："<<trading (data,day_num,2)<<",單日最大獲利:"<<big(data,day_num,2)<<",單日最大損失:"<<small(data,day_num,2)<<",整年停損幾次:"<<sum_slp[2]<<endl;
			cout<<"整年中共交易幾次：" <<sum_numtrade[2]<<endl<<endl;
			cout<<"#策略三(策略一+停損停利)可獲利："<<trading (data,day_num,3)<<",單日最大獲利:"<<big(data,day_num,3)<<",單日最大損失:"<<small(data,day_num,3)<<endl;
			cout<<"整年停損幾次:"<<sum_slp[3]<<",整年停利幾次:"<<sum_tpp[3]<<endl;
			cout<<"整年中共交易幾次：" <<sum_numtrade[3]<<endl<<endl;
			cout<<"#策略四(簡單版葛蘭碧交易法則)可獲利："<<trading (data,day_num,4)<<",單日最大獲利:"<<big(data,day_num,4)<<",單日最大損失:"<<small(data,day_num,4)<<endl;
			cout<<"整年突破幾次:"<<sum_BreakBuy[4]<<",整年跌破幾次:"<<sum_BreakSell[4]<<endl;
			cout<<"整年中共交易幾次：" <<sum_numtrade[4]<<endl<<endl;
			cout<<"#策略五(策略四+停損停利)可獲利："<<trading (data,day_num,5)<<",單日最大獲利:"<<big(data,day_num,5)<<",單日最大損失:"<<small(data,day_num,5)<<endl;
			cout<<"整年停損幾次:"<<sum_slp[5]<<",整年停利幾次:"<<sum_tpp[5]<<endl;
			cout<<"整年突破幾次:"<<sum_BreakBuy[5]<<",整年跌破幾次:"<<sum_BreakSell[5]<<endl;
			cout<<"整年中共交易幾次：" <<sum_numtrade[5]<<endl<<endl;
			cout<<"#策略六(加強版葛蘭碧法則)可獲利："/*(突破+假跌破+支撐+跌破+假突破+反彈)*/<<trading (data,day_num,6)<<",單日最大獲利:"<<big(data,day_num,6)<<",單日最大損失:"<<small(data,day_num,6)<<endl;
			cout<<"整年突破幾次:"<<sum_BreakBuy[6]<<",整年跌破幾次:"<<sum_BreakSell[6]<<endl<<"整年假跌破幾次:"<<sum_FaBreakBuy[6]<<",整年假突破幾次:"<<sum_FaBreakSell[6]<<endl;
			cout<<"整年支撐幾次:"<<sum_callback[6]<<",整年反彈幾次:"<<sum_bouncesell[6]<<endl;
			cout<<"整年中共交易幾次：" <<sum_numtrade[6]<<endl<<endl;
			cout<<"#策略七(加強版葛蘭碧法則+停損停利)可獲利："<<trading (data,day_num,7)<<",單日最大獲利:"<<big(data,day_num,7)<<",單日最大損失:"<<small(data,day_num,7)<<endl;
			cout<<"整年停損幾次:"<<sum_slp[7]<<",整年停利幾次:"<<sum_tpp[7]<<endl;
			cout<<"整年突破幾次:"<<sum_BreakBuy[7]<<",整年跌破幾次:"<<sum_BreakSell[7]<<endl<<"整年假跌破幾次:"<<sum_FaBreakBuy[7]<<",整年假突破幾次:"<<sum_FaBreakSell[7]<<endl;
			cout<<"整年支撐幾次:"<<sum_callback[7]<<",整年反彈幾次:"<<sum_bouncesell[7]<<endl;
			cout<<"整年中共交易幾次：" <<sum_numtrade[7]<<endl;
		}
		else if(k==0)
			break;
		else if(k>9)
		{
			cout<<"您輸入的數字不在服務範圍，請從新輸入，謝謝"<<endl; 
			cout<<"策略一(buy and hold)，請輸入1"<<endl<<"策略二(策略一+停損)，請輸入2"<<endl<<"策略三(策略一+停損停利)，請輸入3"<<endl<<"策略四(簡單版葛蘭碧交易法則)，請輸入4"<<endl;
			cout<<"策略五(策略四+停損停利)，請輸入5"<<endl<<"策略六(加強版葛蘭碧法則)(突破+假跌破+支撐+跌破+假突破+反壓)，請輸入6"<<endl<<"策略七(加強版葛蘭碧法則+停損停利)，請輸入7"<<endl;
			cout<<"如果沒有你覺得適合的，將會直接執行累積獲利最多的，請輸入8"<<endl<<"如果想知道所有策略的答案，請輸入9"<<endl; 
			cout<<"想結束回測，請輸入0"<<endl;	 
		}
		else 
		{
			cout<<"請從新輸入上面有的數字，謝謝"<<endl<<endl;
		}	
	}
	return 0;
}
