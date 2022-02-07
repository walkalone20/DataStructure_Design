#ifndef MAP_H
#define MAP_H

#include <vector>
template<class T1, class T2> class Splay{
private:
    const int N = 100000;
    struct node
    {
        int father;
        int child[2] = {0};
        T1 key;
        T2 v;
        int times;
        int cnt;
    };
    struct node DUMMY;
    std::vector<node> t;
    int tot, root;
    void clear(int x)
    {
        t[x].father = t[x].child[1] = t[x].child[0] = t[x].v = t[x].times = t[x].cnt = 0;
        return;
    }
    void update(int x)
    {
        if(x)
        {
            t[x].cnt = t[x].times;
            if(t[x].child[0])
                t[x].cnt += t[t[x].child[0]].cnt;
            if(t[x].child[1])
                t[x].cnt += t[t[x].child[1]].cnt;
        }
        return;
    }
    void rotate(int x)
    {
        int y = t[x].father;
        int z = t[y].father;
        int k = t[y].child[1] == x;
        t[y].child[k] = t[x].child[k ^ 1];
        t[t[x].child[k ^ 1]].father = y;
        t[x].child[k ^ 1] = y;
        t[y].father = x;
        t[x].father = z;
        if(z)
            t[z].child[t[z].child[1] == y] = x;
        update(y);
        update(x);
    }
    void splay(int x)
    {
        for (int fa; fa = t[x].father; rotate(x))
            if(t[fa].father)
                rotate((t[fa].child[1] == x) ^ (t[t[fa].father].child[1] == fa) ? fa : x);
        root = x;
    }
public:
    Splay()
    {
        t.push_back(DUMMY);
        tot = 0;
        root = 0;
    }
    void insert(T1 x, T2 y)
    {
        if(!root)
        {
            t.push_back(DUMMY);
            tot++;
            t[tot].father = t[tot].child[1] = t[tot].child[0] = 0;
            t[tot].key = x;
            t[tot].v = y;
            t[tot].cnt = t[tot].times = 1;
            root = tot;
            return;
        }
        int now = root, fa = 0;
        while(1)
        {
            if (x == t[now].v)
            {
                t[now].times++;
                update(now);
                update(fa);
                splay(now);
                break;
            }
            fa = now;
            now = t[fa].child[t[fa].v < x];
            if(!now)
            {
                t.push_back(DUMMY);
                tot++;
                t[tot].child[1] = t[tot].child[0] = 0;
                t[tot].father = fa;
                t[tot].cnt = t[tot].times = 1;
                t[fa].child[t[fa].v < x] = tot;
                t[tot].key = x;
                t[tot].v = y;
                update(fa);
                splay(tot);
                break;
            }
        }
    }
    int rank(T1 x)
    {
        int now = root, ans = 0;
        while(1)
        {
            if (x < t[now].v)
                now = t[now].child[0];
            else
            {
                if(t[now].child[0])
                    ans += t[t[now].child[0]].cnt;
                if (t[now].v == x)
                {
                    splay(now);
                    return ans + 1;
                }
                ans += t[now].times;
                now = t[now].child[1];
            }
        }
    }
    T2 kth(T1 x)
    {
        int now = root;
        while(1)
        {
            if (t[now].child[0] && x <= t[t[now].child[0]].cnt)
                now = t[now].child[0];
            else
            {
                int temp = t[now].times;
                if(t[now].child[0])
                    temp += t[t[now].child[0]].cnt;
                if (x <= temp)
                    return t[now].v;
                x -= temp;
                now = t[now].child[1];
            }
        }
    }
    int pre()
    {
        int now = t[root].child[0];
        while(t[now].child[1])
            now = t[now].child[1];
        return now;
    }
    int next()
    {
        int now=t[root].child[1];
        while(t[now].child[0])
            now=t[now].child[0];
        return now;
    }
    void remove(T1 X)
    {
        int place = rank(x);
        if (t[root].times > 1)
        {
            t[root].times--;
            update(root);
            return;
        }
        if(!t[root].child[0] && !t[root].child[1])
        {
            clear(root);
            root = 0;
            return;
        }
        if(!t[root].child[0])
        {
            int oldroot = root;
            root = t[root].child[1];
            t[root].father = 0;
            clear(oldroot);
            return;
        }
        else
            if(!t[root].child[1])
            {
                int oldroot = root;
                root = t[root].child[0];
                t[root].father = 0;
                clear(oldroot);
                return;
            }
            else
            {
                int leftbig = pre(), oldroot = root;
                splay(leftbig);
                t[root].child[1] = t[oldroot].child[1];
                t[t[root].child[1]].father = root;
                clear(oldroot);
                update(root);
            }       
    }
    T2 query(int place)
    {
        return t[place].v;
    }
    int size()
    {
        return t[root].cnt;
    }
};

#endif
