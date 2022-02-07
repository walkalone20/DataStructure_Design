#ifndef STUDENT_HEAD_H
#define STUDENT_HEAD_H

#include "head.hpp"
#include "List.hpp"
#include "cards.hpp"
#include "windows.hpp"
#include "records.hpp"
const string operation_name[5] = {"发卡", "挂失", "解挂", "更改密码", "充值"};
struct card_operation
{
    int op; //对当前卡有什么操作 1 创建;2 挂失 ;3 解挂; 4 改密码
    int card_id;
    time_clock time;
    double money;
    bool res; //操作结果 0表示操作失败，1表示操作成功
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
        cout << "操作:" << operation_name[op] << " "
             << "卡号:" << card_id << " 时间: ";
        time.output();
        cout << " 结果 " << operation_name[res] << endl;
    }
    void foutput(FILE *fp, const char *student_name)
    {
        fprintf(fp, "学生姓名:%s 卡号:%d ", student_name, card_id);
        this->time.foutput(fp);
        fprintf(fp, " 操作:%s", operation_name[op].c_str());
        if(op==4)
            fprintf(fp, " 金额:%lf 结果:%d\n", money, res);
        else
            fprintf(fp, " 结果:%d\n", res);
    }
};
extern windows windows_que[];
string shopping_res_name[6] = {"", "成功", "卡挂失", "密码错误", "暂时注销", "卡中余额不足"};
struct shopping_request
{
    time_clock time; //消费时间
    int windows_id;  //消费窗口
    double money;    //消费金额
    int res;         //消费结果：1：成功，2：卡挂失，3：密码错误，4：暂时注销，5：卡中余额不足
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
        printf(" 窗口:%d 金额:%lf 结果:%s\n", windows_id, money, shopping_res_name[res].c_str());
    }
    void foutput(FILE *fp, const char *student_name, int card_id)
    {
        fprintf(fp, "学生姓名:%s 卡号:%d ", student_name, card_id);
        this->time.foutput(fp);
        fprintf(fp, " 窗口:%d 金额:%lf 结果:%s\n", windows_id, money, shopping_res_name[res].c_str());
    }
};
class student
{
    std::vector<records> total;//总消费记录
    double total_expenditure;//总消费金额
    double part_expenditure;//最后时段的消费金额

public:
    std::vector<shopping_request> request;
    std::vector<card_operation> card_op;
    std::string name;
    int UID;//学号
    bool alive;//是否活跃状态 1 表示可操作，0表示已经销户
    card nowcard;
    List<card> oldcard;
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
        this->nowcard.ID = 0;//默认初始无卡
        this->part_expenditure = 0;
        this->total_expenditure = 0;
    }
    bool charge(double money, time_clock time, FILE *fp = nullptr) //充值：只能给当前激活状态的卡充值
    {
        bool res = 0;
        if(nowcard.charge(money))
            res = 1; //充值成功
        else
            res = 0; //充值失败
        card_operation temp(4, this->nowcard.ID, time, res, money);
        if (fp != nullptr)
            temp.foutput(fp, this->name.c_str());
        this->card_op.emplace_back(temp);
        return res;
    }
    void create_record(int ID, time_clock time, int windows_id, double money, FILE *fp)
    {
        // printf("COMSUME: CARDID:%d WINDOWS_ID:%d\n", ID, windows_id);
        records now(ID, this->nowcard.ID, money, time, windows_id);
        now.foutput(fp);
        windows_que[windows_id].create_records(now);
        total.push_back(now);
        total_expenditure += money;
    }
    bool consume(int temp_id, time_clock time, int windows_id, double money, bool status, FILE *fp = nullptr)
    // 消费序列号；时间；窗口；金额；状态（是否要输入密码）
    {
        if (nowcard.available == 0)
        {
            shopping_request temp(time, windows_id, money, 2);
            // temp.foutput((this->name).c_str(), this->nowcard.ID, "Student_consumption_log.txt");
            this->request.emplace_back(temp);
            return 0;//当前卡被挂失，无法使用
        }
        if (!nowcard.consume(money))
        {
            shopping_request temp(time, windows_id, money, 5);
            // temp.foutput((this->name).c_str(), this->nowcard.ID, "Student_consumption_log.txt");
            this->request.emplace_back(temp);
            return 0; //卡中余额不足，不能消费
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
            // temp.foutput((this->name).c_str(), this->nowcard.ID, "Student_consumption_log.txt");
            this->request.emplace_back(time, windows_id, money, 4);
            return 0;
        }
        if (this->part_expenditure + money >= nowcard.usage_limit)
        {
            if(this->nowcard.verify(status))
            {
                shopping_request temp(time, windows_id, money, 1);
                // temp.foutput((this->name).c_str(), this->nowcard.ID, "Student_consumption_log.txt");
                this->request.emplace_back(temp);
                this->nowcard.balance -= money;
                if (fp != nullptr)
                    this->create_record(temp_id, time, windows_id, money, fp);
                this->part_expenditure = 0;
            }
            else
            {
                shopping_request temp(time, windows_id, money, 3);
                // temp.foutput((this->name).c_str(), this->nowcard.ID, "Student_consumption_log.txt");
                this->request.emplace_back(temp);
                this->nowcard.is_suspended = 1;//触发错误过多
                return 0;
            }
        }
        else
        {
            shopping_request temp(time, windows_id, money, 1);
            // temp.foutput((this->name).c_str(), this->nowcard.ID, "Student_consumption_log.txt");
            this->request.emplace_back(temp);
            if (fp != nullptr)
                this->create_record(temp_id, time, windows_id, money, fp);
            this->part_expenditure += money;
            this->nowcard.balance -= money;
        }
        return 1;//成功消费
    }
    bool change_password(time_clock time)
    {
        bool res = nowcard.change_password();
        card_operation temp(3, nowcard.ID, time, res);
        // temp.foutput(this->name.c_str(), "Student_card_operation_log.txt");
        this->card_op.emplace_back(temp);
        return res;
    }
    bool create_card(int ID, time_clock time, FILE *fp = nullptr) //创建一张新卡
    {
        card now(ID, this->UID);
        oldcard.insert(nowcard);
        nowcard = now;
        card_operation temp(0, nowcard.ID, time, 1);
        if (fp != nullptr)
            temp.foutput(fp, this->name.c_str());
        this->card_op.emplace_back(temp);
        return 1;
    }
    bool lose_card(time_clock time, FILE *fp = nullptr) //挂失当前的学生卡
    {
        if(!nowcard.available)
        {
            card_operation temp(1, nowcard.ID, time, 0);
            if (fp != nullptr)
                temp.foutput(fp, this->name.c_str());
            this->card_op.emplace_back(temp);
            return 0;
        }
        this->nowcard.available = 0;
        card_operation temp(1, nowcard.ID, time, 1);
        if (fp != nullptr)
                temp.foutput(fp, this->name.c_str());
        this->card_op.emplace_back(temp);
        return 1;
    }
    bool restore_card(time_clock time, FILE *fp = nullptr) //将一张已经挂失的卡解挂
    {
        if (this->nowcard.available)
        {
            card_operation temp(2, nowcard.ID, time, 0);
            if (fp != nullptr)
                temp.foutput(fp, this->name.c_str());
            this->card_op.emplace_back(temp);
            //当前卡处于激活状态无法办理解挂
            return 0;
        }
        this->nowcard.available = 1;
        card_operation temp(2, nowcard.ID, time, 1);
        if (fp != nullptr)
            temp.foutput(fp, this->name.c_str());
        this->card_op.emplace_back(temp);
        return 1;
    }
    std::vector<records> output_consume()
    {
        return this->total;
    }
};
#endif
