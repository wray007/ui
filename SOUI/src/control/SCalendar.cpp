/**
 * Copyright (C) 2014-2050 SOUI团队
 * All rights reserved.
 *
 * @file       SCalendar.cpp
 * @brief      SCalendarCore以及SCalendar类源文件
 * @version    v1.0
 * @author     soui
 * @date       2014-05-25
 *
 * Describe  时间控件相关函数实现
 */
#include "souistd.h"
#include "control/SCalendar.h"
#include "helper/STime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SCalendarCore

namespace SOUI{

extern WORD gLunarMonthDay[];
extern BYTE gLunarMonth[];
extern BYTE gLunarHolDay[];

#define REFTYPE_SUJIU 1		//数九
#define REFTYPE_MEIYU 2		//梅雨
#define REFTYPE_SANFU 3		//三伏

#define TITLE_HEIGHT 20
#define FOOTER_HEIGHT 20

//两个子控件的名字
#define NAME_BTN_TODAY    "btn_today"
#define NAME_LABEL_TODAY "label_today"

WORD const DayOrdinal[13]={0,31,59,90,120,151,181,212,243,273,304,334,365};
WORD const DayOrdinalLeap[13]={0,31,60,91,121,152,182,213,244,274,305,335,366};

BOOL SCalendarCore::IsLeapYear(WORD wYear,BOOL &bLeapYear)
{
	if (wYear<1600||wYear>=7000)//压缩算法规定了的年份区间（提取器只能导出此区间的数据，Lunar.dll支持-6000到20000（不含20000）之间的年份）
	{
		return FALSE;
	}
	if (wYear%4==0&&wYear%100!=0||wYear%400==0)//判断闰年的条件
	{
		bLeapYear=TRUE;
	}else
	{
		bLeapYear=FALSE;
	}
	return TRUE;
}

BOOL SCalendarCore::GetDaysNumInYear(WORD wYear, WORD wMonth, WORD wDay,WORD &wDays)
{
	//从日期算出距离元旦的天数
	if (DateCheck(wYear,wMonth,wDay)==0)//对输入的日期进行检查
	{
		return FALSE;
	}
	BOOL bLeapYear=FALSE;
	if (!IsLeapYear(wYear,bLeapYear))
	{
		return FALSE;
	}
	if (bLeapYear==TRUE)
	{
		wDays=DayOrdinalLeap[wMonth-1]+wDay-1;//元旦为序数0，因此减1
	}else
	{
		wDays=DayOrdinal[wMonth-1]+wDay-1;
	}
	return TRUE;
}

BOOL SCalendarCore::GetDateFromDays(WORD wYear, WORD wDays, WORD &wMonth, WORD &wDay)
{
	//从距离元旦的天数算出日期
	if (wDays<1)
	{
		return FALSE;
	}
	BOOL bLeapYear=FALSE;
	if (!IsLeapYear(wYear,bLeapYear))
	{
		return FALSE;
	}
	if (bLeapYear==TRUE)
	{
		if (wDays>366)//超出了该年的总天数
		{
			return FALSE;
		}
	}else
	{
		if (wDays>365)
		{
			return FALSE;
		}
	}
	wMonth=0;
	int i=0;
	for (i=0;i<12;i++)
	{
		if (bLeapYear==TRUE)
		{
			if (wDays>=DayOrdinalLeap[i]&&wDays<DayOrdinalLeap[i+1])//找出月份
			{
				wMonth=i+1;
				wDay=wDays-DayOrdinalLeap[i];//计算出“日”
				break;
			}
		}else
		{
			if (wDays>=DayOrdinal[i]&&wDays<DayOrdinal[i+1])
			{
				wMonth=i+1;
				wDay=wDays-DayOrdinal[i];
				break;
			}
		}
	}
	return TRUE;
}

BOOL SCalendarCore::DateCheck(WORD wYear,WORD wMonth,WORD wDay)
{
	if (wYear<START_YEAR||wYear>=END_YEAR||wMonth>12||wMonth<1||wDay<1||wDay>31)
	{
		return FALSE;
	}
	if (wMonth==4||wMonth==6||wMonth==9||wMonth==11)
	{
		if (wDay==31)
		{
			return FALSE;
		}
	}else if (wMonth==2)
	{
		BOOL bLeapYear=FALSE;
		IsLeapYear(wYear,bLeapYear);
		if (bLeapYear==FALSE)
		{
			if (wDay>28)
			{
				return FALSE;
			}
		}else
		{
			if (wDay>29)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

short SCalendarCore::LunarGetNewYearOrdinal(WORD wLunarYear)
{
	if (wLunarYear<START_YEAR||wLunarYear>=END_YEAR)
	{
		return 0;
	}
	return uLunarNewYearOrdinal[wLunarYear-START_YEAR];//取出农历新年的年内序数
}

WORD SCalendarCore::LunarGetDaysofMonth(WORD wLunarYear,WORD wLunarMonth,BOOL bLeapMonth)
{
	int i=0;//循环变量
	WORD DX_data=0;//该年大小月情况
	WORD Acc_LeapMonth=0;
	if (wLunarYear==START_YEAR-1)//农历还在起始年份的前一年
	{
		if (iPreLeapIndex==-1)//无闰月
		{
			if (bLeapMonth==TRUE)
			{
				return 0;
			}
			return iPreMonth[wLunarMonth-9]-iPreMonth[wLunarMonth-10];
		}else
		{
			Acc_LeapMonth=iPreLeapIndex+9;
			if (Acc_LeapMonth>wLunarMonth)
			{
				if (bLeapMonth==TRUE)
				{
					return 0;
				}
				return iPreMonth[wLunarMonth-9]-iPreMonth[wLunarMonth-10];
			}else
			{
				if ((bLeapMonth==1&&wLunarMonth==Acc_LeapMonth)||wLunarMonth>Acc_LeapMonth)
				{
					return iPreMonth[wLunarMonth-8]-iPreMonth[wLunarMonth-9];
				}else
				{
					return iPreMonth[wLunarMonth-9]-iPreMonth[wLunarMonth-10];
				}
			}
		}
	}else
	{
		if (wLunarYear<START_YEAR||wLunarYear>=END_YEAR||wLunarMonth<1||wLunarMonth>12)
		{
			return 0;
		}
	}
	DX_data=LunarMonthDX[wLunarYear-START_YEAR];//整年大小月情况
	Acc_LeapMonth=LunarGetLeapMonth(wLunarYear);//获取真实闰月月份
	if (bLeapMonth)//指定查询的当前月是闰月
	{
		if (Acc_LeapMonth!=wLunarMonth)
		{
			return 0;//不存在的闰月
		}
		for (i=0;i<(12-wLunarMonth);i++)
		{
			DX_data>>=1;//右移一位，即从末尾开始找该闰月月份所在的位
		}
	}else
	{
		if (DX_data&0x2000)//存在闰月
		{
			if (wLunarMonth<=Acc_LeapMonth)//月份在闰月之前，倒找需要多找一位
			{
				for (i=0;i<=(12-wLunarMonth);i++)
				{
					DX_data>>=1;
				}
			}else
			{
				for (i=0;i<(12-wLunarMonth);i++)//月份在闰月之后
				{
					DX_data>>=1;
				}
			}
		}else
		{
			for (i=0;i<(12-wLunarMonth);i++)//无闰月
			{
				DX_data>>=1;
			}
		}
	}
	if (DX_data&0x1)
	{
		return 30;//大月
	}else
	{
		return 29;//小月
	}
}

BOOL SCalendarCore::LunarExpandDX(WORD wLunarYear,WORD wDayOfMonth[15])
{
	if (wLunarYear<START_YEAR||wLunarYear>=END_YEAR)
	{
		return FALSE;
	}
	int i=0;//循环变量
	WORD pos=0,iLeapMonth=0;//数组写入位置，闰月
	for (i=0;i<15;i++)
	{
		wDayOfMonth[i]=0;//对数组清零
	}
	if (wLunarYear==START_YEAR)
	{
		if (iPreLeapIndex==-1)//处理起始年份之前一年数据
		{
			wDayOfMonth[pos]=iPreMonth[2]-iPreMonth[1];//农历上一年十一月总天数
			pos++;
		}else
		{
			if (iPreLeapIndex==1)//闰十月
			{
				wDayOfMonth[pos]=iPreMonth[3]-iPreMonth[2];
				pos++;
			}else
			{
				//闰十一月或闰十二月
				wDayOfMonth[pos]=iPreMonth[2]-iPreMonth[1];
				pos++;
				wDayOfMonth[pos]=iPreMonth[3]-iPreMonth[2];
				pos++;
			}
		}
		wDayOfMonth[pos]=LunarGetNewYearOrdinal(wLunarYear)-iPreMonth[2];//（闰）十二月
		pos++;
	}else
	{
		iLeapMonth=LunarGetLeapMonth(wLunarYear-1);//取得前一农历年的闰月情况
		if (iLeapMonth<11)//一月至十月的闰月
		{
			wDayOfMonth[pos]=LunarGetDaysofMonth(wLunarYear-1,11,0);//取上一年十一月天数
			pos++;
			wDayOfMonth[pos]=LunarGetDaysofMonth(wLunarYear-1,12,0);//取上一年十二月天数
			pos++;
		}else
		{
			wDayOfMonth[pos]=LunarGetDaysofMonth(wLunarYear-1,11,0);//取上一年十一月的天数
			pos++;
			if (iLeapMonth==11)//闰十一月
			{
				wDayOfMonth[pos]=LunarGetDaysofMonth(wLunarYear-1,11,1);//取上一年闰十一月的天数
				pos++;
				wDayOfMonth[pos]=LunarGetDaysofMonth(wLunarYear-1,12,0);//取上一年十二月天数
				pos++;
			}else if (iLeapMonth==12)
			{
				wDayOfMonth[pos]=LunarGetDaysofMonth(wLunarYear-1,12,0);//取上一年十二月天数
				pos++;
				wDayOfMonth[pos]=LunarGetDaysofMonth(wLunarYear-1,12,1);//取上一年闰十二月天数
				pos++;
			}
		}
	}
	iLeapMonth=LunarGetLeapMonth(wLunarYear);//获取当前农历年的闰月情况
	if (iLeapMonth==0)//无闰月
	{
		for (i=0;i<12;i++)
		{
			wDayOfMonth[pos]=LunarGetDaysofMonth(wLunarYear,i+1,0);//取每个农历月天数
			pos++;
		}
	}else
	{
		for (i=0;i<12;i++)
		{
			if (i==iLeapMonth)
			{
				wDayOfMonth[pos]=LunarGetDaysofMonth(wLunarYear,i,1);//取闰月的天数
				pos++;
			}
			wDayOfMonth[pos]=LunarGetDaysofMonth(wLunarYear,i+1,0);//取非闰月的天数
			pos++;
		}
	}
	return TRUE;
}

WORD SCalendarCore::LunarGetLeapMonth(WORD wLunarYear)
{
	unsigned char Leap_data=0;
	if (wLunarYear<START_YEAR||wLunarYear>=END_YEAR)
	{
		return 0;
	}
	Leap_data=LeapMonth[(wLunarYear-START_YEAR)/2];
	if ((wLunarYear-START_YEAR)%2==1)
	{
		return (Leap_data&0xF);//低4位
	}else
	{
		return (Leap_data&0xF0)>>4;//高4位
	}
}

BOOL SCalendarCore::GetLunar(WORD wYear,WORD wMonth,WORD wDay,WORD &wLunarYear,WORD &wLunarMonth,WORD &wLunarDay,BOOL &bLeapMonth)
{
	WORD DaysNum=0;//年内序数
	if (!GetDaysNumInYear(wYear,wMonth,wDay,DaysNum))
	{
		return FALSE;//日期不正确
	}
	wLunarYear=wYear;
	short LunarNewYear=0,remain_days=0;//农历新年的公历年内序数，剩余天数
	WORD DaysOfLunarMonth[15]={0};//存放农历月份天数
	WORD iLeapMonthPre=0,iLeapMonth=0;//农历上一年闰月，今年闰月
	LunarNewYear=LunarGetNewYearOrdinal(wLunarYear);
	LunarExpandDX(wLunarYear,DaysOfLunarMonth);//获取月份天数，数组从上一年十一月开始到今年（闰）十二月，包含闰月
	iLeapMonthPre=LunarGetLeapMonth(wLunarYear-1);
	if (iLeapMonthPre==0)
	{
		iLeapMonth=LunarGetLeapMonth(wLunarYear);//上一年没有闰月，则查询今年闰月
	}
	bLeapMonth=FALSE;
	remain_days=DaysNum-LunarNewYear;//距离农历新年天数
	int i=0;
	if (iLeapMonthPre>10)
	{
		i=3;//今年正月在“DaysOfLunarMonth”中的索引，上一年十一月或十二月有闰月
	}else
	{
		i=2;//上一年十一月和十二月无闰月时，今年正月的索引
	}
	if (LunarNewYear>DaysNum)//早于农历新年
	{
		wLunarYear-=1;//农历年减1
		while(remain_days<0)
		{
			i--;//第一次先减去是因为当前i是正月，减1表示上一年十二月（或闰十二月）
			remain_days+=DaysOfLunarMonth[i];//加上上一年十二月、十一月的总天数（含闰月）直到日数大于0
		}
		if (iLeapMonthPre>10)//如果上一年十一月或十二月存在闰月
		{
			if (iLeapMonthPre==11)//闰十一月
			{
				if (i==0)//十一月（即在闰月之前）
				{
					wLunarMonth=11+i;//转换到月份
				}else
				{
					wLunarMonth=10+i;
					if (wLunarMonth==iLeapMonthPre)
					{
						bLeapMonth=1;
					}
				}
			}else if (iLeapMonthPre==12)//闰十二月
			{
				if (i<2)//在闰月之前
				{
					wLunarMonth=11+i;
				}else
				{
					wLunarMonth=10+i;
					if (wLunarMonth==iLeapMonthPre)
					{
						bLeapMonth=TRUE;
					}
				}
			}
		}else
		{
			wLunarMonth=11+i;
		}
		wLunarDay=remain_days;
	}else
	{
		while (remain_days>=DaysOfLunarMonth[i])
		{
			remain_days-=DaysOfLunarMonth[i];//寻找农历月
			i++;//移至下个月
		}
		if (iLeapMonthPre>10)
		{
			wLunarMonth=i-2;
		}else
		{
			if (iLeapMonth>0)
			{
				if (i-2<iLeapMonth)
				{
					wLunarMonth=i-1;
				}else
				{
					wLunarMonth=i-2;
					if (wLunarMonth==iLeapMonth)
					{
						bLeapMonth=TRUE;
					}
				}
			}else
			{
				wLunarMonth=i-1;
			}
		}
		wLunarDay=remain_days;
	}
	wLunarDay+=1;//索引转换到数量
	return TRUE;
}

BOOL SCalendarCore::GetGregorian(WORD wLunarYear,WORD wLunarMonth,WORD wLunarDay,BOOL bLeapMonth,WORD &wYear,WORD &wMonth,WORD &wDay)
{
	if (wLunarYear<START_YEAR||wLunarYear>=END_YEAR||wLunarMonth<1||wLunarMonth>12||wLunarDay<1||wLunarDay>30)
	{
		return FALSE;//年、月、日检查
	}
	if (bLeapMonth)
	{
		if (LunarGetLeapMonth(wLunarYear)!=wLunarMonth)
		{
			return FALSE;//闰月检查
		}
	}
	if (wLunarDay>LunarGetDaysofMonth(wLunarYear,wLunarMonth,bLeapMonth))
	{
		return FALSE;//大小月检查
	}
	WORD DaysOfLunarMonth[15]={0};//存放农历月份天数
	WORD iLeapMonthPre=0,iLeapMonth=0;//农历年闰月
	WORD LunarNewYear=0;//农历新年的公历年内序数
	WORD iDaysofYear=0;//一年总天数
	short remain_days=0;//剩余天数
	int i=0;//循环变量
	LunarExpandDX(wLunarYear,DaysOfLunarMonth);//大小月表（农历每月天数）
	LunarNewYear=LunarGetNewYearOrdinal(wLunarYear);//找到正月初一r公历年内序数
	iLeapMonth=LunarGetLeapMonth(wLunarYear);//找出农历年的闰月
	remain_days+=LunarNewYear;//加上正月初一的序数
	if (iLeapMonthPre>10)
	{
		i=3;//今年正月在“DaysOfLunarMonth”中的索引，上一年十一月或十二月有闰月
	}else
	{
		i=2;//上一年十一月和十二月无闰月时，今年正月的索引
	}
	if (iLeapMonth==0)
	{
		if (iLeapMonthPre>10)
		{
			for (;i<wLunarMonth+2;i++)
			{
				remain_days+=DaysOfLunarMonth[i];//年内序数累加
			}
		}else
		{
			for (;i<wLunarMonth+1;i++)
			{
				remain_days+=DaysOfLunarMonth[i];//年内序数累加
			}
		}
	}else
	{
		if (wLunarMonth<iLeapMonth||(bLeapMonth==FALSE&&wLunarMonth==iLeapMonth))//在闰月之前
		{
			for (;i<wLunarMonth+1;i++)
			{
				remain_days+=DaysOfLunarMonth[i];
			}
		}else
		{
			for (;i<wLunarMonth+2;i++)
			{
				remain_days+=DaysOfLunarMonth[i];
			}
		}
	}
	remain_days+=wLunarDay;
	GetDaysNumInYear(wLunarYear,12,31,iDaysofYear);//获取公历年总天数
	wYear=wLunarYear;
	if (remain_days>iDaysofYear)
	{
		remain_days-=iDaysofYear;
		wYear+=1;//下一年
	}
	GetDateFromDays(wYear,remain_days,wMonth,wDay);
	return TRUE;
}

BOOL SCalendarCore::GetJieQi(WORD wYear,WORD wMonth,WORD wJieQi[2])
{
	if (wYear<START_YEAR||wYear>=END_YEAR||wMonth>12||wMonth<1)
	{
		return FALSE;
	}
	int index=0;
	index=(wYear-START_YEAR)*12+wMonth-1;//对应每公历年首个节气的索引（每行12个）加上月份偏移
	wJieQi[0]=15-(SolarTerms[index]>>4);//高4位第一个节气
	wJieQi[1]=(SolarTerms[index]&0x0F)+15;//低4位第二个节气
	return TRUE;
}

short SCalendarCore::GetDayOfWeek(WORD wYear,WORD wMonth,WORD wDay)
{
	WORD uDayOrd=0;
	if (GetDaysNumInYear(wYear,wMonth,wDay,uDayOrd)==0)
	{
		return -1;
	}
	unsigned int DayofWeek=0;
	uDayOrd++;//一年中的第几天，因为GetDaysNumInYear所得到的是索引，因此要加一
	wYear--;
	DayofWeek=(wYear+wYear/4-wYear/100+wYear/400+uDayOrd)%7;//这个只是算星期的通用公式
	return DayofWeek;
}

WORD SCalendarCore::GetDaysOfMonth(WORD wYear,WORD wMonth)
{
	if (wMonth==12)
	{
		return 31;//这里为了简便，判断12月就直接返回
	}
	WORD days1=0,days2=0;
	WORD ret=0;
	ret=GetDaysNumInYear(wYear,wMonth,1,days1);//本月1日在年内的序数
	if (ret==0)
	{
		return ret; 
	}
	wMonth++;
	ret=GetDaysNumInYear(wYear,wMonth,1,days2);//下个月1日的年内序数
	if (ret==0)
	{
		return ret; 
	}
	ret=days2-days1;//下个月1日的序数减本月1日的序数
	return ret;
}

BOOL SCalendarCore::GetExtermSeason(WORD wYear,WORD wMonth,WORD wDay,WORD wType,WORD &sESIndex)
{
	WORD wDayOrder=0;
	if (GetDaysNumInYear(wYear,wMonth,wDay,wDayOrder)==FALSE)
	{
		return FALSE;
	}
	DWORD dwIndex=(wYear-START_YEAR)*5;//索引项
	switch (wType)
	{
	case REFTYPE_SUJIU:
		if (wDayOrder>=ExtermSeason[dwIndex]&&wDayOrder<(ExtermSeason[dwIndex]+63))
		{
			if ((wDayOrder-ExtermSeason[dwIndex])%9==0)
			{
				sESIndex=(wDayOrder-ExtermSeason[dwIndex])/9+2;//三九到九九
				return TRUE;
			}
		}else if (wMonth==12)//12月的冬至（一九）和二九
		{
			WORD wJQ[2]={0};
			GetJieQi(wYear,wMonth,wJQ);//一九就是冬至，因此不需要写出
			if (wDay==wJQ[1]+9)
			{
				sESIndex=1;//二九
				return TRUE;
			}
		}
		break;
	case REFTYPE_MEIYU:
		if (wDayOrder==ExtermSeason[dwIndex+1])
		{
			sESIndex=0;//入梅
			return TRUE;
		}else if (wDayOrder==ExtermSeason[dwIndex+2])
		{
			sESIndex=1;//出梅
			return TRUE;
		}
		break;
	case REFTYPE_SANFU:
		if (wDayOrder==ExtermSeason[dwIndex+3])
		{
			sESIndex=0;//初伏
			return TRUE;
		}else if (wDayOrder==(ExtermSeason[dwIndex+3]+10))
		{
			sESIndex=1;//中伏
			return TRUE;
		}else if (wDayOrder==ExtermSeason[dwIndex+4])
		{
			sESIndex=2;//末伏
			return TRUE;
		}
		break;
	default:
		break;
	}
	return FALSE;
}

void SCalendarCore::FormatYear(WORD  iYear, TCHAR *pBuffer,BOOL bLunar)
{
	if (bLunar)
	{
		TCHAR szText1[][3]={_T("甲"),_T("乙"),_T("丙"),_T("丁"),_T("戊"),_T("己"),_T("庚"),_T("辛"),_T("壬"),_T("癸")};
		TCHAR szText2[][3]={_T("子"),_T("丑"),_T("寅"),_T("卯"),_T("辰"),_T("巳"),_T("午"),_T("未"),_T("申"),_T("酉"),_T("戌"),_T("亥")};
		TCHAR szText3[][3]={_T("鼠"),_T("牛"),_T("虎"),_T("免"),_T("龙"),_T("蛇"),_T("马"),_T("羊"),_T("猴"),_T("鸡"),_T("狗"),_T("猪")};

		_stprintf(pBuffer,_T("%s%s %s"),szText1[(iYear-4)%10],szText2[(iYear-4)%12],szText3[(iYear-4)%12]);
	}else
	{
		_stprintf(pBuffer,_T("%d年"),iYear);
	}
}

void SCalendarCore::FormatMonth(WORD iMonth, TCHAR *pBuffer,BOOL bLunar, BOOL bLeapMonth)
{
	if (bLunar)
	{
		TCHAR szText[][3]={_T("正"),_T("二"),_T("三"),_T("四"),_T("五"),_T("六"),_T("七"),_T("八"),_T("九"),_T("十")};
		if(iMonth<=10)
		{
			if (bLeapMonth==TRUE)
			{
				_stprintf(pBuffer,_T(" 闰%s月"),szText[iMonth -1]);
			}else
			{
				_stprintf(pBuffer,_T(" %s月"),szText[iMonth -1]);
			}
		}else if (iMonth == 11)
		{
			if (bLeapMonth==TRUE)
			{
				_tcscpy(pBuffer, _T("闰冬月"));
			}else
			{
				_tcscpy(pBuffer, _T("冬月"));
			}
		}else if (iMonth==12)
		{
			if (bLeapMonth==TRUE)
			{
				_tcscpy(pBuffer, _T("闰腊月"));
			}else
			{
				_tcscpy(pBuffer, _T("腊月"));
			}
		}
	}else
	{
		_stprintf(pBuffer,_T("%d月"),iMonth);
	}
}

void SCalendarCore::FormatDay(WORD wYear,WORD wMonth,WORD wDay,TCHAR *pBuffer,BOOL bLunar,BOOL bShowHoliday)
{
    if (bLunar)
    {
		TCHAR szText1[][3]={_T("初"),_T("十"),_T("廿")};
		TCHAR szText2[][3]={_T("一"),_T("二"),_T("三"),_T("四"),_T("五"),_T("六"),_T("七"),_T("八"),_T("九"),_T("十")};
		WORD wLunarYear=0,wLunarMonth=0,wLunarDay=0;
		BOOL bLeapMonth=FALSE;
		if (GetLunar(wYear,wMonth,wDay,wLunarYear,wLunarMonth,wLunarDay,bLeapMonth)==FALSE)
		{
			return;
		}
		if (bShowHoliday==TRUE)
		{
			if (GetLunarHoliday(wYear,wMonth,wDay,pBuffer)==TRUE)
			{
				return;
			}
		}
		if (wLunarDay==1)
		{
			WORD wldom=LunarGetDaysofMonth(wLunarYear,wLunarMonth,bLeapMonth);
			TCHAR ptmp[10]={0};
			FormatMonth(wLunarMonth,ptmp,TRUE,bLeapMonth);
			if (wldom==29)
			{
				_stprintf(pBuffer,_T("%s(小)"),ptmp);
				return;
			}else if (wldom==30)
			{
				_stprintf(pBuffer,_T("%s(大)"),ptmp);
				return;
			}
		}
		if(wLunarDay != 20 && wLunarDay !=30)
		{
			_stprintf(pBuffer,_T("%s%s"),szText1[(wLunarDay-1)/10],szText2[(wLunarDay-1)%10]);
		}
		else
		{
			_stprintf(pBuffer,_T("%s十"),szText2[(wLunarDay/10)-1]);
		}
    }else
	{
		_stprintf(pBuffer,_T("%d日"),wDay);
	}
}

BOOL SCalendarCore::GetLunarHoliday(WORD wYear,WORD wMonth,WORD wDay,TCHAR *pBuffer)
{
	WORD wJQ[2]={0};
	if (GetJieQi(wYear,wMonth,wJQ)==FALSE)
	{
		return FALSE;
	}
	TCHAR *Jieqi[24]={_T("小寒"),_T("大寒"),_T("立春"),_T("雨水"),_T("惊蛰"),_T("春分"),_T("清明"),_T("谷雨"),_T("立夏"),_T("小满"),_T("芒种"),_T("夏至"),_T("小暑"),_T("大暑"),_T("立秋"),_T("处暑"),_T("白露"),_T("秋分"),_T("寒露"),_T("霜降"),_T("立冬"),_T("小雪"),_T("大雪"),_T("冬至")};
	TCHAR *Shujiu[9]={_T("一"),_T("二"),_T("三"),_T("四"),_T("五"),_T("六"),_T("七"),_T("八"),_T("九")};
	TCHAR *Meiyu[2]={_T("入梅"),_T("出梅")};
	TCHAR *Sanfu[3]={_T("初伏"),_T("中伏"),_T("末伏")};
	if (wDay==wJQ[0])
	{
		_tcscpy(pBuffer, Jieqi[(wMonth-1)*2]);
		return TRUE;
	}else if (wDay==wJQ[1])
	{
		_tcscpy(pBuffer, Jieqi[wMonth*2-1]);
		return TRUE;
	}
	WORD wLeapMonth=LunarGetLeapMonth(wYear);
	WORD wLunarYear=0,wLunarMonth=0,wLunarDay=0;
	BOOL bLeapMonth=FALSE;
	if (GetLunar(wYear,wMonth,wDay,wLunarYear,wLunarMonth,wLunarDay,bLeapMonth)==FALSE)
	{
		return FALSE;
	}
	if (wLunarMonth==12)
	{
		if ((wLeapMonth!=wLunarMonth)||(wLeapMonth==wLunarMonth&&bLeapMonth==TRUE))
		{
			if (wLunarDay==8)
			{
				_tcscpy(pBuffer, _T("腊八节"));
				return TRUE;
			}else if (wLunarDay==23)
			{
				_tcscpy(pBuffer, _T("小年"));
				return TRUE;
			}else
			{
				if (wLunarDay==LunarGetDaysofMonth(wLunarYear,wLunarMonth,bLeapMonth))
				{
					_tcscpy(pBuffer, _T("除夕"));
					return TRUE;
				}
			}
		}
	}else
	{
		if (bLeapMonth==FALSE)
		{
			if (wLunarMonth==1)
			{
				switch (wLunarDay)
				{
				case 1:
					_tcscpy(pBuffer, _T("春节"));
					return TRUE;
					break;
				case 15:
					_tcscpy(pBuffer, _T("元宵节"));
					return TRUE;
					break;
				default:
					break;
				}
			}else if (wLunarMonth==2)
			{
				if (wLunarDay==2)
				{
					_tcscpy(pBuffer, _T("春龙节"));
					return TRUE;
				}
			}else if (wLunarMonth==5)
			{
				if (wLunarDay==5)
				{
					_tcscpy(pBuffer, _T("端午节"));
					return TRUE;
				}
			}else if (wLunarMonth==7)
			{
				switch (wLunarDay)
				{
				case 7:
					_tcscpy(pBuffer, _T("七夕"));
					return TRUE;
					break;
				case 15:
					_tcscpy(pBuffer, _T("中元节"));
					return TRUE;
					break;
				default:
					break;
				}
			}else if (wLunarMonth==8)
			{
				if (wLunarDay==15)
				{
					_tcscpy(pBuffer, _T("中秋节"));
					return TRUE;
				}
			}else if (wLunarMonth==9)
			{
				if (wLunarDay==9)
				{
					_tcscpy(pBuffer, _T("重阳节"));
					return TRUE;
				}
			}else if (wLunarMonth==10)
			{
				if (wLunarDay==1)
				{
					_tcscpy(pBuffer, _T("祭祖节"));
					return TRUE;
				}else if (wLunarDay==15)
				{
					_tcscpy(pBuffer, _T("下元节"));
					return TRUE;
				}
			}
		}
	}
	WORD wEXindex=0;
	if (GetExtermSeason(wYear,wMonth,wDay,REFTYPE_SUJIU,wEXindex))
	{
		_stprintf(pBuffer,_T("『%s九』"),Shujiu[wEXindex]);
		return TRUE;
	}
	if (GetExtermSeason(wYear,wMonth,wDay,REFTYPE_MEIYU,wEXindex))
	{
		_stprintf(pBuffer,_T("『%s』"),Meiyu[wEXindex]);
		return TRUE;
	}
	if (GetExtermSeason(wYear,wMonth,wDay,REFTYPE_SANFU,wEXindex))
	{
		_stprintf(pBuffer,_T("『%s』"),Sanfu[wEXindex]);
		return TRUE;
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////
//    SCalendarCore
//////////////////////////////////////////////////////////////////////////

SCalendar::SCalendar(WORD iYear, WORD iMonth, WORD iDay)
{
    Init();
   if(!SetDate(iYear, iMonth, iDay))
   {
       CTime tm=CTime::GetCurrentTime();
       SetDate(tm.GetYear(),tm.GetMonth(),tm.GetDay());
   }
}

SCalendar::SCalendar()
{
    Init();
    CTime tm=CTime::GetCurrentTime();
    SetDate(tm.GetYear(),tm.GetMonth(),tm.GetDay());
}

void SCalendar::Init()
{
    m_evtSet.addEvent(EVENTID(EventCalendarSelDay));
    m_nTitleHei=TITLE_HEIGHT;
    m_nFooterHei=FOOTER_HEIGHT;
    m_crWeekend=RGBA(255,0,0,255);
    m_crDay=RGBA(255,0,0,255);
    m_crTitleBack=RGBA(0,255,0,255);
    m_crDayBack=RGBA(0,0,255,255);
    m_pTitleSkin=m_pDaySkin=NULL;

    m_iHoverDay=0;

    TCHAR sztext[][3]={_T("日"),_T("一"),_T("二"),_T("三"),_T("四"),_T("五"),_T("六")};
    for(int i=0;i<7;i++) m_strTitle[i]=sztext[i];
}

/////////////////////////////////////////////////////////////////////////////
// SCalendarCore message handlers
void SCalendar::DrawTitle(IRenderTarget *pRT)
{
   CRect rect ;
   GetClientRect(&rect);

   rect.bottom = rect.top + m_nTitleHei;

   if(m_pTitleSkin)
       m_pTitleSkin->Draw(pRT,rect,0);
   else
       pRT->FillSolidRect(&rect,m_crTitleBack);

   int nWid=rect.Width()/7;
   CRect rcItem=rect;
   rcItem.right=rcItem.left+nWid;

   COLORREF crTxt=pRT->GetTextColor();
   for(int i=0; i <7; i++)
   {
       if(i==0 || i==6 )
           pRT->SetTextColor(m_crWeekend);
       else
           pRT->SetTextColor(crTxt);

       pRT->DrawText(m_strTitle[i],m_strTitle[i].GetLength(),rcItem,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
       rcItem.OffsetRect(nWid,0);
   }
   pRT->SetTextColor(crTxt);
}

void SCalendar::DrawDate(IRenderTarget *pRT)
{

    int days=SCalendarCore::GetDaysOfMonth(m_iYear, m_iMonth);

    for(int i=1;i<=days;i++)
    {
        CRect rcDay=GetDayRect(i);
        DrawDay(pRT,rcDay,i);
    }
}


void SCalendar::DrawDay( IRenderTarget *pRT,CRect & rcDay,WORD iDay )
{
    TCHAR text[3];
    _stprintf(text, _T("%2d"), iDay);
    COLORREF crTxt=pRT->GetTextColor();
    if(iDay==m_iDay)
    {
        if(m_pDaySkin) m_pDaySkin->Draw(pRT,rcDay,2);
        else pRT->FillSolidRect(rcDay,m_crDayBack);
        pRT->SetTextColor(m_crDay);
    }else
    {
        if(m_pDaySkin) m_pDaySkin->Draw(pRT,rcDay,iDay==m_iHoverDay?1:0);
        int iweekday=SCalendarCore::GetDayOfWeek(m_iYear,m_iMonth,iDay);
        if(iweekday==0 || iweekday==6)
            pRT->SetTextColor(m_crWeekend);
    }
    pRT->DrawText(text,-1,rcDay,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
    pRT->SetTextColor(crTxt);
}

void SCalendar::RedrawDay(WORD iDay )
{
    CRect rcDay=GetDayRect(iDay);
    IRenderTarget * pRT=GetRenderTarget(&rcDay,OLEDC_PAINTBKGND);
    DrawDay(pRT,rcDay,iDay);
    ReleaseRenderTarget(pRT);
}

void SCalendar::OnPaint(IRenderTarget * pRT) 
{
    SPainter painter;
    BeforePaint(pRT,painter);
    DrawTitle(pRT);
    DrawDate(pRT);
    AfterPaint(pRT,painter);
}

void SCalendar::GetDate(WORD &iYear, WORD &iMonth, WORD &iDay) 
{
    iYear  = m_iYear;
    iMonth = m_iMonth;
    iDay   = m_iDay;
}

BOOL SCalendar::SetDate(WORD iYear, WORD iMonth, WORD iDay)
{
  if(iYear < START_YEAR || iYear > END_YEAR || iMonth <1 || iMonth >12)
        return FALSE;

  if(iDay <1 || iDay > SCalendarCore::GetDaysOfMonth(iYear, iMonth))
        return FALSE;

  m_iYear   = iYear;
  m_iMonth  = iMonth;
  m_iDay    = iDay;

  m_iHoverDay    = 0;
  return TRUE;
} 


CRect SCalendar::GetDayRect( WORD iDay )
{
    CRect rcClient;
    GetClientRect(&rcClient);
    rcClient.top+=m_nTitleHei;
    rcClient.bottom-=m_nFooterHei;
    
    int col  = SCalendarCore::GetDayOfWeek(m_iYear, m_iMonth,iDay);//计算出iday是星期几
    int row  = (SCalendarCore::GetDayOfWeek(m_iYear, m_iMonth,1)+iDay-1)/7;//计算出iday是第几周
    
    int nWid=rcClient.Width()/7;
    int nHei=rcClient.Height()/6;

    CRect rc(0,0,nWid,nHei);
    rc.OffsetRect(nWid*col,nHei*row);
    rc.OffsetRect(rcClient.TopLeft());
    return rc;
}

WORD SCalendar::HitTest(CPoint pt)
{
    CRect rcClient;
    GetClientRect(&rcClient);
    rcClient.top+=m_nTitleHei;
    rcClient.bottom-=m_nFooterHei;

    int nWid=rcClient.Width()/7;
    int nHei=rcClient.Height()/6;

    pt-=rcClient.TopLeft();
    if(pt.x<0 || pt.y<0) return 0;

    int iCol  = pt.x/nWid;
    int iRow  = pt.y/nHei;


    WORD startcol ,endrow, endcol;
    startcol = SCalendarCore::GetDayOfWeek(m_iYear, m_iMonth, 1);
    endcol   = SCalendarCore::GetDayOfWeek(m_iYear, m_iMonth, SCalendarCore::GetDaysOfMonth(m_iYear,m_iMonth));

    endrow   = (SCalendarCore::GetDaysOfMonth(m_iYear, m_iMonth) + startcol -1)/7 ;
    if(iRow == 0 && iCol < startcol || iRow == endrow && iCol > endcol ||  iRow > endrow)
        return 0;
    return iRow *7 + iCol + 1 - startcol ;
}

void SCalendar::OnLButtonDown(UINT nFlags, CPoint point) 
{
    __super::OnLButtonDown(nFlags,point);
    WORD day = HitTest(point);
    if(day !=0 && day != m_iDay)
    {
        EventCalendarSelDay evt(this);
        evt.wOldDay=m_iDay;
        evt.wNewDay=day;

        FireEvent(evt);
    }
    m_iDay = day;
    Invalidate();
}

void SCalendar::OnMouseMove( UINT nFlags,CPoint pt )
{
    int iDay=HitTest(pt);
    if(iDay!=m_iHoverDay)
    {
        WORD oldHover=m_iHoverDay;
        m_iHoverDay=iDay;
        if(m_pDaySkin)
        {
            if(oldHover!=0) RedrawDay(oldHover);
            if(m_iHoverDay!=0) RedrawDay(m_iHoverDay);
        }
    }
}

void SCalendar::OnMouseLeave()
{
    if(m_iHoverDay!=0)
    {
        WORD oldHover=m_iHoverDay;
        m_iHoverDay=0;
        if(m_pDaySkin) RedrawDay(oldHover);
    }
}

BOOL SCalendar::InitFromXml( pugi::xml_node xmlNode )
{
    BOOL bLoad=__super::InitFromXml(xmlNode);
    if(bLoad)
    {
        SWindow *pBtnToday=FindChildByName(L"btn_today");
        if(pBtnToday)
        {
            pBtnToday->SetID(100);
            pBtnToday->GetEventSet()->subscribeEvent(EventCmd::EventID,Subscriber(&SCalendar::OnTodayClick,this));
        }
        SWindow *pLabelToday=FindChildByName(L"label_today");
        if(pLabelToday)
        {
            CTime today=CTime::GetCurrentTime();
            SStringT str;
            str.Format(_T("%04d-%02d-%02d"),today.GetYear(),today.GetMonth(),today.GetDay());
            pLabelToday->SetWindowText(str);
        }
    }
    return bLoad;
}

bool SCalendar::OnTodayClick( EventArgs *pArg)
{
    CTime today=CTime::GetCurrentTime();
    SetDate(today.GetYear(),today.GetMonth(),today.GetDay());
    Invalidate();
    return true;
}

}//end of namespace
