/*!
 * @file isrDemo.ino
 * @brief 初始化，设置命令词类型，设置唤醒时间，添加命令词，删除命令词，获取识别的命令词ID
 * @copyright    Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license      The MIT License (MIT)
 * @author [qsjhyy](yihuan.huang@dfrobot.com)
 * @version V1.0
 * @date 2022-08-30
 * @url https://github.com/DFRobot/DFRobot_ISRModule
 */
#include "DFRobot_ISRModule.h"

#define I2C_COMMUNICATION  //use I2C for communication, but use the serial port for communication if the line of codes were masked

#ifdef  I2C_COMMUNICATION
DFRobot_ISRModule_I2C isr(&Wire, MODULE_I2C_ADDRESS);
#else
 /* ---------------------------------------------------------------------------------------------------------------------
  *    board   |             MCU                | Leonardo/Mega2560/M0 |    UNO    | ESP8266 | ESP32 |  microbit  |   m0  |
  *     VCC    |            3.3V/5V             |        VCC           |    VCC    |   VCC   |  VCC  |     X      |  vcc  |
  *     GND    |              GND               |        GND           |    GND    |   GND   |  GND  |     X      |  gnd  |
  *     RX     |              TX                |     Serial1 TX1      |     5     |   5/D6  |  D2   |     X      |  tx1  |
  *     TX     |              RX                |     Serial1 RX1      |     4     |   4/D7  |  D3   |     X      |  rx1  |
  * ----------------------------------------------------------------------------------------------------------------------*/
  /* Baud rate cannot be changed  */
#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
SoftwareSerial mySerial(/*rx*/4, /*tx*/5);
DFRobot_ISRModule_UART isr(&mySerial, UART_BAUDRATE);
#elif defined(ESP32)
DFRobot_ISRModule_UART isr(&Serial1, UART_BAUDRATE,/*rx*/D2,/*tx*/D3);
#else
DFRobot_ISRModule_UART isr(&Serial1, UART_BAUDRATE);
#endif
#endif

// 选择示例演示的模型类型(中文或英文)
DFRobot_ISRModule::eSpeechModelType_t moduleType = isr.eSpeechModelChinese;
// DFRobot_ISRModule::eSpeechModelType_t moduleType = isr.eSpeechModelEnglish;

void setup()
{
    Serial.begin(115200);
    /*Wait for the chip to be initialized completely, and then exit*/
    while (!isr.begin(/* type */moduleType, /* duration */5)) {
        Serial.println("Failed to init chip, please check if the chip connection is fine. ");
        delay(1000);
    }
    Serial.println("begin OK!");

    if (isr.eSpeechModelChinese == moduleType) {
        // 添加命令词的编号从1开始(0是唤醒词), 中文以拼音形式添加
        // 不支持中英文混合, 不能含有阿拉伯数字和特殊字符, 错误的命令词会使模型重置
        // 中文拼音转换和英文字标转换, 可使用库目录下的工具完成
        // 详见 DFRobot_ISRModule_ESP32S3\resources\tool\README.md
        isr.addCommandWord(1, "da kai dian nao");
        isr.addCommandWord(2, "bo fang yin yue");
        isr.addCommandWord(3, "zan ting yin yue");

        // 删除命令后, 命令词列表的其他命令词编号也可能发生更改
        // 所以再次用编号删除命令词时, 应先确认命令词的编号是否改变, 可通过命令词识别确认
        // isr.delCommandWord(1);
        // isr.delCommandWord("bo fang yin yue");
    } else if (isr.eSpeechModelEnglish == moduleType) {
        isr.addCommandWord(1, "TfL Mm c qbK");   // tell me a joke
        isr.addCommandWord(2, "TkN nN jc KcMPYoTk");   // turn on the computer
        isr.addCommandWord(3, "TkN eF jc KcMPYoTk");   // turn off the computer
        // isr.delCommandWord(1);
        // isr.delCommandWord("TkN nN jc KcMPYoTk");
    }
}

void loop()
{
    switch (isr.getKeywordID()) {
    case 0:
        Serial.println("Recognize the wake word!");
        break;
    case 1:
        Serial.println("Command word 1 is identified!");
        break;
    case 2:
        Serial.println("Command word 2 is identified!");
        break;
    case 3:
        Serial.println("Command word 3 is identified!");
        break;
    case 0xFF:
        // Serial.println("Keywords not recognized!");
        break;

    default:
        Serial.println("Unknown command word!");
        break;
    }
    delay(100);
}
