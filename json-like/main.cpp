#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

using namespace std;

/*
 *
 * {
 * k:v,
 * k1:{k2:v2}
 * }
 */

class doc_t;
class kv_t {
public:
  string key;
  variant<bool, doc_t *> val;
};

class doc_t {

  bool is_valid(int &flags, doc_t *doc) {
    for (auto kv : doc->data) {
      if (holds_alternative<doc_t *>(kv.val)) {
        if (!is_valid(flags, get<doc_t *>(kv.val))) {
          return false;
        }
      } else { // holds_alternative<bool>
        // bit0 -> false, bit4 -> true
        // 0x11 -> error
        flags |= get<bool>(kv.val) ? 0x10 : 0x01;
        if (flags == 0x11)
          return false;
      }
    }
    return true;
  }

public:
  vector<kv_t> data;

  doc_t(initializer_list<kv_t> list) {
    for (auto kv : list) {
      data.push_back(kv);
    }
  }

  ~doc_t() {
    for (auto kv : data) {
      if (holds_alternative<doc_t *>(kv.val))
        delete get<doc_t *>(kv.val);
    }
  }

  bool is_valid() {
    int flags{0};
    return is_valid(flags, this);
  }
};

class assertion_exception : public exception {
  const string &error;

public:
  assertion_exception(const string &error) : error(error) {}

  const char *what() const noexcept override { return error.c_str(); }
};

void assert(bool expression, const string &error) {
  if (!expression) {
    throw assertion_exception(error);
  }
}

int main() {
  auto make_doc = [](bool a, bool c) {
    return unique_ptr<doc_t>(new doc_t{
        {"a", a},
        {"b", new doc_t{{"c", c}}},
    });
  };

  assert(make_doc(true, true)->is_valid(), "all true");
  assert(make_doc(false, false)->is_valid(), "all false");
  assert(make_doc(true, false)->is_valid() == false, "disallow mix");

  cout << "ok\n";
  return 0;
}
