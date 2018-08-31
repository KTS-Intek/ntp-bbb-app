#ifndef UCMETEREVENTCODES_H
#define UCMETEREVENTCODES_H

#include <QList>

typedef QList<quint32> UcmEvList;
typedef QList<quint8> UcmEvGroupList;

//UC Meter EVENTS   uint32
///секція кодів що є похідними від МТХ3
#define UCM_EV_UNKNOWN_CODE                 0x00
///
#define UCM_EV_ENRG_T1_FAULT                0x01 //значення накоплюючих регістрів тарифу 1 втрачені
#define UCM_EV_ENRG_T2_FAULT                0x02 //значення накоплюючих регістрів тарифу 2 втрачені
#define UCM_EV_ENRG_T3_FAULT                0x03 //значення накоплюючих регістрів тарифу 3 втрачені
#define UCM_EV_ENRG_T4_FAULT                0x04 //значення накоплюючих регістрів тарифу 4 втрачені

#define UCM_EV_ACCESS_LOCKED                0x11  //доступ закритий із-за помилки ключа доступу, очікую на відкриття
#define UCM_EV_ACCESS_UNLOCKED              0x12  //доступ відкритий ( була помилки ключа доступу)

#define UCM_EV_ERR_ACCESS                   0x13 //Неправильний ключ

#define UCM_EV_CASE_OPEN                    0x15 //  заміна, бо найоб 0x14 //Корпус лічильника відкритий, очікую на закриття
#define UCM_EV_CASE_CLOSE                   0x14  // заміна, бо найоб 0x15 //Корпус лічильника закритий

#define UCM_EV_MAGNETIC_ON                  0x16 //виявлено наявність впливу постійного магнітного поля, очікую на закінчення впливу
#define UCM_EV_MAGNETIC_OFF                 0x17 //виявлено закінчення впливу постійного магнітного поля

#define UCM_EV_CHANGE_ACCESS_KEY_0          0x20 //Ключ доступу рівня 0 змінено, стирання даних лічильника, запис важливих парметрів
#define UCM_EV_CHANGE_ACCESS_KEY_1          0x21 //Ключ доступу рівня 1 змінено, пароль для оновлення ПЗ лічильника
#define UCM_EV_CHANGE_ACCESS_KEY_2          0x22 //Ключ доступу рівня 2 змінено,запис операторських параметрів
#define UCM_EV_CHANGE_ACCESS_KEY_3          0x23 //Ключ доступу рівня 3 змінено, найнижчий рівень, публічний, тільки читання

#define UCM_EV_CHANGE_PAR_LOCAL             0x24 //параметри змінено локально (оптичний інтерфейс)
#define UCM_EV_CHANGE_PAR_REMOTE            0x25 //параметри змінено віддалено

#define UCM_EV_CMD_CHANGE_TIME              0x26 //отримана команда зміни часу, час змінено

#define UCM_EV_CMD_RELAY_ON                 0x27 //отримана команда увімкнення реле
#define UCM_EV_CMD_RELAY_OFF                0x28 //отримана команда вимкнення реле

#define UCM_EV_ENERGY_REG_OVERFLOW          0x31 //переповнення накоплюючого регістру енергії
#define UCM_EV_CHANGE_TARIFF_TBL            0x32 //тарифний план змінено
#define UCM_EV_SET_TARIFF_TBL               0x33 //отриманий новий тарифний план
#define UCM_EV_SUMMER_TIME                  0x34 //перехід на літній час
#define UCM_EV_WINTER_TIME                  0x35 //перехід на зимовий час

#define UCM_EV_RELAY_ON                     0x36 //реле увімкнено
#define UCM_EV_RELAY_OFF                    0x37 //реле вимкнено

#define UCM_EV_RESTART                      0x38 //рестарт ПЗ контролера
#define UCM_EV_WD_RESTART                   0x39 //рестарт по Сторожовику

