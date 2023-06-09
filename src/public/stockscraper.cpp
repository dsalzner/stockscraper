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

#include "stockscraper.h"
#include "../CStocks.h"

CStockScraper::CStockScraper() : m_stocks(std::make_unique<CStocks>()) {}

CStockScraper::~CStockScraper() {}

CStockScraper::SStockResult CStockScraper::quoteForIsin(std::string isin, std::string dataProvider) {
  return m_stocks->quoteForIsin(isin, dataProvider);
}

CStockScraper::SStockResult CStockScraper::fundamentalsForIsin(std::string isin, std::string dataProvider) {
  return m_stocks->fundamentalsForIsin(isin, dataProvider);
}
