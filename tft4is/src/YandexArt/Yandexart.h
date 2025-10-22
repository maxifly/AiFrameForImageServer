#pragma once
#include <Arduino.h>
// config
// #define FUSION_HOST "llm.api.cloud.yandex.net"
// #define FUSION_PORT 443
#define FUSION_PERIOD 30000
#define FUSION_TRIES 1
#define FUS_LOG(x) Serial.println(x)
// #define GHTTP_HEADERS_LOG Serial
#include <GSON.h>
#include <ArduinoJson.h>
#include <GyverHTTP.h>
#include "StreamB64.h"
#include "tjpgd/tjpgd.h"
// #ifdef ESP8266
// #include <ESP8266WiFi.h>
// #include <WiFiClientSecure.h>
// #include <WiFiClientSecureBearSSL.h>

// #define FUSION_CLIENT BearSSL::WiFiClientSecure
// #else
// #include <WiFi.h>
// #include <WiFiClientSecure.h>
// #define FUSION_CLIENT WiFiClientSecure
// #endif

#include <WiFiClient.h>
#define FUSION_CLIENT WiFiClient

#define	FINAL_BUF_SIZE		128
#define	JDOC_START_SIZE		140

const char* GEN_AUTO_BODY = "{\"type\": \"auto\"}";

// Константы, определяющие внутреннюю область
const int INTERNAL_X = 48;
const int INTERNAL_Y = 80;
const int INTERNAL_WIDTH = 320;
const int INTERNAL_HEIGHT = 480;

// Вспомогательные макросы для вычисления минимума/максимума
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Макросы для извлечения и установки компонентов RGB565
#define GET_R(color) (((color) >> 11) & 0x1F)
#define GET_G(color) (((color) >> 5) & 0x3F)
#define GET_B(color) ((color) & 0x1F)

#define SET_RGB(r, g, b) ((((r) & 0x1F) << 11) | (((g) & 0x3F) << 5) | ((b) & 0x1F))


class YandexArt {
    typedef std::function<void(int x, int y, int w, int h, uint8_t* buf)> RenderCallback;
    typedef std::function<void()> RenderEndCallback;
    enum class State : uint8_t {
        GetModels,
        Generate,
        Status,
        GetStyles,
    };
   public:


    YandexArt() {
        // Определение статических фильтров
         successFilter["id"] = true;
         successFilter["status"] = true;
         successFilter["error"] = true;

         errorFilter["error"] = true;
    }

    YandexArt(const String& imgs_host, const int& imgs_port) : YandexArt()  {
        setKey(imgs_host, imgs_port);
    }

    void setKey(const String& imgs_host, const int& imgs_port) {
        if (imgs_host.length() && imgs_port != 0) {
            _imgs_host = imgs_host;
            _imgs_port = imgs_port;
        }
    }
    void onRender(RenderCallback cb) {
        _rnd_cb = cb;
    }
    void onRenderEnd(RenderEndCallback cb) {
        _end_cb = cb;
    }
    // 1, 2, 4, 8
    void setScale(uint8_t scale) {
        switch (scale) {
            case 1: _scale = 0; break;
            case 2: _scale = 1; break;
            case 4: _scale = 2; break;
            case 8: _scale = 3; break;
            default: _scale = 0; break;
        }
    }
    bool begin() {
        if (!_imgs_host.length()) return false;
        // if (!_imgs_port.length()) return false;
        return false;
    }

    bool getStyles() {
        // if (!_api_key.length()) return false;
        return false;
        //return request(State::GetStyles, "cdn.fusionbrain.ai", "/static/styles/web");
    }
    bool next_image() {
        status = "wrong config";
        if (!_imgs_host.length()) return false;
        // if (!_imgs_port.length()) return false;

        DynamicJsonDocument jsonDoc(1024);
        jsonDoc["type"] = "auto";

        String id;
        String taskStatus;
        String errorMsg;

        uint8_t tries = FUSION_TRIES;
        while (tries--) {
            //TODO Сильно поменять обработку ответа
            if (performGenerateHttpRequest(_imgs_host, _imgs_port, "/operation/start", "POST", jsonDoc, id, taskStatus, errorMsg)) {
                FUS_LOG("Gen request sent");
                _tmr = millis();
                _uuid = id;
                if (!_uuid.length()) {
                    status = "operation ID unknown";
                    return false;
                } 
                status = "wait result";
                return true;
            } else {
                FUS_LOG("Gen request error");
                delay(2000);
            }
        }
        status = "gen request error";
        return false;        


    }
    bool generate() {
        status = "unsupported";
        if (!_imgs_host.length()) return false;
        // if (!_imgs_port.length()) return false;
        return false;

    }

