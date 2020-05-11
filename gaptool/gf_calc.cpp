/*
  *********************************************************************
  *                                                                   *
  *               Galois Field Arithmetic Library                     *
  * Prototype: Galois Field STD OUT Test                              *
  * Version: 0.0.1仅支持特征2有限域，用处不大                                                    *
  * Author: Arash Partow - 2000                                       *
  * URL: http://www.partow.net/projects/galois/index.html             *
  *                                                                   *
  * Copyright Notice:                                                 *
  * Free use of this library is permitted under the guidelines and    *
  * in accordance with the most current version of the Common Public  *
  * License.                                                          *
  * http://www.opensource.org/licenses/cpl.php                        *
  *                                                                   *
  *********************************************************************
*/
#include <iostream>
#include <vector>
#include <string.h>
#include <assert.h>

namespace galois
{
   typedef int      GFSymbol;
   const   GFSymbol GFERROR = -1;

   class GaloisField
   {

      public:

       GaloisField();
       GaloisField(const int  pwr, const unsigned int* primitive_poly);
       GaloisField(const GaloisField& gf);
      ~GaloisField();

       GaloisField& operator=(const GaloisField& gf);
       bool operator==(const GaloisField& gf);


       inline GFSymbol index(const GFSymbol value) const
       {
          return index_of[value];
       }


       inline GFSymbol alpha(const GFSymbol value) const
       {
          return alpha_to[value];
       }


       inline unsigned int size() const
       {
          return field_size;
       }


       inline unsigned int pwr() const
       {
          return power;
       }


       inline GFSymbol add(const GFSymbol& a, const GFSymbol& b)
       {
          return (a ^ b);
       }


       inline GFSymbol sub(const GFSymbol& a, const GFSymbol& b)
       {
          return (a ^ b);
       }


       inline GFSymbol mul(const GFSymbol& a, const GFSymbol& b)
       {
          #if !defined(NO_GFLUT)
            return mul_table[a][b];
          #else
            if ((a == 0) || (b == 0))
              return 0;
            else
              return alpha_to[fast_modulus(index_of[a] + index_of[b])];
          #endif
       }


       inline GFSymbol div(const GFSymbol& a, const GFSymbol& b)
       {
          #if !defined(NO_GFLUT)
            return div_table[a][b];
          #else
            if ((a == 0) || (b == 0))
              return 0;
            else
              return alpha_to[fast_modulus(index_of[a] - index_of[b] + field_size)];
          #endif
       }


       inline GFSymbol exp(const GFSymbol& a, const int& n)
       {
          #if !defined(NO_GFLUT)
            if (n < 0)
            {
               int b = n;
                while(b < 0) b += field_size;// b could be negative

               if (b == 0)
                 return 1;
               return exp_table[a][b];
            }
            else
              return exp_table[a][n & field_size];
          #else
            if (a != 0)
            {
               if (n < 0)
               {
                  int b = n;
                  while(b < 0) b += field_size;// b could be negative
                  if (b == 0)
                    return 1;
                  return alpha_to[fast_modulus(index_of[a] * b)];
               }
               else if (n == 0)
                 return 1;
               else
                 return alpha_to[fast_modulus(index_of[a] * n)];
            }
            else
              return 0;
          #endif
       }


       inline GFSymbol inverse(const GFSymbol& val)
       {
          #if !defined(NO_GFLUT)
            return mul_inverse[val];
          #else
            return alpha_to[fast_modulus(field_size - index_of[val])];
          #endif
       }


       friend std::ostream& operator << (std::ostream& os, const GaloisField& gf);


      private:

       void     generate_field(const unsigned int* prim_poly);
       GFSymbol fast_modulus  (GFSymbol                    x);
       GFSymbol gen_mul       (const GFSymbol& a, const GFSymbol&     b);
       GFSymbol gen_div       (const GFSymbol& a, const GFSymbol&     b);
       GFSymbol gen_exp       (const GFSymbol& a, const unsigned int& n);
       GFSymbol gen_inverse   (const GFSymbol& val);

      private:

       unsigned int              power;
       unsigned int              field_size;
       unsigned int              prim_poly_hash;
       GFSymbol*                 alpha_to;    // aka exponential or anti-log
       GFSymbol*                 index_of;    // aka log
       GFSymbol*                 mul_inverse; // multiplicative inverse
       GFSymbol**                mul_table;
       GFSymbol**                div_table;
       GFSymbol**                exp_table;

   };


}

namespace galois
{


   GaloisField::GaloisField() : power(0), field_size(0)
   {
      alpha_to       = new GFSymbol  [1];
      index_of       = new GFSymbol  [1];
      mul_inverse    = new GFSymbol  [1];
      mul_table      = new GFSymbol *[1];
      div_table      = new GFSymbol *[1];
      exp_table      = new GFSymbol *[1];
      prim_poly_hash = 0;
   }


   GaloisField::GaloisField(const int  pwr, const unsigned int* primitive_poly) : power(pwr), field_size((1 << power)-1)
   {
      alpha_to    = new GFSymbol [field_size + 1];
      index_of    = new GFSymbol [field_size + 1];

      #if !defined(NO_GFLUT)

        mul_table   = new GFSymbol* [(field_size + 1)];
        div_table   = new GFSymbol* [(field_size + 1)];
        exp_table   = new GFSymbol* [(field_size + 1)];
        mul_inverse = new GFSymbol  [(field_size + 1) * 2];

        for (unsigned int i = 0; i < (field_size + 1); i++)
        {
           mul_table[i] = new GFSymbol [(field_size + 1)];
           div_table[i] = new GFSymbol [(field_size + 1)];
           exp_table[i] = new GFSymbol [(field_size + 1)];
        }

      #else

        mul_table   = new GFSymbol *[1];
        div_table   = new GFSymbol *[1];
        exp_table   = new GFSymbol *[1];
        mul_inverse = new GFSymbol  [1];

      #endif

      prim_poly_hash = 0xAAAAAAAA;

      for (unsigned int i = 0; i < power; i++)
      {
         prim_poly_hash += ((i & 1) == 0) ? (  (prim_poly_hash <<  7) ^ primitive_poly[i] ^ (prim_poly_hash >> 3)) :
                                            (~((prim_poly_hash << 11) ^ primitive_poly[i] ^ (prim_poly_hash >> 5)));
      }

      generate_field(primitive_poly);
   }


