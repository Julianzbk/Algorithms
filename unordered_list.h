#include <iostream>
using std::ostream, std::cout, std::endl;

constexpr int NWORDS = 45392;

template <typename T>
struct ListNode
{
    T data;
    ListNode* next;
    ListNode(T new_data, ListNode* new_next)
        : data(new_data), next(new_next) { }

    static void print(ostream& out, ListNode* L)
    {
        if (L == nullptr)
        {
            out << "[]";
            return;
        }
        out << '[';
        ListNode* p = L;
        for ( ; p->next != nullptr; p = p->next)
        {
            out << p->data;
            out << ", ";
        }
        out << p->data;
        out << "]";
    }

    static ListNode* find(const T& data, ListNode* L)
    {
        for (ListNode* p = L; p != nullptr; p = p->next)
        {
            if (p->data == data)
                return p;
        }
        return nullptr;
    }

    static void delete_list(ListNode* L)
    { 
        ListNode* temp;
        for (ListNode* p = L; p != nullptr; )
        {
            temp = p->next;
            delete p;
            p = temp;
        }
    }

    static void remove(const T& data, ListNode* & L)
    {
        if (L == nullptr)
            return;
        if (L->data == data)
        {
            ListNode* temp = L->next;
            delete L;
            L = temp;
        }

        ListNode* prev = nullptr;
        for (ListNode* p = L; p != nullptr; prev = p, p = p->next)
        {
            if (p->data == data)
            {
                prev->next = p->next;
                delete p;
                return;
            }
        }
    }
};


template <typename T>
class UnorderedList 
{
protected:
    ListNode<T>* head;

public:
    UnorderedList()
        :head(nullptr)
    {
    }

    void insert(const T& data)
    {
        head = new ListNode{data, head};
    }

    bool find(const T& data)
    {
        return ListNode<T>::find(data, head) != nullptr;
    }

    void remove(const T& data)
    {
        ListNode<T>::remove(data, head);
    }

    T pop_min()
    {
        if (head == nullptr)
            throw std::domain_error("UnorderedList is empty");
        T min_elem = head->data;
        ListNode<T>* curr = head->next; // already looked at head,
        ListNode<T>* prev = head;       // now looking after it
        ListNode<T>* min_ptr = head;    // though the min_elem is stil at head
        ListNode<T>* min_prev = nullptr;// and there is no prev for that node
        for (curr = head->next; curr != nullptr; curr = curr->next)
        {
            if (curr->data < min_elem)
            {
                min_ptr = curr;
                min_prev = prev;
                min_elem = curr->data;
            }
            prev = curr;
        }
        if (min_prev != nullptr)
            min_prev->next = min_ptr->next; // unlinks the min_ptr node
        else // == min_ptr is head OR min_ptr is only node
        {
            if (min_ptr == head)
                head = min_ptr->next;
            else
                head = nullptr;
        }
        delete min_ptr;
        return min_elem;
    }

    bool is_empty()
    {
        return head == nullptr;
    }

    bool is_full()
    {
        return false;
    }

    void erase()
    {
        ListNode<T>::delete_list(head);
        head = nullptr;
    }

    void print(ostream& out)
    {
        ListNode<T>::print(out, head);
    }

    ~UnorderedList()
    {
        ListNode<T>::delete_list(head);
    }
};

template <typename T>
ostream& operator << (ostream& out, UnorderedList<T>& L)
{
    L.print(out);
    return out;
}
