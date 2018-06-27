#include "flint_wrapp.hpp"

namespace wrp {

std::size_t modulo(int m, int n) {
  return m >= 0 ? m % n : (n - abs(m % n)) % n;
}

Factor::Factor(slong q) {
  fmpz_init_set_ui(n, q);
  fmpz_mod_poly_init(poly, n);
  fmpz_mod_poly_factor_init(fac);
}
Factor::Factor(const Factor &res) {
  fmpz_init_set_ui(n, res.poly->p);
  fmpz_mod_poly_init(poly, n);
  fmpz_mod_poly_factor_init(fac);
}

Factor::Factor(Factor &&res) {
  fmpz_init_set_ui(n, res.poly->p);
  fmpz_mod_poly_init(poly, n);
  fmpz_mod_poly_factor_init(fac);
  fmpz_mod_poly_clear(res.poly);
  fmpz_mod_poly_factor_clear(res.fac);
  fmpz_clear(res.n);
}

Factor &Factor::operator=(const Factor &res) {
  if (this != &res) {
    fmpz_mod_poly_clear(poly);
    fmpz_mod_poly_factor_clear(fac);
    fmpz_clear(n);
    fmpz_init_set_ui(n, res.poly->p);
    fmpz_mod_poly_init(poly, n);
    fmpz_mod_poly_factor_init(fac);
  }
  return *this;
}

Factor &Factor::operator=(Factor &&res) {
  if (this != &res) {
    fmpz_mod_poly_clear(poly);
    fmpz_mod_poly_factor_clear(fac);
    fmpz_clear(n);
    fmpz_init_set_ui(n, res.poly->p);
    fmpz_mod_poly_init(poly, n);
    fmpz_mod_poly_factor_init(fac);
    fmpz_mod_poly_clear(res.poly);
    fmpz_mod_poly_factor_clear(res.fac);
    fmpz_clear(res.n);
  }
  return *this;
}

Factor::~Factor() {
  fmpz_mod_poly_clear(poly);
  fmpz_mod_poly_factor_clear(fac);
  fmpz_clear(n);
}

void Factor::set_poly_coeff(std::size_t degr, int coeff) {
  fmpz_mod_poly_set_coeff_ui(poly, degr, coeff);
}

fmpz Factor::operator[](std::size_t degr) const {
  return *(poly->coeffs + degr);
}

fmpz_mod_poly_struct Factor::operator[](slong poly_num) const {
  return *(fac->poly + poly_num);
}

std::ostream &operator<<(std::ostream &os, const Factor &prod) {
  for (slong i = 0; i < prod.fac->num; ++i) {
    if (i != 0)
      os << '*';
    os << '(';
    for (slong j = (prod.fac->poly + i)->length - 1; j >= 0; --j) {
      if (*((prod.fac->poly + i)->coeffs + j) == 0)
        continue;
      if ((*((prod.fac->poly + i)->coeffs + j) == 1) && j != 0) {
        goto s;
      }
      fmpz_print((prod.fac->poly + i)->coeffs + j);
    s:
      if (j != 0) {
        if (j != 1) {
          os << "x^" << j << '+';

        } else
          os << "x+";
      }
    }
    if (*(prod.fac->exp + i) != 1)
      os << ")^" << *(prod.fac->exp + i);
    os << ')';
  }
  os << '\n';
  return os;
}
void Factor::set_factorisation() { fmpz_mod_poly_factor(fac, poly); }

} // namespace wrp
