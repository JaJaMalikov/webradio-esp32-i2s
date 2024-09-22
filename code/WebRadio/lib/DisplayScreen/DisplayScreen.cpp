#include "DisplayScreen.h"

DisplayScreen::DisplayScreen() {
}

void DisplayScreen::init() {
    this->u8g2 = new U8G2_SH1106_128X64_NONAME_F_HW_I2C(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
    this->u8g2->begin();
    this->u8g2->enableUTF8Print();

    xTaskCreate(
        [](void *arg){
            DisplayScreen * display_screen = (DisplayScreen *)arg;
            while(1) {
                vTaskDelay(pdMS_TO_TICKS(100));

                if (display_screen->business_state->getIsConnectingToWifi()) {
                    display_screen->connectingScreen();
                    continue;
                }

                if (millis() - display_screen->business_state->getVolumeChangedAtMillis() < 4000) {
                    display_screen->volumeScreen();
                } else {
                    display_screen->standbyScreen();
                }
   
            }
        },
        "displayTask",
        2000,
        this,
        1,
        NULL
    );
}

void DisplayScreen::clear() {
    this->u8g2->clearBuffer();
}


void DisplayScreen::demoScreen() {
 
    this->clear();
    String demo_string = "Démo"; 
    this->u8g2->setFont(u8g2_font_spleen32x64_me); // u8g2_font_7x13B_tf
    this->u8g2->setCursor(2, 64);
    this->u8g2->print(demo_string);
    this->display();
}


void DisplayScreen::display() {
    this->u8g2->sendBuffer();
}


void DisplayScreen::injectBusinesState(BusinessState * business_state) {
    this->business_state = business_state;
}


void DisplayScreen::infoScreen() {
    this->clear();

    this->u8g2->setFont(u8g2_font_6x12_tf);
    this->u8g2->setCursor(5, 10);
    this->u8g2->print("Temperature: ");
    this->u8g2->print(this->business_state->getTemperature());
    this->u8g2->print(" C");
    this->u8g2->setCursor(0, 30);
    this->u8g2->print("Pressure: ");
    this->u8g2->print(this->business_state->getPressure());
    this->u8g2->print(" hPa");

    String text = this->business_state->getDateTime();
    int text_width = this->u8g2->getUTF8Width(text.c_str());
    int x = this->u8g2->getDisplayWidth() / 2 - text_width / 2;
    this->u8g2->drawUTF8(x, 50, text.c_str());    


    this->display();
}



void DisplayScreen::infoTestScreen() {
    this->clear();

    String text = "Texte centré";
    this->u8g2->setFont(u8g2_font_7x13B_tf);
    int text_width = this->u8g2->getUTF8Width(text.c_str());
    int text_height = this->u8g2->getAscent() - this->u8g2->getDescent();

    int x = this->u8g2->getDisplayWidth() / 2 - text_width / 2;
    int y = this->u8g2->getDisplayHeight() / 2 + text_height / 2;

    this->u8g2->drawUTF8(x, y, text.c_str());    
    this->display();
}


// Production screen
void DisplayScreen::standbyScreen() {
    this->clear();

    int text_width = 0;
    int text_height = 0;
    int x = 0;
    int y = 0;

    String formated_time = this->business_state->getFormatedTime();
    this->u8g2->setFont(u8g2_font_osr26_tn);
    text_width = this->u8g2->getUTF8Width(formated_time.c_str());
    text_height = this->u8g2->getAscent() - this->u8g2->getDescent();
    x = this->u8g2->getDisplayWidth() / 2 - text_width / 2;
    y = this->u8g2->getDisplayHeight() / 2 + text_height / 2;
    this->u8g2->drawUTF8(x, y, formated_time.c_str());  
    

    String formated_date = this->business_state->getFormatedDate();
    this->u8g2->setFont(u8g2_font_6x12_tf);
    text_height = this->u8g2->getAscent() - this->u8g2->getDescent();
    x = 0;
    y = text_height;
    this->u8g2->drawUTF8(x, y, formated_date.c_str()); 

    float temperature = this->business_state->getTemperature();
    String formated_temperature = String(temperature) + " C";
    this->u8g2->setFont(u8g2_font_6x12_tf);
    text_width = this->u8g2->getUTF8Width(formated_temperature.c_str());
    text_height = this->u8g2->getAscent() - this->u8g2->getDescent();
    x = this->u8g2->getDisplayWidth() - text_width;
    y = text_height;
    this->u8g2->drawUTF8(x, y, formated_temperature.c_str());  
    
    this->display();
}


void DisplayScreen::volumeScreen() {

    int volume = this->business_state->getPlayingVolume();
    if (this->displayed_volume == volume) {
        return;
    }

    this->clear();

    this->displayed_volume = volume;
    int width = 128;
    int height = 64;

    int max_volume = 21;
    int bar_width = width / max_volume;

    for (int i = 0; i < max_volume; i++) {
        int x = i * bar_width;
        int y = 0;
        int bar_height = (height / max_volume) * i;
        if (i < volume) {
            this->u8g2->drawRBox(x, y + (height - bar_height), bar_width, bar_height, 1);
        } else {
            this->u8g2->drawRFrame(x, y + (height - bar_height), bar_width, bar_height, 1);
        }
    }
    this->display();
}

void DisplayScreen::connectingScreen() {
    this->clear();

    String text = "connecting";
    this->u8g2->setFont(u8g2_font_7x13B_tf);
    int text_width = this->u8g2->getUTF8Width(text.c_str());
    int text_height = this->u8g2->getAscent() - this->u8g2->getDescent();

    int x = this->u8g2->getDisplayWidth() / 2 - text_width / 2;
    int y = this->u8g2->getDisplayHeight() / 2 + text_height / 2;

    this->u8g2->drawUTF8(x, y, text.c_str());

    int width = 128;
    int height = 64;

    int position = millis() / 1000 % 3;

    this->u8g2->drawCircle(80, 48, 5, U8G2_DRAW_ALL);
    this->u8g2->drawCircle(48, 48, 5, U8G2_DRAW_ALL);
    this->u8g2->drawCircle(64, 48, 5, U8G2_DRAW_ALL);

    switch (position) {
        case 0:
            this->u8g2->drawDisc(48, 48, 5, U8G2_DRAW_ALL);
            break;
        case 1:
            this->u8g2->drawDisc(64, 48, 5, U8G2_DRAW_ALL);
            break;
        case 2:
            this->u8g2->drawDisc(80, 48, 5, U8G2_DRAW_ALL);
            break;    
        default:
            break;
    }

    
    this->display();
}

