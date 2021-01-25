#include "tnf_tree.h"
#include <iostream>
#include <string>

const char *test[] = {"3", "1", "8", "4", "2", "1", "7"};

using namespace std;

void tnf_test() {
  tnf::lrtree<string> t;

  for (int i = 0; i < 7; ++i)
    t.push(test[i]);

  auto pf = [](const string v) { cout << v.c_str() << ' '; };
  cout << "inorder\n";
  t.inorder(pf);
  cout << "\n";

  cout << "bfs\n";
  t.bfs(pf);
  cout << "\n";
}
