#include <Arduino.h>

class HallSensor
{
    private:
        short xCoordinate;
        short yCoordinate;
        short value;
        short value8mm;
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
            setMinValue();
            setMaxValue();
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

        void setMinValue()
        {
            this->minValue = getSignal();
        }

        void setMinValue(short minValue)
        {
            this->minValue = minValue;
        }

        short getMinValue()
        {
            return this->minValue;
        }

        void setMaxValue()
        {
            this->maxValue = getSignal();
        }

        void setMaxValue(short maxValue)
        {
            this->maxValue = maxValue;
        }

        short getMaxValue()
        {
            return this->maxValue;
        }

        void setValue8mm()
        {
            this->value8mm = getSignal();
        }

        short getValue8mm()
        {
            return this->value8mm;
        }

        void setValue()
        {
            this->value = getSignal();
            if (getValue() < getMinValue())
            {
                setMinValue(getValue());
            }
            if (getValue() > getMaxValue())
            {
                setMaxValue(getValue());
            }
        }

        short getValue()
        {
            return this->value;
        }

        short getSignal()
        {
            for (int i = 0; i < 4; ++i)
            {
                digitalWrite(multiplexerDigitPins[i], multiplexerCode[i]);
            }
            return transformRawValue(analogRead(getAnalogPin()));
        }

        float getValueInMM()
        {
            return getValue() / ((getMaxValue() - getValue8mm()) / 8);
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