
#include <bits/stdc++.h>
 
using namespace std;



struct Ponto {

    double x, y;
    int id;
    map<int, bool> viscw;
    vector<Ponto> edgepoints;
    vector<int> edges;
    map<int, int> next;
    //map<int, int> edgeangles;
    Ponto(double a, double b){
        x = a;
        y = b;
        id = -1;
    }
    Ponto(double a, double b, int c){
        x = a;
        y = b;
        id = c;
    }
    void add(Ponto a){
        edgepoints.push_back(a);
    }
    void add(int i){
        edges.push_back(i);
    }
    //complexidade O(v)
    void buildedges(vector<Ponto> v){
        for (auto u : edges){
            edgepoints.push_back(v[u]);
        } 
    }
    void sortedges(){
        sort(edgepoints.begin(), 
             edgepoints.begin() + edgepoints.size(), 
             [&](Ponto a, Ponto b) {
             return clockwise(a, b);
             });
    }
    void buildpaths(){   
        for (int i = 0; i < edgepoints.size(); i++){
            if (i == 0)
                next[edgepoints[i].id] = edgepoints[edgepoints.size()-1].id;
            else
                next[edgepoints[i].id] = edgepoints[i-1].id;

            //viscw[edgepoints[i].id] = false;
        }
    }
    //retorna o angulo polar de uma coordenada
    double polar(){
        double angulo = atan2(y, x);
        if (angulo < 0) angulo = angulo + 2*M_PI;
        return angulo;
    }
    //retorna o angulo entre uma linha e um ponto
    double relativo(Ponto c){
        //if (edgeangles[c.id]) return edgeangles[c.id];
        Ponto p(1,0);
        Ponto a = *this - p;
        double angulo = (*this - a).polar() - (c - *this).polar();
        //edgeangles[c.id] = angulo;
        return angulo;
    }

    Ponto operator + (const Ponto p) const { return Ponto(x+p.x, y+p.y); }
    Ponto operator - (const Ponto p) const { return Ponto(x-p.x, y-p.y); }
    Ponto operator * (const double d) const { return Ponto(x*d, y*d); }
    Ponto operator / (const double d) const { return Ponto(x/d, y/d); }


    bool clockwise(Ponto c, Ponto d){
        
        double angc = relativo(c);
        double angd = relativo(d);
        return (angc>angd);
    }

};

 
void printface(vector<int> f){
        cout << f.size(); 
        for (int k = 0; k<f.size(); k++)
            cout << " " << f[k]+1;
        cout << "\n";
}
void printface(vector<Ponto> f){
        cout << f.size(); 
        for (int k = 0; k<f.size(); k++)
            cout << " " << f[k].id+1;
        cout << "\n";
}

void dfscw(vector<Ponto> &v, vector<int> &face, int saida, int vertice, int comeco, int dir){
    int prox;
    //para cada aresta saindo do vertice

    prox = v[vertice].next[saida];
    v[vertice].viscw[prox] = true;
    
    if (vertice == comeco && prox == dir) return;
    dfscw(v, face, vertice, prox, comeco, dir);
    face.push_back(prox);
    return;      
}
 
int main(){
    //clock_t start = clock();
    int n, ed;
    cin >> n >> ed;
    vector<Ponto> v; //listas de adjacencia
    vector<vector<int>> faces;

    //coleta o grafo
    for (int i = 0; i<n; i++){
        double x, y;
        int edgenum, edge;
        cin >> x >> y;
        Ponto a(x, y, i);
        v.push_back(a);
        // armazene cada vertice
        cin >> edgenum;
        for (int j = 0; j<edgenum; j++){
            cin >> edge;
            v[i].add(edge-1);
            // armazene suas arestas
        }
    }
    //clock_t receber = clock() - start;
    for (int i = 0; i<n; i++){
        v[i].buildedges(v);
    }
    //clock_t builde = clock() - receber - start;
    for (int i = 0; i<n; i++){
        v[i].sortedges();
    }
    //clock_t sorte = clock() - builde - receber - start;
    for (int i = 0; i<n; i++){
        v[i].buildpaths();
    }
    //clock_t buildp = clock() - sorte - builde - receber - start;
    //para cada vertice
    for (int i = 0; i<n; i++){
        //para cada aresta
        vector<Ponto> edges = v[i].edgepoints;
        
        for (int u = 0; u < edges.size(); u++){
            //se ainda nao passamos cw
            if (!v[i].viscw[edges[u].id]){
                //passa e coleta
                vector<int> face;
                dfscw(v, face, i, edges[u].id, i, edges[u].id);
                
                //push back no primeiro vertice pra terminar
                face.push_back(edges[u].id);
                face.push_back(face[0]);
                faces.push_back(face);
            }
            
        }
    }
    //clock_t dfs = clock() - buildp - sorte - builde - receber - start;
    cout << faces.size() << endl;

    for (auto f : faces){
        printface(f);
    }
    // cout << "clock receber: " << receber << endl;
    // cout << "clock buildedges: " << builde << endl;
    // cout << "clock sortedges: " << sorte << endl;
    // cout << "clock buildpaths: " << buildp << endl;
    // cout << "clock dfscw: " << dfs << endl;
    return 0;
};


/*
15 0
0 0 3 2 3 6
4 3 3 1 4 8
3 -4 3 1 5 14
8 0 3 2 5 10
5 -4 3 3 4 12
1 0 4 1 7 8 14
2 0 3 6 9 15 
4 2 4 2 6 9 10
4 1 3 7 8 11
7 0 4 4 8 11 12
6 0 3 9 10 13
5 -3 4 5 10 13 14
5 -2 3 11 12 15
3 -3 4 3 6 12 15
3 -2 3 7 13 14

*/