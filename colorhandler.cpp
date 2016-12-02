#include <stdio.h>
#include "crow.h"

#define HELP_STR \
"{" \
    "\"/\": \"HOLS Help Request\"," \
    "\"/set\": \"Color=FFFFFF (6 digit hex, no #)\"," \
    "\"/setOff\": \"No params: turns lights off\"" \
"}"

#define SET_STR \
"{" \
    "\"Function\": Set color\"" \
"}"

#define OFF_STR \
"{" \
    "\"Function\": Set lights to OFF\"" \
"}"

#define REDPIN 22
#define GREENPIN 23
#define BLUEPIN 24

#define OFF_COLOR "000000"

FILE* fp;

void setColor(const char* color);

int main()
{
    fp = fopen("/dev/pi-blaster", "w");
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([]() {
        return HELP_STR;
    });

    CROW_ROUTE(app, "/setOff")
    ([]() {
        setColor(OFF_COLOR);
        return OFF_STR;
    });

    CROW_ROUTE(app, "/set")
    ([](const crow::request& req) {
        setColor(req.url_params.get("color"));
        return SET_STR;
    });

    app.port(18080).run();
    fclose(fp);
}

void setColor(const char* color)
{
    int colorVal = (int)strtol(color, nullptr, 16);
    float r = (colorVal >> 16) / 255.0f;
    float g = (colorVal >> 8 & 0xFF) / 255.0f;
    float b = (colorVal & 0xFF) / 255.0f;
    fprintf(fp, "%d=%.3f\n%d=%.3f\n%d=%.3f\n", REDPIN, r, GREENPIN, g, BLUEPIN, b);
    fflush(fp);
}
