#include <Arduino.h>

class HallSensor
{
    private:
        short xCoordinate;
        short yCoordinate;
        float value;
        float mm;
        short minValue;
        short maxValue;
        short multiplexerNumber;
        short multiplexerCode;


    public:
        HallSensor(short xCoordinate, short yCoordinate, short multiplexerNumber, short multiplexerCode)
        {
            setXCoordinate(xCoordinate);
            setYCoordinate(yCoordinate);
            setMultiplexerNumber(multiplexerNumber);
            setMultiplexerCode(multiplexerCode);
            setMinValue(511);
            setMaxValue(0);
        }

        short transformRawValue(short rawValue)
        {
            return abs(map(rawValue, 0, 1023, -511, 511));
        }

        void setXCoordinate(short xCoordinate)
        {
            this->xCoordinate = xCoordinate;
        }

        short getXCoordinate()
        {
            return this->xCoordinate;
        }

        void setYCoordinate(short yCoordinate)
        {
            this->yCoordinate = yCoordinate;
        }

        short getYCoordinate()
        {
            return this->yCoordinate;
        }

        void setMinValue(short minValue)
        {
            if(minValue >= 0 && minValue <= 511)
            {
                this->minValue = minValue;
            }
        }

        short getMinValue()
        {
            return this->minValue;
        }

        void setMaxValue(short maxValue)
        {
            if(maxValue >= 0 && maxValue <= 511)
            {
                this->maxValue = maxValue;
            }
        }

        short getMaxValue()
        {
            return this->maxValue;
        }

        void setMM(short rawValue8mm)
        {
            setValue(rawValue8mm);
            this->mm = (getMaxValue() - getValue()) / 8;
        }

        float getMM()
        {
            return this->mm;
        }

        void setMultiplexerNumber(short multiplexerNumber)
        {
            if(multiplexerNumber >= 0 && multiplexerNumber <= 8)
            {
                this->multiplexerNumber = multiplexerNumber;
            }
        }

        short getMultiplexerNumber()
        {
            return this->multiplexerNumber;
        }

        void setMultiplexerCode(short multiplexerCode)
        {
            if(multiplexerCode >= 0 && multiplexerCode <= 1111)
            {
                this->multiplexerCode = multiplexerCode;
            }
        }

        short getMultiplexerCode()
        {
            return this->multiplexerCode;
        }

        void setValue(short rawValue)
        {
            rawValue = transformRawValue(rawValue);
            if(rawValue < getMinValue())
            {
                setMinValue(rawValue);
            }
            if(rawValue > getMaxValue())
            {
                setMaxValue(rawValue);
            }
            this->value = static_cast<float>(getMaxValue() - rawValue) / getMM();
        }

        float getValue()
        {
            return this->value;
        }
};