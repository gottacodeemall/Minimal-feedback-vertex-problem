#include<bits/stdc++.h>
using namespace std;
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}
//Code by Ammanamanchi Sai Karthik
//B150310CS National Institute Of Technology Calicut

typedef int Node;
typedef std::pair<int,int> Edge;
typedef std::pair<std::map<std::string, Node>, std::map<Node, std::string> > Mapping;
typedef std::unordered_set<Node> Neighborhood;
typedef std::unordered_map<Node, Neighborhood> AdjacencyList;

#define INVALID_NODE -1

class Graph {

public:
          bool assign_names(const Mapping &m){
            this->mapping = m;
              return true;
          };

          bool has_node(const Node v) const { return adj.find(v) != adj.end(); }
          bool has_edge(const Node u, const Node v) const { return (has_node(u)) && (adj.find(u)->second.find(v) != adj.find(u)->second.end()); }
          bool has_edge(const Edge &e) const { return has_edge(e.first, e.second); }

          long  get_n() const{
            return n;
          };
          long get_m() const{
            return m;
          };

          AdjacencyList get_adjacency_list() const {
            return adj;
          }

          std::string get_node_name(const Node u)const {
             if(mapping.second.find(u) != mapping.second.end()) {
                return mapping.second.find(u)->second;
             } else {
                return "["+patch::to_string(u)+"]";
             }
          }

          void print_tidy() const {
            std::cout << "Number of nodes: " << get_n() << std::endl;
            std::cout << "Number of edges: " << get_m() << std::endl;
            for (const auto &it : get_adjacency_list()) {
              for (const auto &eit : it.second) {
                if(eit > it.first)
                  std::cout << get_node_name(it.first) << " " << get_node_name(eit) << std::endl;
              }
            }
          }

          Graph() {
            n = m = 0;
          }

          bool has_cycle() const {
              if(m >= n && n > 0) {
                return true;
              }
              std::string tmp = "Has_Cycle: Don't use heuristic, because m [="+patch::to_string(m)+"] < n [="+patch::to_string(n)+"]: DFS ";
              //number of edges must be less than the number of nodes
              // trying to find a back edge in DFS if found there exists a cycle
              std::unordered_set<Node> done = std::unordered_set<Node>();
              std::stack<std::pair<Node, Node> > S;
              for(const auto &it : adj) {
                if(done.find(it.first) == done.end()) {
                  S.push(std::make_pair(it.first, INVALID_NODE));
                  done.insert(it.first);
                  while(!S.empty()) {
                    Node v = S.top().first;
                    Node f = S.top().second;
                    S.pop();
                    for(const auto &neighbor : adj.find(v)->second) {
                      if(done.find(neighbor) == done.end()) {
                        S.push(std::make_pair(neighbor, v));
                        done.insert(neighbor);
                      } else {
                        if(neighbor != f) {
                          return true;
                        }
                      }
                    }
                  }
                }
              }
              return false;
          }

          bool add_node(const Node u) {
            if(has_node(u)) {

              return false;
            }
            adj[u] = Neighborhood();
            ++n;

            return true;
          }

          bool add_edge(const Node u, const Node v) {
              if(u==v) {
                  return false;
              }
              if(!has_edge(u,v)) {
                add_node(u);
                add_node(v);
                adj[u].insert(v);
                adj[v].insert(u);
                ++m;
                return true;
              } else {
                return false;
              }
          }

          void remove_edges(const std::set<Edge> &E) {
            std::unordered_set<Node> to_update;
            for(const auto &e : E) {
              if(e.first == e.second) continue;
              if(e.first == INVALID_NODE || e.second == INVALID_NODE) continue;
              if(!has_edge(e)) continue;
              Node u = e.first;
              Node v = e.second;

              adj[u].erase(v);
              adj[v].erase(u);
              to_update.insert(u);
              to_update.insert(v);
              --m;
            }
          }

          bool remove_node(const Node u) {
            if(!has_node(u)) {
              return false;
            }

            std::set<Edge> to_remove;
            for(Neighborhood::const_iterator it = adj[u].begin(); it != adj[u].end(); ++it) {
                to_remove.insert(std::make_pair(*it, u));
            }
            remove_edges(to_remove);

            adj[u].clear();
            adj.erase(u);
            --n;
            return true;
          }

