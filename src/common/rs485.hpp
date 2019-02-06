#pragma once
#include <common/board.hpp>
#include <modm/platform.hpp>
#include <modm/platform/gpio/base.hpp>

#include <modm/processing/timer.hpp>
#include <modm/processing/protothread.hpp>
#include <modm/processing/resumable.hpp>
#include <modm/debug/logger.hpp>
/*
* Written by Nicolai Dynweber Bruhn
*/

/*
 ********* IMPORTANT *************
  Direction might have to change. Right now it is using gpio function setOutput for
  pulldown and pullup.
*/


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

/*
* @brief Namespace for rs485 protocol
*/
namespace rs485
{   
    /**
    * @brief Enum for output on Analog pin for direction
    */
    enum class Direction : bool
    {
        Receive = false,
        Transmit = true,
    };

    /**
    * @brief Enum for sensor commands
    */
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


    /**
    * @brief Enum for Valve commands
    */
    enum class ValveCMD : uint8_t
    {
        SN_N2O_FILL = 0x2,
        SN_FLUSH = 0x3,
        SV_IPA = 0x4,
        SV_N2O = 0x5,
        MV_IPA = 0x6,
        MV_N2O = 0x7,
    };


    /**
    * @brief Enum for system commands
    */
    enum class SystemCMD : uint8_t
    {
        STA = 0xC8,
        TIME = 0xC9,
    };


    /**
    * @brief struct for command contains cmd id, data and amount of data.
    */
    struct Command_t
    {
        uint8_t id;
        uint8_t data[12];
        uint8_t count;
    };

    /**
    * brief a ringbuffer of type T and any size
    */
    template<typename T, size_t SIZE>
    class RingBuffer
    {
    private:
        uint16_t buffer_size = SIZE;
        T buffer[SIZE];
        uint16_t count = 0;
        uint16_t front = 0;
        
    public:

        /**
        * @brief inserts element into ringbuffer if it is not full
        * @param element to insert
        * @returns true if element is inserted
        */
        bool Insert(T element)
        {   
            //insert into ringbuffer if it is not full
            if(count < buffer_size)
            {
                uint16_t pos = (front + count) % buffer_size;
                buffer[pos] = element;
                count++;   
                return true;
            }
            return false;
        }

        /**
        * @brief checks if ringbuffer is empty
        * @returns true if ringbuffer is empty
        */
        bool IsEmpty(){
            return count == 0;
        }


        /**
        * @brief checks if ringbuffer is full
        * @param element to insert
        * @returns true if full
        */
        bool IsFull()
        {
            return count == buffer_size;
        }


        /**
        * @brief pops element from ringbuffer
        * @returns front element and removes it from buffer
        */
        T Pop()
        {
            T t = buffer[front];
            front = (front+1) % buffer_size;
            count--;
            return t;
        }


        /**
        * @brief peeks at an element in the ringbuffer
        * @param position to peek at
        * @returns returns element at position
        */
        T Peek(uint16_t pos = 0)
        {
            return buffer[(front+pos) % buffer_size];
        }


        /**
        * @brief cgetter for amount of elements in the buffer
        * @returns elements in buffer
        */
        uint16_t GetCount()
        {
            return count;
        }
    };


    /**
    * @brief rs485 protocol class - class
    */
    class Rs485 : protected modm::NestedResumable<1>
    {
    private:
        RingBuffer<uint8_t, 1024> input_buffer;
        uint8_t GetCommand_size;
        uint8_t GetCommand_cmd_id;
        uint8_t ReadToBuffer_value;
        /**
        * @brief validates if a valid cmd id is in the buffer
        * @returns true if valid command id is in the buffer
        */
        bool ValidateCommand()
        {
            uint8_t cmd = input_buffer.Peek(2);
            if(cmd <= 0x18 || cmd == 0xC8 || cmd ==0xC9)
                return true;
            return false;
        }


        /**
        * @brief Pops command from ringbuffer
        * @param amount of data bytes
        * @returns command
        */
        Command_t PopCommand(uint8_t size)
        {
            input_buffer.Pop(); // pop 0xFF
            input_buffer.Pop(); // pop 0x01
            Command_t cmd;
            cmd.id = input_buffer.Pop(); // pop cmd id
            for(uint8_t i = 0; i < size; i++)
                cmd.data[i] = input_buffer.Pop(); // pop data bytes
            cmd.count = size;

            input_buffer.Pop(); // pop 0x01
            input_buffer.Pop(); // pop 0xFF
            input_buffer.Pop(); // pop 0xFF
            return cmd;
        }

    public:

        /**
        * @brief initialises uart communication
        */
        Rs485()
        {

            MODM_LOG_INFO.printf("rs485: Setting up uart\n");
            _UART_::connect<_UART_TX_, _UART_RX_>();
            _UART_::initialize<ClockConfiguration, _UART_BAUD_>(); 
            _UART_DIR_::setOutput(Direction::Receive); // THIS MIGHT NEED TO CHANGE
        }


