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
#include <cassert>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset
#include <string>

#include "mbedtls/certs.h"
#include "mbedtls/config.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/debug.h"
#include "mbedtls/entropy.h"
#include "mbedtls/error.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/ssl.h"

#define VERBOSE false

const char *test_root_ca = "-----BEGIN CERTIFICATE-----\n"
                           "MIIFQDCCAyigAwIBAgIBATANBgkqhkiG9w0BAQsFADA5MREwDwYDVQQDDAhteXNl\n"
                           "cnZlcjEXMBUGA1UECgwObXlvcmdhbml6YXRpb24xCzAJBgNVBAYTAk5MMB4XDTEz\n"
                           "MDEwMTAwMDAwMFoXDTE1MTIzMTIzNTk1OVowOTERMA8GA1UEAwwIbXlzZXJ2ZXIx\n"
                           "FzAVBgNVBAoMDm15b3JnYW5pemF0aW9uMQswCQYDVQQGEwJOTDCCAiIwDQYJKoZI\n"
                           "hvcNAQEBBQADggIPADCCAgoCggIBAM+kt+NkVYT0UXw+a6GOU9V3OLgckEdnIb+Q\n"
                           "yRGWt2LwT0TBiKst7b3/4MhdAYz5It4buR3qMOxgzhB310AEnRpu079DB0+UbI73\n"
                           "oTqHEAzQBaEm6cEcV0p1ShY7pSZAeL+aOgW7hspQ/To7nZY5V886gCUc77OCxsR7\n"
                           "zKIEEm1YUxRnPWSvPiENyvKbjmUVyMftuof1pc9oDkCHWJLCTtyegJKyx8Yw0dMp\n"
                           "6uVlXJiRkCW8J7tconEZTvOEgp6H74crEsZmanw3K4JpH5U7vQh3cFle6nIDX7u4\n"
                           "/KecDshrhfP6BKGuu/A4YFVHeSckvKi13FjDUH3jSj1CXjIqqQvrxHNprS04fudw\n"
                           "gTy0Bhw6SgjETCTx/qaDxgv+vVmF8jW4ApynG/0mTSCefdybwC5l7z4GN9l7sdc/\n"
                           "bywwOwyoqTzIFChG5cc6NDUlTJH9SOFpjYXNwaQabQPUoQuXmjDYg6E7MSEpBZWG\n"
                           "GBFKrGnHKL1nvhNyVjtUdKu4oVxHN8p1xYrq/KQRKF2yAf7ougDpwS/aLSCJrf9F\n"
                           "Id+aYFJO382dgBvWpZNWRnN4PLu8ag4Uwth49KoQciDcIowY+JM6BvGnDSRuKZZp\n"
                           "ljZfznEbkmM3n4aqQrORUEP+QiQ7Lv0MxAEElwp8Up0i7V1B7E3LY4Ez24ftF6GV\n"
                           "Kms89/8TAgMBAAGjUzBRMA8GA1UdEwQIMAYBAf8CAQAwHQYDVR0OBBYEFHXecjeJ\n"
                           "DSR1vz+n8sq3sgIzgYqdMB8GA1UdIwQYMBaAFHXecjeJDSR1vz+n8sq3sgIzgYqd\n"
                           "MA0GCSqGSIb3DQEBCwUAA4ICAQCm49yK00taB1P6BHuCyuCz+iTw3nZaIx6r0Er9\n"
                           "vywMorN4JN5G9LM7fiPDkvGxcKb0clyAD8QyVsNr/U3kQt+MoBdDTwIdW6K+D5mh\n"
                           "VZ/bUvKMHOIY3tUiWh0owGG+P7KhRPpVRTTUzfqsLbYiaD0viZvqtVKp82UvlGI2\n"
                           "VIqSIkzdGfNP2WAcm+0vMC0fo4IVovuJIwdokLHQjweaxMrpgowWL7aw5Vv9nxMq\n"
                           "62RrwmmTUKvn2OWt+BJiQWFrRl90p4b9/CzMAg56Q0rHfxopgDoqXYVl5hkLpZz1\n"
                           "1BerfxBS5RSqq6iinmEGPwRFMre5Zsw3pteSRucZC+mhmrUptpiXFdrPhgn3vRwa\n"
                           "n1hJn4xxp8riE9xTFxutiYKhGAvH0Ad+dpIl+EsZ+3cTurGVfvRujkioxfPlcpGz\n"
                           "dazeBB7mliDhq8RuNegCv1xkM9P4ZXRGjTu0poTzSV1erim8rrOQHjlgQB+xSltl\n"
                           "fe+NVJuYADuQHsatQgE659YkGXeR7xE9LsXRuWjt/x8rlnjpZgVmyuH6A9dMVgI8\n"
                           "ARcG9FlXLcgUW7Q7ySqYdiTRjUin2ZS5j+5AzLazGYsE8orrwQgoi9jXk4tFS3V9\n"
                           "IDsERTrDcqZDhoiPbBQMCAEKQeDj1XgUBlkY36YcKYACcn2FxM8UmWrONzGs0wX0\n"
                           "shTAzw==\n"
                           "-----END CERTIFICATE-----\n";

static void cbMbedtlsLogMessage(void *ctx, int level, const char *file, int line, const char *str) {
  logMessage("mbedtls %s:%04d: %s", file, line, str);
}

