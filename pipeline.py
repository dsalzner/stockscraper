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
from conans.client.conan_api import Conan
import subprocess, shlex
scriptDir = os.path.dirname(os.path.realpath(__file__))

try:
    conan_api, _, _ = Conan.factory()
except conans.errors.ConanMigrationError:
    sys.exit(conans.cli.exit_codes.ERROR_MIGRATION)
except conans.errors.ConanException as error:
    sys.exit(conans.cli.exit_codes.ERROR_GENERAL)
      
def createPackageByAPI(conanFile):
    results = conan_api.create(conanFile, user="main", channel="built")
    packageId = results["installed"][0]["recipe"]["id"]
    rootPath = results["installed"][0]["packages"][0]["cpp_info"]["rootpath"]
    return (packageId, rootPath)

def runCmd(cmd):
    print(cmd)
    subprocess.run(shlex.split(cmd), check=True)

cmakeCacheFile = os.path.join(scriptDir, "src", "CMakeCache.txt")
if os.path.isfile(cmakeCacheFile):
    os.remove(cmakeCacheFile)

print("[ ] Create StockScraper")
conanFile = os.path.join(scriptDir, "src", "conanfile.py")
(packageId, rootPath) = createPackageByAPI(conanFile)
runCmd(f'conan create {conanFile} main/built -s build_type="Debug"')

print("[ ] Create StockScraperCli")
conanFile = os.path.join(scriptDir, "src-cli", "conanfile.py")
(packageIdCli, rootPathCli) = createPackageByAPI(conanFile)
runCmd(f'conan create {conanFile} main/built -s build_type="Debug"')

print("[ ] Create ClangFormatToolConan")
conanFile = os.path.join(scriptDir, "test", "ClangToolsConan", "conanfile.py")
recipes = conan_api.search_recipes("clang-tools/16.0.0@main/built")
if len(recipes["results"]) == 0:
    runCmd(f'conan create {conanFile} "16.0.0@main/stable"')

print("[ ] Test ClangToolsTestConan")
conanFile = os.path.join(scriptDir, "test", "ClangToolsTestConan")
runCmd(f'conan test {conanFile} "{packageId}" -s build_type="Debug"')
runCmd(f'conan test {conanFile} "{packageIdCli}" -s build_type="Debug"')

print("[ ] Promote StockScraper 'built' -> 'testing'")
runCmd(f'conan copy --force "{packageId}" main/testing')
#packageId = packageId.replace("built", "testing")

print("[ ] Test GTestTestConan")
conanFile = os.path.join(scriptDir, "test", "GTestTestConan")
runCmd(f'conan test {conanFile} "{packageId}" -s build_type="Debug"')

print(f'[ ] Run cli')
os.chdir(rootPathCli)
cliBinary = os.path.join("bin", "stockscraper-cli")
commandLine = f'{cliBinary} fundamentals US2473617023'
print(commandLine)
os.system(commandLine)
