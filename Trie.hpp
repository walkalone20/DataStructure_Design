#ifndef TRIE_HEAD_H
#define TRIE_HEAD_H

#include "head.hpp"
class trie
{
    struct trie_node
    {
        map<string, int> ch;
        bool flag;       //reach the end of a student's name
        vector<int> UID; // if there's student's name ended here, store the UID of the student
    } NIL;
public:
    vector<trie_node> t;
    trie()
    {
        t.push_back(NIL);
    }
    void insert(string name, int UID)
    {
        int place = 0;
        for (size_t i = 0; i < name.length(); i += 2)
        {
            string now = name.substr(i, 2);
            if (t[place].ch.count(now) == 0)
            {
                t.push_back(NIL);
                t[place].ch[now] = t.size() - 1;
            }
            place = t[place].ch[now];
        }
        t[place].flag = 1;
        t[place].UID.push_back(UID);
    }
    vector<int> get_whole(int place)//get the whole students in this subtree
    {
        queue<int> q;
        q.push(place);
        vector<int> ans;
        while(!q.empty())
        {
            int tp = q.front();
            q.pop();
            for (auto i : t[tp].ch)
            {
                if(t[i.second].flag)
                    for(auto j:t[i.second].UID)
                        ans.push_back(j);
                q.push(i.second);
            }
        }
        return ans;
    }
    void query(string &name, int place, int pos, vector<int> &ans)
    {
        if ((size_t)pos >= name.length())
        {
            for (auto i : t[place].UID)
                ans.push_back(i);
            return;
        }
        if(name[pos]=='*')
        {
            auto now = get_whole(place);
            for (auto j : now)
                ans.push_back(j);
            return;
        }
        if (name[pos] == '?')
        {
            for (auto i : t[place].ch)
                query(name, i.second, pos + 1, ans);
            return;
        }
        string now = name.substr(pos, 2);
        if (t[place].ch.count(now) == 0)
            return;
        query(name, t[place].ch[now], pos + 2, ans);
        return;
    }
};
#endif
