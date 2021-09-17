#include <iostream>
#include <vector>
#include <queue>

//Graph: конструктор по матрице, get_matrix
//Написать бфс, дфс
// Написать вывод компонент связности.

class Graph{
private:
    unsigned int amount_vert;
    std::vector <std::vector <bool>> neighbours_matrix;

public:
    Graph(){
        amount_vert = 0;
    }

    Graph(std::vector <std::vector <bool>> initial_matrix){
        int size_lines = initial_matrix.size();
        bool flag = true; //if everything is OK, the flag stays true;
        for (int i = 0; i < size_lines; i++){
            //It is the checking that matrix is squared;
            if (size_lines != initial_matrix[i].size()){
                flag = false;
            }
        }
        if (flag){
            amount_vert = size_lines;
            neighbours_matrix.insert(neighbours_matrix.begin(), initial_matrix.begin(), initial_matrix.end());
        }
    }

    void add_edge(int vert_1, int vert_2, bool orient = false){
        //add the edge between vert_1 and vert_2;
        //if orient = true, the edge will be oriented from vert_1 to vert_2;
        neighbours_matrix[vert_1][vert_2] = true;
        if ((!orient) && (vert_1 != vert_2)){
            neighbours_matrix[vert_2][vert_1] = true;
        }
    }

    void add_vert(std::vector<bool> edges, bool orient = false){
        //the edge[i] = true means that new vert is connected with vert i;
        if (edges.size() == amount_vert){
            //there is not item new vert to new vert, so will add this;
            edges.push_back(false);
        }
        neighbours_matrix.push_back(edges);
        for (int i = 0; i < amount_vert; i++){
            neighbours_matrix[i].push_back(false);
        }
        if (!orient){
            for (int i = 0; i < edges.size(); i++){
                if (edges[i]){
                    add_edge(i, amount_vert, true);
                }
            }
        }
        amount_vert++;
    }

    std::vector<std::vector <bool>> get_matrix(){
        return neighbours_matrix;
    }

    void print_matrix(){
        std::cout << amount_vert << std::endl;
        for (int i = 0; i < amount_vert; i++){
            for (int j = 0; j < amount_vert; j++){
                if (neighbours_matrix[i][j]){
                    std::cout << 1 << " ";
                } else {
                    std::cout << 0 << " ";
                }
            }
            std::cout << std::endl;
        }
    }

    int size_graph(){
        return amount_vert;
    }

};

class I_Visitor_Concept{
public:
    virtual void initialize_vertex(int vert_number, Graph &G) = 0;
    //This is invoked on every vertex of the graph before the start of the graph search.
    virtual void start_vertex(int vert_number, Graph &G) = 0;
    //	This is invoked on the source vertex once before the start of the search.
    virtual void discover_vertex(int vert_number, Graph &G) = 0;
    // This is invoked when a vertex is encountered for the first time.
    virtual void examine_edge(int vert_from,int vert_to, Graph &G) = 0;
    //This is invoked on every out-edge of each vertex after it is discovered.
    virtual void tree_edge(int vert_from, int vert_to, Graph &G) = 0;
    //This is invoked on each edge as it becomes a member of the edges that form the search tree.
    virtual void back_edge(int vert_from, int vert_to, Graph &G) = 0;
    //This is invoked on the back edges in the graph.
    virtual void forward_or_cross_edge(int vert_from, int vert_to, Graph &G) = 0;
    //This is invoked on forward or cross edges in the graph. In an undirected graph this method is never called.
    virtual void finish_edge(int vert_from, int vert_to, Graph &G) = 0;
    //This is invoked on each non-tree edge as well as on each tree edge after finish_vertex has been called on its target vertex.
    virtual void finish_vertex(int vert_number, Graph &G) = 0;
    //This is invoked on vertex u after finish_vertex has been called for all the vertices in the DFS-tree rooted at vertex u.

    virtual ~I_Visitor_Concept(){
    };
};

class DFS_visitor: public I_Visitor_Concept{
private:
    std::vector <int> coloured; // fill with 0 - white, 1 - black, 2 - gray;
    std::vector <int> path;

    void DFS_visitor_algorithm(int vertex_start_from, Graph &G){
        std::cout << vertex_start_from << " ";
        discover_vertex(vertex_start_from, G);
        for (int i = 0; i < G.size_graph(); i++){
            if (G.get_matrix()[vertex_start_from][i]){
                examine_edge(vertex_start_from, i, G);
            }
        }
        finish_vertex(vertex_start_from, G);
    }

public:
    DFS_visitor(Graph &G){
        path = std::vector<int> (G.size_graph());
        coloured = std::vector<int> (G.size_graph());
        for (int i = 0; i < G.size_graph(); i++){
            initialize_vertex(i, G);
        }
    }

    DFS_visitor(int vertex_start_from, Graph &G): DFS_visitor(G){
        start_vertex(vertex_start_from, G);
    }


