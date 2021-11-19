#include <iostream>
#include <list>
#include <iterator>
#include <memory>
#include <cmath>
#include <cstdlib> //thanks for rand

template <class T>
struct SkipNode{
    std::unique_ptr<T> value = nullptr;
    size_t level_node;
    std::list<SkipNode*> next_ptrs;

    SkipNode (const T& data) = delete;

    SkipNode (T&& data){
        value = std::make_unique(T);
    }

    ~SkipNode(){
        level_node= 0;
        value.release();
        for (SkipNode* i: next_ptrs){
            delete i;
        }
        next_ptrs.erase();
    }
};

class Iterator{

};

template <class T>
class SkipList{
    private:
        double p;
        size_t len_list;
        int level_max;
        std::list<SkipNode> main_data;

        int choose_level(){
            double rand_number = rand(0, 10000) / 10000;
            int counter = 0;
            while ((rand_number <= p) && (counter < level_max)){
                counter++;
                rand_number = rand(0, 10000) / 10000;
            }
            return counter;
        }

        void balance()

    public:
        SkipList(): p(0), len_list(0), level_max(0);

        template <typename U>
        SkipList(U<T>::const_iterator first, U<T>::const_iterator second){
            for (auto i = first; i != second; ++i){
                this->insert(*i);
            }
        }

        void insert(const T& val){

        }

        bool empty(){
            return (len_list == 0);
        }

        size_t size(){
            return len_list;
        }




};