#define UCM_EV_VA_MAX_OK                    0x40 //Відновлення нормальної напруги L1 після підвищеної напруги
#define UCM_EV_VA_MAX_OVER                  0x41 //напруга L1 вище порогу максимальної напруги, очікую на нормальну
#define UCM_EV_VA_MIN_OK                    0x42 //Відновлення нормальної напруги L1 після пониженої напруги
#define UCM_EV_VA_MIN_UNDER                 0x43 //Напруга L1 нижче порогу мінімальної напруги, очікую на нормальну
#define UCM_EV_VB_MAX_OK                    0x44 //Відновлення нормальної напруги L2 після підвищеної напруги
#define UCM_EV_VB_MAX_OVER                  0x45 //напруга L2 вище порогу максимальної напруги, очікую на нормальну
#define UCM_EV_VB_MIN_OK                    0x46 //Відновлення нормальної напруги L2 після пониженої напруги
#define UCM_EV_VB_MIN_UNDER                 0x47 //Напруга L2 нижче порогу мінімальної напруги, очікую на нормальну
#define UCM_EV_VC_MAX_OK                    0x48 //Відновлення нормальної напруги L3 після підвищеної напруги
#define UCM_EV_VC_MAX_OVER                  0x49 //напруга L3 вище порогу максимальної напруги, очікую на нормальну
#define UCM_EV_VC_MIN_OK                    0x4A //Відновлення нормальної напруги L3 після пониженої напруги
#define UCM_EV_VC_MIN_UNDER                 0x4B //Напруга L3 нижче порогу мінімальної напруги, очікую на нормальну
#define UCM_EV_F_MAX_OK                     0x4C //Відновлення нормальної частоти після підвищеної
#define UCM_EV_F_MAX_OVER                   0x4D //Частота вище порогу нормальної, очікую на нормальну
#define UCM_EV_F_MIN_OK                     0x4E //Відновлення нормальної частоти після пониженої
#define UCM_EV_F_MIN_UNDER                  0x4F //Частота нижче порогу нормальної, очікую на нормальну

#define UCM_EV_T_MAX_OK                     0x50 //Відновлення нормальної температури після підвищеної
#define UCM_EV_T_MAX_OVER                   0x51 //Температура вище порогу максимальної температури, очікую на нормальну
#define UCM_EV_T_MIN_OK                     0x52 //Відновлення нормальної температури після пониженої
#define UCM_EV_T_MIN_UNDER                  0x53 //Температура нижче порогу нормальної, очікую на нормальну

#define UCM_EV_IA_MAX_OK                    0x54 //Відновлення допустимого струму L1 після підвищеного
#define UCM_EV_IA_MAX_OVER                  0x55 //Струм L1 вище порогу допустимого, очікую на нормалізацію
#define UCM_EV_IB_MAX_OK                    0x56 //Відновлення допустимого струму L2 після підвищеного
#define UCM_EV_IB_MAX_OVER                  0x57 //Струм L2 вище порогу допустимого, очікую на нормалізацію
#define UCM_EV_IC_MAX_OK                    0x58 //Відновлення допустимого струму L3 після підвищеного
#define UCM_EV_IC_MAX_OVER                  0x59 //Струм L3 вище порогу допустимого, очікую на нормалізацію
#define UCM_EV_PA_MAX_OK                    0x5A //Відновлення допустимої активної споживаної потужності після підвищеної
#define UCM_EV_PA_MAX_OVER                  0x5B //Активна споживана потужність вище максимальної, очікую на нормалізацію
#define UCM_EV_PV_MAX_OK                    0x5C //Відновлення допустимої реактивної споживаної потужності після підвищеної
#define UCM_EV_PV_MAX_OVER                  0x5D //Реактивна споживана потужність вище максимальної, очікую на нормалізацію
#define UCM_EV_IDIFF_OK                     0x5E //Відновлення допустимого диференціального струму
#define UCM_EV_IDIFF_OVER                   0x5F //Перевищення допустимого диференціального струму, очікую на нормалізацію
#define UCM_EV_CLOCK_OK                     0x60 //Нормальний стан RTC відновлено
#define UCM_EV_CLOCK_FAULT                  0x61 //RTC не встановлені
#define UCM_EV_POWER_C_ON                   0x62 //Увімкнення напруги L3
#define UCM_EV_POWER_C_OFF                  0x63 //Вимкнення напруги L3
#define UCM_EV_POWER_B_ON                   0x64 //Увімкнення напруги L2
#define UCM_EV_POWER_B_OFF                  0x65 //Вимкнення напруги L2
#define UCM_EV_POWER_A_ON                   0x66 //Увімкнення напруги L1
#define UCM_EV_POWER_A_OFF                  0x67 //Вимкнення напруги L1

#define UCM_EV_BAT_OK                       0x68 //Відновлення нормальної напруги батареї RTC
#define UCM_EV_BAT_FAULT                    0x69 //Понижена напруга батареї RTC

#define UCM_EV_CAL_OK                       0x6A //Параметри калібровки встановлені
#define UCM_EV_CAL_FAULT                    0x6B //Калібровочні параметри втрачені

