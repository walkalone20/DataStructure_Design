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
        printf("%04d", this->val[0]);
        for (int i = 1; i <= 6;i++)
            printf("%02d", this->val[i]);
    }
    void foutput(char *filename)
    {
        FILE *fp = fopen(filename, "a");
        fprintf(fp, "%04d ", this->val[0]);
        for (int i = 1; i <= 6;i++)
            fprintf(fp, "%02d ", this->val[i]);
        fclose(fp);
    }
    bool operator <(const time_clock &b)const
    {
        for (int i = 0; i <= 6;i++)
            if (this->val[i] != b.val[i])
                return this->val[i] < b.val[i];
        return 0;
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
int trans_to_part(time_clock time)//??????????????????????????????
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
    ans *= 3;//??????????????????
    if (time.val[3] <= 9)
        return ans + 1;
    if (time.val[3] <= 13)
        return ans + 2;
    else
        return ans + 3;
}
class card
{
    const string default_password = "8888";
    const int max_times = 3;//??????????????????????????????????????????????????????????????????
    const double MAX_BALANCE = 1000;//????????????????????????????????????????????????
    const double MAX_LIMIT = 999.99;//??????????????????
    string password;//????????????

public:
    bool operator <(const card &b)const
    {
        return ID < b.ID;
    }
    const double usage_limit = 20;//?????????????????????????????????
    bool available;//???????????????????????????1???????????????
    bool is_suspended;//????????????????????????????????????????????????
    int ID;//??????
    int owner_ID;//?????????????????????????????????
    double balance;//??????
    card() = default;
    card& operator =(const card &s)
    {
        if(this!=&s)
        {
            this->available = s.available;
            this->is_suspended = s.is_suspended;
            this->ID = s.ID;
            this->owner_ID = s.owner_ID;
            this->balance = s.balance;
        }
        return *this;
    }
    card(int ID, int student_ID)
    {
        this->available = 1;//????????????????????????????????????
        this->owner_ID = student_ID;
        this->is_suspended = 0;
        this->ID = ID;
        this->balance = 0;//???????????????0
        this->password = default_password;
    }
    bool change_password()
    {
        string new_password;
        printf("Input the new password\n");
        int times = 0;
        do
        {
            if(times)
                cout << "Same as the old password. Please try a new one.\n";
            cin >> new_password;
            times++;
        } while (new_password == password && times <= max_times);
        if (times > max_times)
        {
            printf("Fail to change the password\n");
            return 0;//??????
        }
        else
        {
            this->password = new_password;
            return 1;
        }
    }
    bool check_password(string &a)//???????????????????????????????????????
    {
        return a == password;
    }
    bool verify(bool status)
    {
        if(status)
        {
            printf("Please input your password\n");
            string input;
            int times = 0;
            do
            {
                if(times)
                    printf("Wrong password.Please input angin\n");
                cin >> input;
            } while (input != this->password && times <= max_times);
            return input == this->password;//??????????????????
        }
        else
            return 1;
    }
    bool charge(double money)
    {
        if(balance>MAX_BALANCE)
            return 0;
        if (money + balance <= MAX_LIMIT)
        {
            balance += money;
            return 1;
        }
        else
            return 0;
    }
    bool consume(double money)
    {
        if (money > balance)
        {
            // printf("No enough balance in this card.\n");
            return 0;//????????????
        }
        else
            return 1;//????????????
    }
};
class records
{
    const long long base = 65537, mod = 1000000009;

public:
    int id;//??????????????????
    int windows_id;//??????????????????
    int consumer_UID;//????????????
    double money;//????????????
    time_clock time;//????????????
    int time_part;//????????????
    long long my_hash;//?????????
    bool operator <(const records &b)const
    {
        return time < b.time;
    }
    records() = default;
    records(const records &s)
    {
        this->id = s.id;
        this->windows_id = s.id;
        this->consumer_UID = s.consumer_UID;
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
            this->windows_id = s.id;
            this->consumer_UID = s.consumer_UID;
            this->money = s.money;
            this->time = s.time;
            this->time_part = s.time_part;
            this->my_hash = s.my_hash;
        }
        return *this;
    }
    records(int ID, int s_UID, double money, time_clock time, int windows_id)
    {
        this->id = ID;
        this->consumer_UID = s_UID;
        this->money = money;
        this->time = time;
        this->windows_id = windows_id;
        this->time_part = trans_to_part(time);
        this->get_hash();
    }
    void foutput(char *filename)
    {
        FILE *fp = fopen(filename, "a");
        fprintf(fp, "??????:%d ??????:%d ??????:%d ??????:", id, consumer_UID, windows_id);
        time.foutput(filename);
        fprintf(fp, " ??????:%.2lf ?????????:%d\n", money, my_hash);
        fclose(fp);
    }
    void output()
    {
        printf("ID:%d UID:%d Windows ID:%d Time:", id, consumer_UID, windows_id);
        time.output();
        printf("Money:%.2lf\n", money);
    }
    long long cal_hash()
    {
        vector<int> val;
        val.push_back(time.get_hash());
        val.push_back(id);
        val.push_back(consumer_UID);
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
class windows
{
    const int SIZE = 60000;//????????????
    int nx(int x)
    {
        return (x + 1) % SIZE;
    }
    int pre(int x)
    {
        return (x - 1 + SIZE) % SIZE;
    }

public:
    int id;//??????????????????
    vector<records> que;
    int nowplace;//???????????????????????????????????????
    double tot_money;//??????????????????
    int old_cnt;//?????????????????????
    int tot_cnt;//???????????????
    double today_money;
    int today_cnt;
    long long xor_hash;//????????????
    vector<long long> record_hash;//????????????????????????????????????
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
        if (tot_cnt > old_cnt && (que[pre(nowplace)].time_part - 1) % 3 != (now.time_part - 1) % 3)
            refresh();
        tot_cnt++;
        tot_money += now.money;
        today_cnt++;
        today_money += now.money;
        if (que[nowplace].my_hash)
            this->xor_hash ^= que[nowplace].my_hash;
        que[nowplace] = now;
        record_hash[nowplace] = que[nowplace].my_hash ^ que[pre(nowplace)].my_hash;
        nowplace = nx(nowplace);
    }
    void output_tot()
    {
        for(auto i:que)
            i.output();
    }
    void output_today()
    {
        if(!today_cnt)
        {
            printf("Today's no items");
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
    int verify()//?????????????????????????????????????????????-1??????????????????????????????????????????
    {
        for (int i = 0; i < SIZE;i++)
            if (que[i].cal_hash() != que[i].my_hash)
                return i;
        for (int i = 0; i < SIZE;i++)
            if (record_hash[i] != (que[i].my_hash ^ que[pre(i)].my_hash))
                return i;
        return -1;
    }
    // vector<records> select(time_clock start, time_clock end = time_inf)
    // {

    // }
};
const string operation_name[5] = {"??????", "??????", "??????", "????????????", "??????"};
struct card_operation
{
    int op; //??????????????????????????? 1 ??????;2 ?????? ;3 ??????; 4 ?????????
    int card_id;
    time_clock time;
    double money;
    bool res; //???????????? 0?????????????????????1??????????????????
    card_operation() = default;
    card_operation(const card_operation &s)
    {
        this->op = s.op;
        this->card_id = s.card_id;
        this->time = s.time;
        this->money = s.money;
        this->res = s.res;
    }
    card_operation(int op, int card_id, time_clock time, bool res, double money = 0)
    {
        this->op = op;
        this->card_id = card_id;
        this->time = time;
        this->res = res;
        this->money = money;
    }
    card_operation& operator=(const card_operation &s)
    {
        if(this!=&s)
        {
            this->op = s.op;
            this->card_id = s.card_id;
            this->time = s.time;
            this->res = s.res;
            this->money = s.money;
        }
        return *this;
    }
    bool operator <(const card_operation &b)const
    {
        return time < b.time;
    }
    void output()
    {
        cout << "??????:" << operation_name[op] << " "
             << "??????:" << card_id << " ??????: ";
        time.output();
        cout << " ?????? " << operation_name[res] << endl;
    }
    void foutput(const char *student_name, char *filename)
    {
        FILE *fp = fopen(filename, "a");
        fprintf(fp, "????????????:%s ??????:%d ", student_name, card_id);
        this->time.foutput(filename);
        fprintf(fp, " ??????:%s", operation_name[op].c_str());
        if(op==4)
            fprintf(fp, " ??????:%lf ??????:%d\n", money, res);
        else
            fprintf(fp, " ??????:%d\n", res);
        fclose(fp);
    }
};
extern windows windows_que[];
string shopping_res_name[6] = {"", "??????", "?????????", "????????????", "????????????", "??????????????????"};
struct shopping_request
{
    time_clock time; //????????????
    int windows_id;  //????????????
    double money;    //????????????
    int res;         //???????????????1????????????2???????????????3??????????????????4??????????????????5?????????????????????
    shopping_request() = default;
    shopping_request(time_clock time, int windows_id, double money, int res)
    {
        this->time = time;
        this->windows_id = windows_id;
        this->res = res;
        this->money = money;
    }
    bool operator <(const shopping_request &b)const
    {
        return this->time < b.time;
    }
    void output()
    {
        time.output();
        printf(" ??????:%d ??????:%lf ??????:%s\n", windows_id, money, shopping_res_name[res].c_str());
    }
    void foutput(const char *student_name, int card_id,char *filename)
    {
        FILE *fp = fopen(filename, "a");
        fprintf(fp, "????????????:%s ??????:%d ", student_name, card_id);
        this->time.foutput(filename);
        fprintf(fp, " ??????:%d ??????:%lf ??????:%s\n", windows_id, money, shopping_res_name[res].c_str());
        fclose(fp);
    }
};
class student
{
    std::vector<records> total;//???????????????
    double total_expenditure;//???????????????
    double part_expenditure;//???????????????????????????

public:
    std::vector<shopping_request> request;
    std::vector<card_operation> card_op;
    std::string name;
    int UID;//??????
    bool alive;//?????????????????? 1 ??????????????????0??????????????????
    card nowcard;
    student() = default;
    student(const student &s)
    {
        this->total = s.total;
        this->total_expenditure = s.total_expenditure;
        this->part_expenditure = s.part_expenditure;
        this->request = s.request;
        this->card_op = s.card_op;
        this->name = s.name;
        this->UID = s.UID;
        this->alive = s.alive;
        this->nowcard = s.nowcard;
    }
    student& operator=(const student &s)
    {
        if(this!=&s)
        {
            this->total = s.total;
            this->total_expenditure = s.total_expenditure;
            this->part_expenditure = s.part_expenditure;
            this->request = s.request;
            this->card_op = s.card_op;
            this->name = s.name;
            this->UID = s.UID;
            this->alive = s.alive;
            this->nowcard = s.nowcard;
        }
        return *this;
    }
    student(string &name, int UID)
    {
        this->alive=1;
        this->name = name;
        this->UID = UID;
        this->nowcard.ID = 0;//??????????????????
        this->part_expenditure = 0;
        this->total_expenditure = 0;
    }
    bool charge(double money, time_clock time)//????????????????????????????????????????????????
    {
        bool res = 0;
        if(nowcard.charge(money))
            res = 1; //????????????
        else
            res = 0; //????????????
        card_operation temp(4, this->nowcard.ID, time, res, money);
        temp.foutput(this->name.c_str(), "Student_card_operation_log.txt");
        this->card_op.emplace_back(temp);
        return res;
    }
    void create_record(int ID, time_clock time,int windows_id,double money)
    {
        records now(ID, this->UID, money, time, windows_id);
        now.foutput("Records_log.txt");
        total.push_back(now);
        total_expenditure += money;
    }
    bool consume(int temp_id, time_clock time, int windows_id, double money,bool status)
    {
        if (nowcard.available == 0)
        {
            shopping_request temp(time, windows_id, money, 2);
            temp.foutput((this->name).c_str(), this->nowcard.ID, "Student_consumption_log.txt");
            this->request.emplace_back(temp);
            return 0;//?????????????????????????????????
        }
        if (!nowcard.consume(money))
        {
            shopping_request temp(time, windows_id, money, 5);
            temp.foutput((this->name).c_str(), this->nowcard.ID, "Student_consumption_log.txt");
            this->request.emplace_back(temp);
            return 0; //?????????????????????????????????
        }
        int now_time_part = trans_to_part(time);
        if (total.empty() || total.back().time_part != now_time_part)
        {
            this->part_expenditure = 0;
            this->nowcard.is_suspended = 0;
        }
        if(this->nowcard.is_suspended)
        {
            shopping_request temp(time, windows_id, money, 4);
            temp.foutput((this->name).c_str(), this->nowcard.ID, "Student_consumption_log.txt");
            this->request.emplace_back(time, windows_id, money, 4);
            return 0;
        }
        if (this->part_expenditure + money >= nowcard.usage_limit)
        {
            if(this->nowcard.verify(status))
            {
                shopping_request temp(time, windows_id, money, 1);
                temp.foutput((this->name).c_str(), this->nowcard.ID, "Student_consumption_log.txt");
                this->request.emplace_back(temp);
                this->nowcard.balance -= money;
                this->create_record(temp_id, time, windows_id, money);
                this->part_expenditure = 0;
            }
            else
            {
                shopping_request temp(time, windows_id, money, 3);
                temp.foutput((this->name).c_str(), this->nowcard.ID, "Student_consumption_log.txt");
                this->request.emplace_back(temp);
                this->nowcard.is_suspended = 1;//??????????????????
                return 0;
            }
        }
        else
        {
            shopping_request temp(time, windows_id, money, 1);
            temp.foutput((this->name).c_str(), this->nowcard.ID, "Student_consumption_log.txt");
            this->request.emplace_back(temp);
            this->create_record(temp_id, time, windows_id, money);
            this->part_expenditure += money;
            this->nowcard.balance -= money;
        }
        return 1;//????????????
    }
    bool change_password(time_clock time)
    {
        bool res = nowcard.change_password();
        card_operation temp(3, nowcard.ID, time, res);
        temp.foutput(this->name.c_str(), "Student_card_operation_log.txt");
        this->card_op.emplace_back(temp);
        return res;
    }
    bool create_card(int ID, time_clock time)//??????????????????
    {
        card now(ID, this->UID);
        nowcard = now;
        card_operation temp(0, nowcard.ID, time, 1);
        temp.foutput(this->name.c_str(), "Student_card_operation_log.txt");
        this->card_op.emplace_back(temp);
        return 1;
    }
    bool lose_card(time_clock time)//????????????????????????
    {
        if(!nowcard.available)
        {
            card_operation temp(1, nowcard.ID, time, 0);
            temp.foutput(this->name.c_str(), "Student_card_operation_log.txt");
            this->card_op.emplace_back(temp);
            return 0;
        }
        this->nowcard.available = 0;
        card_operation temp(1, nowcard.ID, time, 1);
        temp.foutput(this->name.c_str(), "Student_card_operation_log.txt");
        this->card_op.emplace_back(temp);
        return 1;
    }
    bool restore_card(time_clock time)//?????????????????????????????????
    {
        if (this->nowcard.available)
        {
            card_operation temp(2, nowcard.ID, time, 0);
            temp.foutput(this->name.c_str(), "Student_card_operation_log.txt");
            this->card_op.emplace_back(temp);
            //?????????????????????????????????????????????
            return 0;
        }
        this->nowcard.available = 1;
        card_operation temp(2, nowcard.ID, time, 1);
        temp.foutput(this->name.c_str(), "Student_card_operation_log.txt");
        this->card_op.emplace_back(temp);
        return 1;
    }
    std::vector<records> output_consume()
    {
        return this->total;
    }
};
#endif // SYSTEM_HEAD_H
