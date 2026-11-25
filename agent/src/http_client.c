#include <stdio.h>
#include <curl/curl.h>

#include "http_client.h"

#define API_URL "https://192.168.56.102:7069/api/metrics"

int send_metrics_json(const char *json_payload)
{
    if (json_payload == NULL)
    {
        return -1;
    }

    CURL *curl = curl_easy_init();

    if (curl == NULL)
    {
        fprintf(stderr, "Error: Could not initialize libcurl.\n");
        return -1;
    }

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(
        headers,
        "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, API_URL);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    /* Keep HTTPS certificate verification enabled */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

    CURLcode result = curl_easy_perform(curl);

    long response_code = 0;

    if (result == CURLE_OK)
    {
        curl_easy_getinfo(
            curl,
            CURLINFO_RESPONSE_CODE,
            &response_code);
    }
    else
    {
        fprintf(
            stderr,
            "HTTPS request failed: %s\n",
            curl_easy_strerror(result));
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (result == CURLE_OK &&
        response_code >= 200 &&
        response_code < 300)
    {
        fprintf(
            stderr,
            "Metrics accepted by API (HTTP %ld).\n",
            response_code);
        return 0;
    }

    fprintf(
        stderr,
        "API rejected metrics or returned HTTP %ld.\n",
        response_code);

    return -1;
}