#define UCM_EV_V_PAR_OK                     0x6C //Заводські параметри встановлені
#define UCM_EV_V_PAR_FAULT                  0x6D //Заводські параметри втрачено
#define UCM_EV_O_PAR_OK                     0x6E //Параметри встановлено
#define UCM_EV_O_PAR_FAULT                  0x6F //Параметри втрачено

#define UCM_EV_CHANGE_COR_TIME              0x70 //Змінено параметри переходу на літній час

#define UCM_EV_CMD_RELAY_2_ON               0x71 //Включення другого реле
#define UCM_EV_CMD_RELAY_2_OFF              0x72 //Вимкнення другого реле

#define UCM_EV_CRSSZERO_ENT1                0x73 //Перехід через 0 лічильника активної енергії по тарифу 1 при досягненні 1000000.00 кВт
#define UCM_EV_CRSSZERO_ENT2                0x74 //Перехід через 0 лічильника активної енергії по тарифу 2 при досягненні 1000000.00 кВт
#define UCM_EV_CRSSZERO_ENT3                0x75 //Перехід через 0 лічильника активної енергії по тарифу 3 при досягненні 1000000.00 кВт
#define UCM_EV_CRSSZERO_ENT4                0x76 //Перехід через 0 лічильника активної енергії по тарифу 4 при досягненні 1000000.00 кВт
#define UCM_EV_CRSSZERO_VARi1               0x77 //Перехід через 0 лічильника позитивної реактивної енергії по тарифу 1 при досягненні 1000000.00 кВар
#define UCM_EV_CRSSZERO_VARi2               0x78 //Перехід через 0 лічильника позитивної реактивної енергії по тарифу 2 при досягненні 1000000.00 кВар
#define UCM_EV_CRSSZERO_VARi3               0x79 //Перехід через 0 лічильника позитивної реактивної енергії по тарифу 3 при досягненні 1000000.00 кВар
#define UCM_EV_CRSSZERO_VARi4               0x7A //Перехід через 0 лічильника позитивної реактивної енергії по тарифу 4 при досягненні 1000000.00 кВар
#define UCM_EV_CRSSZERO_VARe1               0x7B //Перехід через 0 лічильника негативної реактивної енергії по тарифу 1 при досягненні 1000000.00 кВар
#define UCM_EV_CRSSZERO_VARe2               0x7C //Перехід через 0 лічильника негативної реактивної енергії по тарифу 2 при досягненні 1000000.00 кВар
#define UCM_EV_CRSSZERO_VARe3               0x7D //Перехід через 0 лічильника негативної реактивної енергії по тарифу 3 при досягненні 1000000.00 кВар
#define UCM_EV_CRSSZERO_VARe4               0x7E //Перехід через 0 лічильника негативної реактивної енергії по тарифу 4 при досягненні 1000000.00 кВар

#define UCM_EV_CALFLAG_SET                  0x7F //Калібровочний біт встановлено
#define UCM_EV_CALFLAG_RESET                0x80 //Калібровочний біт скинено

#define UCM_EV_BAD_TEST_EEPROM              0x81 //Тест EEPROM не пройшов
#define UCM_EV_BAD_TEST_FRAM                0x82 //Тест FRAM не пройшов

#define UCM_EV_SET_NEW_SALDO                0x83 //Отримана нова передоплата
#define UCM_EV_SALDO_PARAM_BAD              0x84 //Параметри сальдо втрачені

#define UCM_EV_ACCPARAM_BAD                 0x85 //Акумуляційні параметри втрачено
#define UCM_EV_ACCPARAM_EXT_BAD             0x86 //Додаткові акумуляційні параметри втрачено
#define UCM_EV_CALC_PERIOD_BAD              0x87 //Дані розрахункових періодів втрачено
#define UCM_EV_BLOCK_TARIF_BAD              0x88 //Параметри блочних тарифів втрачено
#define UCM_EV_CALIBR_PARAM_BAD             0x89 //Значення калібровочних параметрів втрачено
#define UCM_EV_WINTER_SUMMER_BAD            0x8A //Значення параметру переходу на зимовий/літній час втрачено
#define UCM_EV_OP_PARAM_BAD                 0x8B //Операторські параметри втрачено
#define UCM_EV_OP_PARAM_EXT_BAD             0x8C //Операторські параметри 2 втрачено
#define UCM_EV_SALDO_EN_BAD                 0x8D //Значення енергій для розрахунку сальдо втрачено

#define UCM_EV_TIME_CORRECT                 0x8E //Корекція часу

#define UCM_EV_COEFF_TRANSF_CHANGE          0x8F //Змінено коефіцієнти трансформації

