
#include <bits/stdc++.h>
 
using namespace std;



class Ponto {
public:
    double x, y;
    int id;
    vector<bool> visited;
    vector<Ponto> edgepoints;
    vector<int> edges;
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
    void visit(int i){
        visited.push_back(i);
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


// Distancia euclidiana de a para b.
double Distancia(Ponto a, Ponto b) {
    double x = (a.x - b.x), y = (a.y - b.y);
    return sqrt(x*x + y*y);
}
// Coeficiente da reta que passa na origem e p.
double Inclinacao(Ponto p) {
    return atan2(p.y, p.x);
}
// Coeficiente da reta orientada de p para q.
double Inclinacaorelativa(Ponto p, Ponto q) {
    return atan2(q.y - p.y, q.x - p.x);
}
// Determina se ao caminhar de a para b e depois de b para c
// estamos fazendo uma curva a esquerda, a direita, ou seguindo em frente.
int TipoCurva(Ponto a, Ponto b, Ponto c) {
    double v = (b.x-a.x)*(c.y-b.y)-(b.y-a.y)*(c.x-b.x);
    return v;
    if (v < 0) return -1; // esquerda.
    if (v > 0) return +1; // direita.
    return 0; // em frente.
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
        int size = f.size();
        cout << size+1 << " "; 
        cout << f[size-1]+1;
        for (int k = 0; k<f.size(); k++)
            cout << " " << f[k]+1;
        cout << "\n";
}

int main(){
    int n, ed;
    cin >> n >> ed;
    //vector<Ponto> v[n];
    vector<Ponto> v; //listas de adjacencia
    int visited[n][n] = {0};
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
    //cout << "breakpoint 1\n";
    for (int i = 0; i<n; i++){
        v[i].build(v);
        v[i].edgesort();
    }
    vector<vector<int>> faces;
    //cout << "breakpoint 2\n";
    //para cada vertice
    for (int i = 0; i<n; i++){
        //para cada aresta
        for (auto u : v[i].edgepoints){
            
            //se ainda nao passamos cw
            if (visited[u.id][i] == 0){
                //passa e coleta
                vector<int> face = dfscw(v, i, u.id, i, u.id);
                face.push_back(u.id);
                faces.push_back(face);
                visited[face[0]][face[face.size()-1]] += 2;
                visited[face[face.size()-1]][face[0]] += 2;
                for (int k = 1; k<face.size(); k++){
                    visited[face[k]][face[k-1]] += 2;
                    visited[face[k-1]][face[k]] += 2;
                }
            }
            if (visited[u.id][i] == 2){
                vector<int> face = dfsccw(v, i, u.id, i, u.id);
                face.push_back(u.id);
                faces.push_back(face);
                visited[face[0]][face[face.size()-1]]+=3;
                visited[face[face.size()-1]][face[0]]+=3;
                for (int k = 1; k<face.size(); k++){
                    visited[face[k]][face[k-1]]+=3;
                    visited[face[k-1]][face[k]]+=3;
                }
            }
            if (visited[u.id][i] == 3){
                vector<int> face = dfscw(v, i, u.id, i, u.id);
                face.push_back(u.id);
                faces.push_back(face);
                visited[face[0]][face[face.size()-1]] = 4;
                visited[face[face.size()-1]][face[0]] = 4;
                for (int k = 1; k<face.size(); k++){
                    visited[face[k]][face[k-1]] = 4;
                    visited[face[k-1]][face[k]] = 4;
                }
            }
                
                // cout << "v: " << i+1 << endl;
                // cout << face.size()+1 << " "; 
                // cout << i+1 << " ";
                //     visited[face[0]][face[face.size()-1]] = 3;
                //     visited[face[face.size()-1]][face[0]] = 3;
                    
                //     cout << face[0]+1;
                // for (int k = 1; k<face.size(); k++){
                //     visited[face[k]][face[k-1]] = 3;
                //     visited[face[k-1]][face[k]] = 3;
                //     cout << " " << face[k]+1;
                // }
                // cout << "\n";
        }
    }
    cout << faces.size() << endl;

    for (auto f : faces){
        printface(f);
    }

    // Ponto a(1, 1);
    // Ponto b(0, 1);
    // Ponto c(1, 2);
    // Ponto d(2, 1);
    // Ponto e(1, 0);
    // cout << TipoCurva(a, b, c) << endl << TipoCurva(a, b, d) << endl;
    // cout << atan2(c-b, b-a) - atan2(b-a, a) << endl;
    // cout << relativo(a, d, d) << endl;
    // cout << relativo(d, a, c) << endl;
    // cout << relativo(d, a, b) << endl;
    // cout << relativo(d, a, e) << endl;
    // cout << "\npolares:\n";
    // cout << a.polar() << endl;
    // cout << b.polar() << endl;
    // cout << c.polar() << endl;
    // cout << d.polar() << endl;
    // cout << e.polar() << endl;
    // return 0;
};