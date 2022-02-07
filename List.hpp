#ifndef LIST_H
#define LIST_H

#include <cstdlib>
template<typename T> class List
{
public:
    struct list_node
    {
        list_node *pre, *next;
        T element;
    };
    int size;
    list_node *head;
    List()
    {
        this->head = (list_node *)malloc(sizeof(list_node));
        this->head->next = this->head->pre = nullptr;
        this->size = 0;
    }
    void insert(T val)
    {
        list_node *temp = (list_node *)malloc(sizeof(list_node));
        temp->element = val;
        temp->pre = temp->next = nullptr;
        ++this->size;
        if (this->size == 1)
        {
            head->next = temp;
            temp->pre = head;
            return;
        }
        else
        {
            temp->next = head->next;
            head->next->pre = temp;
            head->next = temp;
            temp->pre = head;
            return;
        }
    }
    bool is_empty()
    {
        return !size;
    }
    T get_head_element()
    {
        return head->next->element;
    }
    void clear()
    {
        list_node *place = head;
        while(place!=nullptr)
        {
            list_node *old = place;
            place = old->next;
            free(old);
        }
        size = 0;
    }
};
#endif
