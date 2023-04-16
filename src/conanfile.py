import os
from conans import ConanFile, CMake, tools

class StockScraper(ConanFile):
    name = "StockScraper"
    description = "Stock information scraping tool"
    license = "GPL-3.0"
    url = "https://github.com/dsalzner/stockscraper.git"
    homepage = "https://www.dennissalzner.de/"
    settings = "os", "arch", "build_type"
    generators = "cmake"

    def set_version(self):
      git = tools.Git(folder=self.recipe_folder)
      version = tools.load(os.path.join(self.recipe_folder, "VERSION")).strip()
      githash = git.get_revision()[:7]
      self.version = f'{version}_{githash}'

    def requirements(self):
      self.requires("mbedtls/2.24.0")

    def build(self):
      cmake = CMake(self)
      cmake.configure()
      cmake.build()
