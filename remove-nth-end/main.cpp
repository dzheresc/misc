#include <iostream>
#include <initializer_list>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}

    ListNode(std::initializer_list<int> list) {
       auto initialized = false;
       ListNode* p = nullptr;
       for(auto v : list){
           if (initialized){
               p->next = new ListNode(v);
               p = p->next;
           }
           else {
               p = this;
               p->val = v;
               p->next = nullptr;
               initialized = true;
           }
       }
    }
};

class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        auto p = head;
        while(p && n-- > 0){
            p = p->next;
        }
        if (p == nullptr) // remove head
            return head->next; // mem leak head

        auto p2 = head;
        auto prev = p2;
        while(p){
            p = p->next;
            prev = p2;
            p2 = p2->next;
        }

        prev->next = p2->next; // mem leak p2
        return head;
    }
};

void print(ListNode* head){
    std::cout << '[';
    while(head){
        std::cout << head->val << ' ';
        head = head->next;
    }
    std::cout << ']' << '\n';
}

int main() {
    Solution sol;
    print(sol.removeNthFromEnd(new ListNode({1,2,3,4}),1));
    print(sol.removeNthFromEnd(new ListNode({1,2,3,4}),2));
    print(sol.removeNthFromEnd(new ListNode({1,2,3,4}),3));
    print(sol.removeNthFromEnd(new ListNode({1,2,3,4}),4));
    print(sol.removeNthFromEnd(new ListNode({1}),1));
    return 0;
}