   GaloisField::GaloisField(const GaloisField& gf)
   {
      power          = gf.power;
      field_size     = gf.field_size;
      prim_poly_hash = gf.prim_poly_hash;
      alpha_to       = new GFSymbol [field_size + 1];
      index_of       = new GFSymbol [field_size + 1];

      memcpy(alpha_to, gf.alpha_to, (field_size + 1) * sizeof(GFSymbol));
      memcpy(index_of, gf.index_of, (field_size + 1) * sizeof(GFSymbol));

      #if !defined(NO_GFLUT)

        mul_table   = new GFSymbol* [(field_size + 1)];
        div_table   = new GFSymbol* [(field_size + 1)];
        exp_table   = new GFSymbol* [(field_size + 1)];
        mul_inverse = new GFSymbol  [(field_size + 1) * 2];

        for (unsigned int i = 0; i < (field_size + 1); i++)
        {
           mul_table[i] = new GFSymbol [(field_size + 1)];
           div_table[i] = new GFSymbol [(field_size + 1)];
           exp_table[i] = new GFSymbol [(field_size + 1)];
        }

        memcpy(mul_inverse, gf.mul_inverse, (field_size + 1) * sizeof(GFSymbol) * 2);

        memcpy(mul_table, gf.mul_table, (field_size + 1) * sizeof(GFSymbol*));
        memcpy(div_table, gf.div_table, (field_size + 1) * sizeof(GFSymbol*));
        memcpy(exp_table, gf.exp_table, (field_size + 1) * sizeof(GFSymbol*));

        for (unsigned int i = 0; i < (field_size + 1); i++)
        {
           memcpy(mul_table[i], gf.mul_table[i], (field_size + 1) * sizeof(GFSymbol));
           memcpy(div_table[i], gf.div_table[i], (field_size + 1) * sizeof(GFSymbol));
           memcpy(exp_table[i], gf.exp_table[i], (field_size + 1) * sizeof(GFSymbol));
        }

      #endif
   }


   GaloisField::~GaloisField()
   {
      if (alpha_to != NULL) delete [] alpha_to;
      if (index_of != NULL) delete [] index_of;

      #if !defined(NO_GFLUT)

        for (unsigned int i = 0; i < (field_size + 1); i++)
        {
           if (mul_table[i] != NULL) delete [] mul_table[i];
           if (div_table[i] != NULL) delete [] div_table[i];
           if (exp_table[i] != NULL) delete [] exp_table[i];
        }

        if (mul_table   != NULL) delete [] mul_table;
        if (div_table   != NULL) delete [] div_table;
        if (exp_table   != NULL) delete [] exp_table;
        if (mul_inverse != NULL) delete [] mul_inverse;

      #endif
   }


   bool GaloisField::operator==(const GaloisField& gf)
   {
      return (
              (this->power          ==          gf.power) &&
              (this->prim_poly_hash == gf.prim_poly_hash)
             ) ;
   }


   GaloisField& GaloisField::operator=(const GaloisField& gf)
   {
      if (this == &gf)
         return *this;

      if (alpha_to != NULL) delete [] alpha_to;
      if (index_of != NULL) delete [] index_of;

      power          = gf.power;
      field_size     = gf.field_size;
      prim_poly_hash = gf.prim_poly_hash;

      memcpy(alpha_to, gf.alpha_to, (field_size + 1) * sizeof(GFSymbol));
      memcpy(index_of, gf.index_of, (field_size + 1) * sizeof(GFSymbol));

      #if !defined(NO_GFLUT)

        if (mul_table   != NULL) delete [] mul_table;
        if (div_table   != NULL) delete [] div_table;
        if (exp_table   != NULL) delete [] exp_table;
        if (mul_inverse != NULL) delete [] mul_inverse;

        mul_table   = new GFSymbol* [(field_size + 1)];
        div_table   = new GFSymbol* [(field_size + 1)];
        exp_table   = new GFSymbol* [(field_size + 1)];
        mul_inverse = new GFSymbol  [(field_size + 1) * 2];

        for (unsigned int i = 0; i < (field_size + 1); i++)
        {
           mul_table[i] = new GFSymbol [(field_size + 1)];
           div_table[i] = new GFSymbol [(field_size + 1)];
           exp_table[i] = new GFSymbol [(field_size + 1)];
        }

        memcpy(mul_inverse, gf.mul_inverse, (field_size + 1) * sizeof(GFSymbol) * 2);

        memcpy(mul_table, gf.mul_table, (field_size + 1) * sizeof(GFSymbol*));
        memcpy(div_table, gf.div_table, (field_size + 1) * sizeof(GFSymbol*));
        memcpy(exp_table, gf.exp_table, (field_size + 1) * sizeof(GFSymbol*));


        for (unsigned int i = 0; i < (field_size + 1); i++)
        {
           memcpy(mul_table[i], gf.mul_table[i], (field_size + 1) * sizeof(GFSymbol));
           memcpy(div_table[i], gf.div_table[i], (field_size + 1) * sizeof(GFSymbol));
           memcpy(exp_table[i], gf.exp_table[i], (field_size + 1) * sizeof(GFSymbol));
        }

      #endif

      return *this;
   }


