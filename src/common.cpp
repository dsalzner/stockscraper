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

#include "common.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdarg.h> // va_list
#include <stdio.h>  // printf
#include <string>
#include <time.h>

void logMessage(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}

const std::string getTimestamp() {
  time_t now = time(0);
  struct tm tstruct;
  char buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
  return buf;
}

bool checkFileExists(std::string filename) {
  std::ifstream file(filename);
  if (!file.is_open())
    return false;
  return true;
}

std::string stringReplace(std::string str, const std::string &from, const std::string &to) {
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
  return str;
}

std::string getTextBetweenDelimiters(const std::string s, const std::string d1, const std::string d2) {
  size_t pos1 = s.find(d1);
  size_t pos2 = s.find(d2, pos1 + d1.size() + 1);
  if (pos1 == std::string::npos || pos2 == std::string::npos)
    return "";
  pos1 += d1.size();

  if (pos1 > pos2)
    return "";
  return s.substr(pos1, pos2 - pos1);
}

std::string getAndEatTextBetweenDelimiters(std::string &s, std::string d1, std::string d2) {
  size_t pos1 = s.find(d1);
  size_t pos2 = s.find(d2, pos1 + 1);
  if (pos1 == std::string::npos || pos2 == std::string::npos)
    return "";
  if (pos1 > pos2)
    return "";
  pos1 += d1.size();
  std::string result = "" + s.substr(pos1, pos2 - pos1);
  s = s.replace(0, pos2 + d2.length(), "");
  return result;
}
