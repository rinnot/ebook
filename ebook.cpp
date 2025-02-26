#include <iostream>
#include <unordered_map>
#include <map>
#include <iomanip>

using namespace std;

class EbookManager {
public:
    void Read(int user, int page) {
        if (user_progress.count(user)) {
            int old_page = user_progress[user];

            if (--pages_count[old_page] == 0) {
                pages_count.erase(old_page);
            }
        }

        user_progress[user] = page;
        pages_count[page]++;
    }

    double Cheer(int user) const {
        if (!user_progress.count(user)) {
            return 0.0;
        }

        if (user_progress.size() == 1) {
            return 1.0;
        }

        int user_page = user_progress.at(user);
        int users_before = 0;

        auto it = pages_count.begin();
        while (it->first < user_page) {
            users_before += it->second;
            ++it;
        }

        return static_cast<double>(users_before) 
             / static_cast<double>(user_progress.size() - 1);

    }

private:
    unordered_map<int, int> user_progress;
    map<int, int> pages_count;
};

int main() {
    EbookManager manager;

    unsigned int q;
    cin >> q;
    while (q--) {
        string command;
        int user;
        cin >> command >> user;

        if (command == "READ") {
            int page;
            cin >> page;
            manager.Read(user, page);
        } else if (command == "CHEER") {
            cout << manager.Cheer(user) << '\n';
        }
    }
}
