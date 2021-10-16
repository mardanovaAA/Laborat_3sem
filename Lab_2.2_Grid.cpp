#include <iostream>
#include <memory>

template <class T>
class Grid{
    private:
        size_t x_size;
        size_t y_size;
        T* data;
    public:
        Grid(size_t x_size, size_t y_size): x_size(x_size), y_size(y_size){
            if ((x_size == 0) || (y_size == 0)){
                std::cerr << "Be careful with sizes";
            } else {
                data = new T[x_size * y_size];
            }
        };

        Grid(): Grid(3,3){};

        Grid(const Grid<T>& grid) = delete;

        Grid (Grid<T>&& grid): x_size(grid.x_size), y_size(grid.y_size){
            std::swap(data, grid.data);
            grid.data = nullptr;
            grid.x_size = 0;
            grid.y_size = 0;
        }

        T  operator()(size_t x_idx, size_t y_idx) const {
            if ((x_idx > x_size) || (y_idx > y_size)){
                std::cerr << "Be careful with sizes";
            } else {
                T res = data[x_idx * this->y_size + y_idx];
                return res;
            }
        };

        T& operator()(size_t x_idx, size_t y_idx){
            if ((x_idx > x_size) || (y_idx > y_size)){
                std::cerr << "Be careful with sizes";
            } else {
                T& res = data[x_idx * this->y_size + y_idx];
                return res;
            }
        };

        size_t get_xsize() const{
            return x_size;
        };

        size_t get_ysize() const{
            return y_size;
        };

        Grid<T>& operator= (T val){
            for (int i = 0; i < x_size * y_size; i++){
                data[i] = val;
            }
            return *this;
        };

        Grid<T>& operator= (const Grid<T>& grid) = delete;

        Grid<T>& operator= (Grid<T>&& grid){
            std::unique_ptr<Grid> ptr_grid (grid);
            if (ptr_grid == this){
                return *this;
            }
            if ((x_size != grid.x_size) || (y_size != grid.y_size)){
                std::cerr << "Be careful with sizes";
            } else {
                std::swap(data, grid.data);
                grid.data = nullptr;
            }
            return *this;

        }

        template <typename U>
        friend std::ostream& operator<<(std::ostream& out, Grid<U> const& grid2print);

        template <typename U>
        friend std::istream& operator>>(std::istream& in, Grid<U> &grid);

        ~Grid(){
            if (data != nullptr){
                delete [] data;
                data = nullptr;
            }
        }
};


template <typename U>
std::ostream& operator<<(std::ostream& out, Grid<U> const& grid2print){
    for (int i = 0; i < (grid2print.x_size); i ++){
        for (int j = 0; j < (grid2print.y_size); j++){
            out << grid2print.data[i * (grid2print.y_size) +  j] << " ";
        }
        out << "\n";
    }
    return out;
};

template <typename U>
std::istream& operator>>(std::istream& in, Grid<U> &grid){
    for (int i = 0; i < grid.x_size * grid.y_size; i++){
        in >> grid.data[i];
    }
    return in;
};

int main(){
    Grid<int> A;
    A = 5;
    Grid<int> B (std::move(A));
    std::cout << B << std::endl;
    return 0;
}
