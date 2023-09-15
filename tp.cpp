
#include <bits/stdc++.h>
 
using namespace std;

struct Ponto {
    double x, y;
    int id;
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
        edges.push_back(a.id);
    }

    Ponto operator + (const Ponto p) const { return Ponto(x+p.x, y+p.y); }
    Ponto operator - (const Ponto p) const { return Ponto(x-p.x, y-p.y); }
    Ponto operator * (const double d) const { return Ponto(x*d, y*d); }
    Ponto operator / (const double d) const { return Ponto(x/d, y/d); }
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
//retorna o angulo polar de uma coordenada
double polar(Ponto a){
    double angulo = atan2(a.y, a.x);
    if (angulo < 0) angulo = angulo + 2*M_PI;
    return angulo;
}
//retorna o angulo entre uma linha e um ponto
double relativo(Ponto a, Ponto b, Ponto c){
    double angulo = polar(b - a) - polar(c);
    return angulo;
}
bool clockwise(Ponto c, Ponto d){
    Ponto a(0, 0);
    Ponto b(1, 0);
    double angc = relativo(a, b, c);
    double angd = relativo(a, b, d);
    return (angc>angd);
}



int main(){
    
    int n, e;
    cin >> n >> e;
    vector<int> vEdges[n];
    vector<Ponto> vValue;
    for (int i = 0; i<n; i++){
        int x, y, edgenum, edge;
        cin >> x >> y;
        Ponto a(x, y, i+1);
        vValue.push_back(a);
        cin >> edgenum;
        for (int j = 0; j<edgenum; j++){
            cin >> edge;
            vEdges[j].push_back(edge);
        }
    }
    for (int i = 0; i<n; i++)
        sort(vEdges[i].begin(), vEdges[i].begin()+vEdges[i].size(), clockwise);
    

    Ponto a(1, 1);
    Ponto b(-2, 1);
    Ponto c(-4, -2);
    Ponto d(5, -2);
    cout << TipoCurva(a, b, c) << endl << TipoCurva(a, b, d) << endl;
    //cout << atan2(c-b, b-a) - atan2(b-a, a) << endl;
    cout << relativo(a, b, c) << endl;
    cout << polar(b) << endl;
    cout << polar(c) << endl;
    cout << polar(d) << endl;
    cout << polar(b-a) << endl;
    cout << polar(c-b) << endl;
    return 0;
};