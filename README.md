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
./bin/stockscraper-cli quote us2473617023
```

## Fundamentals (debt ratio, P/E-ratio, etc...)

```
./bin/stockscraper-cli fundamentals us2473617023
```

## (Not Implemented) Historic Price Information

```
./bin/stockscraper-cli historic us2473617023
```

## (Not Implemented) List of ISINs in a Stock Market Index

```
./bin/stockscraper-cli index sdax
```

## Import to Spreadsheet

```
./bin/stockscraper-cli fundamentals us2473617023 > out.csv
libreoffice --calc out.csv
```

## Output only one aspect from the Fundamentals

```
./bin/stockscraper-cli fundamentals us2473617023 | grep "Eigenkapitalquote"

2016;Eigenkapitalquote in %;20,86
2017;Eigenkapitalquote in %;23,79
(..)
```

## How to Build

### Set up Conan

```
apt-get -y install python3 python3-pip
pip3 install conan
```

### Build the Project

```
cd src-cli/
conan create ../src
conan install .
conan build .
./bin/stockscraper-cli fundamentals us2473617023
```

### or run only the tests

```
python3 pipeline.py --gtest
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

### v0.0.4

* gtest tests
* enhancements of conan recipes, gtest integration and pipeline
* fundamentals parser adjustment

### v0.0.3

* log message exception handling
* lib and cli separation
* key value pairs for data internally
* clang-tidy

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