    void initialize_vertex(int amount_vert, Graph &G){
        coloured[amount_vert] = 0;
        path[amount_vert] = amount_vert;
    }

    void start_vertex(int vert_number, Graph &G){
        if (coloured[vert_number] == 0){
            DFS_visitor_algorithm(vert_number, G);
        }
    }

    void discover_vertex(int vert_number, Graph &G){
        coloured[vert_number] = 2;
    }

    void examine_edge(int vert_from, int vert_to, Graph &G){
        if (coloured[vert_to] == 0){
            tree_edge(vert_from, vert_to, G);
        } else if (coloured[vert_to] == 2){
            back_edge(vert_from, vert_to, G);
        } else if (coloured[vert_to] == 1){
            forward_or_cross_edge(vert_from, vert_to, G);
        }
    }

    void tree_edge(int vert_from, int vert_to, Graph &G){
        path[vert_to] = vert_from;
        start_vertex(vert_to, G);
    }

    void back_edge(int vert_from, int vert_to, Graph &G){
        int a = 0;
        //Just doing nothing;
    }

    void forward_or_cross_edge(int vert_from, int vert_to, Graph &G){
        int c = 0;
        //Just doing nothing;
    }

    void finish_edge(int vert_from, int vert_to, Graph &G){
        int b = 0;
        //Just doing nothing;

    }

    void finish_vertex(int vert_number, Graph &G){
        coloured[vert_number] = 1;
    }

    std::vector<int> what_visited(){
        return coloured;
    }

    ~DFS_visitor(){
        coloured.clear();
        path.clear();
    }
};

void connected_parts(Graph &G){
    bool flag = true;
    DFS_visitor DFS(G);
    while (flag){
        flag = false;
        int key;
        for (int i = 0; i < G.size_graph(); i++){
            if (DFS.what_visited()[i] == 0){
                flag = true;
                key = i;
            }
        }
        if (flag){
            DFS.start_vertex(key, G);
            std::cout << std::endl;
        }
    }
}

class BFS_visitor: public I_Visitor_Concept{
private:
    std::queue<int> que_vert;
    std::vector<int> coloured;
    std::vector<int> path;

    BFS_visitor_algorithm(int vertex_start_from, Graph &G){
        discover_vertex(vertex_start_from, G);
        while (!que_vert.empty()){
            int current_edge = que_vert.front();
            std::cout << current_edge << std::endl;
            for (int i = 0; i < G.size_graph(); i++){
                if (G.get_matrix()[current_edge][i]){
                    examine_edge(current_edge, i, G);
                }
            }
            finish_vertex(current_edge, G);
        }
    }

public:

    BFS_visitor(Graph &G){
        path = std::vector<int> (G.size_graph());
        coloured = std::vector<int> (G.size_graph());
        for (int i = 0; i < G.size_graph(); i++){
            initialize_vertex(i, G);
        }
    }

    BFS_visitor(int start_from_vert, Graph &G): BFS_visitor(G){
        start_vertex(start_from_vert, G);
    }

    void initialize_vertex(int vert_number, Graph &G){
        coloured[vert_number] = 0;
        path[vert_number] = vert_number;
    }

    void start_vertex(int vert_number, Graph &G){
        if (coloured[vert_number] == 0){
            BFS_visitor_algorithm(vert_number, G);
        }
    }

    void discover_vertex(int vert_number, Graph &G){
        que_vert.push(vert_number);
        coloured[vert_number] = 2;
    }

    void examine_edge(int vert_from,int vert_to, Graph &G){
        if (coloured[vert_to] == 0){
            tree_edge(vert_from, vert_to, G);
        } else if (coloured[vert_to] == 2){
            back_edge(vert_from, vert_to, G);
        } else if (coloured[vert_to] == 1){
            forward_or_cross_edge(vert_from, vert_to, G);
        }
    }

    void tree_edge(int vert_from, int vert_to, Graph &G){
        path[vert_to] = vert_from;
        discover_vertex(vert_to, G);
    }

    void back_edge(int vert_from, int vert_to, Graph &G){
        int a = 0;
        //Just doing nothing;
    }

    void forward_or_cross_edge(int vert_from, int vert_to, Graph &G){
        int c = 0;
        //Just doing nothing;
    }

    void finish_edge(int vert_from, int vert_to, Graph &G){
        int b = 0;
        //Just doing nothing;
    }

    void finish_vertex(int vert_number, Graph &G){
        coloured[vert_number] = 1;
        que_vert.pop();
    }

    ~BFS_visitor(){
        coloured.clear();
        path.clear();
    }
};

int main(){
    std::vector <std::vector <bool>> R;
    for (int i = 0; i < 5; i++){
        R.push_back(std::vector<bool> (5));
    }
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
            bool flag;
            std::cin >> flag;
            R[i][j] = flag;
        }
    }
    Graph G(R);
    BFS_visitor BFS(2, G);
    return 0;
}
