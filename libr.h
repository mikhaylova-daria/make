#ifndef LIBR_H
#define LIBR_H
#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <list>
#include <queue>
#include <vector>
using namespace std;
template <typename U> struct Edge;
template <typename T> struct Vetex;
typedef std::set <int> list_adjacency; // список - сет id смежных вершин



template <typename U>
struct Edge {
    int id;
    int first_v;
    int second_v;
    U weight;
    Edge(){;}
    Edge (int _first_v, int _second_v, U _weight):id(((_first_v + _second_v) * (_first_v + _second_v) + 3 * _first_v + _second_v)/2 ),
                                                                            first_v(_first_v), second_v(_second_v), weight( _weight){;}
    bool operator == (const Edge& edg) const{ return (id == edg.id); }
    bool operator < (const Edge &edg) const { return (id < edg.id); }
    Edge & operator = (const Edge & edg){
        id = edg.id;
        first_v = edg.first_v;
        second_v = edg.second_v;
        weight = edg.weight;
        return (*this);
    }
    friend std::istream& operator >> (std::istream& istr, Edge<U> & edr){
        istr >> edr.first_v >> edr.second_v >> edr.weight;
        edr.id = ((edr.first_v + edr.second_v)* (edr.first_v + edr.second_v) + 3 * edr.first_v + edr.second_v)/2 ;
        return istr;
    }

    friend std::ostream& operator << (std::ostream& ostr, Edge<U> const & edr) {
        ostr <<"№"<< edr.id << " " << edr.first_v << "-" << edr.second_v <<" : "<< edr.weight<<'\n';
        return ostr;
    }

};

template <typename T>
struct Vetex {
    int id;
    T value;
    list_adjacency list_as_start;
    list_adjacency list_as_finish;
    Vetex(){;}
    Vetex(int _id, T _value): id(_id), value(_value){;}
    bool operator == (const Vetex & vtx) const { return (id == vtx.id); }
    bool operator < (const Vetex & vtx) const { return (id < vtx.id); }

    Vetex & operator = (const Vetex & vtx){
        id = vtx.id;
        value = vtx.value;
        list_as_start = vtx.list_as_start;
        list_as_finish = vtx.list_as_finish;
        return (*this);
    }
    friend std::istream& operator >>  (std::istream& istr, Vetex<T> & vtx){
        istr >> vtx.id >> vtx.value;
        return istr;
    }

    friend std::ostream& operator << (std::ostream& ostr, Vetex<T> const & vtx)  {
        ostr << vtx.id << " (" << vtx.value << ")\n";
        return ostr;
    }
};


template < typename T, typename U> class Graph
{

    map <int, Vetex<T> > map_vtx;// ключ id - значение - id, вес вершины список смежности
    map <int, Edge<U> > map_edg; // id ребра, начала, конца, вес

public:
    class iterator_BFS;
    class iterator_DFS;
    Graph(){;}

    bool add_vtx(const Vetex <T> &_vtx);
    bool add_edg(const Edge<U> & _edg);
    bool rem_vtx (const int & _id);
    void rem_edg (const int &_id_first, const int &_id_second);
    void transpose();
    std::list<Vetex<T> > get_vetices();
    std::list<Vetex<T> > get_vetices_outbox(int);
    std::list<Vetex<T> > get_vetices_inbox(int);
    std::list<Edge<U> > get_edges();
    std::list<Edge<U> > get_edges_inbox( int);
    std::list<Edge<U> > get_edges_outbox(int);
    friend std::istream& operator >>  (std::istream& istr, Graph<T, U> & gr){
        std::cout<<"Введите количество вершин, затем вводите вершины в формате : индекс, значение через пробел:\n";
        int n;
        istr>>n;
        for (int i = 0; i < n; ++i) {
            Vetex<T> vtx;
            istr >> vtx;
            try {
                gr.add_vtx(vtx);
            } catch (const char * &a){
                cerr<<a<<" : try again\n";
                --i;
            }
        }
        std::cout<<"Введите количество рёбер, затем рёбра в формате: номер вершины начала, номер вершины к конца и вес через пробел\n";
        istr>>n;
        for (int i = 0; i < n; ++i) {
            Edge<U> edg;
            istr >> edg;

            try {
                gr.add_edg(edg);
            } catch (const char * &a){
                cerr<<a<<" : try again\n";
                --i;
            }
        }
        return istr;
    }

    friend std::ostream& operator << (std::ostream& ostr,const Graph<T, U> & gr)  {
        typename map  <int, Edge<U> > ::const_iterator itr;
        for (itr = gr.map_edg.begin(); itr != gr.map_edg.end(); ++itr) {
            typename map<int, Vetex<T> >::const_iterator itr_1;
            itr_1 = gr.map_vtx.find((*itr).second.first_v);
            typename map<int, Vetex<T> >::const_iterator itr_2;
            itr_2 = gr.map_vtx.find((*itr).second.second_v);
            T value_1 = (*itr_1).second.value;
            T value_2 = (*itr_2).second.value;
            ostr <<"№"<< (*itr).second.id << " " << (*itr).second.first_v <<'('<<value_1<<')'<< "-" << (*itr).second.second_v <<
                                                                          '('<<value_2<<')'<<" : "<< (*itr).second.weight<<'\n';
        }
        typename map <int, Vetex<T> >::const_iterator itr_vtx;
        for (itr_vtx = gr.map_vtx.begin(); itr_vtx != gr.map_vtx.end(); ++itr_vtx){
            if ((*itr_vtx).second.list_as_start.empty() && (*itr_vtx).second.list_as_finish.empty()) {
                ostr << "free_vertex :" << (*itr_vtx).second;
            }
        }
        return ostr;
    }
};
    //Добавление вершины