    bool generatePrmt(Text query) {
        // _uuid = "fbv2ad972upgdk5qnu88";
        return false;

        // status = "wrong config";
        // if (!_imgs_host.length()) return false;
        // // if (!_imgs_port.length()) return false;
        // if (!query.length()) return false;

        // status = "unsupported";
        // return false;

        // // Создание JSON-документа для тела запроса
        // DynamicJsonDocument jsonDoc(JDOC_START_SIZE + (query.length() * 2));
        // jsonDoc["model_uri"] = "art://" + _imgs_port + "/yandex-art/latest";

        
        // // // Создание массива messages
        // JsonArray messages = jsonDoc.createNestedArray("messages");
        // JsonObject message1 = messages.createNestedObject();
        // message1["text"] = query;
        // message1["weight"] = 1;

        // // if (negative.length() > 0) {
            
        // //     JsonObject message2 = messages.createNestedObject();
        // //     message2["text"] = "Исключить " + negative.c_str();
        // //     message2["weight"] = 2;
        // // }

        // // Создание объекта generation_options
        // JsonObject generationOptions = jsonDoc.createNestedObject("generation_options");
        // generationOptions["mime_type"] = "image/jpeg";
        
        // JsonObject aspectRatio = generationOptions.createNestedObject("aspectRatio");
        // aspectRatio["widthRatio"] = width;
        // aspectRatio["heightRatio"] = height;

        // // FUS_LOG("JSON being sent:");
        // // FUS_LOG(json);

        // String id;
        // bool done;
        // String errorMsg;

        // uint8_t tries = FUSION_TRIES;
        // while (tries--) {
        //     if (performGenerateHttpRequest(FUSION_HOST, "/foundationModels/v1/imageGenerationAsync", "POST", jsonDoc, id, done, errorMsg)) {
        //         FUS_LOG("Gen request sent");
        //         _tmr = millis();
        //         _uuid = id;
        //         if (!_uuid.length()) {
        //             status = "operation ID unknown";
        //             return false;
        //         } 
        //         status = "wait result";
        //         return true;
        //     } else {
        //         FUS_LOG("Gen request error");
        //         delay(2000);
        //     }
        // }
        // status = "gen request error";
        // return false;
    }
    bool getImage() {
        if (!_imgs_host.length()) return false;
        if (!_uuid.length()) return false;
        FUS_LOG("Check status...");
        
        String errorMsg;
        String url("/operations/");
        url += _uuid;
        // return performGetImageRequest(FUSION_HOST, url, "GET", errorMsg);
        return false;
    }

    void tick() {
        if (_uuid.length() && millis() - _tmr >= FUSION_PERIOD) {
            _tmr = millis();
            getImage();
        }
    }
    String modelID() { return _id; }
    String styles = "";
    String status = "";
   private:
    String _imgs_host;
    uint16_t _imgs_port;
    String _uuid;
    uint8_t _scale = 0;
    uint32_t _tmr = 0;
    String _id;
    RenderCallback _rnd_cb = nullptr;
    RenderEndCallback _end_cb = nullptr;
    StreamB64* _stream = nullptr;
    uint8_t _finalBuffer[FINAL_BUF_SIZE];

    // static
    static YandexArt* self;

    // Фильтры для успешного ответа и ошибки
    StaticJsonDocument<200> successFilter;
    StaticJsonDocument<100> errorFilter;

    static size_t jd_input_cb(JDEC* jdec, uint8_t* buf, size_t len) {
        if (self) {
            self->_stream->readBytes(buf, len);
        }
        return len;
    }
   
    static int jd_output_cb(JDEC* jdec, void* bitmap, JRECT* rect) {
        if (self && self->_rnd_cb) {
            filter_points(rect->left, rect->top, rect->right - rect->left + 1, rect->bottom - rect->top + 1, (uint8_t*)bitmap);
            // self->_rnd_cb(rect->left, rect->top, rect->right - rect->left + 1, rect->bottom - rect->top + 1, (uint8_t*)bitmap);
        }
        return 1;
    }


