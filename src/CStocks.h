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

#pragma once

#include "CHttpsGet.h"
#include "common.h"
#include <map>
#include <string>

class CStocks {
public:
  struct SStockResult {
    std::string isin = "";
    std::string stockName = "";
    std::string url = "";
    std::string quote = "";
  };

  CStocks();
  SStockResult quoteForIsin(std::string isin, std::string dataProvider = "finanzen-net");
  SStockResult fundamentalsForIsin(std::string isin, std::string dataProvider = "finanzen-net");

private:
  std::map<std::string, SStockResult (*)(std::string)> mQuoteForISIN;
  std::map<std::string, SStockResult (*)(std::string)> mFundamentalsForISIN;

  static SStockResult urlForFinanzenNet(std::string isin);
  static SStockResult quoteForIsinFinanzenNet(std::string isin);
  static SStockResult fundamentalsForIsinFinanzenNet(std::string isin);
};
