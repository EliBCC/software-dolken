#pragma once
#include <common/board.hpp>
#include <modm/platform.hpp>
#include <modm/platform/gpio/base.hpp>

#include <modm/processing/timer.hpp>
#include <modm/processing/protothread.hpp>
#include <modm/processing/resumable.hpp>
#include <modm/debug/logger.hpp>

// Set the log level
#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::WARNING


    /*
    * RS485_DI PA2 Tx
    * RS485_R0 PA3 Rx
    * RS485_DE PA15 direction
    */

#define _UART_ modm::platform::Usart2
#define _UART_TX_ modm::platform::GpioA2::Tx
#define _UART_RX_ modm::platform::GpioA3::Rx
#define _UART_DIR_ modm::platform::GpioA15
#define _UART_BAUD_ 115200
namespace rs485
{
    enum class Direction : uint8_t
    {
        Receive = (uint8_t)modm::platform::Gpio::InputType::PullDown,
        Transmit = (uint8_t)modm::platform::Gpio::InputType::PullUp,
    };

    enum class SensorCMD : uint8_t
    {
        LOAD = 0x0,
        T_N2O = 0x1,
        TC_IPA = 0x8,
        TC_N2O = 0x9,
        TC_1 = 0xA,
        TC_2 = 0xB,
        TC_3 = 0xC,
        TC_4 = 0xD,
        TC_5 = 0xE,
        TC_6 = 0xF,
        PT_N2 = 0x10,
        PT_IPA = 0x11,
        PT_N2O = 0x12,
        PT_FUEL = 0x13,
        PT_OX = 0x14,
        PT_CHAM = 0x15,
        BATTERY = 0x16,
        STACKHEALTH = 0x17,
        T_IPA = 0x18,
    };

    enum class ValveCMD : uint8_t
    {
        SN_N2O_FILL = 0x2,
        SN_FLUSH = 0x3,
        SV_IPA = 0x4,
        SV_N2O = 0x5,
        MV_IPA = 0x6,
        MV_N2O = 0x7,
    };

    enum class SystemCMD : uint8_t
    {
        STA = 0xC8,
        TIME = 0xC9,
    };

    struct Command_t
    {
        uint8_t id;
        uint8_t data[12];
        uint8_t count;
    };

    class RingBuffer
    {
    private:
        uint16_t buffer_size = 1024;
        int8_t buffer[1024];
        uint16_t count = 0;
        uint16_t front = 0;
        
    public:
        bool insert(uint8_t byte)
        {   
            if(count < buffer_size)
            {
                uint16_t pos = (front + count) % buffer_size;
                buffer[pos] = byte;
                count++;   
                return true;
            }
            else
                return false;
        }

        bool IsEmpty(){
            return count == 0;
        }

        bool IsFull()
        {
            return count == 1024;
        }

        uint8_t Pop()
        {
            uint8_t front = buffer[front];
            front = (front+1) % buffer_size;
            count--;
            return front;
        }
        uint8_t Peek(uint8_t pos = 0)
        {
            return buffer[(front+pos) % buffer_size];
        }

        uint16_t GetCount()
        {
            return count;
        }
    };


    class Rs485 : protected modm::NestedResumable<1>
    {
    private:
        RingBuffer input_buffer;
        uint8_t get_cmd_size;
        bool ValidateCommand()
        {
            uint8_t cmd = input_buffer.Peek(2);
            if(cmd <= 0x18 || cmd == 0xC8 || cmd ==0xC9)
                return true;
            return false;
        }

        Command_t PopCommand(uint8_t size)
        {
            input_buffer.Pop();
            input_buffer.Pop();
            Command_t cmd;
            cmd.id = input_buffer.Pop();
            for(uint8_t i = 0; i < size; i++)
                cmd.data[i] = input_buffer.Pop();
            cmd.count = size;

            input_buffer.Pop();
            input_buffer.Pop();
            input_buffer.Pop();
            return cmd;
        }

    public:
        Rs485()
        {

            MODM_LOG_INFO.printf("rs485: Setting up uart\n");
            _UART_::connect<_UART_TX_, _UART_RX_>();
            _UART_::initialize<ClockConfiguration, _UART_BAUD_>(); 
            _UART_DIR_::setInput(modm::platform::Gpio::InputType::PullUp);
        }


