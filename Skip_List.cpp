#include <iostream>
#include <vector>
#include <iterator>
#include <memory> //thanks for unique_ptr
#include <limits> //thanks for max/min
#include <cmath>
#include <random>


template <class T>
struct SkipNode{
    T value;
    int level_node;
    std::vector<SkipNode*> next_ptrs;

    SkipNode (const T& data){
        value = data;
    }

    SkipNode (const T& data, int level): SkipNode(data){
        level_node = level;
        for (int i = 0; i < level_node; i++){
            next_ptrs.push_back(nullptr);
        }
    }

    ~SkipNode(){
        level_node= 0;
        value = 0;
        for (SkipNode* i: next_ptrs){
            delete i;
        }
    }
};

template <class T>
class SkipList{
    private:
        double p;
        int len_list;
        int level_max;
        SkipNode<T>* HEAD;

        int choose_level(){
            int counter = 0;
            std::random_device rd; 
            std::mt19937 mersenne(rd());
            std::uniform_int_distribution<> distr(0, 10000);
            double rand_number = (double)distr(rd) / (10000);
            
            while ((rand_number <= p) && (counter < level_max)){
                counter++;
                rand_number = (double)distr(rd) / (10000);
            }
            return counter;
        }

        void set_head(){
            HEAD = new SkipNode<T>(std::numeric_limits<T>::lowest());

            for (int i = 0; i < level_max; i++){
                HEAD->next_ptrs.push_back(nullptr);
            }
        }

    public:
        SkipList(): p(0.5), len_list(0), level_max(32){
            set_head();
        };

        SkipList(double probability, int level_max): p(probability), level_max(level_max), len_list(0){
            set_head();
        };

        SkipList(double probability): SkipList(probability, 32){};

        SkipList(const SkipList& List){
            p = List.p;
            len_list = List.len_list;
            level_max = List.level_max;
            HEAD = List.HEAD;
        }

        SkipList(SkipList&& List){
            p = List.p;
            len_list = List.len_list;
            level_max = List.level_max;
            HEAD = List.HEAD;

            List.p = 0;
            List.level_max = 0;
            List.len_list = 0;
            List.HEAD = nullptr;
        }

        template <typename Iterat>
        SkipList(Iterat first, Iterat second): SkipList(){
            for (auto i = first; i != second; ++i){
                this->insert(*i);
            }
        }

        SkipList& operator = (const SkipList& List){
            if (&List == this){
                return *this;
            }

            delete HEAD;

            HEAD = new SkipNode<T>;
            HEAD = List.HEAD;
            p = List.p;
            len_list = List.len_list;
            level_max = List.level_max;
            return *this;
        }


        SkipList& operator = (SkipList&& List){
            if (&List == this){
                return *this;
            }

            std::swap(HEAD, List.HEAD);
            std::swap(p, List.p);
            std::swap(len_list, List.len_list);
            std::swap(level_max, List.level_max);

            return *this;
        }


        void insert(const T& val){
            int level = choose_level();
            SkipNode<T>* it = HEAD;
            SkipNode<T>* new_node = new SkipNode<T>(val, level + 1);

            for (auto i = level_max - 1; i >= 0; i--){
                while ((it->next_ptrs[i] != nullptr) && ((*it).next_ptrs[i]->value < val)){
                    it = it->next_ptrs[i];
                }

                if (i <= level){
                    new_node->next_ptrs[i] = it->next_ptrs[i];
                    it->next_ptrs[i] = new_node;
                }
            }

            len_list++;
        }

        template <typename Iter>
        void insert(Iter first, Iter second){
            for (auto i = first; i != second; ++i){
                this->insert(*i);
            }
        }


        template <typename U>
        friend std::ostream& operator << (std::ostream &out, const SkipList<U> &List_out);

        bool empty(){
            return (len_list == 0);
        }

        int size(){
            return len_list;
        }
};

template <typename U>
std::ostream& operator << (std::ostream &out, const SkipList<U> &List_out){
    for (int i = List_out.level_max - 1; i >= 0; i--){
        SkipNode<U>* it = List_out.HEAD;
        while (it->next_ptrs[i] != nullptr){
            it = it->next_ptrs[i];
            out << it->value << " ";
        }
        out << "\n";                
    }

    return out;
}

int main(){
    SkipList<int> List (0.5);
    List.insert(5);
    List.insert(3);
    List.insert(8);
    List.insert(1);

    std::vector<int> v = {5, 3, 8, 1, 1};
    SkipList<int> List1 (v.begin(), v.end());

    std::cout << List1;

    return 0;
}


