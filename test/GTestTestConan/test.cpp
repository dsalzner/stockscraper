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

#include <gtest/gtest.h>
#include "stockscraper.h"
#include <iostream>
#include <sstream>

TEST(quoteForIsin, getQuote) {
  CStockScraper scraper;
  CStockScraper::SStockResult result;
  result = scraper.quoteForIsin("US2473617023");

  std::stringstream ss;
  for (const auto &entry : result.dataSets) {
      ss << entry.date << ";" << entry.key << ";" << entry.value << std::endl;
      EXPECT_EQ("quote", entry.key)  << "quoteForIsin is outputing dataSet with key that is not 'quote'";
      EXPECT_FALSE(entry.value.empty())  << "quoteForIsin is outputing dataSet with empty value";
  }
}

TEST(quoteForIsin, dontPrintToStdOut) {
  ::testing::internal::CaptureStdout();

  CStockScraper scraper;
  CStockScraper::SStockResult result;
  result = scraper.quoteForIsin("US2473617023");

  std::cout.flush();
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_TRUE(output.empty())  << "quoteForIsin is printing stuff to stdout";
}

TEST(quoteForIsin, fundamentalsForIsin) {
  CStockScraper scraper;
  CStockScraper::SStockResult result;
  result = scraper.fundamentalsForIsin("US2473617023");

  std::stringstream ss;
  for (const auto &entry : result.dataSets) {
      ss << entry.date << ";" << entry.key << ";" << entry.value << std::endl;
      EXPECT_FALSE(entry.value.empty())  << "fundamentalsForIsin is outputing dataSet with empty value";
  }

  EXPECT_LE(result.dataSets.size(), 500U) << "fundamentalsForIsin is not outputing <200 entries" << std::endl << "output: " << ss.str();
  EXPECT_GT(result.dataSets.size(), 5U) << "fundamentalsForIsin is not outputing >5 entries" << std::endl << "output: " << ss.str();
}
