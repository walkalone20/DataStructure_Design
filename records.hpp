#ifndef RECORD_HEAD_H
#define RECORD_HEAD_H

#include "head.hpp"
class records
{
    const long long base = 65537, mod = 1000000009;

public:
    int id;//消费记录编号
    int windows_id;//消费窗口编号
    int card_ID;//消费卡号
    double money;//消费数额
    time_clock time;//消费时刻
    int time_part;//时间区块
    long long my_hash;//哈希值
    bool operator <(const records &b)const
    {
        return time < b.time;
    }
    records() = default;
    records(const records &s)
    {
        this->id = s.id;
        this->windows_id = s.windows_id;
        this->card_ID = s.card_ID;
        this->money = s.money;
        this->time = s.time;
        this->time_part = s.time_part;
        this->my_hash = s.my_hash;
    }
    records& operator =(const records& s)
    {
        if(this!=&s)
        {
            this->id = s.id;
            this->windows_id = s.windows_id;
            this->card_ID = s.card_ID;
            this->money = s.money;
            this->time = s.time;
            this->time_part = s.time_part;
            this->my_hash = s.my_hash;
        }
        return *this;
    }
    records(int ID, int card_ID, double money, time_clock time, int windows_id)
    {
        this->id = ID;
        this->card_ID = card_ID;
        this->money = money;
        this->time = time;
        this->windows_id = windows_id;
        this->time_part = trans_to_part(time);
        this->get_hash();
    }
    void output()
    {
        printf("消费序列号:%d 卡号:%d 窗口:%d 时间:", id, card_ID, windows_id);
        time.output();
        printf(" 金额:%.2lf\n", money);
    }
    void foutput(FILE *fp)
    {
        fprintf(fp, "消费序列号:%d 卡号:%d 窗口:%d 时间:", id, card_ID, windows_id);
        time.foutput(fp);
        fprintf(fp, " 金额:%.2lf 哈希值:%d\n", money, my_hash);
    }
    long long cal_hash()
    {
        vector<int> val;
        val.push_back(time.get_hash());
        val.push_back(id);
        val.push_back(card_ID);
        val.push_back(windows_id);
        val.push_back((int)(money * 100));
        val.push_back(time_part);
        long long ans = 0;
        for (auto i : val)
            ans = (ans * base % mod + i) % mod;
        this->my_hash = ans;
        return ans;
    }
    void get_hash()
    {
        this->my_hash = this->cal_hash();
    }
};
bool sort_by_greater_money(records a,records b)
{
    return a.money > b.money;
}
bool sort_by_less_money(records a,records b)
{
    return a.money < b.money;
}
#endif
