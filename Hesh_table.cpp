#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


unsigned int hash1 (std::string str, unsigned int table_len, unsigned int prime = 47){
    unsigned int hash_res  = 0;
    for (int  i=0; i < str.size(); i++){
        hash_res = (prime * hash_res + str[i]) % table_len;
    }
    return hash_res;
}

unsigned int hash2(std::string str, unsigned int table_len, unsigned int prime = 43, unsigned int alpha = 1){
    return ((hash1(str, table_len, prime) * prime + alpha) % table_len);
}

class Hash_list{
private:
    unsigned int table_len;
    std::vector<std::vector <std::string>> hash_arr;
    unsigned int real_size; //the amount of using lines in table
    unsigned int elem_count;//the amount of all elements

    void resize_table(){
        unsigned int new_table_len = table_len * 2;
        std::vector<std::vector <std::string>> new_hash_arr(new_table_len);
        for (int i = 0; i < table_len; i++){
            for (std::string j : hash_arr[i]){
                unsigned int hash_elem = hash1(j, new_table_len);
                new_hash_arr[hash_elem].push_back(j);
            }
        }
        hash_arr.clear();
        hash_arr.insert(hash_arr.begin(), new_hash_arr.begin(), new_hash_arr.end());
        table_len = new_table_len;
    }

public:
    Hash_list(unsigned int size_table){
        table_len = size_table;
        for (int i = 0; i < table_len; i++){
            hash_arr.push_back(std::vector <std::string>());
        }
        real_size = 0;
    }

    Hash_list(): Hash_list(256){
    }

    void add_elem(std::string elem){
        if (real_size > (3*table_len) / 4){
            resize_table();
        }
        unsigned int hash_elem = hash1(elem, table_len);
        if (hash_arr[hash_elem].size() == 0){
            real_size++;
        }
        hash_arr[hash_elem].push_back(elem);
        elem_count++;
    }

    void delete_elem(std::string elem){
        unsigned int hash_elem = hash1(elem, table_len);
        auto it = std::find(hash_arr[hash_elem].begin(), hash_arr[hash_elem].end(), elem);
        if (it < hash_arr[hash_elem].end()){
            int index = std::distance(hash_arr[hash_elem].begin(), it);
            hash_arr[hash_elem].erase(hash_arr[hash_elem].begin() + index);
            if (hash_arr[hash_elem].size() == 0){
                real_size--;
            }
            elem_count--;
        }
    }

    std::string* find_elem(std::string elem){
        unsigned int hash_elem = hash1(elem, table_len);
        auto it = std::find(hash_arr[hash_elem].begin(), hash_arr[hash_elem].end(), elem);
        if (it < hash_arr[hash_elem].end()){
            int index = std::distance(hash_arr[hash_elem].begin(), it);
            return &hash_arr[hash_elem][index];
        } else {
            return nullptr;
        }
    }

    void print_hash_table(){
    //This function is for debug.
        for (int i = 0; i < table_len; i++){
            if (hash_arr[i].size() != 0){
                for (std::string j : hash_arr[i]){
                    std::cout << j << " ";
                }
                std::cout << std::endl;
            }
        }
        std::cout << real_size << " " << elem_count << std::endl;
    }
};

struct Node{
    std::string value;
    bool emptiness = true; //true if the node is empty;
};

class Hashing_double{
private:
    std::vector <Node> hash_table;
    unsigned int table_size=0;
    unsigned int real_size;

    void add_elem_without_checking(std::string elem){
        unsigned int elem_hash = hash1(elem, table_size);
        unsigned int elem_hash2 = hash2(elem, table_size);
        unsigned int counter = 0;
        while ((!hash_table[elem_hash].emptiness) && (counter < table_size)){
            elem_hash = (elem_hash + elem_hash2) % table_size;
            counter++;
        }
        hash_table[elem_hash].value = elem;
        hash_table[elem_hash].emptiness = false;
        real_size++;

    }


    void resize_table(){
        std::vector <Node> old_hash_table;
        old_hash_table.insert(old_hash_table.begin(), hash_table.begin(), hash_table.end());
        hash_table.clear();
        unsigned int old_table_size = table_size;
        hash_table = std::vector <Node>(old_table_size * 2);
        table_size = old_table_size * 2;
        real_size = 0;
        for (int i = 0; i < old_table_size; i++){
            if (!old_hash_table[i].emptiness) {
                add_elem_without_checking(old_hash_table[i].value);
            }
        }
        old_hash_table.clear();
    }

    Node* find_elem_return_node(std::string elem){
        unsigned int elem_hash = hash1(elem, table_size);
        unsigned int elem_hash2 = hash2(elem_hash, table_size);
        bool flag = true;
        int counter = 0;
        while ((flag)&&(counter < table_size)){
            if (hash_table[elem_hash].value == elem){
                flag = false;
            } else {
                elem_hash = (elem_hash + elem_hash2) % table_size;
            }
            counter++;
        }
        if (!flag){
            return &hash_table[elem_hash];
        } else {
            return nullptr;
        }
    }

public:
    Hashing_double(unsigned int size_tab){
        //std::vector <Node> hash_table (size_tab);
        hash_table = std::vector <Node> (size_tab);
        table_size = size_tab;
        real_size = 0;
    }

    Hashing_double(): Hashing_double(256){
    }

    void add_elem(std::string elem){
        if (real_size + 1 > (3*table_size) / 4){
            resize_table();
        }
        add_elem_without_checking(elem);
    }

    void delete_elem(std::string elem){
        Node* node_to_del = find_elem_return_node(elem);
        node_to_del->emptiness = true;
        real_size--;
    }

    std::string* find_elem(std::string elem){
        Node* node = find_elem_return_node(elem);
        return &node->value;
    }

    void print_hash_table(){
        for (int i = 0; i < table_size; i++){
            if (!hash_table[i].emptiness){
                std::cout << hash_table[i].value << std::endl;
            }
        }
        std::cout << real_size << std::endl;
    }
};

int main(){
    Hashing_double hash_example (1);
    hash_example.add_elem("A");
    hash_example.add_elem("D");
    hash_example.print_hash_table();
    std::cout << *hash_example.find_elem("A") << std::endl;
    *hash_example.find_elem("A") = "TTT";
    hash_example.delete_elem("D");
    hash_example.print_hash_table();

    return 0;
}
