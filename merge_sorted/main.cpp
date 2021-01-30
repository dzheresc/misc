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

void print(const char* where, ListNode* head){
    cout << where << " [ ";
    while (head){
        cout << head->val << ' ';
        head = head->next;
    }
    cout << "]\n";
}

class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.empty())
            return nullptr;
        int n = lists.size();
        while (n > 1) {
            for (int i = 0; i < n / 2; ++i) {
                //lists[i] = merge_rec(lists[2 * i], lists[2 * i + 1]);
                lists[i] = merge(lists[2 * i], lists[2 * i + 1]);
            }
            if (n % 2 == 1) {
                lists[n / 2] = lists[n - 1];
            }
            n = (n + 1) / 2;
        }
        return lists[0];
    }

    ListNode* merge_rec(ListNode* l1, ListNode* l2) {
        if (!l1)
            return l2;
        if (!l2)
            return l1;
        if (l1->val < l2->val) {
            l1->next = merge_rec(l1->next, l2);
            return l1;
        } else {
            l2->next = merge_rec(l1, l2->next);
            return l2;
        }
    }

    ListNode* merge(ListNode* l1, ListNode* l2) {
        //print("merge l1", l1);
        //print("merge l2", l2);

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

        //print("merged", head.next);

        return head.next;
    }

    void split(ListNode* head, ListNode** l1, ListNode** l2) {
        // find middle
        // l1 = head
        // l2 = middle->next
        // middle->next = null

        auto slow = head;
        auto fast = head;

        while (fast->next != nullptr && fast->next->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }

        // slow is in the middle
        *l1 = head;
        *l2 = slow->next;
        slow->next = nullptr;
    }

    ListNode* sort(ListNode* head){
        //print("sort::head", head);
        if (head == nullptr) // empty
            return nullptr;

        if (head->next == nullptr) // one element
            return head;

        if (head->next->next == nullptr) { // two elements
            if (head->val > head->next->val){
                auto tmp = head->val;
                head->val = head->next->val;
                head->next->val = tmp;
            }
            return head;
        }

        ListNode* l1, *l2;
        split(head, &l1, &l2);
        //print("sort::split l1", l1);
        //print("sort::split l2", l2);

        l1 = sort(l1);
        l2 = sort(l2);
        return merge(l1, l2);
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
    print("merged", result);
    auto sorted = Solution().sort(make_list({7,4,5,2,1,-1,3,1,7,1}));
    print("sorted", sorted);
    return 0;
}