        modm::ResumableResult<bool> ReadToBuffer()
        {
            RF_BEGIN();

            MODM_LOG_INFO.printf("rs485: Reading from buffer\n");
            _UART_DIR_::setInput((modm::platform::Gpio::InputType) Direction::Receive);
            static uint8_t v;
            while(!input_buffer.IsFull() &&_UART_::read(v)){

            MODM_LOG_INFO.printf("rs485: Read %u\n", v);
                input_buffer.insert(v);
            }
            
            RF_END_RETURN(!input_buffer.IsFull());
        }

        /*
        *
        */
        modm::ResumableResult<bool> GetCommand(Command_t& ref)
        {
            RF_BEGIN();

            MODM_LOG_INFO.printf("rs485: GetCommand\n");
            static uint8_t size = 0;
            while(true)
            {
                if(input_buffer.GetCount() > 3){

                    MODM_LOG_INFO.printf("rs485: GetCommand buffer size less than 3\n");
                    RF_RETURN(false);
                }

                if(input_buffer.Peek(0) != 0xFF || input_buffer.Peek(1) != 0x01)
                {

                    MODM_LOG_INFO.printf("rs485: GetCommand popping front, invalid start bytes\n");
                    input_buffer.Pop();
                    continue;
                }

                if(!ValidateCommand()){
                    MODM_LOG_WARNING.printf("rs485: GetCommand invalid command byte: %u\n", input_buffer.Peek(2));
                    input_buffer.Pop();
                    continue;
                }
                static uint8_t cmd = input_buffer.Peek(2);

                if(cmd == (uint8_t) SystemCMD::STA || cmd == (uint8_t) SensorCMD::STACKHEALTH)
                    get_cmd_size = 1;
                else if(cmd == (uint8_t) SystemCMD::TIME)
                    get_cmd_size = 4;
                else 
                    size = 2;

                
                if(input_buffer.GetCount() > size + 6){
                    MODM_LOG_INFO.printf("rs485: GetCommand returning not enough bytes in buffer\n");
                    RF_RETURN(false);
                }

                if(input_buffer.Peek(size+3) != 0x01 || input_buffer.Peek(size+4) != 0xFF || input_buffer.Peek(size+5) != 0xFF)
                {

                    MODM_LOG_WARNING.printf("rs485: GetCommand invalid end command bytes\n");
                    input_buffer.Pop();
                    continue;
                }
                break;
            }
            ref = PopCommand(size);
            RF_END_RETURN(true);
        }

        modm::ResumableResult<bool> WriteCommand(Command_t cmd)
        {
            RF_BEGIN();

            MODM_LOG_INFO.printf("rs485: Writting cmd id: %u\n", cmd.id);
            _UART_DIR_::setInput((modm::platform::Gpio::InputType)Direction::Transmit);
            _UART_::write(0xFF);
            _UART_::write(0x01);
            _UART_::write(cmd.id);
            for(uint8_t i = 0; i < cmd.count; i++)
                _UART_::write(cmd.data[i]);
            _UART_::write(0x1);
            _UART_::write(0xFF);
            _UART_::write(0xFF); 

            RF_WAIT_WHILE(!_UART_::isWriteFinished());
            _UART_DIR_::setInput((modm::platform::Gpio::InputType)Direction::Receive);
            RF_END_RETURN(true);
        }

