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

#include "CStocks.h"
#include "common.h"
#include <fstream>
#include <iostream>
#include <string>

#define VERBOSE false

CStocks::CStocks() {
  mQuoteForISIN["finanzen-net"] = CStocks::quoteForIsinFinanzenNet;
  mFundamentalsForISIN["finanzen-net"] = CStocks::fundamentalsForIsinFinanzenNet;
}

CStocks::~CStocks() {}

CStockScraper::SStockResult CStocks::quoteForIsin(std::string isin, std::string dataProvider) {
  return mQuoteForISIN[dataProvider](isin);
}

CStockScraper::SStockResult CStocks::fundamentalsForIsin(std::string isin, std::string dataProvider) {
  return mFundamentalsForISIN[dataProvider](isin);
}

CStockScraper::SStockResult CStocks::urlForFinanzenNet(std::string isin) {
  CStockScraper::SStockResult res;
  std::string d1, d2;
  std::string fileName = "stocks-" + getTimestamp() + "-finanzennet-" + isin;

  std::string url = "/aktien/" + isin + "-aktie";
  std::string page = CHttpsGet::getHttpsRequest("www.finanzen.net", 443, url, fileName + "-1.txt");
  if (page.find("Location") != std::string::npos) {
    d1 = "Location: ";
    d2 = "\r\nServer";
    url = getTextBetweenDelimiters(page, d1, d2);
    page = CHttpsGet::getHttpsRequest("www.finanzen.net", 443, url, fileName + "-2.txt");
  }

  d1 = "<section class=\"page-content__container\" id=\"suggestBESearch\">";
  d2 = "</section>";
  if (page.find(d1) != std::string::npos) {
    std::string suggestionTable = getTextBetweenDelimiters(page, d1, d2);

    d1 = "<a href=\"/aktien/";
    d2 = "\">";
    url = getTextBetweenDelimiters(suggestionTable, d1, d2);
    res.stockName = getTextBetweenDelimiters(url, "", "-aktie");
    res.url = "/aktien/" + url;
  }

  return res;
}

CStockScraper::SStockResult CStocks::quoteForIsinFinanzenNet(std::string isin) {
  CStockScraper::SStockResult res = urlForFinanzenNet(isin);
  std::string d1, d2;
  std::string fileName = "stocks-" + getTimestamp() + "-finanzennet-" + isin;

  std::string page = CHttpsGet::getHttpsRequest("www.finanzen.net", 443, res.url, fileName + "-quote.txt");
  if (page.find(isin) != std::string::npos) {
    d1 = "td class=\"table__td\">Kurs</td><td class=\"table__td padding-horizontal-0.00\">";
    d2 = "</td></tr>";
    res.dataSets.emplace_back(CStockScraper::SDataSet{getTimestamp(), "quote", getTextBetweenDelimiters(page, d1, d2)});
  }

  return res;
}

CStockScraper::SStockResult CStocks::fundamentalsForIsinFinanzenNet(std::string isin) {
  CStockScraper::SStockResult res = urlForFinanzenNet(isin);
  res.dataSets.emplace_back(CStockScraper::SDataSet{"", "stockName", res.stockName});

  std::string d1, d2;
  std::string fileName = "stocks-" + getTimestamp() + "-finanzennet-" + isin + "-fundamentals";

  std::string fundamentalsUrl = stringReplace(res.url, "/aktien/", "/bilanz_guv/");
  fundamentalsUrl = stringReplace(fundamentalsUrl, "-aktie", "");

  std::string page = CHttpsGet::getHttpsRequest("www.finanzen.net", 443, fundamentalsUrl, fileName + ".txt");

  d1 = "<article class=\"page-content__item\">";
  d2 = "</article>";
  while (page.find(d1) != std::string::npos) {
    std::string block = getAndEatTextBetweenDelimiters(page, d1, d2);

    std::string d1b = "<tr class=\"table__tr\">";
    std::string d2b = "</tr>";
    std::vector<std::string> years;
    while (block.find(d1b) != std::string::npos) {
      std::string line = getAndEatTextBetweenDelimiters(block, d1b, d2b);

      // -- header
      std::string d1c = "<th class=\"table__th\">";
      std::string d2c = "</th>";

      size_t entryNo = 0;
      while (line.find(d1c) != std::string::npos) {
        std::string entry = getAndEatTextBetweenDelimiters(line, d1c, d2c);
        if (entryNo > 0) {
          years.emplace_back(entry);
        }
        entryNo++;
      }

      // -- line
      d1c = "<td class=\"table__td\">";
      d2c = "</td>";
      entryNo = 0;
      std::string topic = "";
      while (line.find(d1c) != std::string::npos) {
        std::string entry = getAndEatTextBetweenDelimiters(line, d1c, d2c);

        std::string d1d = "<label class=";
        std::string d2d = "</label>";
        if (entry.find(d1d) != std::string::npos) {
          entry = getAndEatTextBetweenDelimiters(entry, d1d, d2d);
          auto junk = getAndEatTextBetweenDelimiters(entry, "\"bguvchart_", "\">");
        }

        if (entryNo == 0) {
          topic = entry;
          if (VERBOSE)
            logMessage("%s\n", entry.c_str());
        } else {
          std::string year = years[(entryNo - 1) % years.size()];
          if (VERBOSE) {
            logMessage("%s: %s\n", year.c_str(), entry.c_str());
          }
          res.dataSets.emplace_back(CStockScraper::SDataSet{year, topic, entry});
        }

        entryNo++;
      }
    }
  }

  return res;
}
