#include <iostream>
#include <set>
#include <random>
#include <fstream>
#include <vector>

class State {
private:

public:
    virtual bool contains(int s) const = 0;
    virtual std::set<int> main_information () const = 0;

    virtual ~State(){};
};

class DiscreteState: public State {
private:
    int const state;

public:
    DiscreteState(int state): state(state) { }

    std::set<int> main_information() const{
        return (std::set<int> {state});
    }


    bool contains(int s) const {
        return s == state;
    }
};


class SegmentState: public State {
private:
    int const beg, end;

public:
    SegmentState(): beg(0), end(-1) { };
    SegmentState(int beg, int end): beg(beg), end(end) { };

    std::set<int> main_information() const{
        return (std::set<int> {beg, end});
    }

    bool contains(int s) const {
        return s >= beg && s <= end;
    }
};

class SetState: public State {
private:
    std::set<int> const states;

public:
    SetState(): states() { }
    SetState(std::set<int> const &src): states(src) { }

    bool contains(int s) const {
        return states.count(s) > 0;
    }

    std::set<int> main_information() const{
        return states;
    }
};

class Set_of_States: public State{
private:
    std::vector<DiscreteState> set_discrete;
    std::vector<SegmentState> set_segment;

public:
    Set_of_States(): set_discrete(), set_segment(){};

    Set_of_States(std::vector<DiscreteState> &set_states): set_discrete(set_states){};

    Set_of_States(std::vector<SegmentState> &set_states): set_segment(set_states){};

    Set_of_States(std::vector<DiscreteState> &set_states1, std::vector<SegmentState> &set_states2):  set_segment(set_states2) {
        for (DiscreteState i: set_states1){
            add(i);
        }
    }

    void add (DiscreteState &One_State){
        bool flag = true;
        for (SegmentState j: set_segment){
            if (j.contains(*One_State.main_information().begin())){
                    flag = false;
            }
        }
        for (DiscreteState j: set_discrete){
            if (j.contains(*One_State.main_information().begin())){
                    flag = false;
            }
        }
        if (flag){
            set_discrete.push_back(One_State);
        }
    }

    void add (SegmentState &S1){
        set_segment.push_back(S1);
    }

    void add (SetState &S1){
        for (int i: S1.main_information()){
            DiscreteState S2 (i);
            add(S2);
        }
    }

    std::set<int> main_information() const{
        return (std::set<int> {0});
    }

    bool contains(int s) const {
        bool flag = false;
        for (DiscreteState i: set_discrete){
            if (i.contains(s)){
                flag = true;
            }
        }
        for (SegmentState i: set_segment){
            if (i.contains(s)){
                flag = true;
            }
        }
        return flag;
    }
};

Set_of_States operator+ (DiscreteState &S1, DiscreteState &S2){
    std::vector<DiscreteState> res_set;
    if (S1.main_information() != S2.main_information()){
        res_set.push_back(S1);
        res_set.push_back(S2);
    } else {
        res_set.push_back(S1);
    }
    return Set_of_States(res_set);
}

Set_of_States operator+ (SegmentState &S1, SegmentState &S2){
    std::vector<SegmentState> res_set = {S1, S2};
    return Set_of_States(res_set);
}

Set_of_States operator+ (SetState &S1, SetState &S2){
    Set_of_States res;
    res.add(S1);
    res.add(S2);
    return res;
}

Set_of_States operator+ (DiscreteState &S1, SegmentState &S2){
    std::vector<DiscreteState> S1_set = {S1};
    std::vector<SegmentState> S2_set = {S2};
    return Set_of_States(S1_set, S2_set);
}



class ProbabilityTest {
private:
    unsigned seed;
    int test_min, test_max;
    unsigned test_count;

public:
    ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count): seed(seed), test_min(test_min),test_max(test_max), test_count(test_count) { }

    float operator()(State const &s) const {
        std::default_random_engine rng(seed);
        std::uniform_int_distribution<int> dstr(test_min,test_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != test_count; ++cnt)
            if (s.contains(dstr(rng))) ++good;

        return static_cast<float>(good)/static_cast<float>(test_count);
    }
};

int main(int argc, const char * argv[]) {
    Set_of_States states_to_check;
    int ceil_test = 100;
    int count_numbers = 0;
    int step = 6;
    int i = 0;
    while (i < ceil_test){
        DiscreteState one_state(i);
        states_to_check.add(one_state);
        count_numbers++;
        i += step;
    }
    std::ofstream out_file;
    out_file.open("./ord_discrete_set_first.txt");
    i = 1;
    ProbabilityTest pt(10,0,100,10000);
    std::cout << pt(states_to_check);
    /*
    while (i < 1000000){
        ProbabilityTest pt(10,0,100,i);
        out_file << pt(states_to_check) << std::endl;
        i += 10;
    }
    */

    return 0;
}

