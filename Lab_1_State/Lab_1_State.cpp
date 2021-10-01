#include <iostream>
#include <set>
#include <random>
#include <fstream>
#include <vector>

//Дописать friend операторы, посчитать дисперсию, залесть в seed

class State {
private:

public:
    virtual bool contains(int s) const = 0;
    virtual std::set<int> main_information () const = 0;

    virtual ~State(){};
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

class DiscreteState: public SetState {
private:

public:
    DiscreteState(int state): SetState(std::set <int> {state}) { };
};

class Set_of_States: public State{
    private:
        std::vector<DiscreteState> set_discrete;
        std::vector<SegmentState> set_segment;

    protected:
         bool existence (int s, std::vector<auto> vec_to_revise) const {
            bool res = false;
            for (auto i: vec_to_revise){
                if (i.contains(s)){
                    res = true;
                }
            }
            return res;
        }

    public:
        Set_of_States(): set_discrete(), set_segment(){};

        Set_of_States(std::vector<DiscreteState> &set_states): set_discrete(set_states){};

        Set_of_States(std::vector<SegmentState> &set_states): set_segment(set_states){};

        Set_of_States(std::vector<DiscreteState> &set_states1, std::vector<SegmentState> &set_states2):  set_segment(set_states2) {
            for (DiscreteState i: set_states1){
                add(i);
            }
        }

        void add (SegmentState &S1){
            set_segment.push_back(S1);
        }

        void add (SetState &S1){
            for (int i: S1.main_information()){
                if ((!existence(i, set_discrete)) && (!existence(i, set_segment))) {
                    set_discrete.push_back(i);
                }
            }
        }

        std::set<int> main_information() const{
            return (std::set<int> {0});
        }

        bool contains(int s) const {
            return ((existence(s, set_discrete)) || (existence(s, set_segment)));
        }

        friend Set_of_States operator- (SegmentState &S1, SetState &S2);

        friend Set_of_States operator+ (SegmentState &S1, SetState &S2);

        friend Set_of_States operator+ (Set_of_States &S1, SetState &S2);

};

Set_of_States operator- (SegmentState &S1, SetState &S2){
    std::vector<SegmentState> res_set;
    int start_segment = *S1.main_information().begin();
    for (int i: S2.main_information()){
        if (S1.contains(i)){
            res_set.push_back(SegmentState(start_segment, i));
            start_segment = i;
        }
    }
    if (*S1.main_information().rbegin() != start_segment){
        res_set.push_back(SegmentState(start_segment, *S1.main_information().rbegin()));
    }
    return Set_of_States(res_set);
}

Set_of_States operator+ (SegmentState &S1, SetState &S2){
    std::vector<DiscreteState> res_set;
    for (int i: S2.main_information()){
        if (!S1.contains(i)){
            res_set.push_back(DiscreteState(i));
        }
    }
    std::vector<SegmentState> res_seg = {S1};
    return Set_of_States(res_set, res_seg);
}

Set_of_States operator+ (Set_of_States &S1, SetState &S2){
    S1.add(S2);
    return S1;
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
    DiscreteState d(1);
    SegmentState s(0,10);
    SetState ss({1, 3, 5, 7, 23, 48, 57, 60, 90, 99});
    std::ofstream out_file;
    out_file.open("./set_state_2.txt");
    int i = 10;
    while (i < 1000){
        ProbabilityTest pt(10,0, i,10000);
        out_file << i << " " << pt(ss) << "\n";
        i += 1;
    }
    out_file.close();

    return 0;
}

