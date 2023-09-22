
#include <bits/stdc++.h>
 
using namespace std;



class Ponto {
public:
    double x, y;
    int id;
    int visited;
    vector<bool> viscw;
    vector<bool> visccw;
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
        visccw.push_back(true);
    }
    void add(int i){
        edges.push_back(i);
    }
    //complexidade O(v)
    void build(vector<Ponto> v){
        for (auto u : edges){
            edgepoints.push_back(v[u]);
            viscw.push_back(false);
            visccw.push_back(false);
        }
    }
    //retorna o angulo polar de uma coordenada
    double polar(){
        double angulo = atan2(y, x);
        if (angulo < 0) angulo = angulo + 2*M_PI;
        return angulo;
    }
    //retorna o angulo entre uma linha e um ponto
    double relativo(Ponto a, Ponto b, Ponto c){
        double angulo = (b - a).polar() - (c - b).polar();
        return angulo;
    }

    Ponto operator + (const Ponto p) const { return Ponto(x+p.x, y+p.y); }
    Ponto operator - (const Ponto p) const { return Ponto(x-p.x, y-p.y); }
    Ponto operator * (const double d) const { return Ponto(x*d, y*d); }
    Ponto operator / (const double d) const { return Ponto(x/d, y/d); }

    bool clockwise(Ponto c, Ponto d){
        Ponto a(1,0);
        Ponto b = *this - a; 
        double angc = relativo(b, *this, c);
        double angd = relativo(b, *this, d);
        return (angc>angd);
    }

    void edgesort(){
        sort(edgepoints.begin(), 
             edgepoints.begin() + edgepoints.size(), 
             [&](Ponto a, Ponto b) {
             return clockwise(a, b);
             });
    }

};

void visit(vector<Ponto> v, int a, int b) {
    vector<Ponto> edges = v[a].edgepoints;

    for (int u = 0; u < edges.size(); u++) {
        if (edges[u].id == b) {
            v[a].viscw[u] = true;
            break;
        }
    }
    vector<Ponto> edgesb = v[b].edgepoints;
    for (int u = 0; u < edgesb.size(); u++) {
        if (edgesb[u].id == a) {
            v[b].visccw[u] = true;
            break;
        }
    }
}

vector<int> dfscw(vector<Ponto> v, int saida, int vertice, int comeco, int dir){
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
            break;
        }
    }
    if (vertice == comeco && prox->id == dir) return face;
    face = dfscw(v, vertice, prox->id, comeco, dir);
    face.push_back(prox->id);
    return face;         
}
vector<int> dfsccw(vector<Ponto> v, int saida, int vertice, int comeco, int dir){
    vector<int> face;
    Ponto* prox;
    for (int i = 0; i < v[vertice].edgepoints.size(); i++){
        if (v[vertice].edgepoints[i].id == saida) {
            if (i==0) i = v[vertice].edgepoints.size();
            prox = &v[vertice].edgepoints[i-1];
            //visit(v, vertice, i);
            break;
        }
    }
    if (vertice == comeco && prox->id == dir) return face;
    face = dfsccw(v, vertice, prox->id, comeco, dir);
    face.push_back(prox->id);
    return face;         
}

void printface(vector<int> f){
        cout << f.size(); 
        for (int k = 0; k<f.size(); k++)
            cout << " " << f[k]+1;
        cout << "\n";
}



int main(){
    int n, ed;
    cin >> n >> ed;;
    vector<Ponto> v; //listas de adjacencia
    vector<vector<int>> faces;
    bool cw[n][n];

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
    
    //ordena
    for (int i = 0; i<n; i++){
        v[i].build(v);
        v[i].edgesort();
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
                face.push_back(edges[u].id);
                face.push_back(face[0]);
                faces.push_back(face);
                //guarda se as arestas foram
                //visitadas, e de que forma
                for (int k = 1; k<face.size(); k++)
                    visit(v, face[k], face[k-1]);

            }
            if (!v[i].visccw[u]){
                vector<int> face = dfsccw(v, i, edges[u].id, i, edges[u].id);
                face.push_back(edges[u].id);
                face.push_back(face[0]);
                faces.push_back(face);
                for (int k = 1; k<face.size(); k++)
                    visit(v, face[k-1], face[k]);
            }
            
        }
    }
    cout << faces.size() << endl;

    for (auto f : faces){
        printface(f);
    }
    return 0;
};