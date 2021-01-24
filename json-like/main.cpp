#include <iostream>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include <memory>

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
    variant<bool,doc_t*> val;
};

class doc_t {

    bool is_valid(optional<bool>& expected, doc_t* doc){
        for(auto kv : doc->data){
            if (holds_alternative<doc_t*>(kv.val)){
                if (!is_valid(expected, get<doc_t*>(kv.val)))
                    return false;
            }
            else { // holds_alternative<bool>
                if (expected.has_value()){
                    if(expected.value() != get<bool>(kv.val))
                        return false;
                }
                else {
                    expected = get<bool>(kv.val);
                }
            }
        }
        return true;
    }

public:
    vector<kv_t> data;
    doc_t(initializer_list<kv_t> list){
        for (auto kv : list) {
            data.push_back(kv);
        }
    }

    bool is_valid(){
        optional<bool> expected;
        return is_valid(expected, this);
    }
};

class assertion_exception: public exception {
    const string& error;
public:
    assertion_exception(const string& error)
        :error(error){
    }

    const char * what() const noexcept override {
        return error.c_str();
    }
};
void assert(bool expression, const string& error){
    if (!expression) {
        throw assertion_exception(error);
    }
}

int main() {
    auto make_doc = [](bool a, bool c){
      return unique_ptr<doc_t>(new doc_t
      {
        {"a", a},
        {"b", new doc_t { {"c", c} }},
      });
    };

    assert(make_doc(true,true)->is_valid(), "all true");
    assert(make_doc(false,false)->is_valid(), "all false");
    assert(make_doc(true,false)->is_valid() == false, "disallow mix");

    cout << "ok\n";
    return 0;
}