    static void filter_points(int x, int y, int width, int height, uint8_t* src_buf) {
        // Вычисляем параметры пересечения прямоугольников
        int intersect_x = MAX(x, INTERNAL_X);
        int intersect_y = MAX(y, INTERNAL_Y);
        int intersect_x_end = MIN(x + width, INTERNAL_X + INTERNAL_WIDTH);
        int intersect_y_end = MIN(y + height, INTERNAL_Y + INTERNAL_HEIGHT);
        
        int new_width = intersect_x_end - intersect_x;
        int new_height = intersect_y_end - intersect_y;
        
        // Проверка на полное вхождение не требуется, так как мы всегда копируем данные и инвертируем цвета
        
        if (new_width <= 0 || new_height <= 0) {
            return;
        }
        
        // Копируем подходящие точки и инвертируем цвета
        for (int dy = 0; dy < new_height; dy++) {
            for (int dx = 0; dx < new_width; dx++) {
                // Вычисляем координаты в исходном буфере
                int src_x = intersect_x - x + dx;
                int src_y = intersect_y - y + dy;
                
                // Индексы в памяти (по 2 байта на точку)
                int src_index = (src_y * width + src_x) * 2;
                int dst_index = (dy * new_width + dx) * 2;
                
                // Извлекаем цвет из исходного буфера
                uint16_t color = (src_buf[src_index] << 8) | src_buf[src_index + 1];
                
                // Инвертируем цвет
                int r = 31 - GET_R(color);
                int g = 63 - GET_G(color);
                int b = 31 - GET_B(color);
                uint16_t inverted_color = SET_RGB(r, g, b);
                
                // Записываем инвертированный цвет в выходной буфер
                self->_finalBuffer[dst_index] = inverted_color >> 8;
                self->_finalBuffer[dst_index + 1] = inverted_color & 0xFF;
            }
        }
        
        // Вызов целевой функции с новыми параметрами
        self->_rnd_cb(intersect_x - INTERNAL_X, intersect_y - INTERNAL_Y, new_width, new_height, self->_finalBuffer);
    }    

    // system
    bool performGenerateHttpRequest(String host, uint16_t port, Text url, Text method, DynamicJsonDocument& jsonDoc, String& id, String& taskStatus, String& errorMsg) {
        FUS_LOG("-0-");
        // Сериализация JSON в строку
        String jsonString;
        serializeJson(jsonDoc, jsonString);
        
        // Установка заголовков
        ghttp::Client::Headers headers;
        headers.add("Content-Type", "application/json");
        // headers.add("Authorization", "Api-Key " + _imgs_host);
        headers.add("Accept", "*/*");
        
        FUSION_CLIENT client;
// #ifdef ESP8266
//         client.setBufferSizes(512, 512);
// #endif
        FUS_LOG("-1-");
        // client.setInsecure();

        IPAddress ip;
        if (ip.fromString(host)) {
             FUS_LOG("-1.1-");
        };
        // ghttp::Client http(client, host.c_str(), port);
        ghttp::Client http(client, ip, port);
        FUS_LOG("-2-");

        // Отправка запроса
        FUS_LOG("Host " + host);
        
        FUS_LOG("Port " + String(port));
        FUS_LOG("Url " + url.toString());
        FUS_LOG("Body " + jsonString);
        FUS_LOG("Headers");
        FUS_LOG(headers);

        bool ok = http.request(url, method, headers, jsonString);
        
        if (!ok) {
            FUS_LOG("Request error");
            http.flush();
            return false;
        }

        // Получение ответа
        ghttp::Client::Response resp = http.getResponse();
        
        if (resp) {
            FUS_LOG("Response received");      
        } else {
            FUS_LOG("Response not exists");
            http.flush();
            return false;               
        }
        
        StreamReader responseBodyReader(resp.body());

        int httpStatus = resp.code();
        FUS_LOG("Status " + String(httpStatus));        
        
        // Проверка HTTP статуса
        if (httpStatus < 200 || httpStatus > 299) {
            // Парсинг ответа с использованием статического фильтра для ошибки

            DynamicJsonDocument docError(100);
            DeserializationError err = deserializeJson(docError, responseBodyReader, DeserializationOption::Filter(errorFilter));

            if (err) {
                FUS_LOG("Failed to parse response JSON for error");
                FUS_LOG(err.c_str());
                http.flush();
                return false;
            }
            
            // Извлечение поля "error"
            if (docError.containsKey("error")) {
                errorMsg = docError["error"].as<String>();
            } else {
                errorMsg = "Unknown error";
            }
            FUS_LOG(errorMsg.c_str());
            http.flush();
            return false;
        }
        
        DynamicJsonDocument docSuccess(200);
        DeserializationError err = deserializeJson(docSuccess, responseBodyReader, DeserializationOption::Filter(successFilter));
        
        if (err) {
            FUS_LOG("Failed to parse response JSON for success");
            FUS_LOG(err.c_str());
            http.flush();
            return false;
        }
        
        // Извлечение интересующих полей
        if (docSuccess.containsKey("id")) {
            id = docSuccess["id"].as<String>();
        } else {
            id = "";
        }
        
        if (docSuccess.containsKey("status")) {
            taskStatus = docSuccess["status"].as<String>();
        } else {
            taskStatus = "error";
        }
        FUS_LOG("Operation id " + id + " status " + taskStatus);
        http.flush();
        return taskStatus != "error";
    }

