#include <iostream>
#include <fstream>
#include <array>

template <class T>
struct Node{
    T value;
    Node* next_node = nullptr;

    Node(T val): value(val){};
};


template <class T>
class  Vector{
    private:
        Node<T>* first_node;
        int len;

        Node<T>* at_node (int place){
            Node<T>* current_node = first_node;
            for (int i = 0; i < place; i++){
                current_node = current_node->next_node;
            }
            return current_node;
        }

    public:
        Vector (){
            len = 0;
            first_node = nullptr;
        }

        Vector (T (&data)[]){
            Node<T>* prev_node = new Node<T>(data[0]);
            first_node = prev_node;
            bool flag = false;
            len = 0;
            for (auto i: data){
                if (flag){
                    Node<T>* cur_node = new Node<T>(i);
                    prev_node->next_node = cur_node;
                    prev_node = cur_node;
                } else {
                    flag = true;
                }
                len++;
            }
        }

        int size () const{
            return len;
        }

        void pop_back(){
            if ((len == 0) || (len == 1)){
                delete first_node;
                first_node = nullptr;
                len = 0;
            } else {
                Node<T>* current_node = at_node(len - 2);
                Node<T>* node_to_del = current_node->next_node;
                delete node_to_del;
                current_node->next_node = nullptr;
                len--;
            }
        }

        T at(int i){
            return at_node(i)->value;
        }

         T front(){
            return at(0);
        }

        T back(){
            return at(len - 1);
        }

        void insert(int i, T val){
            if (i <= len){
                Node<T>* node_to_add = new Node<T>(val);
                if (len == 0){
                    first_node = node_to_add;
                } else {
                    if (i == 0){
                        node_to_add->next_node = first_node;
                        first_node = node_to_add;
                    } else {
                        Node<T>* prev_node = at_node(i-1);
                        node_to_add->next_node = prev_node->next_node;
                        prev_node->next_node = node_to_add;
                    }
                }
                len++;
            }
        }

        void push_back(T val){
            insert(len, val);
        }

        template <class U>
        friend std::ostream& operator<< (std::ostream &out, const Vector<U> &vec);

        ~Vector(){
            Node<T>* cur_node = first_node;
            for (int i = 0; i < len - 1; i++){
                Node<T>* node2del = cur_node;
                cur_node = cur_node->next_node;
                delete node2del;
            }
            delete cur_node;
        }
};

template<typename T>
std::ostream& operator<< (std::ostream &out, const Vector<T> &vec){
    Node<T>* current_node = vec.first_node;
    out << current_node->value << " ";
    for (int i = 0; i < vec.size() - 1; i++){
        current_node = current_node->next_node;
        out << current_node->value << " ";
    }
    return out;
};


int main(){
    Vector<int> vec;
    vec.push_back(5);
    vec.push_back(7);
    vec.push_back(9);
    vec.insert(1, 15);
    vec.pop_back();
    std::cout << vec;
    return 0;
}
