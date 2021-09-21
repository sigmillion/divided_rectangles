// g++ direct1.cpp -o direct1 --std=c++11

#include <stdio.h>
#include <vector>
#include <list>
#include <utility> // for tuple
#include <map>
#include <cmath>
#include <algorithm>

using namespace std;

class binary {
public:
  binary() {}
  ~binary() {}
  double eval(vector<double> x) {
    double y = 0.0;
    for(int i=0; i<x.size(); i++) {
      // y += x[i];
      y += x[i] - x[i]*(x[i]-1);
    }
    return y;
  }
};

class noncvx {
public:
  noncvx() {}
  ~noncvx() {}
  double eval(vector<double> z) {
    double x = z[0];
    double y = z[1];
    double r2 = x*x + y*y - 4.0;
    if(r2 <= 0.0) {
      return 1000.0;
    }
    return 2*x*x + 2*y*y + 2*x*y + 3*x + 3*y + 1 - log(r2);
  }
};

class noncvx_wrap {
private:
  noncvx f;
  vector<double> a; // Lower limits
  vector<double> b; // Upper limits
  vector<double> d;
public:
  noncvx_wrap(vector<double> lo, vector<double> hi) : a(lo), b(hi), d(lo.size(), 0.0) {
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

bool sort_helper_pair(pair<double,int> a, pair<double,int> b) { return (a.first < b.first); }
bool sort_helper_tuple(tuple<int,double,vector<double>,double,vector<double>> a,
		       tuple<int,double,vector<double>,double,vector<double>> b) {
  double a0 = get<1>(a);
  double a1 = get<3>(a);
  double aval = (a0 < a1 ? a0 : a1);
  double b0 = get<1>(b);
  double b1 = get<3>(b);
  double bval = (b0 < b1 ? b0 : b1);
  return (aval < bval);
}
		       
class rectangle {
public:
  //static branin_wrap f;
  //static noncvx_wrap f;
  static binary f;
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

  vector<vector<double>> &corners(void) {
    vector<vector<double>> *x = new vector<vector<double>>;
    int n = c.size();
    // Only do this in one or two dimensions
    if(n == 1) {
      // In one dimension
      vector<double> ct = vector<double>(1, 0.0);
      ct[0] = c[0] - d.get(e[0]) * 0.5;
      x->push_back(ct);
      ct[0] = c[0] + d.get(e[0]) * 0.5;
      x->push_back(ct);
    } else if(n == 2) {
      // In two dimensions
      vector<double> ct = vector<double>(2, 0.0);
      ct[0] = c[0] - d.get(e[0]) * 0.5;
      ct[1] = c[1] - d.get(e[1]) * 0.5;
      x->push_back(ct);
      ct[0] = c[0] - d.get(e[0]) * 0.5;
      ct[1] = c[1] + d.get(e[1]) * 0.5;
      x->push_back(ct);
      ct[0] = c[0] + d.get(e[0]) * 0.5;
      ct[1] = c[1] + d.get(e[1]) * 0.5;
      x->push_back(ct);
      ct[0] = c[0] + d.get(e[0]) * 0.5;
      ct[1] = c[1] - d.get(e[1]) * 0.5;
      x->push_back(ct);
    }
    return *x;
  }

  vector<rectangle*> &divide(void) {
    // Find longest edge
    int emin = *min_element(e.begin(), e.end());
    double dmax = d.get(emin);
    // Compute divided edge length
    double delta = dmax / 3.0;
    // Find all dimensions having the longest side length
    // Evaluate the function at the two centers in this dimension
    vector<tuple<int,double,vector<double>,double,vector<double>>> vals;
    vector<double> x1;
    vector<double> x2;
    double f1, f2;
    for(int i=0; i<e.size(); i++) {
      if(e[i] == emin) {
	x1.assign(c.begin(), c.end());
	x1[i] -= delta;
	f1 = f.eval(x1);
	
	x2.assign(c.begin(), c.end());
	x2[i] += delta;
	f2 = f.eval(x2);

	vals.push_back(tuple<int,double,vector<double>,double,vector<double>>(i,f1,x1,f2,x2));
      }
    }
    // Sort all the dimensions by the function values at their rectangle centers
    sort(vals.begin(), vals.end(), sort_helper_tuple);

    // Divide the rectangles with smallest value first
    vector<rectangle*> *R = new vector<rectangle*>;
    rectangle* r;
    int dim;
    for(int i=0; i<vals.size(); i++) {
      tie(dim, f1, x1, f2, x2) = vals[i];

      r = new rectangle(x1,e,f1);
      r->e[dim]++; // Shrink left rectangle along this dimension
      R->push_back(r);

      r = new rectangle(x2,e,f2);
      r->e[dim]++; // Shrink right rectangle along this dimension
      R->push_back(r);

      e[dim]++; // Shrink center (this) rectangle along this dimension
    }
    return *R;
  }
};

// Initialize the static members
binary rectangle::f;
//noncvx_wrap rectangle::f(vector<double>{-5.0,-5.0}, vector<double>{5.0,5.0});
//noncvx_wrap rectangle::f(vector<double>{0.0,0.0}, vector<double>{1.0,1.0});
//branin_wrap rectangle::f(vector<double>{-5.0,-5.0}, vector<double>{20.0,20.0});
power_list rectangle::d(20);

int main(int argc, char* argv[]) {
  // Data structures
  list<rectangle*> R; // List of rectangles

  // Initialize with unit hypercube
  double epsilon = 1e-4;
  vector<double> c(10,0.5);
  vector<int> e(10,0);
  rectangle* r = new rectangle(c,e);
  //rectangle* r = new rectangle(vector<double>{0.5,0.5}, vector<int>{0,0});
  double fmin = r->fc;
  R.push_back(r);

  for(int t=0; t<50; t++) {
  
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
      // Divide the rectangle and add new rectangles to the list
      vector<rectangle*> Rt = r->divide();
      for(auto r : Rt) {
	R.push_back(r);
      }

    }
  }

  // Print out all the rectangles
  for(auto r : R) {
    printf("[");
    for(auto ct : r->c) {
      printf("%f, ",ct);
    }
    printf("]\n");
  }
  printf("Number of rectangles = %lu\n",R.size());
  return 0;
}

