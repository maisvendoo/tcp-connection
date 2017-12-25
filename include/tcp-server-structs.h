// 29 11 2017

#ifndef TCP_SERVER_STRUCTS_H
#define TCP_SERVER_STRUCTS_H

//#include    <QtGlobal>
//#include    <string.h>
//#include    <QMetaType>

//#include    "structures-display.h"
//#include    "additional.h"

//#define     DATA_BUFF_SIZE      1024


/*!
  * \enum TcpCommand
  * \brief Перечислитель команд клиентов TCP сервера
  */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//enum TcpCommand_old
//{
//    ZERO = 0x00,
//    AUTHORIZATION = 0x01,
//    GET_BLOK_DATA = 0x02,
//    GET_DISPLAY1_DATA = 0x03,
//    GET_DISPLAY2_DATA = 0x04,
//    GET_VIDEOSYSTEM_DATA = 0x05,
//    LAUNCHER_COMMAND = 0x06,
//    GET_DATA = 0x09
//};


/*!
 * \struct client_cmd_t
 * \brief Команда, принимаемая от клиента
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//#pragma pack(push, 1)
//struct client_cmd_t
//{
//    TcpCommand_old id;
//    uint64_t   data_size;
//    uint8_t    data[DATA_BUFF_SIZE];

//    client_cmd_t()
//    {
//        id = ZERO;
//        data_size = 0;
//        memset(data, 0, DATA_BUFF_SIZE);
//    }
//};
//#pragma pack(pop)


/*!
 * \struct commands_from_clients_t
 * \brief Команда, принимаемая от клиента
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//#pragma pack(push, 1)
//struct commands_from_clients_t
//{
//    /*!
//      \name Параметры, каждый бит которых отвечает за состояние соответствующего
//        элемента управления.
//      бит 0  - Главный комрессор 1
//      бит 1  - Главный комрессор 2
//      бит 2  - не занят
//      бит 3  - не занят
//      бит 4  - не занят
//      бит 5  - не занят
//      бит 6  - не занят
//      бит 7  - не занят
//      */
//    ///@{
//    bits_t<quint8>  brakes_status1_On;
//    bits_t<quint8>  brakes_status1_Off;

//    bits_t<quint8>  brakes_status2_On;
//    bits_t<quint8>  brakes_status2_Off;
//    ///@}


//    /*!
//      \name Параметры, каждый бит которых отвечает за состояние соответствующего
//        элемента управления.
//      бит 0  - Токоприёмник 1
//      бит 1  - Разъединитель переменного тока
//      бит 2  - Токоприёмник 2
//      бит 3  - ГВ 1
//      бит 4  - Разъединитель постоянного тока
//      бит 5  - ГВ 2
//      бит 6  - БВ 1
//      бит 7  - БВ 2
//      бит 8  - БУП1 1
//      бит 9  - БУП1 2
//      бит 10 - ПСН 1
//      бит 11 - ПСН 2
//      бит 12 - БУП2 1
//      бит 13 - БУП2 2
//      бит 14 - не занят
//      бит 15 - не занят
//      */
//    ///@{
//    bits_t<quint16> power_system1_On;
//    bits_t<quint16> power_system1_Off;

//    bits_t<quint16> power_system2_On;
//    bits_t<quint16> power_system2_Off;

//    bits_t<quint16> power_sys_blocked1_On;
//    bits_t<quint16> power_sys_blocked1_Off;

//    bits_t<quint16> power_sys_blocked2_On;
//    bits_t<quint16> power_sys_blocked2_Off;
//    ///@}

//    // Флаг включения режима Аудит
//    bool auditMode; ///< Флаг включения режима Аудит

//    // Флаги сброса манометров
//    bool resetManometerTMPM; ///< Флаги сброса манометров
//    bool resetManometerTC12; ///< Флаги сброса манометров

//    //
//    bool terminateModel;

//    // Конструктор
//    commands_from_clients_t()
//        : auditMode(false)
//        , resetManometerTMPM(false)
//        , resetManometerTC12(false)
//        , terminateModel(false)
//    {

//    }

//    void from_refs_ptr(ref_states_cmd_t* ref)
//    {
//        if (ref == Q_NULLPTR)
//            return;