    bool performGetImageRequest(Text host, Text url, Text method, String& errorMsg) {
        FUSION_CLIENT client;
        return false;
// // #ifdef ESP8266
//         // client.setBufferSizes(512, 512);
// // #endif
//         client.setInsecure();
//         ghttp::Client http(client, host.str(), FUSION_PORT);
//         // Установка заголовков
//         ghttp::Client::Headers headers;
//         headers.add("Content-Type", "application/json");
//         headers.add("Authorization", "Api-Key " + _imgs_host);
//         headers.add("Accept", "*/*");


//         FUS_LOG("Host " + host.toString());
//         FUS_LOG("Url " + url.toString());
//         // FUS_LOG("Body " + jsonString);
//         FUS_LOG("Headers");
//         FUS_LOG(headers);


//         bool ok = http.request(url, method, headers);

//         if (!ok) {
//             FUS_LOG("Request error");
//             http.flush();
//             return false;
//         }

//         ghttp::Client::Response resp = http.getResponse();

//         int httpStatus = resp.code();
//         FUS_LOG("Response code: " + String(httpStatus));

//         if (resp) {
//             FUS_LOG("Response");      
//         } else {
//             FUS_LOG("Response not exists");
//             http.flush();
//             return false;               
//         }

//         if (httpStatus < 200 || httpStatus > 299) {
//             FUS_LOG("Parsing error ...");
//             // Парсинг ответа с использованием статического фильтра для ошибки

//             StreamReader responseBodyReader(resp.body());
//             DynamicJsonDocument docError(100);
//             DeserializationError err = deserializeJson(docError, responseBodyReader, DeserializationOption::Filter(errorFilter));

//             if (err) {
//                 FUS_LOG("Failed to parse response JSON for error");
//                 FUS_LOG(err.c_str());
//                 http.flush();
//                 return false;
//             }
            
//             // Извлечение поля "error"
//             if (docError.containsKey("error")) {
//                 errorMsg = docError["error"].as<String>();
//             } else {
//                 errorMsg = "Unknown error";
//             }
//             FUS_LOG(errorMsg.c_str());
//             http.flush();
//             return false;
//         } else {
//                 FUS_LOG("Parsing success document ...");
//                 bool ok = parseStatus(resp.body());
//                 http.flush();
//                 return ok;
//         }

//         return false;
    }

    String readValue(Stream& stream) {
        String result = "";
        int c;
        
        int nextChar = stream.peek();

        while ((c = stream.read()) != -1) {
             // Читаем символы из потока
            if (c == 'n' || c == 't' || c == 'f') {
                // Это null или true или false
                result = String((char)c) + stream.readStringUntil(',');
                break;
            } else if ( c == '"') {
               result = stream.readStringUntil('"');
               break;
            } else if ( c == ',' || c == '}') {
                break;
            }
                // Это какое-то неожиданное число
                // Просто пропустим
            
        }

        return result;
    }

    bool parseStatus(Stream& stream) {
        bool found = false;
        bool insideResult = false;
        while (stream.available()) {
            stream.readStringUntil('"');
            String key = stream.readStringUntil('"');
            if (key == "response") {
                insideResult = true;
                continue;
            }
            if (insideResult && key == "image") {
                FUS_LOG("image found");
                found = true;
                break;
            }

            // Только что прочли ключ, значит едем до двоеточия
            stream.readStringUntil(':');
            FUS_LOG("Key " + key);            

            // Теперь читаем значение
            String val = readValue(stream);
            FUS_LOG("Val " + val);

            if (!key.length() ) {
                FUS_LOG("Key not found");
                break;
            }

            // TODO 
            if (key == "done") {
                switch (Text(val).hash()) {
                    case SH("true"):
                        FUS_LOG("Done is true");
                        // В ответе должен быть image
                        _uuid = "";
                        break;
                    case SH("false"):
                        FUS_LOG("Done is false");
                        return true;
                        break;
                    default:
                        FUS_LOG("Done is unknown");
                        return true;
                }
            }
        }

        if (found) {
            stream.readStringUntil('"');
            uint8_t* workspace = new uint8_t[TJPGD_WORKSPACE_SIZE];
            if (!workspace) {
                FUS_LOG("allocate error");
                return false;
            }
            JDEC jdec;
            jdec.swap = 0;
            JRESULT jresult = JDR_OK;
            StreamB64 sb64(stream);
            _stream = &sb64;
            self = this;
            jresult = jd_prepare(&jdec, jd_input_cb, workspace, TJPGD_WORKSPACE_SIZE, 0);
            if (jresult == JDR_OK) {
                jresult = jd_decomp(&jdec, jd_output_cb, _scale);
                if (jresult == JDR_OK && _end_cb) _end_cb();
            } else {
                FUS_LOG("jdec error");
            }
            self = nullptr;
            delete[] workspace;
            status = jresult == JDR_OK ? "gen done" : "jpg error";
            return jresult == JDR_OK;
        }
        return true;
    }
};

YandexArt* YandexArt::self __attribute__((weak)) = nullptr;
