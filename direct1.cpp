#include <stdio.h>
#include <vector>
#include <list>
#include <utility> // for pair
#include <map>
#include <cmath>
#include <algorithm>

using namespace std;

class branin {
private:
  double a;
  double b;
  double c;
  double r;
  double s;
  double t;
  double d;

public:
  branin() {
    a = 1.0;
    b = 5.1/(4.0*M_PI*M_PI);
    c = 5.0/M_PI;
    r = 6.0;
    s = 10.0;
    t = 1/(8.0*M_PI);
    d = s*(1.0-t);
  }

  ~branin() {}
  
  double eval(vector<double> x) {
    double val = x[1] - b*x[0]*x[0] + c*x[0] - r;
    return a*val*val + d*cos(x[0]) + s;
  }
};

class branin_wrap {
private:
  branin f;
  vector<double> a; // Lower limits
  vector<double> b; // Upper limits
  vector<double> d;
public:
  branin_wrap(vector<double> lo, vector<double> hi) : a(lo), b(hi), d(lo.size(), 0.0) {
    for(int i=0; i<lo.size(); i++) {
      d[i] = b[i] - a[i];
    }
  }

  double eval(vector<double> x) {
    vector<double> y(x.size(), 0.0);
    for(int i=0; i<x.size(); i++) {
      y[i] = a[i] + d[i]*x[i];
    }
    return f.eval(y);
  }

};

class power_list {
public:
  int n;
  vector<double> powers;
  power_list(int N=20) : powers(N) {
    for(int i=0, v=1; i<N; i++, v*=3) {
      powers[i] = 1.0/((double)v);
    }
  }
  ~power_list() {}
  double get(int n) {
    if(n >= powers.size()) {
      int v=1;
      for(int i=0; i<powers.size(); i++, v*=3) {}
      for(int i=powers.size(); i<=n; i++, v*=3) {
	powers.push_back(1.0/((double)v));
      }
    }
    return powers[n];
  }
};

class rectangle {
public:
  static branin_wrap f;
  vector<double> c; // center of rectangle
  vector<int> e; // side length exponents [0, 1, 2, 3, etc.]
  static power_list d; // actual side lengths d=(1/3)^e = [1, 1/3, 1/9, 1/27, 1/81, etc.]
  double fc; // function evaluation at center of rectangle
  rectangle(vector<double> C, vector<int> E) : c(C), e(E) {
    fc = f.eval(c); // Evaluate at the center
  }
  rectangle(vector<double> C, vector<int> E, double Fc) : c(C), e(E), fc(Fc) {} // For testing only 
  
  ~rectangle() {}
  
  int get_exp(void) {
    int themin = e[0];
    for(auto i : e) {
      if(i<themin) {
	themin = i;
      }
    }
    return themin;
  }
  double get_dim(void) {
    return d.get(get_exp());
  }
  vector<pair<double,int>> &get_vals(void) {
    vector<pair<double,int>>* vals = new vector<pair<double,int>>;
    int emin = get_exp();
    for(int i=0; i<e.size(); i++) {
      if(e[i] == emin) {
	vals->push_back(pair<double,int>(0.0,i));
      }
    }
    double delta = d.get(e[vals->at(0).second]) / 3.0;
    for(int i=0; i<vals->size(); i++) {
      vector<double> x1(c);
      vector<double> x2(c);
      x1[vals->at(i).second] -= delta;
      x2[vals->at(i).second] += delta;
      double f1 = f.eval(x1);
      double f2 = f.eval(x2);
      if(f1<f2) {
	vals->at(i).first = f1;
      } else {
	vals->at(i).first = f2;
      }
    }
    return *vals;
  }
};

// Initialize the static members
branin_wrap rectangle::f(vector<double>{-5.0,-5.0}, vector<double>{20.0,20.0});
power_list rectangle::d(20);

bool sort_helper(pair<double,int> a, pair<double,int> b) { return (a.first<b.first); }

int main(int argc, char* argv[]) {
  // Data structures
  list<rectangle*> R; // List of rectangles

  // Initialize with unit hypercube
  double epsilon = 1e-4;
  rectangle* r = new rectangle(vector<double>{0.5,0.5}, vector<int>{0,0});
  double fmin = r->fc;
  R.push_back(r);

  // Find set of potentially optimal rectangles
  // Suppose you have a set of rectangles.  Sort them by their function values for each side length exponent set.
  vector<rectangle*> points(20, NULL);
  for(auto r : R) {
    int d = r->get_exp();
    rectangle* p = points[d];
    if(p != NULL) {
      if(r->fc < p->fc) {
	points[d] = r;
      }
    } else {
      points[d] = r;
    }
  }
  vector<rectangle*> subpoints;
  for(auto p : points) {
    if(p != NULL) {
      subpoints.push_back(p);
    }
  }

  // Draw lines to find out which points will be kept in the set of potentially optimal rectangles.
  list<rectangle*> optimal;
  optimal.push_back(subpoints[0]); // Push the first one on.  It has the largest side length.
  for(int i=1; i<subpoints.size(); i++) { // Now look at all the others.
    int ibest = i;
    double best = 2.0 * (optimal.back()->fc - subpoints[i]->fc) / (optimal.back()->get_dim() - subpoints[i]->get_dim());
    for(int j=i+1; j<subpoints.size(); j++) {
      double tbest = 2.0 * (optimal.back()->fc - subpoints[j]->fc) / (optimal.back()->get_dim() - subpoints[j]->get_dim());
      if(tbest > best) {
	best = tbest;
	ibest = j;
      }
    }
    if(best > 0.0 && ibest == i) { // Include other check (epislon) here.
      optimal.push_back(subpoints[i]);
    }
  }

  // Process each optimal rectangle.
  for(auto r : optimal) {
    // Find all dimension with maximal side length
    vector<pair<double,int>> vals = r->get_vals();
    sort(vals.begin(), vals.end(), sort_helper); // Check smallest value first
    for(int i=0; i<vals.size(); i++) {
      printf("%d, vals[i].first = %f, vals[i].second = %d\n",i,vals[i].first,vals[i].second);
    }
    
    // Now divide the rectangle
    
    // Add the three new rectangles to the rectangle list
    
    // Lastly remove the original rectangle from the list

  }
  
  return 0;
}