          void print() const {
            std::cout << "Number of nodes: " << get_n() << std::endl;
            std::cout << "Number of edges: " << get_m() << std::endl;
            if(get_m() > 1000 || get_n() > 500) {
              std::cout << "Graph too big, skipping complete printing."<<std::endl;
              return;
            }
            for (const auto &it : get_adjacency_list()) {
              std::cout << "Edges outgoing from " << get_node_name(it.first) << ":" << std::endl;
              for (const auto &eit : it.second) {
                std::cout << get_node_name(it.first) << " -> " << get_node_name(eit) << std::endl;
              }
            }
            std::cout << "---------------------------" << std::endl;

          }

      private:
          AdjacencyList adj;
          Mapping mapping;
          int n;
          int m;
};

class GraphData {
  public:
    Graph graph;
    std::set<Node> necessary_nodes;
    Mapping mapping;
};

GraphData read_graph() {
  GraphData result;
  int current_node_id = 0;

  string line;
  while (getline(cin, line)) {
    if(line.empty()) {
         cout << "Empty line."<<endl;
        continue;
    }
    if(line.at(0) == '#') {
         cout << "Got comment: "<<line<<endl;
        continue; // Comments
    }
    istringstream iss(line);

    char src[50], dst[50];
    int x = sscanf(line.c_str(), "%s %s", src, dst);
    if(x != 2) {
      throw std::runtime_error("Can't parse file. Wrong format?");
    }

    string src_str(src), dst_str(dst);

    Node s, t;
    if(result.mapping.first.find(src_str) == result.mapping.first.end()) {
      result.mapping.first[src_str] = current_node_id;
      result.mapping.second[current_node_id] = src_str;
      s = current_node_id;
      ++current_node_id;
    } else {
      s = result.mapping.first[src_str];
    }

    if(result.mapping.first.find(dst_str) == result.mapping.first.end()) {
      result.mapping.first[dst_str] = current_node_id;
      result.mapping.second[current_node_id] = dst_str;
      t = current_node_id;
      ++current_node_id;
    } else {
      t = result.mapping.first[dst_str];
    }
//self loop
    if(s == t) {
      result.necessary_nodes.insert(s);
    } else {
      result.graph.add_edge(s, t);
    }
  }

  // remove all nodes having self loops after inserting all edges
  for(const auto &it : result.necessary_nodes) {
    result.graph.remove_node(it);
  }

  return result;
}

int main(){
  clock_t tStart = clock();
      //std::ifstream in("in.txt");
      //std::cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!
      //std::ofstream out("out.txt");
      //std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
	GraphData graph_data = read_graph();
  	//cout<<"reading complete"<<endl;
    //graph_data.graph.print();
    if(graph_data.graph.get_n() == 0) {
        cout << "Edge case: No not-necessary vertices. Outputting necessary vertices only."<<endl;
       for(const auto &it : graph_data.necessary_nodes) {
          cout << graph_data.mapping.second[it] << endl;
       }
       return 0;
    }

    long numnodes=graph_data.graph.get_n();
    long count=0;
    vector<string> v;
    for(long i=0;i<pow(2,numnodes);i++){
      Graph g=graph_data.graph;
      Mapping node_names=graph_data.mapping;
      g.assign_names(node_names);
      Neighborhood vertexset;
      vertexset.clear();
      for(long j=0;j<numnodes;j++){
        if(i&1<<j){
          g.remove_node(j);
          vertexset.insert(j);
        }
      }
      if(g.has_cycle()==0){
        for(Neighborhood::const_iterator it = vertexset.begin(); it != vertexset.end(); ++it){
          v.push_back(node_names.second[(*it)]);
          count++;
        }
        break;
      }

    }
    cout<<"size of the minimum vertex cover is "<<count<<endl;
    cout<<"{ ";
    for(auto i:v){
    	cout<<i<<"  ";
    }
    cout<<"}"<<endl;

    cout<<"Execution Time = "<<((double)(clock() - tStart)/CLOCKS_PER_SEC)<<endl;

}