#define UCM_EV_RELAY_HARD_BAD_OFF           0x90 //Реле вимкнено механічно
#define UCM_EV_RELAY_HARD_ON                0x91 //Реле увімкнено після механічного впливу. Відновлено стан реле
#define UCM_EV_RELAY_HARD_BAD_ON            0x93 //Реле увімкнено механічно
#define UCM_EV_RELAY_HARD_OFF               0x94 //Реле вимкнено після механічного впливу. Відновлено стан реле

#define UCM_EV_METER_TROUBLE                0x95 //Збій лічильника

#define UCM_EV_CASE_KLEMA_OPEN              0x96 //Клемна коробка лічильника відкрита, очікую на закриття
#define UCM_EV_CASE_KLEMA_CLOSE             0x97 //Клемна коробка лічильника закрита

#define UCM_EV_CHANGE_TARIFF_TBL_2          0x98 //Тарифний план 2 змінено
#define UCM_EV_CHANGE_TARIFF_TBL_3          0x99 //Тарифний план 3 змінено

#define UCM_EV_CASE_MODULE_OPEN             0x9A //Відсік модуля лічильника відкрито
#define UCM_EV_CASE_MODULE_CLOSE            0x9B //Відсік модуля лічильника закрито

#define UCM_EV_SET_SALDO_PARAM              0x9C //Параметри сальдо встановлено
#define UCM_EV_POWER_OVER_RELAY_OFF         0x9D //Вимкнення реле після перевищення активної потужності

#define UCM_EV_CHANGE_PARAM_CANAL1          0x9E //Змінено параметр профілю навантаження 1
#define UCM_EV_CHANGE_PARAM_CANAL2          0x9F //Змінено парамтер профілю навантаження 2
#define UCM_EV_CHANGE_PARAM_CANAL3          0xA0 //Змінено параметр профілю навантаження 3
#define UCM_EV_CHANGE_PARAM_CANAL4          0xA1 //Змінено параметр профілю навантаження 4
#define UCM_EV_CHANGE_PARAM_CANAL5          0xA2 //Змінено параметр профілю навантаження 5
#define UCM_EV_CHANGE_PARAM_CANAL6          0xA3 //Змінено параметр профілю навантаження 6

#define UCM_EV_CRSSZERO_EXP_ENT1           0xA4 //Перехід через 0 лічильника активної експортної енергії по тарифу 1 при досягненні 1000000.00 кВт
#define UCM_EV_CRSSZERO_EXP_ENT2           0xA5 //Перехід через 0 лічильника активної експортної енергії по тарифу 2 при досягненні 1000000.00 кВт
#define UCM_EV_CRSSZERO_EXP_ENT3           0xA6 //Перехід через 0 лічильника активної експортної енергії по тарифу 3 при досягненні 1000000.00 кВт
#define UCM_EV_CRSSZERO_EXP_ENT4           0xA7 //Перехід через 0 лічильника активної експортної енергії по тарифу 4 при досягненні 1000000.00 кВт
#define UCM_EV_CRSSZERO_EXP_VARi1          0xA8 //Перехід через 0 лічильника реактивної позитивної експортної енергії по тарифу 1 при досягненні 1000000.00 кВар
#define UCM_EV_CRSSZERO_EXP_VARi2          0xA9 //Перехід через 0 лічильника реактивної позитивної експортної енергії по тарифу 2 при досягненні 1000000.00 кВар
#define UCM_EV_CRSSZERO_EXP_VARi3          0xAA //Перехід через 0 лічильника реактивної позитивної експортної енергії по тарифу 3 при досягненні 1000000.00 кВар
#define UCM_EV_CRSSZERO_EXP_VARi4          0xAB //Перехід через 0 лічильника реактивної позитивної експортної енергії по тарифу 4 при досягненні 1000000.00 кВар
#define UCM_EV_CRSSZERO_EXP_VARe1          0xAC //Перехід через 0 лічильника реактивної негативної експортної енергії по тарифу 1 при досягненні 1000000.00 кВар
#define UCM_EV_CRSSZERO_EXP_VARe2          0xAD //Перехід через 0 лічильника реактивної негативної експортної енергії по тарифу 2 при досягненні 1000000.00 кВар
#define UCM_EV_CRSSZERO_EXP_VARe3          0xAE //Перехід через 0 лічильника реактивної негативної експортної енергії по тарифу 3 при досягненні 1000000.00 кВар
#define UCM_EV_CRSSZERO_EXP_VARe4          0xAF //Перехід через 0 лічильника реактивної негативної експортної енергії по тарифу 4 при досягненні 1000000.00 кВар

