find_path(LIBFLINT_INCLUDE_DIR NAMES flint/mpz_mod_poly.h flint/fmpz_mod_poly_factor.h)
find_library(LIBFLINT_LIBRARY NAMES flint)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIBFLINT LIBFLINT_INCLUDE_DIR LIBFLINT_LIBRARY)

if(LIBFLINT_FOUND AND NOT TARGET flint::flint)
    add_library(flint::flint SHARED IMPORTED)
    set_target_properties(flint::flint PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
        IMPORTED_LOCATION "${LIBFLINT_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${LIBFLINT_INCLUDE_DIR}")
endif()

mark_as_advanced(LIBFLINT_INCLUDE_DIR LIBFLINT_LIBRARY)

