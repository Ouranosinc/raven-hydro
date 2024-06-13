# Changes

## 0.3.1 (2024-06-13)

* Updated the `RavenHydroFramework` to v3.8.1.
* `RavenHydroFramework` now compiles with the ``Release`` build type by default.
* `raven-hydro` now requires `scikit-build-core` v0.9 and `cmake` v3.26.1.

## 0.3.0 (2024-03-07)

* Updated the `RavenHydroFramework` to v3.8.
* `raven-hydro` now builds `RavenHydroFramework` directly from the GitHub official source code base.
    * Additionally, the `raven-hydro` package now includes the `RavenHydroFramework` source code in the source distribution.
* `raven-hydro` now supports Python 3.12.
* `raven-hydro` now requires:
    * `cmake` >=3.26.0
    * `hatch-fancy-pypi-readme`
    * `hatchling`
    * `scikit-build-core` >=v0.8.0
    * `setuptools_scm` >=8.0.0
* Now using `dependabot` for dependency and workflow management.
* Now using TestPyPI and PyPI Trusted Publishers infrastructure for signing and verification of packages.
* Numerous packaging fixes to ensure conformant and license-compliant packaging.

## 0.2.4 (2023-08-25)

* Raven-hydro now supports `cmake` v2.37 and follows better Python discovery conventions.

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
