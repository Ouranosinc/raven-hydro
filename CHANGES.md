# Changes

## 0.2.3 (2023-07-04)

* Raven-hydro now supports building raven model without NetCDF4 support (see README.md for details).

## 0.2.2 (2023-06-05)

* Building Raven-hydro from sources available on pip within an Anaconda environment is now supported.

## 0.2.1 (2023-05-25)

* Raven-hydro only offers source distributions until issues arising from library linking/bundling have been resolved.

## 0.2.0 (2023-05-16)

* Updated Raven version to v3.7
* Added GitHub Workflows for releasing source and binary builds on TestPyPI and PyPI.
* Conditional fetching/verifying of Raven source code is now handled by pyproject.toml keyword argument (`ALWAYS_DOWNLOAD = false` ).
* Added the Raven source code to the source distribution.
* Now using pre-commit for linting checks.
* Fixed issues with licensing (now bundles raven_COPYING.txt).

## 0.1.0 (2023-05-16)

* First official release on PyPI
