#include "flint_wrapp.hpp"
#include <boost/bind.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi.hpp>
#include <complex>
#include <iostream>
#include <string>

namespace client {
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

struct term {
  boost::optional<int> coefficient;
  boost::optional<int> x_exponent;
};

std::size_t modulo(slong m, slong n) {
  return m >= 0 ? m % n : (n - abs(m % n)) % n;
}
void set(const term &tt, wrp::Factor &prod, slong q) {
  int coefficient = (tt.coefficient) ? *tt.coefficient : 1;
  int x_exponent = (tt.x_exponent) ? *tt.x_exponent : 0;
  prod.set_poly_coeff(x_exponent, modulo(coefficient, q));
}

} // namespace client

namespace client {
template <typename Iterator>
struct polynomial_parser
    : qi::grammar<Iterator, std::vector<term>(), ascii::space_type> {
  polynomial_parser() : polynomial_parser::base_type(start) {
    namespace phx = boost::phoenix;
    using ascii::char_;
    using qi::_val;
    using qi::eps;
    using qi::int_;
    using qi::lexeme;
    using qi::lit;

    exponent %= '^' >> int_;

    x_term %= 'x' >> (exponent | eps[_val = 1]);

    poly_term = eps[_val = term()] >>
                -int_[phx::bind(&term::coefficient, _val) = qi::_1] >>
                -x_term[phx::bind(&term::x_exponent, _val) = qi::_1];

    negative_poly_term =
        eps[_val = term()] >>
        (int_[phx::bind(&term::coefficient, _val) = -1 * qi::_1] |
         eps[phx::bind(&term::coefficient, _val) = -1]) >>
        -x_term[phx::bind(&term::x_exponent, _val) = qi::_1];

    start = eps[_val = std::vector<term>()] >>
            poly_term[phx::push_back(_val, qi::_1)] >>
            *(('+' >> poly_term[phx::push_back(_val, qi::_1)]) |
              ('-' >> negative_poly_term[phx::push_back(_val, qi::_1)]));
  }

  qi::rule<Iterator, int(), ascii::space_type> exponent;
  qi::rule<Iterator, int(), ascii::space_type> x_term;
  qi::rule<Iterator, term(), ascii::space_type> poly_term;
  qi::rule<Iterator, term(), ascii::space_type> negative_poly_term;
  qi::rule<Iterator, std::vector<term>(), ascii::space_type> start;
};
} // namespace client

int main() {

  std::cout << "Enter Z/nZ[x]\n";
  std::string str0;
  int q;
  std::getline(std::cin, str0);
  q = stoi(str0);
  std::cout << "Enter the polynom, please:\n";
  std::cout << "(for example: x^5+3x^4-x^2+2x+4)\n";
  using boost::spirit::ascii::space;
  typedef std::string::const_iterator iterator_type;
  typedef client::polynomial_parser<iterator_type> polynomial_parser;

  polynomial_parser pparser;
  std::string str;
  wrp::Factor prod(q);

  while (getline(std::cin, str)) {
    if (str.empty() || str[0] == 'q' || str[0] == 'Q')
      break;

    std::vector<client::term> poly;
    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();
    bool r = phrase_parse(iter, end, pparser, space, poly);

    if (r && iter == end) {
      std::cout << boost::fusion::tuple_open('[');
      std::cout << boost::fusion::tuple_close(']');
      std::cout << boost::fusion::tuple_delimiter(", ");
      for (std::vector<client::term>::iterator it = poly.begin();
           it != poly.end(); ++it) {
        client::set(*it, prod, q);
      }
    } else {
      std::cout << "-------------------------\n";
      std::cout << "Parsing failed\n";
      std::cout << "-------------------------\n";
    }
  }

  std::cout << "\n-------------------------\n";
  prod.set_factorisation();
  std::cout << "Result:\n";
  std::cout << prod;
  std::cout << "---------------------------\n";

  return 0;
}
