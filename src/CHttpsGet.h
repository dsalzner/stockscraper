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

static void cbMbedtlsLogMessage( void *ctx, int level, const char *file, int line, const char *str);
std::string prepareHeader(std::string host, std::string path);
std::string httpsGetRequest(std::string host, int port, std::string requestHeader);
std::string getHttpsRequest(std::string host, int port, std::string path, std::string tag);