   void GaloisField::generate_field(const unsigned int* prim_poly)
   {
      /*
         Note: It is assumed that the degree of the primitive
               polynomial will be equivelent to the m value as
               in GF(2^m)
      */

      /*
         need to update using stanford method for prim-poly generation.
      */
      int mask = 1;

      alpha_to[power] = 0;

      for (unsigned int i = 0; i < power; i++)
      {
         alpha_to[i]           = mask;
         index_of[alpha_to[i]] = i;

         if (prim_poly[i] != 0)
         {
            alpha_to[power] ^= mask;
         }

         mask <<= 1;
      }

      index_of[alpha_to[power]] = power;

      mask >>= 1;

      for (unsigned int i = power + 1; i < field_size; i++)
      {
         if (alpha_to[i - 1] >= mask)
           alpha_to[i] = alpha_to[power] ^ ((alpha_to[i - 1] ^ mask) << 1);
         else
           alpha_to[i] = alpha_to[i - 1] << 1;

         index_of[alpha_to[i]] = i;
      }

      index_of[0] = GFERROR;
      alpha_to[field_size] = 1;

      #if !defined(NO_GFLUT)

        for (unsigned int i = 0; i < field_size + 1; i++)
        {
           for (unsigned int j = 0; j < field_size + 1; j++)
           {
              mul_table[i][j] = gen_mul(i,j);
              div_table[i][j] = gen_div(i,j);
              exp_table[i][j] = gen_exp(i,j);
           }
        }

        for (unsigned int i = 0; i < (field_size + 1); i++)
        {
           mul_inverse[i] = gen_inverse(i);
           mul_inverse[i + (field_size + 1)] = mul_inverse[i];
        }

      #endif
   }


   GFSymbol GaloisField::fast_modulus(GFSymbol x)
   {
      while (x >= (int)field_size)
      {
         x -= (int)field_size;
         x  = (x >> power) + (x & (int)field_size);
      }

      return x;
   }


   GFSymbol GaloisField::gen_mul(const GFSymbol& a, const GFSymbol& b)
   {
      if ((a == 0) || (b == 0))
        return 0;
      else
        return alpha_to[fast_modulus(index_of[a] + index_of[b])];
   }


   GFSymbol GaloisField::gen_div(const GFSymbol& a, const GFSymbol& b)
   {
      if ((a == 0) || (b == 0))
        return 0;
      else
        return alpha_to[fast_modulus(index_of[a] - index_of[b] + field_size)];
   }


   GFSymbol GaloisField::gen_exp(const GFSymbol& a, const unsigned int& n)
   {
      if (a != 0)
      {
         if (n == 0)
           return 1;
         else
           return alpha_to[fast_modulus(index_of[a] * n)];
      }
      else
        return 0;
   }


   GFSymbol GaloisField::gen_inverse(const GFSymbol& val)
   {
      return alpha_to[fast_modulus(field_size - index_of[val])];
   }


   std::ostream& operator << (std::ostream& os, const GaloisField& gf)
   {
      for(unsigned int i = 0; i < gf.field_size + 1; i++)
      {
         os << i << "\t" << gf.alpha_to[i] << "\t" << gf.index_of[i] << std::endl;
      }

      return os;
   }

}

namespace galois
{

   class GaloisFieldElement
   {

      public:

       GaloisFieldElement(GaloisField* _gf = NULL, GFSymbol v = -1);
       GaloisFieldElement(const GaloisFieldElement& gfe);
      ~GaloisFieldElement(){}

       inline GaloisFieldElement& operator=(const GaloisFieldElement& gfe)
       {
          if (this == &gfe)
            return *this;

          gf          = gfe.gf;
          poly_value  = gfe.poly_value;

          return *this;
       }


       inline GaloisFieldElement& operator=(const GFSymbol& v)
       {
          poly_value  = v & gf->size();
          return *this;
       }


       inline GaloisFieldElement& operator+=(const GaloisFieldElement& gfe)
       {
          poly_value ^= gfe.poly_value;
          return *this;
       }


       inline GaloisFieldElement& operator+=(const GFSymbol& v)
       {
          poly_value ^= v;
          return *this;
       }


       inline GaloisFieldElement& operator-=(const GaloisFieldElement& gfe)
       {
           *this += gfe;
           return *this;
       }


       inline GaloisFieldElement& operator-=(const GFSymbol& v)
       {
          *this += v;
          return *this;
       }


       inline GaloisFieldElement& operator*=(const GaloisFieldElement& gfe)
       {
          poly_value = gf->mul(poly_value,gfe.poly_value);
          return *this;
       }


       inline GaloisFieldElement& operator*=(const GFSymbol& v)
       {
          poly_value = gf->mul(poly_value,v);
          return *this;
       }


       inline GaloisFieldElement& operator/=(const GaloisFieldElement& gfe)
       {
          poly_value = gf->div(poly_value,gfe.poly_value);
          return *this;
       }


       inline GaloisFieldElement& operator/=(const GFSymbol& v)
       {
          poly_value = gf->div(poly_value,v);
          return *this;
       }


       inline GaloisFieldElement& operator^=(const int& n)
       {
          poly_value = gf->exp(poly_value,n);
          return *this;
       }


       inline bool operator==(const GaloisFieldElement& gfe) const
       {
          return (
                  (gf  == gfe.gf) &&
                  (poly_value == gfe.poly_value)
                 );
       }


       inline bool operator==(const GFSymbol& v) const
       {
          return (poly_value == v);
       }


