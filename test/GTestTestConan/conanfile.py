"""
StockScraper
Copyright (C) 2023 D.Salzner <mail@dennissalzner.de>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

import os
from conans import ConanFile, CMake, tools
from conans.errors import ConanInvalidConfiguration

class GTestTestConan(ConanFile):
    settings = "os", "arch", "build_type"
    generators = "cmake"

    def build_requirements(self):
        self.tool_requires("gtest/1.8.1")
        
    def configure(self):
        if self.settings.build_type != "Debug":
            raise ConanInvalidConfiguration("GTest test recipe can only be run on Debug")

    def imports(self):
        self.copy("*", "bin", "bin")
        self.copy("*.h")
        
    def build(self):
        self.cmake = CMake(self)
        self.cmake.configure()
        self.cmake.build()
        
    def test(self):
        target_test = "RUN_TESTS" if self.settings.os == "Windows" else "test"
        self.cmake.build(target=target_test)
