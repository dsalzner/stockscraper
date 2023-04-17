/*
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
*/

#include "public/stockscraper.h"
#include <iomanip> // setw
#include <iostream>
#include <string>
#define VERSION "0.0.1"
#define STR_ALIGN std::left << std::setw(40)

int main(int argc, char *argv[]) {
  std::string mode = "";
  std::string param = "";
  if (argc == 3) {
    mode = argv[1];
    param = argv[2];
  }

  CStockScraper scraper;
  if (mode == "quote" && param.length() == 12) {
    CStockScraper::SStockResult res = scraper.quoteForIsin(param);
    for (const auto &entry : res.dataSets) {
      std::cout << entry.date << ";" << entry.key << ";" << entry.value << std::endl;
    }
    return 0;
  }
  if (mode == "fundamentals" && param.length() == 12) {
    CStockScraper::SStockResult res = scraper.fundamentalsForIsin(param);
    for (const auto &entry : res.dataSets) {
      std::cout << entry.date << ";" << entry.key << ";" << entry.value << std::endl;
    }
    return 0;
  }

  for (uint8_t i = 0; i < 80; i++)
    std::cout << "-";
  std::cout << std::endl << "StockScraper " << VERSION << std::endl;
  std::cout << "Copyright (C) 2023 D.Salzner <mail@dennissalzner.de>" << std::endl;
  for (uint8_t i = 0; i < 80; i++)
    std::cout << "-";
  std::cout << std::endl << std::endl;

  std::cout << STR_ALIGN << "quote <ISIN>";
  std::cout << "stock price" << std::endl;

  std::cout << STR_ALIGN << "fundamentals <ISIN>";
  std::cout << "stock fundamentals" << std::endl;

  std::cout << std::endl;
  std::cout << "-- not implemented features --" << std::endl;

  std::cout << STR_ALIGN << "historic <ISIN>";
  std::cout << "historic price information" << std::endl;

  std::cout << STR_ALIGN << "index [dax, mdax, s&p_500, cac_40, ...]" << std::right << std::setw(12);
  std::cout << "list of ISINs in a stock market index" << std::endl;

  return 0;
}
