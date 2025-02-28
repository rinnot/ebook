#include <iostream>
#include <iomanip>
#include <vector>
#include <numeric>

using namespace std;

class EBookManager {
public:
    EBookManager()
        : users_page_(MAX_USER_COUNT_ + 1, 0)
        , users_at_page_(MAX_PAGE_COUNT_ + 1, 0)
        , prefix_sum_(MAX_PAGE_COUNT_ + 2, 0) {
    }

    void ReadPage(int user, int page) {
        if (CheckUserExists(user)) {
            int old_page = users_page_[user];
            users_page_[user] = page;
            --users_at_page_[old_page];
            ++users_at_page_[page];
        } else {
            ++user_count_;
            users_page_[user] = page;
            ++users_at_page_[page];
        }

        UpdatePrefixSum();
    }

    double CheerUser(int user) const {
        if (!CheckUserExists(user)) {
            return 0.;
        }
        if (user_count_ == 1) {
            return 1.;
        }

        int page = users_page_[user];
        int less_users = prefix_sum_[page];  // Сумма от 0 до page - 1

        return static_cast<double>(less_users) / (user_count_ - 1);
    }

private:
    static const int MAX_USER_COUNT_ = 100'000;
    static const int MAX_PAGE_COUNT_ = 1'000;
    vector<int> users_page_;
    vector<int> users_at_page_;
    vector<int> prefix_sum_;
    int user_count_ = 0;

    bool CheckUserExists(int user) const {
        return user_count_ != 0 && user < users_page_.size() && users_page_[user] != 0;
    }

    void UpdatePrefixSum() {
        partial_sum(users_at_page_.begin(), users_at_page_.end(), prefix_sum_.begin() + 1);
    }
};

void ProcessRequestsFromStream(std::istream& input, EBookManager& book) {
    size_t count;
    input >> count;

    ios::fmtflags old_flags = cout.flags();
    cout << setprecision(6);

    for (size_t i = 0; i < count; ++i) {
        std::string type;
        input >> type;

        if (type == "CHEER") {
            int user;
            input >> user;
            double cheer = book.CheerUser(user);
            cout << cheer << endl;
        } else if (type == "READ") {
            int user, page;
            input >> user >> page;
            book.ReadPage(user, page);
        }
    }

    cout.flags(old_flags);
}

int main() {
    EBookManager book;
    ProcessRequestsFromStream(cin, book); 
    return 0;
}