//        brakes_status1_On.container |= ref->brakes_status1.getOnBitArray();
//        brakes_status1_Off.container |= ref->brakes_status1.getOffBitArray();

//        brakes_status2_On.container |= ref->brakes_status2.getOnBitArray();
//        brakes_status2_Off.container |= ref->brakes_status2.getOffBitArray();

//        power_system1_On.container |= ref->power_system1.getOnBitArray();
//        power_system1_Off.container |= ref->power_system1.getOffBitArray();

//        power_system2_On.container |= ref->power_system2.getOnBitArray();
//        power_system2_Off.container |= ref->power_system2.getOffBitArray();

//        power_sys_blocked1_On.container |= ref->power_sys_block1.getOnBitArray();
//        power_sys_blocked1_Off.container |= ref->power_sys_block1.getOffBitArray();

//        power_sys_blocked2_On.container |= ref->power_sys_block2.getOnBitArray();
//        power_sys_blocked2_Off.container |= ref->power_sys_block2.getOffBitArray();

//        auditMode = auditMode || ref->auditMode;

//        resetManometerTMPM = resetManometerTMPM || ref->resetManometerTMPM;
//        resetManometerTC12 = resetManometerTC12 || ref->resetManometerTC12;
//    }
//};
//#pragma pack(pop)