        /**
        * @brief reads all available bytes to the ringbuffer
        * @returns true if no more bytes can be read from uart connection
        */
        modm::ResumableResult<bool> ReadToBuffer()
        {
            RF_BEGIN();

            MODM_LOG_INFO.printf("rs485: Reading from buffer\n");

            // Setup direction for receive
            _UART_DIR_::setOutput( Direction::Receive); // THIS MIGHT NEED TO CHANGE

            // read all available bytes in buffer and add them to the ringbuffer
            while(!input_buffer.IsFull() &&_UART_::read(ReadToBuffer_value)){
                MODM_LOG_INFO.printf("rs485: Read %u\n", ReadToBuffer_value);
                input_buffer.Insert(ReadToBuffer_value);
            }
            
            // The while-loop can only end if ringbuffer is full or if there is no more bytes to receives. 
            // If there is no more bytes to receive then true is returned and therefor the ringbuffer is not full
            RF_END_RETURN(!input_buffer.IsFull());
        }

        /**
        * @brief gets a command from ringbuffer
        * @param reference to object to put the found command
        * @returns true if a valid command is found
        */
        modm::ResumableResult<bool> GetCommand(Command_t& ref)
        {
            RF_BEGIN();

            MODM_LOG_INFO.printf("rs485: GetCommand\n");
            while(true)
            {
                // ringbuffer must contain at least 3 bytes to validate command start message and command id.
                if(input_buffer.GetCount() > 3){

                    MODM_LOG_INFO.printf("rs485: GetCommand buffer size less than 3\n");
                    RF_RETURN(false);
                }

                //validate first two bytes in buffer is command start
                if(input_buffer.Peek(0) != 0xFF || input_buffer.Peek(1) != 0x01)
                {
                    //pop front if it is not command start.
                    MODM_LOG_INFO.printf("rs485: GetCommand popping front, invalid start bytes\n");
                    input_buffer.Pop();
                    continue;
                }

                //Validate command id in the ringbuffer at position 2
                if(!ValidateCommand()){
                    MODM_LOG_WARNING.printf("rs485: GetCommand invalid command byte: %u\n", input_buffer.Peek(2));
                    input_buffer.Pop();
                    continue;
                }
                GetCommand_cmd_id = input_buffer.Peek(2);

                // Determine number of payload bytes based on command id.
                if(GetCommand_cmd_id == (uint8_t) SystemCMD::STA || GetCommand_cmd_id == (uint8_t) SensorCMD::STACKHEALTH)
                    GetCommand_size = 1;
                else if(GetCommand_cmd_id == (uint8_t) SystemCMD::TIME)
                    GetCommand_size = 4;
                else 
                    GetCommand_size = 2;

                //Validate buffer contains the full command. must contain atleast 0xFF 0x01 cmd_id, payload bytes and 0x01 0xFF 0xFF
                if(input_buffer.GetCount() > GetCommand_size + 8){
                    MODM_LOG_INFO.printf("rs485: GetCommand returning not enough bytes in buffer\n");
                    RF_RETURN(false);
                }

                // Validate command ends in with end of command bytes.
                if(input_buffer.Peek(GetCommand_size+3) != 0x01 || input_buffer.Peek(GetCommand_size+4) != 0xFF || input_buffer.Peek(GetCommand_size+5) != 0xFF ||
                    input_buffer.Peek(GetCommand_size+6) != 0xFF || input_buffer.Peek(GetCommand_size+7) != 0xFF)
                {

                    MODM_LOG_WARNING.printf("rs485: GetCommand invalid end command bytes\n");
                    input_buffer.Pop();
                    continue;
                }
                // Break because buffer contains a valid command at front.
                break;
            }
            //Pop command from ringbuffer
            ref = PopCommand(GetCommand_size);
            RF_END_RETURN(true);
        }

        /**
        * @brief writes a command over uart
        * @param command to write
        */
        modm::ResumableResult<bool> WriteCommand(Command_t& cmd)
        {
            RF_BEGIN();

            MODM_LOG_INFO.printf("rs485: Writting cmd id: %u\n", cmd.id);
            //Set direction pin for transmission
            _UART_DIR_::setOutput(Direction::Transmit); // MIGHT NEED TO CHANGE

            // Start command bytes
            _UART_::write(0xFF);
            _UART_::write(0x01);

            // Write command id
            _UART_::write(cmd.id);

            //Write payload bytes
            for(uint8_t i = 0; i < cmd.count; i++)
                _UART_::write(cmd.data[i]);
            
            // Write end of command bytes
            _UART_::write(0x01);
            _UART_::write(0xFF);
            _UART_::write(0xFF);
            _UART_::write(0xFF);
            _UART_::write(0xFF); 
            
            //Wait until the command has been transmitted
            RF_WAIT_WHILE(!_UART_::isWriteFinished());

            //Switch back to receive mode
            _UART_DIR_::setOutput(Direction::Receive); // MIGHT NEED TO CHANGE
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