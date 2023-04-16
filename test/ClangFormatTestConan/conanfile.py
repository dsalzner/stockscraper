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
import fnmatch
import subprocess
from conans import ConanFile
from conans.errors import ConanInvalidConfiguration

class ClangFormatTestConan(ConanFile):
    settings = "os", "arch", "build_type"
    generators = "cmake"

    def build_requirements(self):
        self.test_requires(self.tested_reference_str)
        self.tool_requires("clang-format/16.0.0@main/stable")
        
    def configure(self):
        if self.settings.build_type != "Debug":
            raise ConanInvalidConfiguration("ClangFormat test recipe can only be run on Debug")
     
    def test(self):
        print(f'Package to test {self.tested_reference_str}')
        packagePath = self.deps_cpp_info[self.tested_reference_str.split("/")[0]].rootpath
        print(f'[ ] packagePath {packagePath}')
        fileMatches = []
        ignoreFiles = ["CMakeCXXCompilerId.cpp"]
        for root, dirs, files in os.walk(packagePath):
            for filename in files:
                if not filename in ignoreFiles:
                    if filename.endswith(('.cpp', '.h', '.hpp', '.c')):
                        fullName = os.path.join(root, filename)
                        fileMatches.append(fullName)
        print(f'[ ] fileMatches {fileMatches}')

        clangFormatBinary = os.path.join(self.deps_cpp_info["clang-format"].rootpath, "bin", "clang-format")
        
        # -- write config
        commandLine = f'{clangFormatBinary} -style=llvm -dump-config'
        clangFormatConfig = subprocess.check_output(commandLine, shell=True).decode('utf8').strip()
        clangFormatConfig = clangFormatConfig.replace("ColumnLimit:     80", "ColumnLimit:    120")
        clangFormatConfig = clangFormatConfig.replace("  AfterControlStatements: true", "  AfterControlStatements: false")
        clangFormatConfig = clangFormatConfig.replace("...", "")
        with open(".clang-format", "w") as tf:
            tf.write(clangFormatConfig)

        issuesFound = False
        for fileName in fileMatches:
            style = "LLVM"
            
            # -- dry run to display errors
            commandLine = f'{clangFormatBinary} -style=file:.clang-format --dry-run {fileName}'
            clangFormatResult = subprocess.check_output(commandLine, shell=True).decode('utf8').strip()
            for (lineNo, line) in enumerate(clangFormatResult.split("\n")):
                if lineNo > 1:
                    print(str(lineNo) + " - " + line)
                    issuesFound = True

            # -- auto-patch
            commandLine = f'{clangFormatBinary} -style=file:.clang-format --i {fileName}'
            subprocess.check_output(commandLine, shell=True).decode('utf8').strip()

        if issuesFound:
            raise Exception(f'clang-format is reporting code formatting issues. Files in {packagePath} have been auto-formatted, you may overwrite your source code with them')

