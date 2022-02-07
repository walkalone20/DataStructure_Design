#ifndef INIT_H
#define INIT_H

#include "head.hpp"
#include "records.hpp"
#include "cards.hpp"
#include "windows.hpp"
#include "students.hpp"
#include "trie.hpp"

extern const int MAX_WINDOWS_SIZE;
extern const int card_start_dfn;
extern windows windows_que[];
extern student stu[];
extern int card_dfn;
extern int student_dfn; // student inner id
extern int shopping_dfn;// shopping record number
extern map<int, int> card_to_student_ID; // card id mapping to students
extern map<int, int> ID_to_inner_ID;
extern trie student_name;
int trans_dfn_to_id(int x)
{
    int num = 3;
    int temp = x;
    while(temp)
    {
        num += temp % 10;
        temp /= 10;
    }
    int last = 9 - num % 10;
    return 3000000 + x * 10 + last;
}
string char_to_string(char *a)
{
    int len = strlen(a);
    string ans = "";
    for (int i = 0; i < len;i++)
        ans += a[i];
    return ans;
}
bool create_account()//read the create_account file to initialize
{
    time_clock start_time(2021, 9, 1, 0, 0, 0, 0); // set the time for creating account
    FILE *fp = fopen("kh001.txt", "r");
    char buf[1000];
    fscanf(fp, "%s", buf);
    if (strcmp(buf, "KH"))
    {
        fclose(fp);
        return 0;
    }
    int UID;
    string name;
    while (fscanf(fp, "%d,", &UID) != EOF)
    {
        fscanf(fp, "%s", buf);
        name = char_to_string(buf);
        name.pop_back();
        card_dfn++;
        student now_student(name, UID);
        int real_card_id = trans_dfn_to_id(card_dfn);
        now_student.create_card(real_card_id, start_time);
        stu[student_dfn] = now_student;
        card_to_student_ID[real_card_id] = student_dfn;
        ID_to_inner_ID[UID] = student_dfn;
        student_name.insert(name, UID);
        student_dfn++;
    }
    fclose(fp);
    return 1;
}
void delete_account(int UID)// delete an account from this system
{
    ID_to_inner_ID.erase(UID);
}
bool initialize_windows()
{
    FILE *now = fopen("wz003.txt", "r");
    string check;
    char buf[1000];
    fscanf(now, "%s", buf);
    if (strcmp(buf, "WZ"))
    {
        fclose(now);
        return 0;
    }
    int id, place;
    for (int i = 1; i <= MAX_WINDOWS_SIZE;i++)
    {
        fscanf(now, "%d,%d;", &id, &place);
        windows_que[id] = windows(id, place);
    }
    fclose(now);
    return 1;
}
const int operation_name_head[5] = {-71, -67, -77, -49, -78};
//��ʧ:-71 -46 -54 -89 ���??:-67 -30 -71 -46 ��ֵ:-77 -28 -42 -75, ����:-49 -6 -69 -89, ����:-78 -71 -65 -88
int string_to_i(string a)
{
    int ans = 0;
    for (auto i : a)
        ans = ans * 10 + i - 48;
    return ans;
}
string to_string(char *a)
{
    string ans = "";
    int len = strlen(a);
    for (int i = 0; i < len;i++)
        ans += a[i];
    return ans;
}
struct req
{
    time_clock time;
    int op_head;
    int UID;
    int card_id;
    double money;
    int windows_id;
    req() = default;
    req(time_clock time, int op_head, int UID, int card_id, double money = 0, int windows_id = 0)
    {
        this->time = time;
        this->op_head = op_head;
        this->UID = UID;
        this->card_id = card_id;
        this->money = money;
        this->windows_id = windows_id;
    }
    bool operator <(req &b)const
    {
        return time < b.time;
    }
};
double string_to_double(string s)
{
    double ratio = 0.1;
    double ans = 0;
    bool flag = 0;
    for (size_t i = 0; i < s.length();i++)
    {
        if(s[i]=='.')
        {
            flag = 1;
            continue;
        }
        if(!flag)
            ans = ans * 10 + s[i] - 48;
        else
        {
            ans += (s[i] - 48) * ratio;
            ratio /= 10;
        }
    }
    return ans;
}
bool do_operation()
{
    FILE *cz = fopen("cz002.txt", "r");
    FILE *xf = fopen("xf014.txt", "r");
    char buf1[1005], buf2[1005];
    fscanf(cz, "%s", buf1);
    fscanf(xf, "%s", buf2);
    if (strcmp(buf1, "CZ") || strcmp(buf2, "XF"))
    {
        fclose(cz);
        fclose(xf);
        return 0;
    }
    FILE *fp2 = fopen("Student_operation_log.txt", "a");
    FILE *fp1 = fopen("Records.txt", "a");
    FILE *fp3 = fopen("Student_consumption_log.txt", "a");
    string now_op;
    int item = 0;
    time_clock nowtime;
    vector<req> all;
    while (fscanf(cz, "%04d", &(nowtime.val[0])) != EOF)
    {    
        for (int i = 1; i <= 6; i++)
            fscanf(cz, "%02d", &(nowtime.val[i]));
        fscanf(cz, ",%s", buf1);
        now_op = to_string(buf1);
        string op = now_op.substr(0, 4);
        string ID_string = now_op.substr(5, 10); //学号长度固定为10
        int ID = string_to_i(ID_string);
        if (ID_to_inner_ID.count(ID) == 0)
            continue;
        double money = 0;
        if(now_op.length()>16)
        {
            string str_money = now_op.substr(16);
            str_money.pop_back();
            money = string_to_double(str_money);
        }
        all.emplace_back(nowtime, now_op[0], ID, 0, money);
        item++;
        if (item % 100000 == 0)
            printf("已完成%d条记录读取\n", item);
    }
    int now_windows_id = 0;
    while (fscanf(xf, "%s", buf2) != EOF)
    {
        string info = char_to_string(buf2);
        if (info[0] == 'W')
        {
            now_windows_id = string_to_i(info.substr(1));
            continue;
        }
        item++;
        if (item % 100000 == 0)
            printf("已完成%d条记录读取\n", item);
        time_clock now_time;
        string card_id_str = info.substr(0, 7);
        string time_upper = info.substr(8, 8);
        string time_lower = info.substr(17, 8);
        string money_str = info.substr(26);
        int card_id = string_to_i(card_id_str);
        money_str.pop_back();
        now_time.val[0] = string_to_i(time_upper.substr(0, 4));
        now_time.val[1] = string_to_i(time_upper.substr(4, 2));
        now_time.val[2] = string_to_i(time_upper.substr(6, 2));
        now_time.val[3] = string_to_i(time_lower.substr(0, 2));
        now_time.val[4] = string_to_i(time_lower.substr(2, 2));
        now_time.val[5] = string_to_i(time_lower.substr(4, 2));
        now_time.val[6] = string_to_i(time_lower.substr(6, 2));
        double money = string_to_double(money_str);
        if (card_to_student_ID.count(card_id) == 0)
            continue;
        all.emplace_back(now_time, -69, 0, card_id, money, now_windows_id);
    }
    item = 0;
    sort(all.begin(), all.end());
    for (auto i : all)
    {
        switch (i.op_head)
        {
            case -78:
            {
                int now_id = ID_to_inner_ID[i.UID];
                assert(now_id <= 10000);
                card_to_student_ID.erase(stu[now_id].nowcard.ID);
                card_dfn++;
                int now_card_id = trans_dfn_to_id(card_dfn);
                stu[now_id].create_card(now_card_id, nowtime, fp2);
                card_to_student_ID[now_card_id] = now_id;
                break;
            }
            
            case -77:
            {
                int now_id = ID_to_inner_ID[i.UID];
                assert(now_id <= 10000);
                stu[now_id].charge(i.money, i.time, fp2);
                break;
            }
            case -71:
            {
                int now_id = ID_to_inner_ID[i.UID];
                assert(now_id <= 10000);
                bool res = stu[now_id].lose_card(i.time, fp2);
                if (res)
                    card_to_student_ID.erase(stu[now_id].nowcard.ID);
                break;
            }
            case -69:
            {
                int stu_id = card_to_student_ID[i.card_id];
                assert(stu_id <= 10000);
                shopping_dfn++;
                stu[stu_id].consume(shopping_dfn, i.time, i.windows_id, i.money, 0, fp3, fp1);
                break;
            }
            case -67:
            {
                int now_id = ID_to_inner_ID[i.UID];
                assert(now_id <= 10000);
                bool res = stu[now_id].restore_card(i.time, fp2);
                if (res)
                    card_to_student_ID[stu[now_id].nowcard.ID] = now_id;
                break;
            }
            case -49:
                delete_account(i.UID);
                break;
            default:
                printf("Invalid operation %d\n", i.op_head);
                break;
        }
        item++;
        if (item % 100000 == 0)
            printf("完成%d条数据写入\n", item);
    }
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(cz);
    fclose(xf);
    return 1;
}
#endif
