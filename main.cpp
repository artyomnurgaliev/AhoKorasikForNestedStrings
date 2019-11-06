#include <algorithm>
#include <cstddef>
#include <deque>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

struct AhoCorasickNode {
  size_t count_of_nested_substrings = 0;
  size_t is_terminal = 0;
  // Stores tree structure of nodes.
  std::map<char, AhoCorasickNode> trie_transitions_;

  // Stores cached transitions of the automaton_, contains
  // only pointers to the elements of trie_transitions_.
  std::unordered_map<char, AhoCorasickNode *> automaton_transitions_cache_;

  AhoCorasickNode *suffix_link_ = nullptr;
  AhoCorasickNode *terminal_link_ = nullptr;
};

AhoCorasickNode *GetAutomatonTransition(AhoCorasickNode *node, char ch) {
  AhoCorasickNode *res = nullptr;
  if (node->automaton_transitions_cache_.find(ch) ==
      node->automaton_transitions_cache_.end()) {
    if (node->suffix_link_ == nullptr) {
      return node;
    }
    res = GetAutomatonTransition(node->suffix_link_, ch);
    node->automaton_transitions_cache_[ch] = res;
  } else {
    return node->automaton_transitions_cache_[ch];
  }
  return res;
}

class AhoCorasick {
 public:
  AhoCorasick() = default;

  AhoCorasick(const AhoCorasick &) = delete;

  AhoCorasick &operator=(const AhoCorasick &) = delete;

  AhoCorasick(AhoCorasick &&) = delete;

  AhoCorasick &operator=(AhoCorasick &&) = delete;

  AhoCorasickNode Root() { return root_; }

 private:
  friend class AhoCorasickBuilder;

  AhoCorasickNode root_;
};

class AhoCorasickBuilder {
 public:

  void AddString(std::string string) {
    strings_.push_back(std::move(string));
  }

  size_t Build() {
    auto automaton = std::make_unique<AhoCorasick>();
    automaton->root_ = AhoCorasickNode();
    automaton->root_.count_of_nested_substrings = 0;
    for (const auto &string : strings_) {
      AddString(&automaton->root_, string);
    }
    size_t res = CalculateLinks(&automaton->root_);
    return res;
  }

 private:
  static void AddString(AhoCorasickNode *root, const std::string &string) {
    auto curr_node = root;
    for (auto ch : string) {
      if (curr_node->trie_transitions_.find(ch) ==
          curr_node->trie_transitions_.end()) {
        curr_node->trie_transitions_[ch] = AhoCorasickNode();
      }
      curr_node->automaton_transitions_cache_[ch] =
          &(curr_node->trie_transitions_[ch]);
      curr_node = &(curr_node->trie_transitions_[ch]);
    }
    curr_node->is_terminal = 1;
  }

  static size_t CalculateLinks(AhoCorasickNode *root) {
    size_t res = 0;
    std::queue<AhoCorasickNode *> queue;
    root->suffix_link_ = nullptr;
    root->terminal_link_ = nullptr;
    queue.push(root);
    while (!queue.empty()) {
      auto curr_node = queue.front();
      queue.pop();
      for (auto &trie_transition : curr_node->trie_transitions_) {
        queue.push(&(trie_transition.second));
        auto prev = curr_node;
        char ch = trie_transition.first;
        while (prev->suffix_link_ != nullptr) {
          if (prev->suffix_link_->trie_transitions_.find(ch) ==
              prev->suffix_link_->trie_transitions_.end()) {
            prev = prev->suffix_link_;
          } else {
            trie_transition.second.suffix_link_ =
                &(prev->suffix_link_->trie_transitions_.find(ch)->second);
            break;
          }
        }
        if (prev->suffix_link_ == nullptr) {
          trie_transition.second.suffix_link_ = root;
        }
        if (trie_transition.second.suffix_link_ == nullptr) {
          trie_transition.second.terminal_link_ = nullptr;

        } else {
          if (trie_transition.second.suffix_link_->is_terminal == 0) {
            trie_transition.second.terminal_link_ =
                trie_transition.second.suffix_link_->terminal_link_;
          } else {
            trie_transition.second.terminal_link_ =
                trie_transition.second.suffix_link_;
          }
        }

        if (trie_transition.second.suffix_link_ == nullptr) {
          if (trie_transition.second.is_terminal) {
            trie_transition.second.count_of_nested_substrings = 1;
          } else {
            trie_transition.second.count_of_nested_substrings = 0;
          }
        } else {

          size_t count_of_nested_substrings_in_prev = curr_node->count_of_nested_substrings;

          size_t count_of_nested_substrings_in_suffix_link = trie_transition.second.suffix_link_->count_of_nested_substrings;

          trie_transition.second.count_of_nested_substrings =
              std::max(count_of_nested_substrings_in_prev, count_of_nested_substrings_in_suffix_link);

          if (trie_transition.second.is_terminal != 0) {
            trie_transition.second.count_of_nested_substrings++;
          }

          if (res < trie_transition.second.count_of_nested_substrings) {
            res = trie_transition.second.count_of_nested_substrings;
          }
        }
      }
    }
    return res;
  }

  std::vector<std::string> strings_;
};

class NestedSubstringFinderBuilder {
 public:
  void AddString(std::string string) {
    strings_.push_back(std::move(string));
  }

  size_t CalcCountOfNestedSubstrings() {
    AhoCorasickBuilder aho_corasick_builder;
    for (const auto &string : strings_) {
      aho_corasick_builder.AddString(string);
    }
    return aho_corasick_builder.Build();
  }

  void Reset() {
    strings_.clear();
  }

 private:
  std::vector<std::string> strings_;
};

void calc_result() {
  std::string input;
  std::string result;
  size_t count;
  std::cin >> count;
  NestedSubstringFinderBuilder nested_substring_finder_builder;
  while (count != 0) {
    for (size_t i = 0; i < count; ++i) {
      std::cin >> input;
      nested_substring_finder_builder.AddString(input);
    }
    result += std::to_string(nested_substring_finder_builder.CalcCountOfNestedSubstrings()) + "\n";
    std::cin >> count;
    nested_substring_finder_builder.Reset();
  }
  std::cout << result;
}

int main() {
  calc_result();
}