std::string prepareHeader(std::string host, std::string path) {
  std::string userAgent = "curl/7.81.0";
  std::string request = "GET " + path + " HTTP/2\r\nHost: " + host + "\r\nuser-agent: " + userAgent +
                        "\r\naccept: */*\r\nConnection: close\r\n\r\n";
  return request;
}

std::string httpsGetRequest(std::string host, int port, std::string requestHeader) {
  mbedtls_net_context server_fd;
  mbedtls_entropy_context entropy;
  mbedtls_ctr_drbg_context ctr_drbg;
  mbedtls_ssl_context ssl;
  mbedtls_ssl_config conf;
  mbedtls_x509_crt cacert;

  // init
  mbedtls_net_init(&server_fd);
  mbedtls_ssl_init(&ssl);
  mbedtls_ssl_config_init(&conf);
  mbedtls_x509_crt_init(&cacert);
  mbedtls_ctr_drbg_init(&ctr_drbg);

  int ret = 1;

  // init entropy
  mbedtls_entropy_init(&entropy);
  const char *pers = "ssl_client1";
  ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers));
  if (ret != 0) {
    logMessage("failed - mbedtls_ctr_drbg_seed returned %d\n", ret);
    return "";
  }

  // init certificates
  ret = mbedtls_x509_crt_parse_file(&cacert, "/etc/ssl/certs/ca-certificates.crt");
  if (ret < 0) {
    logMessage("failed - mbedtls_x509_crt_parse returned -0x%x\n", (unsigned int)-ret);
    return "";
  }

  // connect to server
  ret = mbedtls_net_connect(&server_fd, host.c_str(), std::to_string(port).c_str(), MBEDTLS_NET_PROTO_TCP);
  if (ret != 0) {
    logMessage("failed - mbedtls_net_connect returned %d\n", ret);
    return "";
  }

  // setup ssl/tls
  ret = mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM,
                                    MBEDTLS_SSL_PRESET_DEFAULT);
  if (ret != 0) {
    logMessage("failed - mbedtls_ssl_config_defaults returned %d\n", ret);
    return "";
  }

  mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
  mbedtls_ssl_conf_ca_chain(&conf, &cacert, NULL);
  mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
  mbedtls_ssl_conf_dbg(&conf, cbMbedtlsLogMessage, stdout);

  ret = mbedtls_ssl_setup(&ssl, &conf);
  if (ret != 0) {
    logMessage("failed - mbedtls_ssl_setup returned %d\n", ret);
    return "";
  }

  ret = mbedtls_ssl_set_hostname(&ssl, host.c_str());
  if (ret != 0) {
    logMessage("failed - mbedtls_ssl_set_hostname returned %d\n", ret);
    return "";
  }

  mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, NULL);

  // handshake
  while ((ret = mbedtls_ssl_handshake(&ssl)) != 0) {
    if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
      logMessage("failed - mbedtls_ssl_handshake returned -0x%x\n", (unsigned int)-ret);
      return "";
    }
  }

  // verify server certificate
  uint32_t flags;
  if ((flags = mbedtls_ssl_get_verify_result(&ssl)) != 0) {
    char vrfy_buf[512];
    logMessage("warning server certificate verification failed - mbedtls_ssl_get_verify_result\n");
    mbedtls_x509_crt_verify_info(vrfy_buf, sizeof(vrfy_buf), "  ! ", flags);
    logMessage("%s\n", vrfy_buf);
  }

  // write get request
  int len;
  assert(requestHeader.size() < 1024);
  unsigned char buf[1024];
  len = sprintf((char *)buf, "%s", requestHeader.c_str()); // TODO: use directly
  while ((ret = mbedtls_ssl_write(&ssl, buf, len)) <= 0) {
    if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
      logMessage("failed - mbedtls_ssl_write returned %d\n", ret);
      return "";
    }
  }
  len = ret;

  // read response
  std::string response = "";
  while (true) {
    if (VERBOSE)
      logMessage(".\n");
    len = sizeof(buf) - 1;
    memset(buf, 0, sizeof(buf));
    ret = mbedtls_ssl_read(&ssl, buf, len);
    if (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE)
      continue;
    if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY)
      break;
    if (ret < 0) {
      logMessage("failed - mbedtls_ssl_read returned %d\n", ret);
      break;
    }
    if (ret == 0) {
      if (VERBOSE)
        logMessage("\nFinished\n");
      break;
    }

    len = ret;

    if (VERBOSE)
      logMessage("Received %d bytes\n", len);

    if (response.length() == 0) {
      if (VERBOSE)
        logMessage("MSG %s\n", (const char *)buf);
    }

    response += (const char *)buf;
  }

  mbedtls_ssl_close_notify(&ssl);

  // clean-up
  mbedtls_net_free(&server_fd);
  mbedtls_x509_crt_free(&cacert);
  mbedtls_ssl_free(&ssl);
  mbedtls_ssl_config_free(&conf);
  mbedtls_ctr_drbg_free(&ctr_drbg);
  mbedtls_entropy_free(&entropy);

  return response;
}

std::string getHttpsRequest(std::string host, int port, std::string path, std::string tag) {
  if (!checkFileExists(tag)) {
    std::ofstream out(tag);

    std::string requestHeader = prepareHeader(host, path);
    out << requestHeader;
    out << "---------" << std::endl;
    out.flush();

    std::string res = httpsGetRequest(host, port, requestHeader);
    out << res;
    out.close();
  }

  std::ifstream ifs(tag);
  std::string tmp((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
  return tmp;
}
