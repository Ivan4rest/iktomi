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
        short analogPin;

    public:

        short multiplexerDigitPins[4];
        bool multiplexerCode[4];

        HallSensor(){}

        HallSensor(short xCoordinate, short yCoordinate, short analogPin, short multiplexerCode)
        {
            setXCoordinate(xCoordinate);
            setYCoordinate(yCoordinate);
            setMultiplexerCode(multiplexerCode);
            setMinValue(511);
            setMaxValue(0);
            setAnalogPin(analogPin);
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
            if (minValue >= 0 && minValue <= 511)
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
            if (maxValue >= 0 && maxValue <= 511)
            {
                this->maxValue = maxValue;
            }
        }

        short getMaxValue()
        {
            return this->maxValue;
        }

        void setMM()
        {
            this->mm = (getMaxValue() - getValue()) / 8;
        }

        float getMM()
        {
            return this->mm;
        }

        void setValue(short rawValue)
        {
            rawValue = transformRawValue(rawValue);
            if (rawValue < getMinValue())
            {
                setMinValue(rawValue);
            }
            if (rawValue > getMaxValue())
            {
                setMaxValue(rawValue);
            }
            this->value = static_cast<float>(getMaxValue() - rawValue) / getMM();
        }

        float getValue()
        {
            for (int i = 0; i < 3; ++i)
            {
                digitalWrite(multiplexerDigitPins[i], multiplexerCode[i]);
            }
            setValue(analogRead(getAnalogPin()));
            return this->value;
        }

        void setAnalogPin(short analogPin)
        {
            if (analogPin >= 0 && analogPin < 9)
            {
                this->analogPin = analogPin;
            }
        }

        short getAnalogPin()
        {
            return this->analogPin;
        }

        void setMultiplexerCode(short multiplexerCode)
        {
            for (int i = 0; i < 4; ++i)
            {
                this->multiplexerCode[i] = multiplexerCode % 2;
                multiplexerCode /= 2;
            }
        }
};