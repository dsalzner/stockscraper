# StockScraper

A minimal stock scraping utility that pulls stock information from public web sites.

## Disclaimer

This tool is purely for educational purposes.

* Web scraping can be against the Terms of Service of the web sites you are scraping information from.
* We keep traffic on the web sites as low as possible by caching
* Keep in mind that you may be violating copyright, if you publish information gathered by this tool

The HTML code of the pages that are being scraped from may change over time. This may make adjustments in the code required.

## Design Goals

The tool is indended to be

* *single-purpose*
  * the tools does one thing and one thing only: it pulls data from the internet
* *simple*
  * it provides information in the simplest form possible: a comma-separated file
* *minimal bloat*
  * only one dependency, mbedtls-2.24.0, for HTTPS communication
  * instead of elaborate DOM-Parsers it uses simple string tokenization
* *light-weight*
  * the compiled binary for Linux is <1 MB
  * the code size is <700 LOC
* *compatible*
  * the code can easily be moved to any platform that C++ code can be compiled for
  * with minimal adaptations it can run on Linux, Windows, Android, Arduino or esp8266
* *cached*
  * the result of every request is stored in a timestamped file
  * this makes debugging easier
  * and requests are only made if the last request is from the previous day
* *fast*
  * since the underlying mbedtls can handle HTTPS communcations on very low-power micro-controllers
  * `time ./bin/stockscraper quote US7427181091` 0m0,731s (uncached)
  * `time ./bin/stockscraper quote US7427181091` 0m0,005s (cached)
* *tested by use*
  * or "eat your own dog food" principle

## Usage

The StockScraper tool uses the "International Securities Identification Number" (ISIN) as defined in ISO 6166 in the parameters.

### Current Stock Price

```
./bin/stockscraper quote US2473617023
```

## Fundamentals (debt ratio, P/E-ratio, etc...)

```
./bin/stockscraper fundamentals US2473617023
```

## (Not Implemented) Historic Price Information

```
./bin/stockscraper historic US2473617023
```

## (Not Implemented) List of ISINs in a Stock Market Index

```
./bin/stockscraper historic US2473617023
```

## Import to Spreadsheet

```
./bin/stockscraper fundamentals US2473617023 > out.csv
libreoffice --calc out.csv
```

## Output only one aspect from the Fundamentals

```
./build/main.bin us2473617023 | grep -E "^; |Eigenkapitalquote" | grep -B1 "Eigenkapitalquote"

; 2016; 2017; 2018; 2019; 2020; 2021; 2022; 
Eigenkapitalquote in %; 20,86; 23,79; 22,71; 23,80; 2,13; 4,87; 8,21;
```

## How to Build

### Set up Conan

```
apt-get -y install python3 python3-pip
pip3 install conan
conan download -r conancenter "mbedtls/2.24.0@" -p f34868f16603ab1572661da51e4f3294771a98bf
```

### Build the Project

```
cd src/
conan install .
conan build .
./bin/stockscraper
```

## How to generate your own HTTP certificate

Using the tools provided with mbedtls:

```
programs/pkey/gen_key type=rsa rsa_keysize=4096 filename=our_key.key

programs/x509/cert_write selfsign=1 issuer_key=our_key.key                    \
                         issuer_name=CN=myserver,O=myorganization,C=NL        \
                         not_before=20130101000000 not_after=20151231235959   \
                         is_ca=1 max_pathlen=0 output_file=my_crt.crt
```

and copy the contents of "my_crt.crt" into the variable "const char* test_root_ca" in CHttpsGet.cpp

## Release History

### v0.0.2

* clang-format
* package_test
* gtest
* wall werror
* package promotion pipeline

### v0.0.1

* stock quote and fundamentals from one data provider
* mbedtls-library for HTTPS-Get-Requests
* command-line interface
* Conan/CMake Build
* readme and license

##

