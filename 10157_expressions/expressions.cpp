#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

const int MAXDIGITS = 1000;

class bignum {
   public:
      bignum () { digits.resize(MAXDIGITS, 0); lastdigit = 0; positive = true;}
      bignum (int);
      bignum (string);
      
      void print        ();
      void zero_justify ();
      void digit_shift (int);
      
      bignum operator+  (bignum);
      bignum operator-  (bignum);
      bignum operator*  (bignum);
      bignum operator/  (bignum);
      bool   operator<  (bignum);
      bool   operator== (bignum);
      bool   operator<= (bignum b) { return ((*this < b) or (*this == b));}
      
      vector<char> digits;
      bool positive;
      int lastdigit; 
};

bignum::bignum (string s) {
   unsigned int i;
    

   if (s[0] == '-') {
       positive = false;
       s.erase(s.begin());
   } else {
      positive = true;
   }

   digits.resize(MAXDIGITS, 0);

   reverse(s.begin(), s.end());

   for (i = 0; i < s.size(); i++) {
      digits[i] = s[i] - '0';
   }

   lastdigit = (s.size() - 1);
}

bignum::bignum (int s) {
   int i;
   int t;
   
   positive = s >= 0 ? true : false;
   digits.resize(MAXDIGITS, 0);
   
   lastdigit = -1;
   
   t = abs(s);
   
   while (t > 0) {
      lastdigit++;
      digits[lastdigit] = t % 10;
      t /= 10;
   }
   
   if (s == 0) lastdigit = 0;
}

void bignum::print () {
   int i;
   
   if (!positive) cout << "-";
   
   for (i = lastdigit; i >= 0; i--) {
      cout << char('0' + digits[i]);
   }  
   
}

void bignum::zero_justify () { // transform 00042 in 42
   while (lastdigit > 0 && digits[lastdigit] == 0)
      lastdigit--;
   
   if (lastdigit == 0 && digits[0] == 0)
      positive = true; // hack to avoid -0
}

void bignum::digit_shift (int d) {
   int i;
   
   if (lastdigit == 0 && digits[0] == 0) return;
   
   for (i = lastdigit; i >= 0; i--) {
      digits[i+d] = digits[i];
   }
   
   for (i = 0; i < d; i++) digits[i] = 0;
   
   lastdigit += d;
}

bignum bignum::operator+ (bignum b) { // c = a + b
   bignum c, &a = *this;
   int carry;
   int i;

   if (a.positive == b.positive) c.positive = a.positive;
   else { // if only one is negative, then do the substraction
      if (!a.positive) {
         a.positive = true;
         c = b - a;
         a.positive = false;
      } else {
         b.positive = true;
         c = a - b;
         b.positive = false;
      }
      return c;
   }
   
   c.lastdigit = max(a.lastdigit, b.lastdigit) + 1;
   carry = 0;
   
   for (i = 0; i <= c.lastdigit; i++) {
      c.digits[i] = (carry + a.digits[i] + b.digits[i]) % 10;
      carry       = (carry + a.digits[i] + b.digits[i]) / 10;
   }
   
   c.zero_justify();
   
   return c;
}

bignum bignum::operator- (bignum b) { // c = a - b
   int borrow;
   int v;
   int i;
   bignum c, &a = *this;
   
   if (!a.positive || !b.positive) {
      b.positive = !b.positive;
      c = a + b;
      b.positive = !b.positive;
      return c;
   }
   
   if (a < b) {
      c = b - a;
      c.positive = !c.positive;
      return c;
   }
   
   c.lastdigit = max(a.lastdigit, b.lastdigit);
   borrow = 0;
   
   for (i = 0; i <= c.lastdigit; i++) {
      v = a.digits[i] - borrow - b.digits[i];
      if (a.digits[i] > 0)
         borrow = 0;
      if (v < 0) {
         v += 10;
         borrow = 1;
      }
      
      c.digits[i] = v % 10;
   }
   c.zero_justify();
   
   return c;
}