       inline bool operator!=(const GaloisFieldElement& gfe) const
       {
          return (
                  (gf  != gfe.gf) ||
                  (poly_value != gfe.poly_value)
                 );
       }


       inline bool operator!=(const GFSymbol& v) const
       {
          return (poly_value != v);
       }


       inline bool operator<(const GaloisFieldElement& gfe)
       {
          return (poly_value < gfe.poly_value);
       }


       inline bool operator<(const GFSymbol& v)
       {
          return (poly_value < v);
       }

       inline bool operator>(const GaloisFieldElement& gfe)
       {
          return (poly_value > gfe.poly_value);
       }


       inline bool operator>(const GFSymbol& v)
       {
          return (poly_value > v);
       }

       inline GFSymbol index() const
       {
          return gf->index(poly_value);
       }


       inline GFSymbol poly() const
       {
          return poly_value;
       }


       inline GaloisField* field() const
       {
          return gf;
       }


       inline GFSymbol inverse() const
       {
          return gf->inverse(poly_value);
       }


       friend std::ostream& operator << (std::ostream& os, const GaloisFieldElement& gfe);

      private:

       GaloisField* gf;
       GFSymbol     poly_value;

   };

   GaloisFieldElement operator + (const GaloisFieldElement& a, const GaloisFieldElement& b);
   GaloisFieldElement operator - (const GaloisFieldElement& a, const GaloisFieldElement& b);
   GaloisFieldElement operator * (const GaloisFieldElement& a, const GaloisFieldElement& b);
   GaloisFieldElement operator * (const GaloisFieldElement& a, const GFSymbol& b          );
   GaloisFieldElement operator * (const GFSymbol& a,           const GaloisFieldElement& b);
   GaloisFieldElement operator / (const GaloisFieldElement& a, const GaloisFieldElement& b);
   GaloisFieldElement operator ^ (const GaloisFieldElement& a, const int& b               );

}

namespace galois
{

   GaloisFieldElement::GaloisFieldElement(GaloisField* _gf, GFSymbol v)
   {
      if (_gf != NULL)
      {
         gf         = _gf;
         poly_value = v & gf->size();
      }
      else
        poly_value = v;
   }


   GaloisFieldElement::GaloisFieldElement(const GaloisFieldElement& gfe)
   {
      gf          = gfe.gf;
      poly_value  = gfe.poly_value;
   }


   std::ostream& operator << (std::ostream& os, const GaloisFieldElement& gfe)
   {
      os << gfe.poly_value;
      return os;
   }


   GaloisFieldElement operator+(const GaloisFieldElement& a, const GaloisFieldElement& b)
   {
      GaloisFieldElement result  = a;
      result += b;
      return result;
   }


   GaloisFieldElement operator-(const GaloisFieldElement& a, const GaloisFieldElement& b)
   {
      GaloisFieldElement result  = a;
      result -= b;
      return result;
   }


   GaloisFieldElement operator*(const GaloisFieldElement& a, const GaloisFieldElement& b)
   {
      GaloisFieldElement result  = a;
      result *= b;
      return result;
   }


   GaloisFieldElement operator*(const GaloisFieldElement& a, const GFSymbol& b)
   {
      GaloisFieldElement result  = a;
      result *= b;
      return result;
   }


   GaloisFieldElement operator*(const GFSymbol& a, const GaloisFieldElement& b)
   {
      GaloisFieldElement result  = b;
      result *= a;
      return result;
   }


   GaloisFieldElement operator/(const GaloisFieldElement& a, const GaloisFieldElement& b)
   {
      GaloisFieldElement result  = a;
      result /= b;
      return result;
   }


   GaloisFieldElement operator^(const GaloisFieldElement& a, const int& b)
   {
      GaloisFieldElement result  = a;
      result ^= b;
      return result;
   }

}

namespace galois
{

   class GaloisFieldPolynomial
   {

      public:

       GaloisFieldPolynomial(GaloisField* _gf);
       GaloisFieldPolynomial(GaloisField* _gf = NULL, const unsigned int size = 0, GaloisFieldElement* gfe = NULL);
       GaloisFieldPolynomial(const GaloisFieldPolynomial& polynomial);
       GaloisFieldPolynomial(const GaloisFieldElement& gfe);
      ~GaloisFieldPolynomial(){};

       bool valid() const;
       unsigned int deg() const;
       GaloisField* field() const;
       void set_degree(const unsigned int& x);
       void simplify();

       GaloisFieldPolynomial& operator = (const GaloisFieldPolynomial& polynomial);
       GaloisFieldPolynomial& operator = (const GaloisFieldElement&           gfe);
       GaloisFieldPolynomial& operator+= (const GaloisFieldPolynomial& polynomial);
       GaloisFieldPolynomial& operator+= (const GaloisFieldElement&           gfe);
       GaloisFieldPolynomial& operator-= (const GaloisFieldPolynomial& polynomial);
       GaloisFieldPolynomial& operator-= (const GaloisFieldElement&           gfe);
       GaloisFieldPolynomial& operator*= (const GaloisFieldPolynomial& polynomial);
       GaloisFieldPolynomial& operator*= (const GaloisFieldElement&           gfe);
       GaloisFieldPolynomial& operator/= (const GaloisFieldPolynomial&    divisor);
       GaloisFieldPolynomial& operator/= (const GaloisFieldElement&           gfe);
       GaloisFieldPolynomial& operator%= (const GaloisFieldPolynomial&    divisor);
       GaloisFieldPolynomial& operator%= (const unsigned int&               power);
       GaloisFieldPolynomial& operator^= (const int&                            n);
       GaloisFieldPolynomial& operator<<=(const unsigned int&                   n);
       GaloisFieldPolynomial& operator>>=(const unsigned int&                   n);