        /*
        void PerformValveCmd(Command_t cmd)
        {
            uint16_t val = *((uint16_t*) cmd.data);
            if(cmd.id == (uint8_t) ValveCMD::SN_N2O_FILL)
            {
                //Solenoid valve for oxidizer fill
                return;
            }
            else if(cmd.id == (uint8_t) ValveCMD::SN_FLUSH)
            {
                //Solenoid valve for flush
                return;
            }
            else if(cmd.id == (uint8_t) ValveCMD::SV_IPA)
            {
                //Servo valve from oxidizer tank
                return;
            }
            else if(cmd.id == (uint8_t) ValveCMD::SV_N2O)
            {
                //Servo valve from oxidizer tank
                return;
            }
            else if(cmd.id == (uint8_t) ValveCMD::MV_IPA)
            {
                //Main fuel servo valve
                return;
            }
            else if(cmd.id == (uint8_t) ValveCMD::MV_N2O)
            {
                //Main oxidizer servo valve
                return;
            }
        }


        void PerformSensorCMD(Command_t cmd)
        {
            if(cmd.id == (uint8_t) SensorCMD::LOAD || (cmd.id > 0x7 && cmd.id < 0x10))
            {
                //Signed 2 Bytes
                int16_t val = *((int16_t *) cmd.data);
                if(cmd.id == (uint8_t) SensorCMD::LOAD)
                {
                    //Load cell from test stand
                    return;
                }
                else if(cmd.id == (uint8_t) SensorCMD::TC_IPA)
                {
                    //Thermocouple in fuel tank
                    return;
                } 
                else if(cmd.id == (uint8_t) SensorCMD::T_N2O)
                {
                    //Thermocouple in oxidizer tank
                    return;
                }
                else if(cmd.id == (uint8_t) SensorCMD::TC_1)
                {
                    //Thermocouple 1 in engine
                    return;
                }
                else if(cmd.id == (uint8_t) SensorCMD::TC_2)
                {
                    //Thermocouple 2 in engine
                    return;
                }
                else if(cmd.id == (uint8_t) SensorCMD::TC_3)
                {
                    //Thermocouple 3 in engine
                    return;
                }
                else if(cmd.id == (uint8_t) SensorCMD::TC_4)
                {
                    //Thermocouple 4 in engine
                    return;
                }
                else if(cmd.id == (uint8_t) SensorCMD::TC_5)
                {
                    //Thermocouple 5 in engine
                    return;
                }
                else if(cmd.id == (uint8_t) SensorCMD::TC_6)
                {
                    //Thermocouple 6 in engine
                    return;
                }
            }
            else if(cmd.id == (uint8_t) SensorCMD::STACKHEALTH)
            {
                //unsigned 1 byte
                if(cmd.id == (uint8_t) SensorCMD::STACKHEALTH)
                {
                    //A byte where the 7 lower bits indicate OK status for Main, 
                    bool* status = (bool*)cmd.data[0];
                    return;
                }
            }
            else
            {
                //Unsigned 2 cmd.
                uint16_t val = *((uint16_t*) cmd.data);
                if(cmd.id == (uint8_t) SensorCMD::T_N2O)
                {
                    //Oxidizer tank
                    return;
                }
                else if(cmd.id == (uint8_t) SensorCMD::PT_N2)
                {
                    //Pressure transducer for high pressure tank
                    return;
                }

                else if(cmd.id == (uint8_t) SensorCMD::PT_IPA)
                {
                    //Pressure transducer for fuel tank
                    return;
                }

                else if(cmd.id == (uint8_t) SensorCMD::PT_N2O)
                {
                    //Pressure transducer for oxidizer tank
                    return;
                }

                else if(cmd.id == (uint8_t) SensorCMD::PT_FUEL)
                {
                    //Pressure transducer for fuel inject
                    return;
                }

                else if(cmd.id == (uint8_t) SensorCMD::PT_OX)
                {
                    //Pressure transducer for oxidizer inject
                    return;
                }

                else if(cmd.id == (uint8_t) SensorCMD::PT_CHAM)
                {
                    //Pressure transducer for chamber
                    return;
                }

                else if(cmd.id == (uint8_t) SensorCMD::BATTERY)
                {
                    //Battery voltage
                    return;
                }
            }
        }

        void PerformSystemCMD(Command_t cmd)
        {
            if(cmd.id == (uint8_t) SystemCMD::STA)
            {
                //State (maximum of 255 states) 
                uint8_t state = cmd.data[0]; 
                return;
            }
            else if(cmd.id == (uint8_t) SystemCMD::TIME)
            {
                //System time (up to 4294967295 ms)
                uint64_t time = *((uint64_t*)cmd.data);
                return;
            }
        }
        bool PerformCmd(Command_t cmd)
        {
            if(cmd.id > 0x1 && cmd.id < 0x8)
            {
                PerformValveCmd(cmd);
            }
            else if(cmd.id == 0 || (cmd.id > 0x7 && cmd.id < 0x19))
                PerformSensorCMD(cmd);
            else if(cmd.id == 0xC8 || cmd.id == 0xC9)
                PerformSystemCMD(cmd);
            else{
                //unknown command
                RF_RETURN(false);
            }
        }*/
    };

    

}