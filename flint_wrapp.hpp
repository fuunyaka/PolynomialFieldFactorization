#ifndef FLINT_WRAPP_HPP
#define FLINT_WRAPP_HPP
#include <cctype>
#include <cstdlib>
#include <flint/fmpz_mod_poly.h>
#include <iomanip>

namespace wrp {

std::size_t modulo(int m, int n);
std::istream &operator>>(std::istream &os, std::string str);

class Factor {
  fmpz_mod_poly_t poly;
  fmpz_mod_poly_factor_t fac;
  fmpz_t n;

public:
  Factor(slong q);

  Factor(const Factor &res);
  Factor(Factor &&res);
  Factor &operator=(const Factor &res);
  Factor &operator=(Factor &&res);

  ~Factor();

  void set_poly_coeff(std::size_t degr, int coeff);

  fmpz operator[](std::size_t degr) const;

  void set_poly_n(slong q);
  fmpz_t &operator[](std::size_t degr);

  fmpz_mod_poly_struct operator[](slong poly_num) const;

  friend std::ostream &operator<<(std::ostream &os, const Factor &prod);

  void set_factorisation();
};

} // namespace wrp

#endif // FLINT_WRAPP_HPP