       GaloisFieldElement&    operator[] (const unsigned int&                term);
       GaloisFieldElement     operator() (const GaloisFieldElement&         value);
       GaloisFieldElement     operator() (GFSymbol                          value);

       const GaloisFieldElement&  operator[](const unsigned int&        term) const;
       const GaloisFieldElement   operator()(const GaloisFieldElement& value) const;
       const GaloisFieldElement   operator()(GFSymbol                  value) const;

       bool operator==(const GaloisFieldPolynomial& polynomial) const;
       bool operator!=(const GaloisFieldPolynomial& polynomial) const;

       GaloisFieldPolynomial derivative();

       friend std::ostream& operator << (std::ostream& os, const GaloisFieldPolynomial& polynomial);

      private:

       void simplify(GaloisFieldPolynomial& polynomial);

       GaloisField* gf;
       std::vector< GaloisFieldElement > poly;
   };

   GaloisFieldPolynomial operator + (const GaloisFieldPolynomial& a, const GaloisFieldPolynomial& b);
   GaloisFieldPolynomial operator + (const GaloisFieldPolynomial& a, const GaloisFieldElement&    b);
   GaloisFieldPolynomial operator + (const GaloisFieldElement&    a, const GaloisFieldPolynomial& b);
   GaloisFieldPolynomial operator + (const GaloisFieldPolynomial& a, const GFSymbol&              b);
   GaloisFieldPolynomial operator + (const GFSymbol&              a, const GaloisFieldPolynomial& b);
   GaloisFieldPolynomial operator - (const GaloisFieldPolynomial& a, const GaloisFieldPolynomial& b);
   GaloisFieldPolynomial operator - (const GaloisFieldPolynomial& a, const GaloisFieldElement&    b);
   GaloisFieldPolynomial operator - (const GaloisFieldElement&    a, const GaloisFieldPolynomial& b);
   GaloisFieldPolynomial operator - (const GaloisFieldPolynomial& a, const GFSymbol&              b);
   GaloisFieldPolynomial operator - (const GFSymbol&              a, const GaloisFieldPolynomial& b);
   GaloisFieldPolynomial operator * (const GaloisFieldPolynomial& a, const GaloisFieldPolynomial& b);
   GaloisFieldPolynomial operator * (const GaloisFieldElement&    a, const GaloisFieldPolynomial& b);
   GaloisFieldPolynomial operator * (const GaloisFieldPolynomial& a, const GaloisFieldElement&    b);
   GaloisFieldPolynomial operator / (const GaloisFieldPolynomial& a, const GaloisFieldPolynomial& b);
   GaloisFieldPolynomial operator / (const GaloisFieldPolynomial& a, const GaloisFieldElement&    b);
   GaloisFieldPolynomial operator % (const GaloisFieldPolynomial& a, const GaloisFieldPolynomial& b);
   GaloisFieldPolynomial operator % (const GaloisFieldPolynomial& a, const unsigned int&      power);
   GaloisFieldPolynomial operator ^ (const GaloisFieldPolynomial& a, const int&                   n);
   GaloisFieldPolynomial operator <<(const GaloisFieldPolynomial& a, const unsigned int&          n);
   GaloisFieldPolynomial operator >>(const GaloisFieldPolynomial& a, const unsigned int&          n);
   GaloisFieldPolynomial         gcd(const GaloisFieldPolynomial& a, const GaloisFieldPolynomial& b);

}

namespace galois
{

   GaloisFieldPolynomial::GaloisFieldPolynomial(GaloisField* _gf)
   {
      gf = _gf;
      poly.clear();
   }


   GaloisFieldPolynomial::GaloisFieldPolynomial(GaloisField* _gf, const unsigned int size, GaloisFieldElement* gfe)
   {
      gf = _gf;
      poly.resize(size + 1);

      if (gfe != NULL)
      {
         for(unsigned int i = 0; i <= size; i++)
         {
            poly[i] = gfe[i];
         }
      }
      else
      {
         for(unsigned int i = 0; i < poly.size(); i++)
         {
            poly[i] = GaloisFieldElement(gf,0);
         }
      }
   }


   GaloisFieldPolynomial::GaloisFieldPolynomial(const GaloisFieldPolynomial& polynomial)
   {
      gf   = polynomial.gf;
      poly = polynomial.poly;
   }


   GaloisFieldPolynomial::GaloisFieldPolynomial(const GaloisFieldElement& gfe)
   {
      gf = gfe.field();
      poly.clear();
      poly.push_back(gfe);
   }


   bool GaloisFieldPolynomial::valid() const
   {
      return (poly.size() > 0);
   }


   unsigned int GaloisFieldPolynomial::deg() const
   {
      return static_cast<unsigned int>(poly.size() - 1);
   }


   GaloisField* GaloisFieldPolynomial::field() const
   {
      return gf;
   }


   void GaloisFieldPolynomial::set_degree(const unsigned int& x)
   {
      poly.resize(x - 1,GaloisFieldElement(gf,0));
   }


   GaloisFieldPolynomial& GaloisFieldPolynomial::operator=(const GaloisFieldPolynomial& polynomial)
   {
      if (this == &polynomial)
        return *this;

      gf   = polynomial.gf;
      poly = polynomial.poly;

      return *this;
   }


   GaloisFieldPolynomial& GaloisFieldPolynomial::operator=(const GaloisFieldElement& gfe)
   {
      poly.clear();
      gf = gfe.field();
      poly.push_back(gfe);
      return *this;
   }


