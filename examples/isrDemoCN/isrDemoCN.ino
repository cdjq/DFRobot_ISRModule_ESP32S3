/*!
 * @file isrDemoCN.ino
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

#ifdef I2C_COMMUNICATION
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
SoftwareSerial mySerial(/*rx*/ 4, /*tx*/ 5);
DFRobot_ISRModule_UART isr(&mySerial, UART_BAUDRATE);
#elif defined(ESP32)
DFRobot_ISRModule_UART isr(&Serial1, UART_BAUDRATE, /*rx*/ D2, /*tx*/ D3);
#else
DFRobot_ISRModule_UART isr(&Serial1, UART_BAUDRATE);
#endif
#endif


void setup() {
  Serial.begin(115200);
  /*Wait for the chip to be initialized completely, and then exit*/
  while (!isr.begin(/* type */ isr.eSpeechModelChinese, /* duration */ 10)) {
    Serial.println("Failed to init chip, please check if the chip connection is fine. ");
    delay(1000);
  }
  Serial.println("begin OK!");

  Serial.println("add command word...\n");
  /*!
   * note:
   * 添加命令词的编号从1开始(0是唤醒词), 中文以拼音形式添加, 命令词编号 1~254, 词条字节数小于120
   * 不支持中英文混合, 不能含有阿拉伯数字和特殊字符, 错误的命令词会使模型重置
   * 不能重复添加相同编号的命令词进行修改, 可删除后重新添加
   * 中文拼音转换, 可使用库目录下的工具完成
   * 详见 DFRobot_ISRModule_ESP32S3\resources\tool\README.md
   */
  isr.addCommandWord(1, "jing tian tian qi zen me yang");
  isr.addCommandWord(2, "ming tian you yu ma");
  isr.addCommandWord(3, "she zhi ming tian qi dian nao zhong");

  // isr.delCommandWord(1);  //删除指定ID命令词
  // isr.delCommandWord("she zhi ming tian qi dian nao zhong"); //删除指定名称命令词
  Serial.println("------------detect start------------\n");
}

void loop() {
  uint8_t id = isr.getKeywordID();
  switch (id) {
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
      Serial.print(id);
      Serial.println(" Unknown command word!");
      break;
  }
  delay(150);
}
