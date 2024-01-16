/*!
 * @file DFRobot_ISRModule.h
 * @brief Define the basic structure of class DFRobot_ISRModule
 * @details 初始化，设置命令词类型，设置唤醒时间，添加命令词，删除命令词，获取识别的命令词ID
 * @copyright	Copyright (c) 2021 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [qsjhyy](yihuan.huang@dfrobot.com)
 * @version V1.0
 * @date 2024-01-06
 * @url https://github.com/DFRobot/DFRobot_ISRModule
 */
#ifndef _DFRobot_ISR_Module_H_
#define _DFRobot_ISR_Module_H_

#include "Arduino.h"
#include "Wire.h"

#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
#include "SoftwareSerial.h"
#else
#include "HardwareSerial.h"
#endif

// #define ENABLE_DBG ///< Enable this macro to see the detailed running process of the program
#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif


class DFRobot_ISRModule
{
public:
#define MODULE_I2C_ADDRESS   0x14   //< Sensor device address

#define UART_BAUDRATE        ((uint32_t)57600)
#define CMD_READ_REGBUF      0xBB
#define CMD_WRITE_REGBUF     0xCC
#define TIME_OUT             200    //< 200 ms time out

#define VOICE_ID_REG         0x00   // 识别到的语音ID
#define WAKEUP_TIME_REG      0x01   // 唤醒持续时长 (单位: s)
#define MODEL_TYPE_REG       0x02   // 语言模型种类 (cn or en)
#define ADD_CMD_REG          0x03   // 添加关键词 (编号, 词条)
#define DEL_CMD_BY_ID_REG    0x04   // 删除关键词 (通过编号)
#define DEL_CMD_BY_STR_REG   0x05   // 删除关键词 (通过词条)

#define CMD_ERROR_REG        0x03   // 错误寄存器 0 为没有错误
#define REG_ISR_PID          0xAAU
#define REG_ISR_VID          0xACU
#define REG_ISR_VERSION      0xAEU

#define MODULE_DFR0715_PID       0x42CB   ///< Sensor PID
#define MODULE_DFR0715_VID       0x3343   ///< Sensor VID
#define MODULE_DFR0715_VERSION   0x0100   ///< Sensor VERSION

  /**
   * @enum eSpeechModelType_t
   * @brief 语音识别模型的类型
   */
  typedef enum {
    eSpeechModelSTDBY = 0x00,
    eSpeechModelChinese = 0x01,
    eSpeechModelEnglish = 0x02,
  }eSpeechModelType_t;

public:
  /**
   * @fn DFRobot_ISRModule
   * @brief Constructor
   * @return None
   */
  DFRobot_ISRModule();

  /**
   * @fn begin
   * @brief subclass initialization function
   * @param type - 语音识别模型的类型: 待机，中文，英文
   * @param duration - 唤醒持续时间. range: 0~255, unit: second.
   * @return bool type, means returning initialization status
   * @retval true NO_ERROR
   */
  virtual bool begin(eSpeechModelType_t type = eSpeechModelChinese, uint8_t duration = 10);

  /**
   * @fn addCommandWord
   * @brief 添加关键词(词条, 编号)
   * @param num - 命令词编号
   * @param str - 命令词词条
   * @return None
   */
  void addCommandWord(uint8_t num, String str);

  /**
   * @fn delCMDByNUM
   * @brief 删除关键词（编号 or 词条）
   * @param num - 命令词编号
   * @param str - 命令词词条
   * @return None
   */
  void delCommandWord(uint8_t num);
  void delCommandWord(String str);

  /**
   * @fn getKeywordID
   * @brief 获取识别结果（ID）
   * @return 返回识别结果
   * @retval 0 : 识别到唤醒词
   * @retval 1~254 : 识别到对应编号的命令词
   * @retval 0xFF : 未识别到关键词
   */
  uint8_t getKeywordID(void);

private:

  /**
   * @fn setSpeechModelType
   * @brief 设置语音识别模型的类型
   * @param type - 语音识别模型的类型: 待机，中文，英文
   * @return None
   */
  void setSpeechModelType(eSpeechModelType_t type);

  /**
   * @fn setWakeupTime
   * @brief 设置退出唤醒时间
   * @param duration - 唤醒持续时间. range: 0, 6~120, unit: second.
   * @note 6秒为下限, 除开零以外, 小于6秒也设为6秒; 0为不退出
   * @return None
   */
  void setWakeupTime(uint8_t duration);

  /**
   * @fn writeReg
   * @brief Write register function, designed as virtual function, implemented by derived class
   * @param reg  Register address 8bits
   * @param pBuf Storage and buffer for data to be written
   * @param size Length of data to be written
   * @return None
   */
  virtual void writeReg(uint8_t reg, void* pBuf, size_t size) = 0;

  /**
   * @fn readReg
   * @brief Read register function, designed as virtual function, implemented by derived class
   * @param reg  Register address 8bits
   * @param pBuf Storage and buffer for data to be read
   * @param size Length of data to be read
   * @return uint8_t type, indicates returning read register status
   * @retval 0 Reading succeeded
   * @retval 1 Reading failed
   */
  virtual uint8_t readReg(uint8_t reg, void* pBuf, size_t size) = 0;

};

class DFRobot_ISRModule_I2C:public DFRobot_ISRModule
{
public:
  DFRobot_ISRModule_I2C(TwoWire* pWire = &Wire, uint8_t addr = MODULE_I2C_ADDRESS);
  bool begin(eSpeechModelType_t type = eSpeechModelChinese, uint8_t duration = 10);
protected:
  virtual void writeReg(uint8_t reg, void* pBuf, size_t size);
  virtual uint8_t readReg(uint8_t reg, void* pBuf, size_t size);
private:
  TwoWire* _pWire;
  uint8_t _deviceAddr;
};

class DFRobot_ISRModule_UART:public DFRobot_ISRModule
{
public:
#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
  DFRobot_ISRModule_UART(SoftwareSerial* sSerial, uint32_t Baud = UART_BAUDRATE);
#else
  DFRobot_ISRModule_UART(HardwareSerial* hSerial, uint32_t Baud = UART_BAUDRATE, uint8_t rxpin = 0, uint8_t txpin = 0);
#endif

  bool begin(eSpeechModelType_t type = eSpeechModelChinese, uint8_t duration = 10);
protected:
  virtual void writeReg(uint8_t reg, void* pBuf, size_t size);
  virtual uint8_t readReg(uint8_t reg, void* pBuf, size_t size);
private:
#if defined(ARDUINO_AVR_UNO) || defined(ESP8266)
  SoftwareSerial* _serial;
#else
  HardwareSerial* _serial;
#endif
  uint32_t _baud;
  uint8_t _rxpin;
  uint8_t _txpin;
};

#endif/* _DFRobot_ISR_Module_H_ */