template < typename T, typename U>
    bool Graph<T, U>::add_vtx(const Vetex <T> &_vtx) {
        std::pair<int, Vetex<T> > vtx;
        vtx.first = _vtx.id;
        vtx.second = _vtx;
        if (map_vtx.insert(vtx).second != true){
            throw ("recurring_id");
            return false;
        }
        return true;
    }

    //Добавление ребра
template < typename T, typename U>
     bool Graph<T, U>::add_edg (const Edge<U> & _edg) {
        pair<int, Edge<U> > edg;
        edg.first = _edg.id;
        edg.second = _edg;
        typename map<int, Vetex<T> >::iterator itr_1, itr_2;
        itr_1 = map_vtx.find(edg.second.first_v);
        itr_2 = map_vtx.find(edg.second.second_v);
        if (itr_1 == map_vtx.end() || itr_2 == map_vtx.end()) {
            throw ("unknown_vetex");
        }
        if ((itr_1->second.list_as_start.insert(edg.second.second_v)).second == false) { //вставили в список для вершины начала вторую вершину, кратные рёбра запрещены
            throw ("recurring_id");
        }
        if ((itr_2->second.list_as_finish.insert(edg.second.first_v)).second == false) { //вставили в список для вершины начала вторую вершину, кратные рёбра запрещены
            throw ("recurring_id");
        }
        map_edg.insert(edg).second; // добавили в мэп графов
        return true;
    }

     // Удаление вершины
template < typename T, typename U>
     bool Graph<T, U>::rem_vtx (const int & _id) {
         typename map <int, Vetex<T> >::iterator itr_id;
         itr_id = map_vtx.find(_id);
         if (itr_id == map_vtx.end()){
             return false;
         } else {
             typename list_adjacency::iterator itr_i;
             itr_i = (*itr_id).second.list_as_start.begin();
             for (; itr_i != (*itr_id).second.list_as_start.end(); ++itr_i) {
                 map_edg.erase((((_id + (*itr_i))* (_id + (*itr_i)) + 3 * _id + (*itr_i))/2));
             }
             typename list_adjacency::iterator itr_j;
             itr_j = (*itr_id).second.list_as_finish.begin();
             for (; itr_j != (*itr_id).second.list_as_finish.end(); ++itr_j) {
                 map_edg.erase(((((*itr_j)+ _id)* ((*itr_j) + _id) + 3 * (*itr_j)+ _id)/2));
             }
             return true;
         }
     }

//удаление ребра по двум вершинам
template < typename T, typename U>
     void Graph<T, U>::rem_edg (const int &_id_first, const int &_id_second) {
         map_edg.erase((((_id_first + (_id_second))* (_id_first + (_id_second)) + 3 * _id_first + (_id_second))/2));
         return;
     }

//Транспонирование
     template < typename T, typename U>
          void Graph<T, U>::transpose () {
              typename map <int, Vetex<T> >::iterator itr;
              itr = map_vtx.begin();
              for (; itr != map_vtx.end(); ++ itr) {
                  std::swap ((*itr).second.list_as_start, (*itr).second.list_as_finish);
              }
              typename map <int, Edge<U> >::iterator itr_edg;
              itr_edg = map_edg.begin();
              map <int, Edge<U> > new_map_edg;
              for (; itr_edg != map_edg.end(); ++ itr_edg) {
                  Edge<U> edg((*itr_edg).second.second_v, (*itr_edg).second.first_v, (*itr_edg).second.weight);
                  new_map_edg.insert(pair<int, Edge<U> >(edg.id, edg));
              }
              this->map_edg = new_map_edg;
              return;
          }
