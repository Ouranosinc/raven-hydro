#include "RavenInclude.h"
#include <pybind11/pybind11.h>

#ifdef _RVNETCDF_
    const bool __HAS_NETCDF__ = true;
#else
    const bool __HAS_NETCDF__ = false;
#endif

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

PYBIND11_MODULE(_core, m) {
  m.doc() =
      R"pbdoc(A Python wrapper to setup and build the hydrologic modelling framework Raven.)pbdoc";

  m.attr("__raven_version__") = __RAVEN_VERSION__;
  m.attr("__netcdf__") = __HAS_NETCDF__;

#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "dev";
#endif
}