/*!
 * \struct data_from_train_t
 * \brief Структура данных поезда для передачи серверу
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//#pragma pack(push, 1)

//struct data_from_train_t
//{
//    double  railwayCoord;   ///< Координата
//    int     dirTrain;       ///< Направление движения
//    int     dirTravel;      ///< Направление чёт/не чёт
//    double  velocity;       ///< Скорость
//    quint16 auditVelocity;  ///< Заданная скорость (Аудит)
//    double  acceleration;   ///< Ускорение
//    double  pTM;            ///< Давление в ТМ
//    double  pUR;            ///< Давление в УР
//    double  pBC;            ///< Давление в ТЦ
//    bool    doubleSections; ///< Две секции?

//    data_from_train_t()
//        : railwayCoord(0.0)
//        , dirTrain(1)
//        , dirTravel(1)
//        , velocity(0.0)
//        , auditVelocity(0)
//        , acceleration(0.0)
//        , pTM(0.0)
//        , pUR(0.0)
//        , pBC(0.0)
//        , doubleSections(false)
//    {

//    }
//};

//#pragma pack(pop)


/*!
 * \struct data_from_traction_drive_t
 * \brief Структура данных тягового двигателя для передачи серверу
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//#pragma pack(push, 1)

//struct data_from_traction_drive_t
//{
//    int8_t tractions[4];
//    int8_t brakes[2];
//    double fullPower;

//    data_from_traction_drive_t()
//        : tractions{0, 0, 0, 0}
//        , brakes{0, 0}
//        , fullPower(0.0)

//    {

//    }

//    void setTractionLevels(double tr1, double tr2, double tr3, double tr4)
//    {
//        tractions[0] = static_cast<int8_t>(tr1);
//        tractions[1] = static_cast<int8_t>(tr2);
//        tractions[2] = static_cast<int8_t>(tr3);
//        tractions[3] = static_cast<int8_t>(tr4);
//    }

//    void setBrakeLevels(double br1, double br2)
//    {
//        brakes[0] = br1;
//        brakes[1] = br2;
//    }
//};

//#pragma pack(pop)


/*!
 * \struct data_from_panel_t
 * \brief Структура данных приборной панели для передачи серверу
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//#pragma pack(push, 1)

//struct data_from_panel_t
//{
//    uint8_t refTraction;

//    data_from_panel_t()
//        : refTraction(0)
//    {

//    }

//    void setRefTractionLevels(double tr)
//    {
//        refTraction = static_cast<uint8_t>(tr);
//    }
//};

//#pragma pack(pop)


/*!
 * \struct
 * \brief
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//#pragma pack(push, 1)

//struct data_from_compressor_t
//{
//    bool curState1;
//    bool refState1;

//    bool curState2;
//    bool refState2;

//    bool curState3;
//    bool refState3;

//    bool curState4;
//    bool refState4;

//    double pressurePM;

//    data_from_compressor_t()
//        : curState1(false)
//        , refState1(false)
//        , pressurePM(0.0)
//    {

//    }
//};

//#pragma pack(pop)


/*!
 * \struct
 * \brief
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//#pragma pack(push, 1)

//struct data_from_pantograph_t
//{
//    bool refState1;
//    bool state1;
//    bool curBlocked1;
//    bool refBlocked1;

//    bool refState2;
//    bool state2;
//    bool curBlocked2;
//    bool refBlocked2;

//    data_from_pantograph_t()
//        : refState1(false)
//        , state1(false)
//        , refState2(false)
//        , state2(false)
//    {

//    }
//};

//#pragma pack(pop)

/*!
 * \struct
 * \brief
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//#pragma pack(push, 1)

//struct data_from_GVBV_t
//{
//    // состяние ГВ
//    bool main_sw_refState1;
//    bool main_sw_state1;
//    bool main_sw_curBlocked1;
//    bool main_sw_refBlocked1;
//    bool main_sw_refState2;
//    bool main_sw_state2;
//    bool main_sw_curBlocked2;
//    bool main_sw_refBlocked2;

//    // состояние БВ
//    bool fast_sw_refState1;
//    bool fast_sw_state1;
//    bool fast_sw_curBlocked1;
//    bool fast_sw_refBlocked1;
//    bool fast_sw_refState2;
//    bool fast_sw_state2;
//    bool fast_sw_curBlocked2;
//    bool fast_sw_refBlocked2;

//    // состояние разъединителей
//    bool sw_ac_ref;
//    bool sw_ac;
//    bool sw_ac_curBlocked;
//    bool sw_ac_refBlocked;

//    bool sw_dc_ref;
//    bool sw_dc;
//    bool sw_dc_curBlocked;
//    bool sw_dc_refBlocked;

//    // Состояние БУП
//    bool tcu1_refState;
//    bool tcu1_state;
//    bool tcu1_curBlocked;
//    bool tcu1_refBlocked;

//    bool tcu2_refState;
//    bool tcu2_state;
//    bool tcu2_curBlocked;
//    bool tcu2_refBlocked;

//    bool tcu3_refState;
//    bool tcu3_state;
//    bool tcu3_curBlocked;
//    bool tcu3_refBlocked;

//    bool tcu4_refState;
//    bool tcu4_state;
//    bool tcu4_curBlocked;
//    bool tcu4_refBlocked;

//    // Род тока от УКРТ
//    int currentType;

//    // Состояние ПСН
//    bool hbu1_refState;
//    bool hbu1_state;
//    bool hbu1_curBlocked;
//    bool hbu1_refBlocked;

//    bool hbu2_refState;
//    bool hbu2_state;
//    bool hbu2_curBlocked;
//    bool hbu2_refBlocked;

//    // Напряжение КС
//    double Uks;

//    // Ток КС
//    double Iks;

//    data_from_GVBV_t()
//        : main_sw_refState1(false)
//        , main_sw_state1(false)
//        , main_sw_curBlocked1(false)
//        , main_sw_refBlocked1(false)

//        , main_sw_refState2(false)
//        , main_sw_state2(false)
//        , main_sw_curBlocked2(false)
//        , main_sw_refBlocked2(false)

//        , fast_sw_refState1(false)
//        , fast_sw_state1(false)
//        , fast_sw_curBlocked1(false)
//        , fast_sw_refBlocked1(false)

//        , fast_sw_refState2(false)
//        , fast_sw_state2(false)
//        , fast_sw_curBlocked2(false)
//        , fast_sw_refBlocked2(false)

//        , sw_ac_ref(false)
//        , sw_ac(false)
//        , sw_ac_curBlocked(false)
//        , sw_ac_refBlocked(false)

//        , sw_dc_ref(false)
//        , sw_dc(false)
//        , sw_dc_curBlocked(false)
//        , sw_dc_refBlocked(false)

//        , tcu1_refState(false)
//        , tcu1_state(false)
//        , tcu1_curBlocked(false)
//        , tcu1_refBlocked(false)

//        , tcu2_refState(false)
//        , tcu2_state(false)
//        , tcu2_curBlocked(false)
//        , tcu2_refBlocked(false)

//        , tcu3_refState(false)
//        , tcu3_state(false)
//        , tcu3_curBlocked(false)
//        , tcu3_refBlocked(false)

//        , tcu4_refState(false)
//        , tcu4_state(false)
//        , tcu4_curBlocked(false)
//        , tcu4_refBlocked(false)

//        , currentType(0)

//        , hbu1_refState(false)
//        , hbu1_state(false)
//        , hbu1_curBlocked(false)
//        , hbu1_refBlocked(false)

//        , hbu2_refState(false)
//        , hbu2_state(false)
//        , hbu2_curBlocked(false)
//        , hbu2_refBlocked(false)

//        , Uks(0.0)
//        , Iks(0.0)
//    {

//    }
//};

//#pragma pack(pop)

/*!
 * \struct
 * \brief Данные от подсистемы непрямодействующего тормоза
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//#pragma pack(push,1)

//#define TRAIN_VEHICLES_NUM 5

//struct data_from_pneumatic_brakes_t
//{
//    double pTM;
//    double pUR;
//    double pBC;

//    double train1CylPressTrolley1[TRAIN_VEHICLES_NUM];
//    double train1CylPressTrolley2[TRAIN_VEHICLES_NUM];
//    double train2CylPressTrolley1[TRAIN_VEHICLES_NUM];
//    double train2CylPressTrolley2[TRAIN_VEHICLES_NUM];

//    bool isEDBready;
//    bool isPBready;
//    bool isEmegencyBrake;
//    bool isParkingBrake;

//    data_from_pneumatic_brakes_t()
//    {
//        pTM = pUR = pBC = 0;

//        memset(train1CylPressTrolley1, 0, sizeof(double) * TRAIN_VEHICLES_NUM);
//        memset(train1CylPressTrolley2, 0, sizeof(double) * TRAIN_VEHICLES_NUM);
//        memset(train2CylPressTrolley1, 0, sizeof(double) * TRAIN_VEHICLES_NUM);
//        memset(train2CylPressTrolley2, 0, sizeof(double) * TRAIN_VEHICLES_NUM);

//        isEDBready = false;
//        isPBready = false;
//        isEmegencyBrake = false;
//        isParkingBrake = false;
//    }
//};

//#pragma pack(pop)

//------------------------------------------------------------------------------
// Структура данных передаваемых видеосистеме
//------------------------------------------------------------------------------
//#pragma pack(push, 1)

//struct videosystem_data_t
//{
//    quint64 count;           ///< Счетчик сообщений 8
//    float   railway_coord;   ///< Текущая координата (м) 12
//    float   velocity;        ///< Скорость (м/с) 16
//    float   time;            ///< Модельное время 20
//    float   deltaTime;       ///< Интервал между отправками 24
//    int     dir;             ///< Направление изменения координаты 28
//    quint32 railway_num;     ///< Номер участка 32
//    bool    spot_light_low;  ///< Прожектор тускло 33
//    bool    spot_light_high; ///< Прожектор ярко 34

//    videosystem_data_t()
//        : count(0)
//        , railway_coord(0.0)
//        , velocity(0.0)
//        , time(0.0)
//        , deltaTime(0.0)
//        , dir(0)
//        , railway_num(0)
//        , spot_light_low(false)
//        , spot_light_high(false)
//    {

//    }

//    void operator<<(const videosystem_data_t& data)
//    {
//        quint64 tmp = this->count + 1;
//        *this = data;
//        this->count = tmp;
//    }
//};

//#pragma pack(pop)


/*!
 * \struct es2g_data_t
 * \brief Структура все данных передаваемых от модели серверу
 */
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//#pragma pack(push, 1)

//struct es2g_data_t
//{
//    data_from_train_t               train;
//    data_from_traction_drive_t      tractionDrive;
//    data_from_panel_t               panel;
//    data_from_compressor_t          compressor;
//    data_from_pantograph_t          pantograph1;
//    data_from_pantograph_t          pantograph2;
//    data_from_GVBV_t                power_circuit1;
//    data_from_GVBV_t                power_circuit2;
//    data_from_pneumatic_brakes_t    pneumatic_brakes;
//    diagnostic_screen_t             diagnostic;
//    videosystem_data_t              videosystem;
//};

//#pragma pack(pop)


#endif // TCP_SERVER_STRUCTS_H
