#include <iostream>
#include <vector>

using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.empty())
            return nullptr;
        int n = lists.size();
        while (n > 1) {
            for (int i = 0; i < n / 2; ++i) {
                //lists[i] = merge(lists[2 * i], lists[2 * i + 1]);
                lists[i] = mergeTwoLists(lists[2 * i], lists[2 * i + 1]);
            }
            if (n % 2 == 1) {
                lists[n / 2] = lists[n - 1];
            }
            n = (n + 1) / 2;
        }
        return lists[0];
    }

    ListNode* merge(ListNode* l1, ListNode* l2) {
        if (!l1)
            return l2;
        if (!l2)
            return l1;
        if (l1->val < l2->val) {
            l1->next = merge(l1->next, l2);
            return l1;
        } else {
            l2->next = merge(l1, l2->next);
            return l2;
        }
    }

    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode head{-1000,nullptr};
        ListNode *p = &head;

        while(l1 && l2){
            if (l1->val < l2->val){
                p->next = l1;
                l1 = l1->next;
            } else { // l1->val >= l2->val
                p->next = l2;
                l2 = l2->next;
            }

            p = p->next;
        }

        if (l1){
            p->next = l1;
        }
        if (l2){
            p->next = l2;
        }

        return head.next;
    }

};

ListNode* make_list(initializer_list<int> data){
    ListNode head;
    ListNode* node = &head;

    for(auto val : data){
        node->next = new ListNode(val);
        node = node->next;
    }
    return head.next;
}

int main() {
    auto l1 = make_list({1,2});
    auto l2 = make_list({2,3});
    auto l3 = make_list({3,4,5});
    vector<ListNode*> vec{l1,l2,l3};
    auto result = Solution().mergeKLists(vec);
    return 0;
}