   GaloisFieldPolynomial& GaloisFieldPolynomial::operator+=(const GaloisFieldPolynomial& polynomial)
   {
      if (gf == polynomial.gf)
      {
         if (poly.size() < polynomial.poly.size())
         {
            unsigned int j = 0;
            for (unsigned int i = 0; i < poly.size(); i++)
            {
               poly[i] += polynomial.poly[j++];
            }

            for ( ; j < polynomial.poly.size(); j++)
            {
               poly.push_back(polynomial.poly[j]);
            }
         }
         else
         {
            unsigned int i = 0;
            for (unsigned int j = 0; j < polynomial.poly.size(); j++)
            {
               poly[i++] += polynomial.poly[j];
            }
         }

         simplify(*this);
      }

      return *this;
   }


   GaloisFieldPolynomial& GaloisFieldPolynomial::operator+=(const GaloisFieldElement& gfe)
   {
      poly[0] += gfe;
      return *this;
   }


   GaloisFieldPolynomial& GaloisFieldPolynomial::operator-=(const GaloisFieldPolynomial& gfe)
   {
      return (*this += gfe);
   }


   GaloisFieldPolynomial& GaloisFieldPolynomial::operator-=(const GaloisFieldElement& gfe)
   {
      poly[0] -= gfe;
      return *this;
   }


   GaloisFieldPolynomial& GaloisFieldPolynomial::operator*=(const GaloisFieldPolynomial& polynomial)
   {
      if (gf == polynomial.gf)
      {
         GaloisFieldPolynomial product(gf,deg() + polynomial.deg() + 1);

         for (unsigned int  i= 0; i < poly.size(); i++)
         {
            for (unsigned int j = 0; j < polynomial.poly.size(); j++)
            {
               product.poly[i + j] += poly[i] * polynomial.poly[j];
            }
         }

         simplify(product);
         poly = product.poly;
      }
      return *this;
   }


   GaloisFieldPolynomial& GaloisFieldPolynomial::operator*=(const GaloisFieldElement& gfe)
   {
      if (gf == gfe.field())
      {
         for(unsigned int i = 0; i < poly.size(); i++)
         {
            poly[i] *= gfe;
         }
      }
      return *this;
   }


   GaloisFieldPolynomial& GaloisFieldPolynomial::operator/=(const GaloisFieldPolynomial& divisor)
   {
      if (
          (gf            ==    divisor.gf) &&
          (deg()         >= divisor.deg()) &&
          (divisor.deg() >=             0)
         )
      {
         GaloisFieldPolynomial  quotient(gf, deg() - divisor.deg() + 1);
         GaloisFieldPolynomial remainder(gf, divisor.deg() - 1);

         for(int i = deg(); i >= 0; i--)
         {
            if (i <= (int)quotient.deg())
            {
               quotient[i] = remainder[remainder.deg()] / divisor[divisor.deg()];

               for(int j = remainder.deg(); j > 0; j--)
               {
                  remainder[j] = remainder[j - 1] + (quotient[i] * divisor[j]);
               }

               remainder[0] = poly[i] + (quotient[i] * divisor[0]);
            }
            else
            {
               for(int j = remainder.deg(); j > 0; j--)
               {
                  remainder[j] = remainder[j - 1];
               }
               remainder[0] = poly[i];
            }
         }

         simplify(quotient);

         poly = quotient.poly;
      }

      return *this;
   }


   GaloisFieldPolynomial& GaloisFieldPolynomial::operator/=(const GaloisFieldElement& gfe)
   {
      if (gf == gfe.field())
      {
         for (unsigned int i = 0;  i < poly.size(); i++)
         {
            poly[i] /= gfe;
         }
      }
      return *this;
   }


   GaloisFieldPolynomial& GaloisFieldPolynomial::operator%=(const GaloisFieldPolynomial& divisor)
   {
      if (
          (gf            ==    divisor.gf) &&
          (deg()         >= divisor.deg()) &&
          (divisor.deg() >=             0)
         )
      {
         GaloisFieldPolynomial  quotient(gf, deg() - divisor.deg() + 1);
         GaloisFieldPolynomial remainder(gf, divisor.deg() - 1);

         for(int i = deg(); i >= 0; i--)
         {
            if (i <= (int)quotient.deg())
            {
               quotient[i] = remainder[remainder.deg()] / divisor[divisor.deg()];

               for(int j = remainder.deg(); j > 0; j--)
               {
                  remainder[j] = remainder[j - 1] + (quotient[i] * divisor[j]);
               }

               remainder[0] = poly[i] + (quotient[i] * divisor[0]);
            }
            else
            {
               for(int j = remainder.deg(); j > 0; j--)
               {
                  remainder[j] = remainder[j - 1];
               }
               remainder[0] = poly[i];
            }
         }

         simplify(remainder);
         poly = remainder.poly;
      }

      return *this;
   }


   GaloisFieldPolynomial& GaloisFieldPolynomial::operator%=(const unsigned int& power)
   {
      if (poly.size() >= power)
        poly.resize(power);
      simplify(*this);
      return *this;
   }


   GaloisFieldPolynomial& GaloisFieldPolynomial::operator^=(const int& n)
   {
      GaloisFieldPolynomial result = *this;

      for (int i = 0; i < n; i++)
      {
         result *= *this;
      }

      *this = result;

      return *this;
   }


   GaloisFieldPolynomial& GaloisFieldPolynomial::operator<<=(const unsigned int& n)
   {
      if (poly.size() > 0)
      {
         std::size_t initial_size = poly.size();
         poly.resize(poly.size() + n, GaloisFieldElement(gf,0));

         for(std::size_t i = initial_size - 1; static_cast<int>(i) >= 0; i--)
         {
            poly[i + n] = poly[i];
         }

         for(unsigned int i = 0; i < n; i++)
         {
            poly[i] = 0;
         }
      }
      return *this;
   }