#define UCM_EV_EM_MAGNETIC_ON               0xB0 //Виявлено наявність впливу змінного магнітного поля
#define UCM_EV_EM_MAGNETIC_OFF              0xB1 //Виявлено закінчення впливу змінного магнітного поля

///секція кодів що є похідними від МТХ1+0х100
#define UCM_EV_ENERGY_REG_FAULT            0x101 // Значення накопичувальних регістрів втрачено

#define UCM_EV_4K_POWERSALDO_OK            0x150 // Перевищення потужності в режимі кредиту відсутнє
#define UCM_EV_4K_POWERSALDO_OVER          0x151 // Перевищена потужність в режимі кредиту
#define UCM_EV_4K_POWER_GOOD_DIO           0x17E //Відсутній сигнал POWER_GOOD

#define UCM_EV_4K_CURRENT_UNEQUIL_FAULT    0x1B2 // Виявлено нерівність струмів
#define UCM_EV_4K_CURRENT_UNEQUIL_OK       0x1B3 // Закінчення періоду нерівності струмів
#define UCM_EV_4K_BIPOLAR_POWER_FAULT      0x1B4 // Виявлені різнополярні потужності у фазі та нейтралі
#define UCM_EV_4K_BIPOLAR_POWER_OK         0x1B5 // Закінчення періоду різнополярних потужностей у фазі та нейтралі
#define UCM_EV_4K_RESET_EM_FLAG            0x1B6 // Скинуто екран електромагнітного впливу
#define UCM_EV_4K_RESET_MAGN_FLAG          0x1B7 // Скинуто екран магнітного впливу

///Global section                         +0x200
//BUS
#define UCM_EV_BUS_I2C_FAULT                0x200 //


//Meter STATE OK
#define UCM_EV_MSTATE_DIAGNOSTIC_OK         0x300 //
#define UCM_EV_MSTATE_POWERON               0x301 //
#define UCM_EV_MSTATE_POWEROFF              0x302 //
#define UCM_EV_SIMPLE_PROGRAM_ERROR         0x303// помилка без перезапуску програми
#define UCM_EV_FATAL_PROGRAM_ERROR          0x304// помилка з перезапуском програми
#define UCM_EV_ILLEGAL_OPERATION            0x305// недопустима операція


#define UCM_EV_RELAY_2_ON                   0x400 //реле 2 увімкнено
#define UCM_EV_RELAY_2_OFF                  0x401 //реле 2 вимкнено


#define UCM_EV_CLEAR_LOADPROFILEDATA        0x500 //Очистка профілю навантаження
#define UCM_EV_CUMULATION_AND_RESET         0x501 //кумуляція і обнулення
#define UCM_EV_DAY_P_MAXIMUM_RESET          0x502 //обнулення добових максимальних потужностей
#define UCM_EV_MONTH_P_MAXIMUM_RESET        0x503 //обнулення місячних максимальних потужностей
#define UCM_EV_DAY_P_RESET                  0x504 //обнулення добових потужностей
#define UCM_EV_MONTH_P_RESET                0x505 //обнулення місячних потужностей


//Події що фіксуюють лише початок зміни стану
///voltage section
#define UCM_EV_VA_MIN_UNDER_SS              0x600 //Напруга L1 нижче порогу мінімальної напруги, не очікую на нормальну
#define UCM_EV_VB_MIN_UNDER_SS              0x601 //Напруга L2 нижче порогу мінімальної напруги, не очікую на нормальну
#define UCM_EV_VC_MIN_UNDER_SS              0x602 //Напруга L3 нижче порогу мінімальної напруги, не очікую на нормальну

#define UCM_EV_VA_MAX_OVER_SS               0x603 //напруга L1 вище порогу максимальної напруги, не очікую на нормальну
#define UCM_EV_VB_MAX_OVER_SS               0x604 //напруга L2 вище порогу максимальної напруги, не очікую на нормальну
#define UCM_EV_VC_MAX_OVER_SS               0x605 //напруга L3 вище порогу максимальної напруги, не очікую на нормальну
#define UCM_EV_CASE_OPEN_SS                 0x606 //Корпус лічильника відкритий, не очікую на закриття
#define UCM_EV_CASE_KLEMA_OPEN_SS           0x607 //Клемна коробка лічильника відкрита, не очікую на закриття

#define UCM_EV_VA_ON_SS                     0x608 //Є напруга L1
#define UCM_EV_VB_ON_SS                     0x609 //Є напруга L2
#define UCM_EV_VC_ON_SS                     0x60A //Є напруга L3

