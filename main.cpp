#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <algorithm>
using namespace std;

void initiate_transactions(list<list<string>>& transactions);
void print_transactions(list<list<string>>& transactions);
void print_items_support(unordered_map<string, int>& items_support);

class FP_Node {
public:
    string item;
    int count;
    FP_Node* parent;
    unordered_map<string, FP_Node*> children;

    FP_Node(string item) {
        this->item = item;
        count = 1;
        parent = nullptr;
    }
};

class FP_Tree {
private:
    FP_Node* root;
    int min_support;
    int min_confidence;

    unordered_map<string, int> calculate_items_support(list<list<string>>& transactions);
    void remove_infrequent_items(unordered_map<string, int>& items_support, list<list<string>>& transactions);
    void remove_item(string item_to_remove, list<list<string>>& transactions);
    void sort_items(unordered_map<string, int>& items_support, list<list<string>>& transactions);
    void build_tree(list<list<string>>& transactions);
    void print_recursive(FP_Node* node, int depth);
public:
    FP_Tree(int min_support, int min_confidence);
    void create(list<list<string>>& transactions);
    void print();
};

FP_Tree::FP_Tree(int min_support, int min_confidence) {
    this->root = nullptr;
    this->min_support = min_support;
    this->min_confidence = min_confidence;
}

void FP_Tree::create(list<list<string>>& transactions) {
    unordered_map<string, int> items_support = calculate_items_support(transactions);
    print_items_support(items_support);

    remove_infrequent_items(items_support, transactions);
    sort_items(items_support, transactions);

    print_transactions(transactions);
    print_items_support(items_support);

    build_tree(transactions);
}

unordered_map<string, int> FP_Tree::calculate_items_support(list<list<string>>& transactions) {
    unordered_map<string, int> items_support;

    for (auto& transaction : transactions) {
        unordered_set<string> items;
        auto it = transaction.begin();

        while (it != transaction.end()) {
            if (items.find(*it) == items.end()) {
                items.insert(*it);
                items_support[*it] += 1;
                it++;
            } else {
                it = transaction.erase(it);
            }
        }
    }

    return items_support;
}

void FP_Tree::remove_infrequent_items(unordered_map<string, int>& items_support, list<list<string>>& transactions) {
    auto it = items_support.begin();

    while (it != items_support.end()) {
        if (it->second < min_support) {
            remove_item(it->first, transactions);
            it = items_support.erase(it);
        } else {
            it++;
        } 
    }
}

void FP_Tree::remove_item(string item_to_remove, list<list<string>>& transactions) {
    for (list<string>& transaction : transactions) {
        transaction.remove(item_to_remove);
    }
}

void FP_Tree::sort_items(unordered_map<string, int>& items_support, list<list<string>>& transactions) {
    for (list<string>& transaction : transactions) {
        transaction.sort([&items_support](const string& a, const string& b) {
            if (items_support[a] == items_support[b])
                return a < b;
            return items_support[a] > items_support[b];
        });
    }
}

void FP_Tree::build_tree(list<list<string>>& transactions) {
    root = new FP_Node("");
    root->count = 0;

    for (list<string>& transaction : transactions) {
        FP_Node* parent = root;

        for (string item : transaction) {
            auto it = parent->children.find(item);

            if (it == parent->children.end()) {
                FP_Node* new_node = new FP_Node(item);
                new_node->parent = parent;
                parent->children[item] = new_node;
                parent = new_node;
            } else {
                it->second->count += 1;
                parent = it->second;
            }
        }
    }
}

void FP_Tree::print() {
    cout << "FP-Tree Structure:\n";
    for (auto& pair : root->children) {
        print_recursive(pair.second, 1);
    }
}

void FP_Tree::print_recursive(FP_Node* node, int depth) {
    if (node == nullptr) return;

    // Indent according to depth
    for (int i = 0; i < depth; ++i) cout << "  ";

    cout << "-> " << node->item << " (" << node->count << ")\n";

    // Recurse for children
    for (auto& pair : node->children) {
        print_recursive(pair.second, depth + 1);
    }
}

int main() {
    list<list<string>> transactions;

    initiate_transactions(transactions);
    print_transactions(transactions);

    FP_Tree fp_tree(3, 4);
    fp_tree.create(transactions);
    fp_tree.print();

    return 0;
}

void initiate_transactions(list<list<string>>& transactions) {
    transactions.push_back({"M", "O", "N", "K", "E", "Y"});
    transactions.push_back({"D", "O", "N", "K", "E", "Y"});
    transactions.push_back({"M", "A", "K", "E"});
    transactions.push_back({"M", "U", "C", "K", "Y"});
    transactions.push_back({"C", "O", "O", "K", "I", "E"});
}

void print_transactions(list<list<string>>& transactions) {
    for (list<string>& transaction : transactions) {
        for (string item : transaction) {
            cout << item << ", ";
        }

        cout << endl;
    }
}

void print_items_support(unordered_map<string, int>& items_support) {
    for (auto& pair : items_support) {
        cout << pair.first << " : " << pair.second << endl;
    }
}