   GaloisFieldPolynomial& GaloisFieldPolynomial::operator>>=(const unsigned int& n)
   {
      if (n <= poly.size())
      {
         for(unsigned int i = 0;  i <= deg() - n; i++)
         {
            poly[i] = poly[i + n];
         }

         poly.resize(poly.size() - n,GaloisFieldElement(gf,0));
      }
      else if (n >= deg() + 1)
      {
         poly.resize(0,GaloisFieldElement(gf,0));
      }
      return *this;
   }


   const GaloisFieldElement& GaloisFieldPolynomial::operator[](const unsigned int& term) const
   {
      assert(term < poly.size());
      return poly[term];
   }


   GaloisFieldElement& GaloisFieldPolynomial::operator[](const unsigned int& term)
   {
      assert(term < poly.size());
      return poly[term];
   }


   GaloisFieldElement GaloisFieldPolynomial::operator()(const GaloisFieldElement& value)
   {
      GaloisFieldElement result(gf,0);

      if (poly.size() > 0)
      {
         result = poly[poly.size() - 1];
         for(std::size_t i = poly.size() - 2; ((int)i) >= 0; i--)
         {
            result = poly[i] + (result * value);
         }
      }
      return result;
   }


   const GaloisFieldElement GaloisFieldPolynomial::operator()(const GaloisFieldElement& value) const
   {
      GaloisFieldElement result(gf,0);

      if (poly.size() > 0)
      {
         result = poly[poly.size() - 1];
         for(std::size_t i = poly.size() - 2; static_cast<int>(i) >= 0; i--)
         {
            result = poly[i] + (result * value);
         }
      }
      return result;
   }


   GaloisFieldElement GaloisFieldPolynomial::operator()(GFSymbol value)
   {
      return (*this)(GaloisFieldElement(gf,value));
   }


   const GaloisFieldElement GaloisFieldPolynomial::operator()(GFSymbol value) const
   {
      return (*this)(GaloisFieldElement(gf,value));
   }


   bool GaloisFieldPolynomial::operator==(const GaloisFieldPolynomial& polynomial) const
   {
      if (gf == polynomial.gf)
      {
         if (poly.size() != polynomial.poly.size())
           return false;
         else
         {
            for (unsigned int i = 0; i < poly.size(); i++)
            {
               if (poly[i] != polynomial.poly[i])
                 return false;
            }
            return true;
         }
      }
      else
        return false;
   }


   bool GaloisFieldPolynomial::operator!=(const GaloisFieldPolynomial& polynomial) const
   {
      return !(*this == polynomial);
   }


   GaloisFieldPolynomial GaloisFieldPolynomial::derivative()
   {
      if ((*this).poly.size() > 1)
      {
         GaloisFieldPolynomial deriv(gf,deg());
         for (unsigned int i = 0; i < poly.size() - 1; i++)
         {
            if (((i + 1) & 1) == 1)
              deriv.poly[i] = poly[i + 1];
            else
              deriv.poly[i] = 0;
         }
         simplify(deriv);
         return deriv;
      }
      return GaloisFieldPolynomial(gf,0);
   }


   void GaloisFieldPolynomial::simplify()
   {
      simplify(*this);
   }


   void GaloisFieldPolynomial::simplify(GaloisFieldPolynomial& polynomial)
   {
      if (poly.size() > 0)
      {
         std::size_t last = polynomial.poly.size() - 1;

         while((last >= 0) && (polynomial.poly.size() > 0))
         {
            if (polynomial.poly[last] == 0)
              polynomial.poly.pop_back();
            else
              break;

            last = polynomial.poly.size() - 1;
         }
      }
   }


   GaloisFieldPolynomial operator+(const GaloisFieldPolynomial& a, const GaloisFieldPolynomial& b)
   {
      GaloisFieldPolynomial result = a;
      result += b;
      return result;
   }


   GaloisFieldPolynomial operator + (const GaloisFieldPolynomial& a, const GaloisFieldElement& b)
   {
      GaloisFieldPolynomial result = a;
      result += b;
      return result;
   }


   GaloisFieldPolynomial operator + (const GaloisFieldElement& a, const GaloisFieldPolynomial& b)
   {
      GaloisFieldPolynomial result = b;
      result += a;
      return result;
   }


   GaloisFieldPolynomial operator + (const GaloisFieldPolynomial& a, const GFSymbol& b)
   {
      return a + GaloisFieldElement(a.field(),b);
   }


   GaloisFieldPolynomial operator + (const GFSymbol& a, const GaloisFieldPolynomial& b)
   {
      return b + GaloisFieldElement(b.field(),a);
   }


   GaloisFieldPolynomial operator - (const GaloisFieldPolynomial& a, const GaloisFieldPolynomial& b)
   {
      GaloisFieldPolynomial result = a;
      result -= b;
      return result;
   }

   GaloisFieldPolynomial operator - (const GaloisFieldPolynomial& a, const GaloisFieldElement& b)
   {
      GaloisFieldPolynomial result = a;
      result -= b;
      return result;
   }


   GaloisFieldPolynomial operator - (const GaloisFieldElement& a, const GaloisFieldPolynomial& b)
   {
      GaloisFieldPolynomial result = b;
      result -= a;
      return result;
   }


   GaloisFieldPolynomial operator - (const GaloisFieldPolynomial& a, const GFSymbol& b)
   {
      return a - GaloisFieldElement(a.field(),b);
   }


   GaloisFieldPolynomial operator - (const GFSymbol& a, const GaloisFieldPolynomial& b)
   {
      return b - GaloisFieldElement(b.field(),a);
   }


