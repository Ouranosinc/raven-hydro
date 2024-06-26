## Raven-hydro

[![License](https://img.shields.io/github/license/Ouranosinc/raven-hydro.svg)](https://github.com/Ouranosinc/raven-hydro/blob/main/LICENSE)

<!-- cut after this -->

Raven-hydro is a Python-based installer for the hydrologic modelling framework [Raven](https://raven.uwaterloo.ca/).

- Free software: Apache Software License v2.0

### About

[Raven](https://raven.uwaterloo.ca) is a hydrological modeling framework that lets hydrologists build hydrological models by combining different hydrological processes together. It can also be used to emulate a variety of existing lumped and distributed models. Model structure, parameters, initial conditions and forcing files are configured in text files, which Raven parses to build and run hydrological simulations. A detailed description about modeling capability of Raven can be found in the [docs](https://raven.uwaterloo.ca/files/v3.8/RavenManual_v3.8.pdf).

### Purpose

The goal of this project is to provide a Python-based multiplatform installer for the Raven hydrological model using [scikit-build-core](https://github.com/scikit-build/scikit-build-core) and [pybind11](https://github.com/pybind/pybind11). The compiled binary is built with support for [NetCDF4](https://www.unidata.ucar.edu/software/netcdf/), and uses a custom `CMakeLists.txt`, rather than the one provided with the [Raven source code](https://raven.uwaterloo.ca/Downloads.html), in order to leverage *pip* for handling the installation and management of binaries and libraries.

This project supports [RavenPy](https://github.com/CSHS-CWRA/RavenPy) by providing a dedicated library for downloading, compiling, and installing the Raven binary (with NetCDF4 support by default) on multiple platforms, as well as providing version control and updates via *pip*/PyPI.

The source code for Raven is not included in this repository, but is fetched from the releases of [RavenHydroFramework GitHub repository](https://github.com/CSHS-CWRA/RavenHydroFramework) and compiled during the installation process.

### Features

- Download, compile, version control, and package updating of the Raven Hydrologic model via *pip*/PyPI
- Preserve copyright notices and licensing information
- Multiplatform support (macOS, Linux, Windows, BSD, etc.)

### Installation

> **Warning**
> By default, this build of Raven requires that NetCDF4 libraries are installed on the system, exposed on the `$PATH`, and discoverable using the `FindNetCDF.cmake` helper script.
>
> On Linux, this can be provided by the `libnetcdf-dev` system library; On macOS by the `netcdf` homebrew package; And on Windows by using UNIDATA's [pre-built binaries](https://docs.unidata.ucar.edu/netcdf-c/current/winbin.html).
>
> This can also be provided by the `libnetcdf` package available at [conda-forge](https://anaconda.org/conda-forge/libnetcdf), though this is not guaranteed to work on macOS at time of writing (further research required).

`raven-hydro` can be installed from PyPI using the following command:

```shell
$ pip install raven-hydro
```

To install `raven-hydro` from PyPI **without NetCDF support**:

```shell
$ pip install raven-hydro -Ccmake.define.USE_NETCDF=false
```

For development purposes, we recommend cloning the repository and performing an `--editable` installation:

```shell
$ git clone git@github.com:Ouranosinc/raven-hydro
$ cd raven-hydro
$ pip install --editable .
```

<!-- but before this -->