#define UCM_EV_VA_OFF_SS                    0x60B //Відсутня напруга L1
#define UCM_EV_VB_OFF_SS                    0x60C //Відсутня напруга L2
#define UCM_EV_VC_OFF_SS                    0x60D //Відсутня напруга L3

#define UCM_EV_WRONG_PHASE_SS               0x60E //Зворотня послідовність фаз

///load section
#define UCM_EV_P_MAX_OVER_SS                0x660 //Потужність вище максимальної



///sensors section
#define UCM_EV_MAGNETIC_ON_SS               0x712 //виявлено наявність впливу постійного магнітного поля, не очікую на закінчення впливу

///climate section
#define UCM_EV_T_NNORMAL_SS                 0x813 //Температура перевищує поріг допустимих значень, не очікую на нормалізацію

///accsess & params
#define UCM_EV_ACCESS_LOCKED_SS             0x914 //доступ закритий із-за помилки ключа доступу,не очікую на відкриття
#define UCM_EV_MAGIC_BUTTON_PRESSED_SS      0x915 //натиснута кнопка, дозволу запису параметрів



//set new meter settings
#define UCM_EV_SET_PARAM                    0x1000 // узагальнена подія змін параметрів лічильника, параметризація.


///[HELLO DLMS]

///STANDARD EVENT LOG
//RESET AND SUPPLY DIRUPTIONS   +0x2000
#define UCM_EV_STANDARD_RESET_WTH_DATA_LOSS             0x2001
#define UCM_EV_STANDARD_RESET_WTHOUT_DATA_LOSS          0x2002
//#define UCM_EV_STANDARD_POWER_OUTAGES                   UCM_EV_MSTATE_POWEROFF //0x2003
#define UCM_EV_STANDARD_NO_CONNECTION_2_N               0x2004



//CHANGE OF PARAMETERS
#define UCM_EV_STANDARD_CHNG_PRMTRS_REGISTER            0x201E //30
#define UCM_EV_STANDARD_CHNG_PRMTRS_COMM_PORT           0x201F //31
//#define UCM_EV_STANDARD_CHNG_PASSWD_2_READ              UCM_EV_CHANGE_ACCESS_KEY_3// 0x2020 //32
//#define UCM_EV_STANDARD_CHNG_PASSWD_4_PRM               UCM_EV_CHANGE_ACCESS_KEY_2//  0x2021 //33
//#define UCM_EV_STANDARD_CHNG_PASSWD_4_FIRMWR            UCM_EV_CHANGE_ACCESS_KEY_1//  0x2022 //34

//#define UCM_EV_STANDARD_CHNG_TIME_OF_SSN_TRANSTN        UCM_EV_CHANGE_COR_TIME// 0x2024 //36
#define UCM_EV_STANDARD_CHNG_MIN_TIME_BTWN_INVOICN      0x2025 //37
#define UCM_EV_STANDARD_CHNG_PRD_4_LOADPROFILE          0x2026 //38
#define UCM_EV_STANDARD_CHNG_SYNC                       0x2027 //39
#define UCM_EV_STANDARD_CHNG_PROGRAM_NAME               0x2028 //40


#define UCM_EV_ACIN1_NO_VOLTAGE                    0x2032//50 - ACIN1 state changed to on
#define UCM_EV_ACIN1_VOLTAGE                       0x2033//51 - ACIN1 state changed to off
#define UCM_EV_ACIN2_NO_VOLTAGE                    0x2034//52 - ACIN2 state changed to on
#define UCM_EV_ACIN2_VOLTAGE                       0x2035//53 - ACIN2 changed to off

//CHANGING PARAMETERS OF PWR QUALITY SETT
#define UCM_EV_STANDARD_CHNG_DURTN_OF_VLTG_SGS_SWLS     0x205A //90
#define UCM_EV_STANDARD_CHNG_LIMIT_OF_PWR_OUTAGE        0x205B //91
#define UCM_EV_STANDARD_CHNG_RATED_VOLTAGE              0x205C //92
#define UCM_EV_STANDARD_CHNG_UPPR_LIMIT_PERMSSBL_VLTG   0x205D //93
#define UCM_EV_STANDARD_CHNG_LOWR_LIMIT_PERMSSBL_VLTG   0x205E //94
#define UCM_EV_STANDARD_CHNG_LACK_OF_VTG                0x205F //95

// RESET
#define UCM_EV_STANDARD_PASSWRD_RESET                   0x2060 //96
#define UCM_EV_STANDARD_ALL_2_FACTORY_SETT              0x2061 //97


