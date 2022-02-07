#ifndef WINDOWS_HEAD_H
#define WINDOWS_HEAD_H

#include "head.hpp"
#include "records.hpp"
class windows
{
    const int SIZE = 60000;//窗口容量
    int nx(int x)
    {
        return (x + 1) % SIZE;
    }
    int pre(int x)
    {
        return (x - 1 + SIZE) % SIZE;
    }

public:
    int id;//消费窗口编号
    vector<records> que;
    int nowplace;//记录当前即将写入的存储位置
    double tot_money;//总消费总金额
    int old_cnt;//之前的消费单数
    int tot_cnt;//总消费单数
    double today_money;
    int today_cnt;
    long long xor_hash;//校验需要
    vector<long long> record_hash;//记录两相邻哈希值用于判断
    windows(const windows &s)
    {
        this->id = s.id;
        this->que = s.que;
        this->nowplace = s.nowplace;
        this->tot_money = s.tot_money;
        this->old_cnt = s.old_cnt;
        this->tot_cnt = s.tot_cnt;
        this->today_money = s.today_money;
        this->today_cnt = s.today_cnt;
        this->xor_hash = s.xor_hash;
        this->record_hash = s.record_hash;
    }
    windows& operator=(const windows &s)
    {
        if(this!=&s)
        {
            this->id = s.id;
            this->que = s.que;
            this->nowplace = s.nowplace;
            this->tot_money = s.tot_money;
            this->old_cnt = s.old_cnt;
            this->tot_cnt = s.tot_cnt;
            this->today_money = s.today_money;
            this->today_cnt = s.today_cnt;
            this->xor_hash = s.xor_hash;
            this->record_hash = s.record_hash;
        }
        return *this;
    }
    windows() = default;
    windows(int id, int start_place)
    {
        this->nowplace = nx(start_place % SIZE);
        this->tot_cnt = this->old_cnt = start_place;
        this->tot_money = 0;
        this->id = id;
        this->xor_hash = 0;
        que.resize(SIZE);
        for (int i = 0; i < SIZE;i++)
            que[i].time_part = -1;
        this->record_hash.resize(SIZE);
    }
    void refresh()
    {
        today_cnt = 0;
        today_money = 0;
    }
    void create_records(records &now)
    {
        // printf("WINDOWS:INPUT SHOPPINGDFN:%d WINDOWSID:%d\n", now.card_ID, now.windows_id);
        if (tot_cnt > old_cnt && (que[pre(nowplace)].time_part - 1) % 3 != (now.time_part - 1) % 3)
            refresh();
        tot_cnt++;
        tot_money += now.money;
        today_cnt++;
        today_money += now.money;
        if (que[nowplace].my_hash)
            this->xor_hash ^= que[nowplace].my_hash;
        this->que[nowplace] = now;
        // printf("WINDOWS:INPUT SHOPPINGDFN:%d WINDOWSID:%d\n", this->que[nowplace].card_ID, this->que[nowplace].windows_id);
        record_hash[nowplace] = que[nowplace].my_hash ^ que[pre(nowplace)].my_hash;
        nowplace = nx(nowplace);
    }
    void output_tot()
    {
        for (auto i : this->que)
            i.output();
    }
    void output_today()
    {
        if(!today_cnt)
        {
            printf("Today's no items\n");
            return;
        }
        int place = pre(nowplace);
        for (int i = 1; i <= today_cnt;i++)
            place = pre(place);
        for (int i = 1; i <= today_cnt; i++, place = nx(place))
        {
            que[i].output();
            printf("\n");
        }
        return;
    }
    int verify()//检测是否有数据丢失或未知篡改，-1表示无修改，否则返回篡改地址
    {
        for (int i = 0; i < SIZE;i++)
            if (que[i].cal_hash() != que[i].my_hash)
                return i;
        for (int i = 0; i < SIZE;i++)
            if (record_hash[i] != (que[i].my_hash ^ que[pre(i)].my_hash))
                return i;
        return -1;
    }
    vector<records> select(time_clock start, time_clock end = time_inf)
    {
        vector<records> ans;
        for (int i = 0; i < SIZE;i++)
            if (que[i].time >= start && que[i].time <= end)
                ans.push_back(que[i]);
        return ans;
    }
};
#endif