// список вершин
   template < typename T, typename U>
          std::list<Vetex<T> > Graph<T, U>::get_vetices(){
              typename map <int, Vetex<T> > ::iterator itr;
              std::list<Vetex<T> > answer;
              for (itr = map_vtx.begin(); itr != map_vtx.end(); ++itr) {
                  answer.push_back((*itr).second);
              }
              return answer;
          }

          // список исходящих вершин
   template < typename T, typename U>
          std::list<Vetex<T> > Graph<T, U>::get_vetices_outbox(int id){
             typename map <int, Vetex<T> > ::iterator itr;
              std::list<Vetex<T> > answer;
              itr = map_vtx.find(id);
              if (itr == map_vtx.end()) {
                  cout << "unknown_vetex\n";
                  return answer;
              }
              list_adjacency::iterator itr_i;
              itr_i= (*itr).second.list_as_start.begin();
              for (; itr_i != (*itr).second.list_as_start.end(); ++itr_i) {
                  typename map <int, Vetex<T> >::iterator itr_id;
                  itr_id = map_vtx.find(*itr_i);
                  answer.push_back((*itr_id).second);
              }
              return answer;
          }

          // список входящих вершин
   template < typename T, typename U>
          std::list<Vetex<T> > Graph<T, U>::get_vetices_inbox(int id){
             typename map <int, Vetex<T> > ::iterator itr;
              std::list<Vetex<T> > answer;
              itr = map_vtx.find(id);
              if (itr == map_vtx.end()) {
                  cout << "unknown_vetex\n";
                  return answer;
              }
              list_adjacency::iterator itr_i;
              itr_i= (*itr).second.list_as_finish.begin();
              for (; itr_i != (*itr).second.list_as_finish.end(); ++itr_i) {
                  typename map <int, Vetex<T> >::iterator itr_id;
                  itr_id = map_vtx.find(*itr_i);
                  answer.push_back((*itr_id).second);
              }
              return answer;
          }

          // список исходящих рёбер
   template < typename T, typename U>
          std::list<Edge<U> > Graph<T, U>::get_edges_outbox(int id){
              typename map <int, Vetex<T> > ::iterator itr;
               std::list<Edge<U> > answer;
               itr = map_vtx.find(id);
               if (itr == map_vtx.end()) {
                   cout << "unknown_vetex\n";
                   return answer;
               }
               list_adjacency::iterator itr_i;
               itr_i= (*itr).second.list_as_start.begin();
               for (; itr_i != (*itr).second.list_as_start.end(); ++itr_i) {
                   typename map <int, Edge<U> >::iterator itr_id;
                   int id_2 = (*itr_i);
                   itr_id = map_edg.find((((id + (id_2))* (id + (id_2)) + 3 * id + (id_2))/2));
                   answer.push_back((*itr_id).second);
               }
               return answer;
          }

          // список входящих рёбер
   template < typename T, typename U>
          std::list<Edge<U> > Graph<T, U>::get_edges_inbox(int id){
              typename map <int, Vetex<T> > ::iterator itr;
               std::list<Edge<U> > answer;
               itr = map_vtx.find(id);
               if (itr == map_vtx.end()) {
                   cout << "unknown_vetex\n";
                   return answer;
               }
               list_adjacency::iterator itr_i;
               itr_i= (*itr).second.list_as_finish.begin();
               for (; itr_i != (*itr).second.list_as_finish.end(); ++itr_i) {
                   typename map <int, Edge<U> >::iterator itr_id;
                   int id_2 = (*itr_i);
                   itr_id = map_edg.find((((id_2 + id)* (id_2 + id) + 3 * id_2 + (id))/2));
                   answer.push_back((*itr_id).second);
               }
               return answer;

          }

          //список рёбер
   template < typename T, typename U>
          std::list<Edge<U> > Graph<T, U>::get_edges(){
                  typename map <int, Edge<U> > ::iterator itr;
                  std::list<Edge<U> > answer;
                  for (itr = map_edg.begin(); itr != map_edg.end(); ++itr) {
                      answer.push_back((*itr).second);
                  }
                  return answer;
              }

   template < typename T, typename U >
          class Graph<T, U>::iterator_BFS {
              Graph <T, U> * gr;
              vector <Vetex<T> *>  tree_BFS;
              typename vector <Vetex<T> *>::iterator current;

          public :
                  iterator_BFS(Graph<T, U> * _gr,int id):gr(_gr) {
                       Vetex <T> * p_vtx;
                       queue <int> gray;
                       typename map <int, Vetex<T> >::iterator itr;
                       itr = gr->map_vtx.find(id);
                       p_vtx =& (itr->second);
                       tree_BFS.push_back(p_vtx); // первая пойденнная вершина
                       gray.push(p_vtx->id);
                       while (!gray.empty()) {
                           int current_first_gray = gray.front();
                           gray.pop();
                           itr = gr->map_vtx.find(current_first_gray);
                           list_adjacency list_as_start_of_current_gray_vtx = itr->second.list_as_start;
                           list_adjacency::iterator itr_i;
                           for (itr_i = list_as_start_of_current_gray_vtx.begin(); itr_i != list_as_start_of_current_gray_vtx.end(); ++itr_i){
                               typename map <int, Vetex<T> >::iterator itr_j;
                               itr_j = gr->map_vtx.find(*itr_i);
                               p_vtx = & (itr_j->second);
                               typename vector <Vetex<T> *>::iterator vector_iterator;
                               vector_iterator = find(tree_BFS.begin(), tree_BFS.end(), p_vtx);
                               if (vector_iterator == tree_BFS.end()) { // если её ещё нет в дереве, значит вершина белая
                                   tree_BFS.push_back(p_vtx);
                                   gray.push(p_vtx->id);
                               }
                           }
                       }
                       current = tree_BFS.begin(); // поставили текущую вершину на начало
                  }
                  iterator_BFS& operator++(){
                      ++current;
                      return (*this);
                  }

                  Vetex<T> & operator*() const
                  {
                     return (**current);
                  }

                  iterator_BFS begin() {
                      iterator_BFS x(*this);
                       x.current = tree_BFS.begin();
                      return x;
                  }

                  iterator_BFS end() {
                      iterator_BFS x(*this);
                       x.current = tree_BFS.end();
                      return x;
                  }


                  iterator_BFS (const iterator_BFS &itr): gr(itr.gr),  tree_BFS(itr.tree_BFS), current(itr.current){}

                  bool operator == (const iterator_BFS &itr) const
                  { return ((*current) == (*itr.current)); }

                  bool operator != (const iterator_BFS &itr) const
                  { return !(itr == *this); }

          };



          template < typename T, typename U >
                 class Graph<T, U>::iterator_DFS {
                     Graph <T, U> * gr;
                     vector <Vetex<T> *>  tree_DFS;
                     vector <Vetex<T> *>  tree_DFS_black;
                     typename vector <Vetex<T> *>::iterator current;

                     void DFS_visit(Vetex<T>* x){
                         tree_DFS.push_back(x);
                         list_adjacency list_as_start_of_current_gray_vtx = x->list_as_start;
                         list_adjacency::iterator itr_i;
                         for (itr_i = list_as_start_of_current_gray_vtx.begin(); itr_i != list_as_start_of_current_gray_vtx.end(); ++itr_i){
                             typename map <int, Vetex<T> >::iterator itr_j;
                             itr_j = gr->map_vtx.find(*itr_i);
                             Vetex<T> * p_vtx = & (itr_j->second);
                             typename vector <Vetex<T> *>::iterator vector_iterator;
                             vector_iterator = find(tree_DFS.begin(), tree_DFS.end(), p_vtx);
                             if (vector_iterator == tree_DFS.end()) { // если её ещё нет в дереве, значит вершина белая
                                 DFS_visit(p_vtx);
                             }
                         }
                         tree_DFS_black.push_back(x);
                         return;
                     }

                 public :
                         iterator_DFS(Graph<T, U> * _gr):gr(_gr) {
                              typename map<int, Vetex<T> >::iterator itr_i;
                              for (itr_i = gr->map_vtx.begin(); itr_i != gr->map_vtx.end(); ++itr_i){
                                  Vetex<T> * p_vtx = & (itr_i->second);
                                  typename vector <Vetex<T> *>::iterator vector_iterator;
                                  vector_iterator = find(tree_DFS.begin(), tree_DFS.end(), p_vtx);
                                  if (vector_iterator == tree_DFS.end()) { // если её ещё нет в дереве, значит вершина белая
                                      DFS_visit(p_vtx);
                                  }
                              }
                              current = tree_DFS_black.begin(); // поставили текущую вершину на начало
                         }

                         iterator_DFS& operator++(){
                             ++current;
                             return (*this);
                         }

                         Vetex<T> & operator*() const
                         {
                            return (**current);
                         }

                         iterator_DFS begin() {
                             iterator_DFS x(*this);
                              x.current = tree_DFS_black.begin();
                             return x;
                         }

                         iterator_DFS end() {
                             iterator_DFS x(*this);
                              x.current = tree_DFS_black.end();
                             return x;
                         }


                         iterator_DFS (const iterator_DFS &itr): gr(itr.gr),  tree_DFS(itr.tree_DFS), current(itr.current){}

                         bool operator == (const iterator_DFS &itr) const
                         { return ((*current) == (*itr.current)); }

                         bool operator != (const iterator_DFS &itr) const
                         { return !(itr == *this); }

                 };

#endif // GRAPH_H

#endif // LIBR_H