///FIRMWARE UPDATE LOG
//UPDATE
#define UCM_EV_FIRMWARE_CHNG_PROGRAM                    0x2501


///CLOCK SYNC LOG
//CLOCK SYNC
#define UCM_EV_CLOCK_SYNC                               0x2601




///POWER FAILURE (OUTAGES)
//POWER OUTAGES AND RECOVERY
//#define UCM_EV_PWR_OUTAGE_OUTG_IN_ALL_PHASES            UCM_EV_MSTATE_POWEROFF// 0x2701
//#define UCM_EV_PWR_OUTAGE_OUTG_L1                       UCM_EV_POWER_A_OFF// 0x2702
//#define UCM_EV_PWR_OUTAGE_OUTG_L2                       UCM_EV_POWER_B_OFF// 0x2703
//#define UCM_EV_PWR_OUTAGE_OUTG_L3                       UCM_EV_POWER_C_OFF// 0x2704
//#define UCM_EV_PWR_OUTAGE_RTRN_IN_ALL_PHASES            UCM_EV_MSTATE_POWERON// 0x2705
//#define UCM_EV_PWR_OUTAGE_RTRN_L1                       UCM_EV_POWER_A_ON// 0x2706
//#define UCM_EV_PWR_OUTAGE_RTRN_L2                       UCM_EV_POWER_B_ON// 0x2707
//#define UCM_EV_PWR_OUTAGE_RTRN_L3                       UCM_EV_POWER_C_ON// 0x2708


///POWER QUALITY
//VOLTAGE SAGS AND SWELLS



#define UCM_EV_PWR_QLT_VLTG_SAG_10_UN_L1               0x2901
#define UCM_EV_PWR_QLT_VLTG_SAG_10_UN_L2               0x2902
#define UCM_EV_PWR_QLT_VLTG_SAG_10_UN_L3               0x2903
#define UCM_EV_PWR_QLT_VLTG_SAG_20_UN_L1               0x2904
#define UCM_EV_PWR_QLT_VLTG_SAG_20_UN_L2               0x2905
#define UCM_EV_PWR_QLT_VLTG_SAG_20_UN_L3               0x2906
#define UCM_EV_PWR_QLT_VLTG_SAG_50_UN_L1               0x2907
#define UCM_EV_PWR_QLT_VLTG_SAG_50_UN_L2               0x2908
#define UCM_EV_PWR_QLT_VLTG_SAG_50_UN_L3               0x2909
#define UCM_EV_PWR_QLT_VLTG_SWLL_10_UN_L1              0x290A
#define UCM_EV_PWR_QLT_VLTG_SWLL_10_UN_L2              0x290B
#define UCM_EV_PWR_QLT_VLTG_SWLL_10_UN_L3              0x290C
#define UCM_EV_PWR_QLT_VLTG_RTRN_2_ACCPTBL_VALUE_L1    0x290D
#define UCM_EV_PWR_QLT_VLTG_RTRN_2_ACCPTBL_VALUE_L2    0x290E
#define UCM_EV_PWR_QLT_VLTG_RTRN_2_ACCPTBL_VALUE_L3    0x290F

#define UCM_EV_IN_MAX_OVER                              0x2917 //23 Over current in neutral
#define UCM_EV_IN_MAX_OK                                0x291B //27 Current I1 returned to an acceptable value

#define UCM_EV_PWR_QLT_VLTG_VSS_RCURR_ON               0x291E//30 Revers current detected at least in one of phases
#define UCM_EV_PWR_QLT_VLTG_VSS_RCURR_OFF              0x291F//31 No reverse current

///FRAUD DETECTION
//Theft events register
//#define UCM_EV_THFT_OPENING_COVER                       UCM_EV_CASE_OPEN//  0x3101
//#define UCM_EV_THFT_CLOSING_COVER                       UCM_EV_CASE_CLOSE// 0x3102
//#define UCM_EV_THFT_DETECTON_MAGNETIC_FIELD             UCM_EV_MAGNETIC_ON// 0x3103
//#define UCM_EV_THFT_DISAPPRNCE_OF_MAGNETIC_FIELD        UCM_EV_MAGNETIC_OFF//    0x3104
#define UCM_EV_THFT_DTCTN_OF_PHASE_AND_ZERO_RPLC_BEG    0x3105
#define UCM_EV_THFT_DTCTN_OF_PHASE_AND_ZERO_RPLC_END    0x3106
//#define UCM_EV_THFT_REMOVNG_COVER_OF_TRMNL_STRIP        UCM_EV_CASE_KLEMA_OPEN//  0x3107
//#define UCM_EV_THFT_CLOSING_COVER_OF_TRMNL_STRIP        UCM_EV_CASE_KLEMA_CLOSE//    0x3108
//#define UCM_EV_THFT_DIFF_CURRENT_DETECTION_BEG          UCM_EV_IDIFF_OVER//    0x3109
//#define UCM_EV_THFT_DIFF_CURRENT_DETECTION_END          UCM_EV_IDIFF_OK//    0x310A

