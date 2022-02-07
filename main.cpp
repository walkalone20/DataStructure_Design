#include "head.hpp"
#include "records.hpp"
#include "cards.hpp"
#include "windows.hpp"
#include "students.hpp"
#include "trie.hpp"
#include "init.hpp"
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
        printf("FAIL TO DO OPERATIONS AND SPEND\n");
        return 0;
    }
    else
        printf("SUCCESSFULLY DO OPERATIONS AND SPEND\n");
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
