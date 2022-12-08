#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <deque>
#include <utility>

using namespace std;
class TopoSort {
public:
    //topological sort, time: O(N+E) N is node size, and E is edge size
    vector<int> sort(int n, const vector<pair<int, int>>& edges) {
        vector<int> res;
        unordered_map<int, unordered_set<int>> graph;
        vector<int> indegrees(n);
        for(const auto& p : edges) {
            indegrees[p.second]++;
            graph[p.first].insert(p.second);
        }
        //BFS
        deque<int> deq;
        for(int i=0; i<indegrees.size(); i++) {
            if(!indegrees[i])
                deq.push_back(i);
        }
        while(!deq.empty()) {
            auto cur = deq.front();
            deq.pop_front();
            res.push_back(cur);
            for(auto i : graph[cur]) {
                indegrees[i]--;
                if(!indegrees[i])
                    deq.push_back(i);
            }
        }
        if(res.size()<n) //check cycles
            res.clear();
        return res;
    }
};


void print_topo_sort(int n, const vector<pair<int,int>>& edges) {
    TopoSort topo;
    auto sorted = topo.sort(n,edges);
    cout << "Topological sort: ";
    if(!sorted.size())
        cout << "circular graph detected" << endl;
    else {
        for(const auto& i : sorted)
            cout << i << " ";
        cout << endl;
    }
}

int main(int argc, char **argv) {
    int n = 7; 
    vector<pair<int, int>> edges = {
        {0,3}, {0,5}, 
        {1,5}, {2,5},
        {3,4}, {3,5},
        {4,6}, {5,4}
    };
    print_topo_sort(n, edges);

    vector<pair<int, int>> edges2 = {
        {0,3}, {0,5}, 
        {1,5}, {2,5},
        {3,4}, {3,5},
        {4,6}, {5,4}, {5,1}
    };
    print_topo_sort(n, edges2);

    return 0;
}
