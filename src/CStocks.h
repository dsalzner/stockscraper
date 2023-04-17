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
#include "public/stockscraper.h"
#include <map>
#include <string>
#include <vector>

class CStocks {
public:
  CStocks();
  ~CStocks();
  CStockScraper::SStockResult quoteForIsin(std::string isin, std::string dataProvider = "finanzen-net");
  CStockScraper::SStockResult fundamentalsForIsin(std::string isin, std::string dataProvider = "finanzen-net");

private:
  std::map<std::string, CStockScraper::SStockResult (*)(std::string)> mQuoteForISIN;
  std::map<std::string, CStockScraper::SStockResult (*)(std::string)> mFundamentalsForISIN;

  static CStockScraper::SStockResult urlForFinanzenNet(std::string isin);
  static CStockScraper::SStockResult quoteForIsinFinanzenNet(std::string isin);
  static CStockScraper::SStockResult fundamentalsForIsinFinanzenNet(std::string isin);
};
