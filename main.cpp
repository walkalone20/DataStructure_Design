#include "head.hpp"
#include "records.hpp"
#include "cards.hpp"
#include "windows.hpp"
#include "students.hpp"
#include "trie.hpp"
const int MAX_STUDENT = 20000;
const int card_start_dfn = 12345;
const int MAX_WINDOWS_SIZE = 99;//maximum windows number
windows windows_que[MAX_WINDOWS_SIZE + 1];
student stu[MAX_STUDENT];
int card_dfn = card_start_dfn;
int student_dfn = 0; // student inner id
int shopping_dfn = 0;// shopping record number
map<int, int> card_to_student_ID; // card id mapping to students
map<int, int> ID_to_inner_ID;
trie student_name;
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
#define FILE_IN
#ifdef FILE_IN
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
bool do_operation()
{
    FILE *now = fopen("cz002.txt", "r");
    string check;
    char buf[1005];
    fscanf(now, "%s", buf);
    if (strcmp(buf, "CZ") != 0)
    {
        fclose(now);
        return 0;
    }
    FILE *fp2 = fopen("Student_operation_log.txt", "a");
    string now_op;
    time_clock nowtime;
    while (fscanf(now, "%04d", &(nowtime.val[0])) != EOF)
    {
        for (int i = 1; i <= 6; i++)
            fscanf(now, "%02d", &(nowtime.val[i]));
        fscanf(now, ",%s", buf);
        now_op = to_string(buf);
        string op = now_op.substr(0, 4);
        string ID_string = now_op.substr(5, 10); //学号长度固定为10
        int ID = string_to_i(ID_string);
        if (ID_to_inner_ID.count(ID) == 0)
        {
            // printf("No such student\n");
            continue;
        }
        int now_id = ID_to_inner_ID[ID];
        switch (op[0])
        {
            case -71:
            {
                bool res = stu[now_id].lose_card(nowtime, fp2);
                if (res)
                    card_to_student_ID.erase(stu[now_id].nowcard.ID);
                break;
            }
            case -67:
            {
                bool res = stu[now_id].restore_card(nowtime, fp2);
                if (res)
                    card_to_student_ID[stu[now_id].nowcard.ID] = now_id;
                break;
            }
            case -77:
            {
                now_op.pop_back();
                string charge_num = now_op.substr(16);
                int money = string_to_i(charge_num);
                stu[now_id].charge((double)money, nowtime, fp2);
                break;
            }
            case -49:
                delete_account(ID);
                break;
            case -78:
            {
                card_to_student_ID.erase(stu[now_id].nowcard.ID);
                card_dfn++;
                int now_card_id = trans_dfn_to_id(card_dfn);
                stu[now_id].create_card(now_card_id, nowtime, fp2);
                card_to_student_ID[now_card_id] = now_id;
                break;
            }
            default:
                printf("Invalid operation\n");
                break;
        }
    }
    fclose(fp2);
    fclose(now);
    return 1;
}
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
bool spending()//spending money
{
    FILE *now = fopen("xf014.txt", "r");
    char buf[1005];
    fscanf(now, "%s", buf);
    if (strcmp(buf, "XF") != 0)
    {
        fclose(now);
        return 0;
    }
    FILE *fp1 = fopen("Student_consumption_log.txt", "a");
    string info;
    int now_windows_id = 0;
    int item = 0;
    while (fscanf(now, "%s", buf) != EOF)
    {
        info = char_to_string(buf);
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
        int stu_id = card_to_student_ID[card_id];
        shopping_dfn++;
        stu[stu_id].consume(shopping_dfn, now_time, now_windows_id, money, 0, fp1);
    }
    fclose(fp1);
    fclose(now);
    return 1;
}
vector<records> merge(time_clock start, time_clock end, vector<int> &id)
{
    vector<records> info[100];
    for (auto i : id)
        info[i] = windows_que[i].select(start, end);
    int place[100] = {0};
    vector<records> ans;
    priority_queue<pair<records, int>, vector<pair<records, int>>, greater<pair<records, int>>> choose;
    for (int i = 1; i <= MAX_WINDOWS_SIZE;i++)
        if (place[i] != info[i].size())
            choose.emplace(info[i][place[i]], i);
    while(!choose.empty())
    {
        auto this_turn = choose.top();
        choose.pop();
        ans.push_back(this_turn.first);
        place[this_turn.second]++;
        if(place[this_turn.second]<info[this_turn.second].size())
            choose.emplace(info[this_turn.second][place[this_turn.second]], this_turn.second);
    }
    return ans;
}
void query_student(string name)
{
    vector<int> ans;
    student_name.query(name, 0, 0, ans);
    if (ans.empty())
    {
        printf("NO SUCH STUDENT\n");
        return;
    }
    for(auto i:ans)
    {
        int id = ID_to_inner_ID[i];
        cout << "Name: " << stu[id].name << " ID:" << i << endl;
    }
}
bool init()
{
    bool res1 = create_account();
    if(!res1)
    {
        printf("FAIL TO CREATE ACCOUNT\n");
        return 0;
    }
    else
        printf("SUCCESSFULLY CREATE ACCOUNT\n");
    bool res2 = initialize_windows();
    if(!res2)
    {
        printf("FAIL TO INITIALIZE WINDOWS\n");
        return 0;
    }        
    else
        printf("SUCCESSFULLY INITIALIZE WINDOWS\n");
    bool res3 = do_operation();
    if(!res3)
    {
        printf("FAIL TO DO OPERATIONS\n");
        return 0;
    }
    else
        printf("SUCCESSFULLY DO OPERATIONS\n");
    bool res4 = spending();
    if(!res4)
    {
        printf("FAIL TO SPEND\n");
        return 0;
    }
    else
        printf("SUCCESSFULLY SPEND\n");
    return 1;
}
void query_windows()
{
    vector<int> query;
    int n, x;
    scanf("%d", &n);
    for (int i = 1; i <= n;i++)
    {
        scanf("%d", &x);
        query.push_back(x);
    }
    time_clock start, end;
    start.input();
    end.input();
    auto ans = merge(start, end, query);
    for(auto i:ans)
        i.output();
}
vector<pair<int, int>> find_similar(int student_id)//inner id
{
    map<pair<int, int>, int> habit;
    auto now = stu[student_id].output_consume();
    for(auto j:now)
    {
        int time_part = j.time_part, windows_id = j.windows_id;
        habit[make_pair(time_part, windows_id)]++;
    }
    // for(auto i:habit)
    //     printf("TIMEPART:%d WINDOWS:%d TIMES:%d\n", i.first.first, i.first.second, i.second);
    vector<pair<int,int>> ans;
    for (int i = 0; i < student_dfn;i++)
    {
        if (i == student_id || stu[i].alive == 0)
            continue;
        // printf("SEARCH:%d\n", i);
        map<pair<int, int>, int> temp;
        auto now = stu[i].output_consume();
        for(auto j:now)
        {
            int time_part = j.time_part, windows_id = j.windows_id;
            temp[make_pair(time_part, windows_id)]++;
        }
        pair<int, int> times;
        times.second = i;
        times.first = 0;
        for(auto j:temp)
            if (habit.count(j.first))
                times.first -= min(habit[j.first], j.second);
        // printf("%d %d\n", i, times.first);
        ans.push_back(times);
        temp.clear();
    }
    // printf("END\n");
    sort(ans.begin(), ans.end());
    return ans;
}
void query_similar()
{
    int UID;
    scanf("%d", &UID);
    if (ID_to_inner_ID.count(UID) == 0)
    {
        printf("不合法学号\n");
        return;
    }
    // printf("INNER ID:%d\n", ID_to_inner_ID[UID]);
    auto ans = find_similar(ID_to_inner_ID[UID]);
    bool flag = 0;
    // printf("RETURN\n");
    for(auto i:ans)
    {
        if (i.first > -5)
            break;
        flag = 1;
        printf("姓名:%s 学号:%d 相似次数:%d\n", stu[i.second].name.data(), stu[i.second].UID, -i.first);
    }
    if(!flag)
        printf("无相似学生\n");
}
int main()
{
    // freopen("output.txt", "w", stdout);
    init();
    // while(1)
    // {
    //     query_similar();
    // }
    // for (int i = 0; i < student_dfn;i++)
    // {
    //     printf("STUDENT INFOMATION:");
    //     cout << stu[i].UID << " " << stu[i].name << endl;
    //     sort(stu[i].request.begin(), stu[i].request.end());
    //     printf("Comsumption:\n");
    //     for(auto j:stu[i].request)
    //         j.output();
    //     printf("Card Operation:\n");
    //     sort(stu[i].card_op.begin(), stu[i].card_op.end());
    //     for(auto j:stu[i].card_op)
    //         j.output();
    // }
    return 0;
}
#endif
