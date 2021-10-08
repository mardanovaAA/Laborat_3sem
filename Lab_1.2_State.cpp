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

        friend std::ostream& operator<< (std::ostream &out, const State &to_print);

        virtual ~State(){};
};

std::ostream& operator<< (std::ostream &out, const State &to_print){
    for (int i: to_print.main_information()){
        out << i << " ";
    }
    out << std::endl;
    return out;
}


class SegmentState: public State {
    private:
        int const beg, end;

    public:
        SegmentState(): beg(0), end(-1) { };
        SegmentState(int beg, int end): beg(beg), end(end) { };

        std::set<int> main_information() const {
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

        std::set<int> main_information() const {
            return states;
        }
};

class DiscreteState: public SetState {
    private:

    public:
        DiscreteState(int state): SetState(std::set <int> {state}) {};
};

class Segments_minus_DiscreteSet_State: virtual public SegmentState, virtual public SetState{
    private:

    public:
        Segments_minus_DiscreteSet_State(int beg, int end, std::set<int> dis_set): SetState(dis_set), SegmentState(beg, end){};

        friend std::ostream& operator<< (std::ostream &out, const Segments_minus_DiscreteSet_State &to_print);

        std::set<int> main_information() const{
            std::set<int> res;
            res.insert(*SegmentState::main_information().begin());
            for (int i: SetState::main_information()){
                if (SegmentState::contains(i)){
                    res.insert(i);
                }
            }
            res.insert(*SegmentState::main_information().rbegin());
            return res;
        }

        bool contains(int s) const{
            return ((SegmentState::contains(s))&&(!SetState::contains(s)));
        }
};

std::ostream& operator<< (std::ostream &out, const Segments_minus_DiscreteSet_State &to_print){
    out << "(" << *to_print.SegmentState::main_information().begin() << ", ";
    for (int i: to_print.SetState::main_information()){
        if (to_print.SegmentState::contains(i)){
            out << i << ") (" << i << ", ";
        }
    }
    out << *to_print.SegmentState::main_information().rbegin() << ")" << std::endl;
    return out;
}

class Segments_plus_DiscreteSet_State: virtual public SetState, virtual public SegmentState{
private:

public:
    Segments_plus_DiscreteSet_State(int beg, int end, std::set<int> dis_set): SetState(dis_set), SegmentState(beg, end) {};

    friend std::ostream& operator<< (std::ostream &out, const Segments_plus_DiscreteSet_State &to_print);

    bool contains(int s) const{
        return ((SegmentState::contains(s))||(!SetState::contains(s)));
    }
};

std::ostream& operator<< (std::ostream &out, const Segments_plus_DiscreteSet_State &to_print){
    out << "(" << *to_print.SegmentState::main_information().begin() << ", " << *to_print.SegmentState::main_information().rbegin() << ") ";
    for (int i: to_print.SetState::main_information()){
        if (!to_print.SegmentState::contains(i)){
            out << i << " ";
        }
    }
    out << std::endl;
    return out;
}

class Segments_plus_minus_DiscreteSet_State: virtual public SegmentState, virtual public SetState{
    private:

    protected:
        SetState dis_set_to_min;
        SetState dis_set_to_plus;

    public:
        Segments_plus_minus_DiscreteSet_State(int beg, int end, std::set<int> dis_set_to_min, std::set<int> dis_set_to_plus):
        SegmentState(beg, end), dis_set_to_min(dis_set_to_min), dis_set_to_plus(dis_set_to_plus) {};

        friend std::ostream& operator<< (std::ostream &out, const Segments_plus_minus_DiscreteSet_State &to_print);

        bool contains(int s) const{
            return (((SegmentState::contains(s)) && (!dis_set_to_min.contains(s))) || (dis_set_to_plus.contains(s)));
        }
};

std::ostream& operator<< (std::ostream &out, const Segments_plus_minus_DiscreteSet_State &to_print){
    for (int i: to_print.dis_set_to_plus.main_information()){
        if (!to_print.SegmentState::contains(i)){
            out << i << " ";
        }
    }
    out << Segments_minus_DiscreteSet_State(*to_print.SegmentState::main_information().begin(),
                                            *to_print.SegmentState::main_information().rbegin(), to_print.dis_set_to_min.main_information());
    return out;
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
    ProbabilityTest pt(10,0,100,100000);
    Segments_plus_minus_DiscreteSet_State try_test (3, 7, {4}, {5, 9});
    std::cout << pt(d) << std::endl;
    std::cout << pt(s) << std::endl;
    std::cout << pt(ss) << std::endl;
    std::cout << d;
    std::cout << s;
    std::cout << ss;
    std::cout << try_test;

    return 0;
}