   GaloisFieldPolynomial operator * (const GaloisFieldPolynomial& a, const GaloisFieldPolynomial& b)
   {
      GaloisFieldPolynomial result = a;
      result *= b;
      return result;
   }


   GaloisFieldPolynomial operator * (const GaloisFieldElement& a, const GaloisFieldPolynomial& b)
   {
      GaloisFieldPolynomial result = b;
      result *= a;
      return result;
   }


   GaloisFieldPolynomial operator * (const GaloisFieldPolynomial& a, const GaloisFieldElement& b)
   {
      GaloisFieldPolynomial result = a;
      result *= b;
      return result;
   }

   GaloisFieldPolynomial operator / (const GaloisFieldPolynomial& a, const GaloisFieldPolynomial& b)
   {
      GaloisFieldPolynomial result = a;
      result /= b;
      return result;
   }


   GaloisFieldPolynomial operator / (const GaloisFieldPolynomial& a, const GaloisFieldElement& b)
   {
      GaloisFieldPolynomial result = a;
      result /= b;
      return result;
   }


   GaloisFieldPolynomial operator % (const GaloisFieldPolynomial& a, const GaloisFieldPolynomial& b)
   {
      GaloisFieldPolynomial result = a;
      result %= b;
      return result;
   }

   GaloisFieldPolynomial operator % (const GaloisFieldPolynomial& a, const unsigned int& power)
   {
      GaloisFieldPolynomial result = a;
      result %= power;
      return result;
   }

   GaloisFieldPolynomial operator ^ (const GaloisFieldPolynomial& a, const int& n)
   {
      GaloisFieldPolynomial result = a;
      result ^= n;
      return result;
   }


   GaloisFieldPolynomial operator<<(const GaloisFieldPolynomial& a, const unsigned int& n)
   {
      GaloisFieldPolynomial result = a;
      result <<= n;
      return result;
   }


   GaloisFieldPolynomial operator>>(const GaloisFieldPolynomial& a, const unsigned int& n)
   {
      GaloisFieldPolynomial result = a;
      result >>= n;
      return result;
   }


   GaloisFieldPolynomial gcd(const GaloisFieldPolynomial& a, const GaloisFieldPolynomial& b)
   {
      if ((*a.field()) == (*b.field()))
      {
         if ((!a.valid()) && (!b.valid())) return GaloisFieldPolynomial();
         if (!a.valid()) return b;
         if (!b.valid()) return a;

         GaloisFieldPolynomial x = a % b;
         GaloisFieldPolynomial y = b;
         GaloisFieldPolynomial z = x;

         while ((z = y % x).valid())
         {
            y = x;
            x = z;
         }
         return x;
      }
      else
        return GaloisFieldPolynomial();
   }


   std::ostream& operator << (std::ostream& os, const GaloisFieldPolynomial& polynomial)
   {

      if (polynomial.deg() >= 0)
      {
/*
         for (unsigned int i = 0; i < polynomial.poly.size(); i++)
         {
            os << polynomial.poly[i].index()
               << ((i != (polynomial.deg())) ? " " : "");
         }

         std::cout << " poly form: ";
*/
         for (unsigned int i = 0; i < polynomial.poly.size(); i++)
         {
            os << polynomial.poly[i].poly()
               << " "
               << "x^"
               << i
               << ((i != (polynomial.deg())) ? " + " : "");
         }
      }
      return os;
   }

}

void printTable(galois::GaloisField& gf)
{
	int n=gf.size()+1;
	std::cout<<"[R"<<n<<"Add]"<<std::endl;
	for (unsigned int i = 0; i < n; i++)
	{
		for (unsigned int j = 0; j < n; j++)
		{
			galois::GFSymbol symb=gf.add(i,j);
			std::cout<<symb+1<<" ";
		}
		std::cout<<std::endl;
	}
	std::cout<<"[R"<<n<<"Mul]"<<std::endl;
	for (unsigned int i = 0; i < n; i++)
	{
		for (unsigned int j = 0; j < n; j++)
		{
			galois::GFSymbol symb=gf.mul(i,j);
			std::cout<<symb+1<<" ";
		}
		std::cout<<std::endl;
	}
}

//GF(2)上的多项式f(x)=x^2+1的剩余类全体为：~0，~1，~x，~(x+1)
//程序崩溃
void R4()
{
	unsigned int poly[] = {1,0,1};
	galois::GaloisField gf(2,poly);
	std::cout << gf;
}

void F8()
{
	unsigned int poly[] = {1,1,0,1};
	galois::GaloisField gf(3,poly);
	std::cout << gf;
	printTable(gf);
}

//GF(2)上的多项式f(x)=x^2+x+1的剩余类全体为：~0，~1，~x，~(x+1)
void F4()
{
	unsigned int poly[] = {1,1,1};
	galois::GaloisField gf(2,poly);
	std::cout << gf;
	printTable(gf);
}

void F16()
{
/*
   p(x) = 1x^4+1x^3+0x^2+0x^1+1x^0
          1    1    0    0    1
*/
	unsigned int poly[5] = {1,0,0,1,1};

/*
  A Galois Field of type GF(2^4)
*/
	galois::GaloisField gf(4,poly);
	std::cout << gf;
	printTable(gf);
}

void F256()
{
	/*
	   p(x) = 1x^8+1x^7+0x^6+0x^5+0x^4+0x^3+1x^2+1x^1+1x^0
			  1    1    0    0    0    0    1    1    1
	*/
	unsigned int poly[9] = {1,1,1,0,0,0,0,1,1};

	/*
	  A Galois Field of type GF(2^8)
	*/

	galois::GaloisField gf(8,poly);
	std::cout << gf;
}

int main()
{
	F8();
	F4();
	F16();

	system("pause");
	return 0;
}