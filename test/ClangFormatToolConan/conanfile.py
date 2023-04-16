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
from conan import ConanFile, tools

class ClangFormatToolConan(ConanFile):
    name = "clang-format"
    homepage = "https://clang.llvm.org/docs/ClangFormat.html"
    description = "Clang-Format Tool Recipe"
    no_copy_source = True
    settings = "os"
    url = "https://github.com/llvm/llvm-project/releases"
    license = "Apache 2.0 License with LLVM exceptions"
    build_type="Any"

    def source(self):
        tools.files.get(self, self.conan_data["binaries"][str(self.settings.os)][self.version]["url"], strip_root=True)
                        
    def package(self):
        self.copy("clang-format", src=os.path.join(self.source_folder, "bin"), dst=os.path.join(self.package_folder, "bin"), keep_path=True)

    def package_info(self):
        self.env_info.path.append(os.path.join(self.package_folder, "bin"))
