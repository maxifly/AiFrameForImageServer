#pragma once
#include <GyverDBFile.h>
#include <LittleFS.h>
GyverDBFile db(&LittleFS, "settings.db");

enum kk : size_t {
    wifi_ssid,
    wifi_pass,
    imgs_host,
    imgs_port,
    gen_query,
    auto_gen,
    auto_prd,
};

void db_init() {
    LittleFS.begin();
    db.begin();
    db.init(kk::wifi_ssid, "");
    db.init(kk::wifi_pass, "");
    db.init(kk::imgs_host, "0.0.0.0");
    db.init(kk::imgs_port, 80);

    db.init(kk::gen_query, "");

    db.init(kk::auto_gen, 0);
    db.init(kk::auto_prd, 5);
}