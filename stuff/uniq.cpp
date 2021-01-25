#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

// time O(n^2)
// space O(1)
bool is_uniq(char *s) {
  for (; *s; s++)
    for (char *p = s + 1; *p; p++)
      if (*s == *p)
        return false;
  return true;
}

// time (n)
// space (1)
bool is_uniq2(char *s) {
  char cnt[256];
  for (int i = 256; i-- > 0;)
    cnt[i] = 0;

  for (; *s; s++) {
    cnt[*s]++;
    if (cnt[*s] > 1)
      return false;
  }
  return true;
}

// reverse string in-place
void reverse(char *s) {
  char *p = s;
  while (*p)
    p++;
  p--;

  while (s < p) {
    char c = *s;
    *s = *p;
    *p = c;
    s++;
    p--;
  }
}

// shift string left
void shift1(char *s) {
  for (; *s; s++)
    *s = *(s + 1);
}

// remove sequential dups
void rmdup(string &s) { s.erase(std::unique(s.begin(), s.end()), s.end()); }

void c_cnt(char *s, char *cnt) {
  while (*s)
    cnt[*s++]++;
}

// check if two strings are anagrams
bool anagram(char *s1, char *s2) {
  char cnt1[256];
  char cnt2[256];
  for (int i = 256; i--;)
    cnt1[i] = cnt2[i] = 0;

  c_cnt(s1, cnt1);
  c_cnt(s2, cnt2);

  for (int i = 256; i--;)
    if (cnt1[i] != cnt2[i])
      return false;
  return true;
}

// Write a method to replace all spaces in a string with �%20�.
char *encode(char *s) {
  int length = 0;
  int space = 0;

  char *t = s;
  for (; *t; length++)
    if (*t++ == ' ')
      space++;

  t = new char[length + space * 2 + 1];
  char *t2 = t;
  while (*s) {
    if (*s == ' ') {
      *t++ = '%';
      *t++ = '2';
      *t++ = '0';
    } else
      *t++ = *s;
    s++;
  }
  *t = 0;
  return t2;
}

// rotate matrix 90 degrees
template <int n> void mx90(int m[n][n]) {
  // mirror diag
  for (int r = 0; r < n; r++)
    for (int c = 0; c < r; c++) {
      int tmp = m[r][c];
      m[r][c] = m[c][r];
      m[c][r] = tmp;
    }
  // flip vert
  for (int row = 0; row < n; row++) {
    for (int left = 0, right = n - 1; left < right; left++, right--) {
      int tmp = m[row][left];
      m[row][left] = m[row][right];
      m[row][right] = tmp;
    }
  }

  for (int r = 0; r < n; r++) {
    for (int c = 0; c < n; c++) {
      cout << m[r][c] << ' ';
    }
    cout << '\n';
  }
}

int mx[4][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};

void uniq_test() {

  //  string dups{"abbccddeeffx"};
  //  rmdup(dups);
  //  cout << dups << endl;

  mx90<4>(mx);
}
