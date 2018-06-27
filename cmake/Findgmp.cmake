find_path(LIBGMP_INCLUDE_DIR NAMES gmp.h)
find_library(LIBGMP_LIBRARY NAMES gmp)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIBGMP LIBGMP_INCLUDE_DIR LIBGMP_LIBRARY)

if(LIBGMP_FOUND AND NOT TARGET gmp::gmp)
    add_library(gmp::gmp SHARED IMPORTED)
    set_target_properties(gmp::gmp PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
        IMPORTED_LOCATION "${LIBGMP_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${LIBGMP_INCLUDE_DIR}")
endif()

mark_as_advanced(LIBGMP_INCLUDE_DIR LIBGMP_LIBRARY)
