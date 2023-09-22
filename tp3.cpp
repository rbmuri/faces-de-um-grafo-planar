
#include <bits/stdc++.h>
 
using namespace std;



class Ponto {
public:
    double x, y;
    int id;
    int visited;
    vector<bool> viscw;
    vector<Ponto> edgepoints;
    vector<int> edges;
    Ponto(double a, double b){
        x = a;
        y = b;
        id = -1;
        visited = 0;
    }
    Ponto(double a, double b, int c){
        x = a;
        y = b;
        id = c;
        visited = 0;
    }
    void add(Ponto a){
        edgepoints.push_back(a);
        viscw.push_back(true);
    }
    void add(int i){
        edges.push_back(i);
    }
    //complexidade O(v)
    void build(vector<Ponto> v){
        for (auto u : edges){
            edgepoints.push_back(v[u]);
            viscw.push_back(false);
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
        Ponto p(1,0);
        Ponto a = *this - p;
        double angulo = (*this - a).polar() - (c - *this).polar();
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

    void edgesort(){
        sort(edgepoints.begin(), 
             edgepoints.begin() + edgepoints.size(), 
             [&](Ponto a, Ponto b) {
             return clockwise(a, b);
             });
    }
    // void bucket()
    // {
    //     int n = 10;
    //     // 1) Create n empty buckets
    //     vector<Ponto> b[n];
    
    //     // 2) Put array elements
    //     // in different buckets
    //     for (int i = 0; i < n; i++) {
    
    //         // Index in bucket
    //         int bi = n * relativo(edgepoints[i]);
    //         b[bi].push_back(edgepoints[i]);
    //     }
    
    //     // 3) Sort individual buckets
    //     for (int i = 0; i < n; i++)
    //         sort(b[i].begin(), b[i].end(),
    //             [&](Ponto a, Ponto b) {
    //             return clockwise(a, b);
    //             });
    
    //     // 4) Concatenate all buckets into arr[]
    //     int index = 0;
    //     for (int i = 0; i < n; i++)
    //         for (int j = 0; j < b[i].size(); j++)
    //             edgepoints[index++] = b[i][j];
    // }

};


vector<int> dfscw(vector<Ponto> &v, int saida, int vertice, int comeco, int dir){
    vector<int> face;
    Ponto* prox;
    //para cada aresta saindo do vertice
    for (int i = 0; i < v[vertice].edgepoints.size(); i++){
        //encontra a de onde viemos
        if (v[vertice].edgepoints[i].id == saida) {
            //safeguard para loop
            if (i+1==v[vertice].edgepoints.size()) i = -1;
            //designa o proximo como o depois
            prox = &v[vertice].edgepoints[i+1];
            v[vertice].viscw[i+1] = true;
            break;
        }
    }
    if (vertice == comeco && prox->id == dir) return face;
    face = dfscw(v, vertice, prox->id, comeco, dir);
    face.push_back(prox->id);
    return face;         
}


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

int main(){
    int n, ed;
    cin >> n >> ed;;
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

    for (int i = 0; i<n; i++){
        v[i].build(v);
        // v[i].edgesort();
    }
    
    //para cada vertice
    for (int i = 0; i<n; i++){
        //para cada aresta
        vector<Ponto> edges = v[i].edgepoints;
        for (int u = 0; u < edges.size(); u++){
            //se ainda nao passamos cw
            if (!v[i].viscw[u]){
                //passa e coleta
                vector<int> face = dfscw(v, i, edges[u].id, i, edges[u].id);
                //push back no primeiro vertice pra terminar
                face.push_back(edges[u].id);
                face.push_back(face[0]);
                faces.push_back(face);

            }
            
        }
    }
    cout << faces.size() << endl;

    for (auto f : faces){
        printface(f);
    }
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