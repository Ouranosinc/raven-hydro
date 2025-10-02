# Changes

## 0.4.1 (2025-10-02)

* Fixed a bug in `CMakeLists.txt` that caused issues for environments where HDF5 was not easily discoverable.
* Fixed a bug in the `FindNetCDF.cmake` that negatively impacted discoverability of NetCDF installations in Anaconda environments.
* Updated the list of allowed connections in GitHub workflows for releases.

## 0.4.0 (2025-02-12)

* Updated the `RavenHydroFramework` to v4.0.1.
* ``CMakeLists.txt`` now mutes more deprecation warnings on GCC compilers.
* Support for ARM64 (Apple Silicon) architectures has been added.
* Licensing information has been updated to reflect current year.

## 0.3.2 (2024-11-07)

* `raven-hydro` now officially supports `Python` 3.13 and has dropped support for `Python` 3.8.
* `raven-hydro` now requires `scikit-build-core` v0.10.
* Several development dependencies have been updated to their latest versions.

## 0.3.1 (2024-06-17)

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

* `raven-hydro` now supports `cmake` v2.37 and follows better Python discovery conventions.

## 0.2.3 (2023-07-04)

* `raven-hydro` now supports building raven model without NetCDF4 support (see README.md for details).

## 0.2.2 (2023-06-05)

* Building `raven-hydro` from sources available on pip within an Anaconda environment is now supported.

## 0.2.1 (2023-05-25)

* `raven-hydro` only offers source distributions until issues arising from library linking/bundling have been resolved.

## 0.2.0 (2023-05-16)

* Updated `RavenHydroFramework` version to v3.7
* Added GitHub Workflows for releasing source and binary builds on TestPyPI and PyPI.
* Conditional fetching/verifying of `RavenHydroFramework` source code is now handled by `pyproject.toml` keyword argument (`ALWAYS_DOWNLOAD = false` ).
* Added the `RavenHydroFramework` source code to the source distribution.
* Now using `pre-commit` for linting checks.
* Fixed issues with licensing (now bundles raven_COPYING.txt).

## 0.1.0 (2023-05-16)

* First official release on PyPI
