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

class StockScraperLib(ConanFile):
    name = "StockScraperLib"
    url = "https://github.com/dsalzner/stockscraper.git"
    author = "dennissalzner.de"
    license = "GPL-3.0"
    settings = "os", "arch", "compiler", "build_type"
    generators = "cmake"
    exports = "*"
    description = "Stock information scraping tool - Library"
    options = {"shared": [True, False]}
    default_options = "shared=False"

    def set_version(self):
        try:
            git = tools.Git(folder=self.recipe_folder)
            githash = git.get_revision()[:7]
        except:
            githash = "na"
        version = tools.load(os.path.join(self.recipe_folder, "..", "VERSION")).strip()
        self.version = f'{version}_{githash}'

    def requirements(self):
        self.requires("mbedtls/2.24.0")

    def build(self):
        shared = {"BUILD_SHARED_LIBS": self.options.shared}
        cmake = CMake(self)
        cmake.configure(defs=shared)
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include")
        self.copy("*.lib", dst="lib", src="lib", keep_path=False)
        self.copy("*.dll", dst="bin", src="bin", keep_path=False)
        self.copy("*.dylib", dst="bin", src="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)
        if self.settings.build_type == "Debug":
            self.copy("*.h", dst="src", keep_path=False)
            self.copy("*.cpp", dst="src", keep_path=False)
            self.copy("public/*.cpp", dst="include", keep_path=False)
        self.copy("public/*.h", dst="include", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["stockscraper"]
