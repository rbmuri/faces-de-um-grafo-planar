#include <bits/stdc++.h>

enum Color {
  finished = -1,
  visited = 0,
  notVisited = 1
};

enum Curve {
  right = 1,
  straight = 0,
  left = -1
};

std::string letter (int a) {
  switch (a) {
    case 1:
    return "a";
      break;
    case 2:
    return "b";
      break;
    case 3:
    return "c";
      break;
    case 4:
    return "d";
      break;
    case 5:
    return "e";
      break;
    case 6:
    return "f";
      break;
    case 7:
    return "g";
      break;
    case 8:
    return "h";
      break;
    case 9:
    return "i";
      break;
    case 10:
    return "j";
      break;
    case 11:
    return "k";
      break;
    case 12:
    return "l";
      break;
    case 13:
    return "m";
      break;
    default:
      return "abc";
  }
}

class Point {
  public:
  
    int index;

    double x, y;
    Color color; // 1 if not visited, 0 if already visited, -1 if finished
    std::vector<int> neighbours;
    std::vector<int> sortedNeighbours;

    Point(double x, double y, std::vector<int> neighbours, int index) {
      this->x = x;
      this->y = y;
      this->neighbours = neighbours;
      this->index = index;
      this->color = Color::notVisited;
    };

    double distance(Point point) {
      double x = (this->x - point.x);
      double y = (this->y - point.y);

      return sqrt(x*x + y*y);
    };

    // Coeficiente da reta que passa na origem e p.
    // Inclination relative to the origin.
    double inclination() {
      return atan2(this->y, this->x);
    };

    // Coeficiente da reta orientada de p para q.
    // Inclination relative to another point.
    long double inclination(Point other) {
      return atan2(other.y - this->y, other.x - this->x);
    };
};

Curve TipoCurva(Point a, Point b, Point c) {
 double v = a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y);
 if (v < 0) return Curve::left; // esquerda.
 if (v > 0) return Curve::right; // direita.
 return Curve::straight; // em frente.
}

std::vector<Point> G;

class Edge {
  public:
    int index;

    int a, b;
    bool visited;

    Edge(int a, int b, int index) {
      this->index = index;

      this->a = a;
      this->b = b;
      visited = false;
    }

    double internalProduct(const Edge& other, const std::vector<Point>& points) {
      int ux = points[this->b].x - points[this->a].x;
      int uy = points[this->b].y - points[this->a].y;
      int vx = points[other.b].x - points[other.a].x;
      int vy = points[other.b].y - points[other.a].y;
      return ux * vx + uy * vy;
    }

    double magnitude(const std::vector<Point>& points) {
      int ux = points[this->b].x - points[this->a].x;
      int uy = points[this->b].y - points[this->a].y;
      return std::sqrt(ux * ux + uy * uy);
    }
};

double angleBetweenEdges(Edge& edge1, Edge& edge2, const std::vector<Point>& points) {
    double dotProductValue = edge1.internalProduct(edge2, points);
    double magnitude1 = edge1.magnitude(points);
    double magnitude2 = edge2.magnitude(points);

    double cosTheta = dotProductValue / (magnitude1 * magnitude2);

    if (magnitude1 * magnitude2 == 0) {
      cosTheta = 1;
    }

    return std::acos(cosTheta);
}

// get directed edges based on points
Edge& getEdge(int a, int b, std::vector<Edge>& edges) {
  for (auto& edge: edges) {
    if (edge.a == a && edge.b == b) {
      return edge;
    }
  }
}

void DFS(Edge& currentEdge, std::vector<std::vector<Point>>& faces, std::vector<Edge>& edges) {

  faces[faces.size() - 1].push_back(G[currentEdge.a]);

  if (currentEdge.visited) {
    // std::cout << faces[faces.size() - 1].size() << " ";
    // for (auto& v : faces[faces.size() - 1]) {
    //   std::cout << letter(v.index + 1) << " ";
    // }
    // std::cout << std::endl;

    std::vector<Point> newFace;

    faces.push_back(newFace);
    return;
  }
  
  currentEdge.visited = true;
  
  Edge inverted = getEdge(currentEdge.b, currentEdge.a, edges);

  // std::cout << "A inversa de " << currentEdge.a << ", " << currentEdge.b << " é " << inverted.a << ", " << inverted.b << std::endl;
  
  int next = -1;

  for (int i = 0; i < G[currentEdge.b].neighbours.size(); i++) {
    if (currentEdge.a == G[currentEdge.b].neighbours[i]) {
      next = G[currentEdge.b].neighbours[i + 1];
      break;
    }
  }
  
  // for (int neighbour : G[currentEdge.b].neighbours) {
  //   std::cout << letter(neighbour + 1) << " ";
  // }
  // std::cout << std::endl;

  DFS(getEdge(currentEdge.b, next, edges), faces, edges);
}

int main(int argc, char** argv) {

  int verticesCount, edgesCount;
  std::vector<std::vector<Point>> faces;
  std::vector<Edge> edges;

  std::cin >> verticesCount >> edgesCount;

  for (int i = 0; i < verticesCount; i++) {
    double x, y;

    std::cin >> x >> y;

    int neighboursCount;

    std::cin >> neighboursCount;

    std::vector<int> neighbours;

    int k = 0;

    for (int j = 0; j < neighboursCount; j++) {
      int neighbour;
      std::cin >> neighbour;

      neighbours.push_back(neighbour - 1);
      edges.push_back(Edge(i, neighbour - 1, k));
      k++;
    }
    
    G.push_back(Point(x, y, neighbours, i));
  }

  for (Point& v : G) {
    std::vector<int>& neighbours = v.neighbours;

    std::sort(neighbours.begin(), neighbours.end(),
    [&v](int p, int q) { return v.inclination(G[p]) < v.inclination(G[q]); });

    neighbours.push_back(neighbours[0]);
  }

  std::vector<Point> firstFace;
  faces.push_back(firstFace);

  for (auto& edge : edges) {
    if (!edge.visited) {
      DFS(edge, faces, edges);
    }
  }

  faces.pop_back();
  std::cout<< faces.size() << std::endl;

  for (auto& f : faces) {
    std::cout << f.size() << " ";
    for (auto& v : f) {
      std::cout << v.index + 1 << " ";
    }
    std::cout << std::endl;
  }
  
  return 0;
}

/*
8 11
0 0 2 2 3
1 1 4 1 4 5 7
1 -1 5 1 4 5 6 7
2 0 2 2 3
4 0 3 2 3 6
4 -1.5 2 3 5
-3 0 3 2 3 8
-2 0 1 7
*/

/*
12 23
-3 1 2 2 8
-1 2 5 1 3 5 7 8
0.5 3 3 2 5 4
2.5 1.65 3 3 5 6
0.5 1.65 5 2 3 4 6 7
2 0.15 4 4 5 7 10
0 0.5 6 2 5 6 8 9 10
-1.5 0.5 4 1 2 7 9 
-0.75 -1.5 4 7 8 10 11
1.75 -1.4 5 6 7 9 11 12
0.25 -3 3 9 10 12
2.9 -2.85 2 10 11
*/

/*
11 15
0 0 3 2 3 9
1 1 4 1 4 5 7
1 -1 5 1 4 5 6 7
2 0 2 2 3
4 0 3 2 3 6
4 -1.5 2 3 5
-3 0 3 2 3 8
-2 0 1 7
0.5 0 3 1 10 11
0.75 0.5 2 11 9
0.75 -0.5 2 10 9
*/