bignum bignum::operator* (bignum b) {
   bignum c, &a = *this;
   bignum row;//, tmp;
   int i, j;
   
   row = a;

   for (i = 0; i <= b.lastdigit; i++) {
      for (j = 1; j <= b.digits[i]; j++) {
         c = row + c;
         //tmp = row + c;
         //c = tmp;
      }
      row.digit_shift(1);
   }
   
   c.positive = (a.positive && b.positive) || (!a.positive && !b.positive);
   c.zero_justify();
   
   return c;
}

bignum bignum::operator/ (bignum b) {
   bignum c, &a = *this;
   bignum row;
   bool asign, bsign;
   int i, j;
   
   c.positive = (a.positive && b.positive) || (!a.positive && !b.positive);
   
   asign = a.positive;
   bsign = b.positive;
   
   a.positive = b.positive = true;
   
   c.lastdigit = a.lastdigit;
   
   for (i = a.lastdigit; i >= 0; i--) {
      row.digit_shift(1);
      row.digits[0] = a.digits[i];
      c.digits[i] = 0;
      while (!(row < b)) {
         c.digits[i]++;
         row = row - b;
      }
   }
   
   c.zero_justify();
   a.positive = asign;
   b.positive = bsign;
   
   return c;
}

bool bignum::operator< (bignum b) { // a < b
   bignum &a = *this;
   int i;
   
   if (!a.positive &&  b.positive) return true;
   if ( a.positive && !b.positive) return false;
   
   if (a.lastdigit < b.lastdigit) return a.positive;
   if (b.lastdigit < a.lastdigit) return !a.positive;
   
   for (i = a.lastdigit; i >= 0; i--) {
      if (a.digits[i] < b.digits[i])
         return a.positive;
      if (b.digits[i] < a.digits[i])
         return !a.positive;
   }
   
   return false;
}

bool bignum::operator== (bignum b) {
   bignum &a = *this;
   int i;
   
   if (a.positive != b.positive) return false;
   
   if (a.lastdigit != b.lastdigit) return false;
   
   for (i = a.lastdigit; i >= 0; i--) {
      if (a.digits[i] != b.digits[i])
         return false;
   }
   
   return true;   
}


const int MAX = 11;

bignum c_mat[MAX][MAX];
bignum f_mat[MAX][MAX];

bignum c(int n, int d) {
    if (d > n) d = n;
    if (n == 0) return bignum(1);
    if ((n < 0) || (d < 0) || (n > MAX)) return bignum(0);

    return c_mat[n][d];
}

bignum f(int n, int d) {
    if ((n < 0) || (d < 0) || (d > n) || (n > MAX)) return bignum(0);
    
    return f_mat[n][d];
}

//sum(k = 0, n-1) [f(k,d-1)*c(n-k-1,d) + c(k,d-2)*f(n-k-1,d)]
void initialize() {
    for (int n = 0; n < MAX; n++) {
        f_mat[n][0] = bignum(0);
        f_mat[n][1] = bignum(1);
        f_mat[n][n] = bignum(1);

        c_mat[n][0] = bignum(0);
        c_mat[n][1] = bignum(1);
    }
    f_mat[0][0] = bignum(1);
    c_mat[0][0] = bignum(1);

    for (int n = 2; n < MAX; n++) {
        for (int d = 2; d < n; d++) {
            //bignum accum(0);
            //for (int k = 0; k < n; k++) {
            //    accum = accum + f(k,d-1)*c(n-k-1,d) + c(k,d-2)*f(n-k-1,d);
            //}
            //f_mat[n][d] = accum;

            bignum accum = f(n-1,d-1);
            for (int k = 1; k < n; k++) {
                accum = accum + f(k,d)*c(n-k,d-1) + c(k,d)*f(n-k,d);
            }
            f_mat[n][d] = accum;
        }

        for (int d = 2; d <= n; d++) {
            c_mat[n][d] = c_mat[n][d-1] + f_mat[n][d];
        }
    }
}

int main () {
    int n, d;
    bignum result;

    initialize();

    for (n = 1; n <= 7; n++) {
        for (d = 1; d <= n; d++) {
            f_mat[n][d].print();
            if (d < n) cout << " | ";
        }
        cout << endl;
    }

    while (cin >> n >> d) {
        result = f(n/2,d);
        result.print();
        cout << endl;
    }

    return 0;
}
