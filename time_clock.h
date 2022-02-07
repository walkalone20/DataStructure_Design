#ifndef SYSTEM_HEAD_H
#define SYSTEM_HEAD_H

#include <bits/stdc++.h>
using namespace std;
const int inf = 0x3f3f3f3f;
struct time_clock
{
    const long long hash_base = 2051, hash_mod = 1000000007;
    int val[7];
    //0:year 1:month 2:day 3:hour 4:minute 5:second 6:m_second
    time_clock() = default;
    time_clock(int year, int month, int day, int hour, int minute, int second, int m_second)
    {
        val[0] = year;
        val[1] = month;
        val[2] = day;
        val[3] = hour;
        val[4] = minute;
        val[5] = second;
        val[6] = m_second;
    }
    time_clock(const time_clock &s)
    {
        for (int i = 0; i <= 6;i++)
            this->val[i] = s.val[i];
    }
    time_clock& operator =(const time_clock& s)
    {
        if(this!=&s)
            for (int i = 0; i <= 6;i++)
                this->val[i] = s.val[i];
        return *this;
    }
    void output()
    {
        printf("%04d ", this->val[0]);
        for (int i = 1; i <= 6;i++)
            printf("%02d ", this->val[i]);
    }
    void foutput(FILE *fp)
    {
        fprintf(fp, "%04d ", this->val[0]);
        for (int i = 1; i <= 6;i++)
            fprintf(fp, "%02d ", this->val[i]);
    }
    bool operator ==(const time_clock &b)const
    {
        for (int i = 0; i <= 6;i++)
            if (this->val[i] != b.val[i])
                return 0;
        return 1;
    }
    bool operator <(const time_clock &b)const
    {
        for (int i = 0; i <= 6;i++)
            if (this->val[i] != b.val[i])
                return this->val[i] < b.val[i];
        return 0;
    }
    bool operator <=(const time_clock &b)const
    {
        return *this == b || *this < b;
    }
    bool operator >=(const time_clock &b)const
    {
        return *this == b || !(*this < b);
    }
    int input()
    {
        int res = scanf("%04d", &this->val[0]);
        for (int i = 1; i <= 6;i++)
            scanf("%02d", &this->val[i]);
        return res;
    }
    long long get_hash()
    {
        long long ans = 0;
        for (int i = 0; i <= 6;i++)
            ans = (ans * hash_base % hash_mod + val[i]) % hash_mod;
        return ans;
    }
};
const time_clock time_inf(inf, inf, inf, inf, inf, inf, inf);
const int start_year = 2000;
const int non_leap_month_day[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const int leap_month_day[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
bool is_leap(int year)
{
    if (year % 4)
        return 0;
    if (year % 100 == 0 && year % 400)
        return 0;
    return 1;
}
int trans_to_part(time_clock time)//从时间转化到时间区块
{
    time.val[0] -= start_year;
    int ans = 0;
    for (int i = 0; i < time.val[0];i++)
        if (is_leap(i + start_year))
            ans += 366;
        else
            ans += 365;
    if (is_leap(time.val[0]))
        for (int i = 1; i < time.val[1]; i++)
            ans += leap_month_day[i];
    else
        for (int i = 1; i < time.val[1]; i++)
            ans += non_leap_month_day[i];
    ans += time.val[2];
    ans *= 3;//一天三个时段
    if (time.val[3] <= 9)
        return ans + 1;
    if (time.val[3] <= 13)
        return ans + 2;
    else
        return ans + 3;
}
#endif // SYSTEM_HEAD_H