#define UCM_EV_THFT_KOPEN_SUS_ON                        0x3157//87 terminal cover opened in security suspend mode





///COMMUNICATION IFACE
//COMMUNICATION EVENTS
#define UCM_EV_COMM_IFACE_PLC_PORT_END                 0x3301
#define UCM_EV_COMM_IFACE_PLC_PORT_BEG                 0x3302
#define UCM_EV_COMM_IFACE_OPT_PORT_END                 0x3303
#define UCM_EV_COMM_IFACE_OPT_PORT_BEG                 0x3304
#define UCM_EV_COMM_IFACE_ETH_PORT_END                 0x3305
#define UCM_EV_COMM_IFACE_ETH_PORT_BEG                 0x3306
#define UCM_EV_COMM_IFACE_3GPP_PORT_END                0x3307
#define UCM_EV_COMM_IFACE_3GPP_PORT_BEG                0x3308




//changed sett with text tag   <code><space><txt tag>

#define UCM_EV_SET_PARAM_TXT               0x51000 //


//event_code for meters
#define ZBR_EVENT_OTHER                     1
#define ZBR_EVENT_METER_OPEN                2//
#define ZBR_EVENT_KLEMA_OPEN                3
#define ZBR_EVENT_BAT_LOW                   4//RTC bat
#define ZBR_EVENT_EEPROM_ERROR              5//CE102v10
#define ZBR_EVENT_MAGNET                    6//magnetic field
#define ZBR_EVENT_HARDWARE                  7//i2c or etc.
#define ZBR_EVENT_ACCESS                    8//only if denied
#define ZBR_EVENT_RELAY                     9 //Load ON, Load Off
#define ZBR_EVENT_INTERNAL_ERROR            10
#define ZBR_EVENT_DATETIME_CORRECTED        11// Before: DT. After: DT or Error
#define ZBR_EVENT_DATETIME_NOT_CORRECT      12// Before: DT. After: DT or Error

#define ZBR_EVENT_UNKNOWN_PROTOCOL          13//protocol not supported or not ready to use
#define ZBR_EVENT_NO_EVNT_4_THIS_DATE       14//


#define ZBR_EVENT_VOLTAGE_PARAM             16//параметри напруги, - U,I,P,Q - менше/більше норми, відновлення нормального стану після менше/більше норми
#define ZBR_EVENT_CHANGED_PARAM             17//зміна параметрів шляхом запису
#define ZBR_EVENT_CLIMAT                    18//події пов'язані з виміром величин, що описують стан клімату (температура, тиск, вологість)
#define ZBR_EVENT_METERING_EV               19//події пов'язані з виміром ел. величин, по яким виконується облік енергії
#define ZBR_EVENT_DST_STATE_CHANGED         20//зміна значення регістру, що відповідає за Літній/Нормальний час
#define ZBR_EVENT_METER_ONOFF               21//подія увімкнення/вимкнення лічильника, пропадання напруги на всіх фазах
#define ZBR_EVENT_PARAM_FAULT               22//збій параметрів, або регістрів, втрата даних
#define ZBR_EVENT_BILLING                   23//події пов'язані з оплатою рахунку, поповнення, списання
#define ZBR_EVENT_MODULE_OPEN               24//відкрито відсік для зовнішнього модуля


#define ZBR_EVENT_MAX_EVNT_CODE             56
#define ZBR_EVENT_DAY_DONE                  255//Ставиться в кінці таблиці в кінці дня по локальному часі (означа що за вказаний день в локальному часі всі дані зібрано, не може бути за поточний день)

//event4matilda
#define ZBR_EVENT_DATETIME_NEED2CORR        51// Before: DT. After: DT or Error
#define ZBR_EVENT_DATETIME_NOT_CORR         52// Before: DT. After: DT or Error
#define ZBR_EVENT_DATETIME_CORR_DONE        53// Before: DT. After: DT or Error
#define ZBR_EVENT_INVALID_ARGUMENT          54//

#define ZBR_EVENT_OTHER_4_MTDLD             55



#endif // UCMETEREVENTCODES_H
