## Raven-hydro

[![License](https://img.shields.io/github/license/Ouranosinc/raven-hydro.svg)](https://github.com/Ouranosinc/raven-hydro/blob/main/LICENSE)

<!-- cut after this -->

Raven-hydro is a Python-based installer for the hydrologic modelling framework [Raven](http://raven.uwaterloo.ca/).

- Free software: Apache Software License v2.0

### About

[Raven](http://raven.uwaterloo.ca) is a hydrological modeling framework that lets hydrologists build hydrological models by combining different hydrological processes together. It can also be used to emulate a variety of existing lumped and distributed models. Model structure, parameters, initial conditions and forcing files are configured in text files, which Raven parses to build and run hydrological simulations. A detailed description about modeling capability of Raven can be found in the [docs](https://www.civil.uwaterloo.ca/raven/files/v3.6/RavenManual_v3.6.pdf).

### Purpose

The goal of this project is to provide a multiplatform installer for the Raven hydrological model using [scikit-build-core](https://github.com/scikit-build/scikit-build-core) and [pybind11](https://github.com/pybind/pybind11). The compiled binary is built with support for [NetCDF4](https://www.unidata.ucar.edu/software/netcdf/), and uses a custom `CMakeLists.txt`, rather than the one provided with the [Raven source code](http://raven.uwaterloo.ca/Downloads.html), in order to leverage the `pip` to handle the installation of binaries and libraries.

This project differs from [RavenPy](https://github.com/CSHS-CWRA/RavenPy) by solely providing a means for downloading, compiling, and installing the Raven binary on multiple platforms and as well as providing version control and updating via pip/PyPI.

### Features

- Download and compile the Raven hydrological model with pip
- Multiplatform support (macOS, Linux, Windows, BSD, etc.)

### Installation

`raven-hydro` can be installed from GitHub using the following command:

```shell
$ pip install git+https://github.com/Ouranosinc/raven-hydro
```

For development purposes, we recommend cloning the repository and performing an `--editable` installation:

```shell
$ git clone git@github.com:Ouranosinc/raven-hydro
$ cd raven-hydro
$ pip install --editable .
```

<!-- but before this -->
