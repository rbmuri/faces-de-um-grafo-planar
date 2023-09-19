
#include <bits/stdc++.h>
 
using namespace std;



class Ponto {
public:
    double x, y;
    int id;
    int visited;
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
    }
    void add(int i){
        edges.push_back(i);
    }
    //complexidade O(v)
    void build(vector<Ponto> v){
        for (auto u : edges)
            edgepoints.push_back(v[u]);
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
    if (vertice == comeco && (*prox).id == dir) return face;
    face = dfscw(v, vertice, (*prox).id, comeco, dir);
    face.push_back((*prox).id);
    return face;         
}
vector<int> dfsccw(vector<Ponto> v, int saida, int vertice, int comeco, int dir){
    vector<int> face;
    Ponto* prox;
    for (int i = 0; i < v[vertice].edgepoints.size(); i++){
        if (v[vertice].edgepoints[i].id == saida) {
            if (i==0) i = v[vertice].edgepoints.size();
            prox = &v[vertice].edgepoints[i-1];
            break;
        }
    }
    if (vertice == comeco && (*prox).id == dir) return face;
    face = dfsccw(v, vertice, (*prox).id, comeco, dir);
    face.push_back((*prox).id);
    return face;         
}

void printface(vector<int> f){
        cout << f.size(); 
        for (int k = 0; k<f.size(); k++)
            cout << " " << f[k]+1;
        cout << "\n";
}
void visit(vector<Ponto> v, int a, int b){
    
    for (auto u : v[a].edgepoints){
        if (u.id == b){
            u.visited += 3;
        }
    }
    for (auto u : v[b].edgepoints){
        if (u.id == a){
            u.visited += 2;
        }
    }

}

int main(){
    int n, ed;
    cin >> n >> ed;;
    vector<Ponto> v; //listas de adjacencia
    //vector<vector<int>> visited(n, vector<int>(n, 0));
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
    
    //ordena
    for (int i = 0; i<n; i++){
        v[i].build(v);
        v[i].edgesort();
    }
    
    //para cada vertice
    for (int i = 0; i<n; i++){
        //para cada aresta
        for (auto u : v[i].edgepoints){
            
            //se ainda nao passamos cw
            if (u.visited == 0){
                //passa e coleta
                vector<int> face = dfscw(v, i, u.id, i, u.id);
                face.push_back(u.id);
                face.push_back(face[0]);
                faces.push_back(face);
                //guarda se as arestas foram
                //visitadas, e de que forma
                for (int k = 1; k<face.size(); k++){
                    visit(v, face[k], face[k-1]);
                    // visited[face[k]][face[k-1]] += 3;
                    // visited[face[k-1]][face[k]] += 2;
                }
            }
            if (u.visited == 2){
                vector<int> face = dfsccw(v, i, u.id, i, u.id);
                face.push_back(u.id);
                face.push_back(face[0]);
                faces.push_back(face);
                for (int k = 1; k<face.size(); k++){
                    visit(v, face[k-1], face[k]);
                }
            }
            if (u.visited == 3){
                vector<int> face = dfscw(v, i, u.id, i, u.id);
                face.push_back(u.id);
                face.push_back(face[0]);
                faces.push_back(face);
                for (int k = 1; k<face.size(); k++){
                    visit(v, face[k], face[k-1]);
                }
            }
        }
    }
    cout << faces.size() << endl;

    for (auto f : faces){
        printface(f);
    }
    return 0;
};