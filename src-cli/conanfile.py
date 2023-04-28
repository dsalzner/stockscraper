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

class StockScraperCli(ConanFile):
    name = "StockScraperCli"
    description = "Stock information scraping tool - Command-Line Interface"
    version = "0.0.1"
    author = "dennissalzner.de"
    url = "https://github.com/dsalzner/stockscraper.git"
    license = "GPL-3.0"
    settings = "os", "compiler", "arch", "build_type"
    generators = "cmake"
    cmake = None
    exports = "*"

    def set_version(self):
        try:
            git = tools.Git(folder=self.recipe_folder)
            githash = git.get_revision()[:7]
        except:
            githash = "na"
        version = tools.load(os.path.join(self.recipe_folder, "..", "VERSION")).strip()
        self.version = f'{version}_{githash}'

    def requirements(self):
        self.requires(f'StockScraperLib/{self.version}@main/built')

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        if self.settings.build_type == "Debug":
            self.copy("*.h", keep_path=False)
            self.copy("*.cpp", keep_path=False)
        self.copy("bin/*", dst="bin", keep_path=False)
