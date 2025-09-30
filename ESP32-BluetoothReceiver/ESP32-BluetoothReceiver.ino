#include "ESP_I2S.h"
#include "WiFi.h"
#include <BluetoothA2DPSink.h>

// GPIO's
#define RGBLED_RED      14   // GPIO14
#define RGBLED_GREEN    13   // GPIO13
#define RGBLED_BLUE     12   // GPIO12
#define PCM5102_LCK     15   // GPIO15
#define PCM5102_DIN     2    // GPIO2
#define PCM5102_BCK     4    // GPIO4
#define PCM5102_SCK     16   // GPIO16

// Bluetooth settings
#define BLUETOOTH_NAME "Skydive Hoogeveen"

// I2C audio settings
#define SAMPLERATE      44100

// WiFi settings
#define HOSTNAME        "Bluetooth-Audio"
#define SSID            "Skydive Hoogeveen Private"
#define PASSWORD        "xxxxxxxxxxxx"


I2SClass i2s;
BluetoothA2DPSink a2dp_sink(i2s);
bool wifi_connected;



/* WiFi event handler */
static void wifi_connection_state_changed(WiFiEvent_t event)
{
    switch (event) {

        case WIFI_EVENT_STA_CONNECTED:
            wifi_connected = true;
            Serial.println("Connected to access point");
            break;

        case WIFI_EVENT_STA_DISCONNECTED:
            Serial.println("Disconnected from WiFi access point");
            wifi_connected = false;
            break;

        case WIFI_EVENT_AP_STADISCONNECTED:
            Serial.println("WiFi client disconnected");
            wifi_connected = false;
            break;

        default:
        break;
  }
}

/* Bluetooth event handler */
static void bt_connection_state_changed(esp_a2d_connection_state_t state, void *ptr) {
    switch (state) {

        case ESP_A2D_CONNECTION_STATE_DISCONNECTED:
            Serial.println("Bluetooth disconnected");
            setLEDColor((wifi_connected ? 0 : 255), 255, 0);
            break;

        case ESP_A2D_CONNECTION_STATE_CONNECTING:
            Serial.println("Bluetooth connecting");
            setLEDColor(255, 0, 255);
            break;

        case ESP_A2D_CONNECTION_STATE_CONNECTED:
            Serial.println("Bluetooth connected");
            setLEDColor(0, 0, 255);
            break;

        case ESP_A2D_CONNECTION_STATE_DISCONNECTING:
            Serial.println("Bluetooth disconnecting");
            setLEDColor(255, 0, 255);
            break;
    }
}


// Arduino setup
void setup() {
    Serial.begin(115200);
    Serial.println("Setup");

    // Initialize WiFi
    Serial.println("Initializing WiFi...");
    wifi_connected = false;
	WiFi.mode(WIFI_STA);
    WiFi.setHostname(HOSTNAME);
	WiFi.begin(SSID, PASSWORD);
    WiFi.onEvent(wifi_connection_state_changed);

    // Initialize RGB LED
    pinMode(RGBLED_RED, OUTPUT);
    pinMode(RGBLED_GREEN, OUTPUT);
    pinMode(RGBLED_BLUE, OUTPUT);
    setLEDColor(255, 0, 0);

    // Initialize I2C
    pinMode(PCM5102_SCK, OUTPUT);
    digitalWrite(PCM5102_SCK, 0);
    i2s.setPins(PCM5102_BCK, PCM5102_LCK, PCM5102_DIN);
    if (!i2s.begin(I2S_MODE_STD, SAMPLERATE, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO, I2S_STD_SLOT_BOTH)) {
        Serial.println("Failed to initialize I2S!");
        while (1); // do nothing
    }

    // Bluetooth A2DP: Enable auto-reconnect
    a2dp_sink.set_auto_reconnect(true);

    // Bluetooth A2DP: Make device discoverable
    a2dp_sink.set_discoverability(ESP_BT_GENERAL_DISCOVERABLE);

    // Bluetooth A2DP: Set connection callbacks
    a2dp_sink.set_on_connection_state_changed(bt_connection_state_changed, nullptr);

    // Bluetooth A2DP: Start the sink
    a2dp_sink.start(BLUETOOTH_NAME);

    // Setup completed
    setLEDColor((wifi_connected ? 0 : 255), 255, 0);
    Serial.println("Setup completed");
}

// Arduino main loop
void loop() {
    delay(1);
}

// Set LED color
void setLEDColor(int r, int g, int b) {
    analogWrite(RGBLED_RED, r);
    analogWrite(RGBLED_GREEN, g);
    analogWrite(RGBLED_BLUE, b);
}
