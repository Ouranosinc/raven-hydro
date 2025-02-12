###################################################################################
# Apache Software License 2.0
#
# Copyright 2023-2025, Trevor James Smith, David Huard
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
###################################################################################

from .libraven import __doc__, __netcdf__, __version__ as __raven_version__

from ._version import __version__

__all__ = ["__doc__", "__netcdf__", "__raven_version__", "__version__"]
