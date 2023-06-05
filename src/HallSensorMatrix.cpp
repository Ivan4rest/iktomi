#include <Arduino.h>
#include "HallSensor.cpp"

class HallSensorMatrix
{
    private:
        static const short matrixSize {12};
        short indexesOfWorkingSensors[3][2] { {1, 1}, {1, 2}, {2, 1} };
        const float error = 0.1;

    public:

        HallSensor hallSensors[matrixSize][matrixSize];

        HallSensorMatrix()
        {
            setHallSensors();
        }

        float getError()
        {
            return this->error;
        }

        void setHallSensors()
        {
            short analogPin;
            short digitalPin;
            short multiplexerCode = 0;
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    analogPin = 0;
                    digitalPin = 18;
                    for (int n = i * 3; n < i * 3 + 3; ++n)
                    {
                        for (int k = j * 3; k < j * 3 + 3; ++k)
                        {
                            hallSensors[n][k] = HallSensor(n * 4 + 2, k * 4 + 2, analogPin, multiplexerCode);
                            for (int d = 0; d < 4; ++d)
                            {
                                hallSensors[n][k].multiplexerDigitPins[d] = digitalPin++;
                            }
                            analogPin++;
                        }
                    }
                    ++multiplexerCode;
                }
            }
        }

        void calibrate()
        {
            for (int i = 0; i < matrixSize; ++i)
            {
                for (int j = 0; j < matrixSize; ++j)
                {
                    hallSensors[i][j].setMM();
                }
            }
        }

        void findWorkingSensors()
        {
            float checkValue = 0;
            float currentValue = 0;
            short index0 = indexesOfWorkingSensors[0][0];
            short index1 = indexesOfWorkingSensors[0][1];

            if (index0 < 1)
                index0 = 1;
            
            if (index1 < 1)
                index1 = 1;
            
            if (index0 > matrixSize - 2)
                index0 = matrixSize - 2;
            
            if (index1 > matrixSize - 2)
                index1 = matrixSize - 2;
            
            for (int i = index0 - 1; i < index0 + 1; ++i)
            {
                for (int j = index1 - 1; j < index1 + 1; ++j)
                {
                    currentValue = hallSensors[i][j].getValue();
                    if ((currentValue > (hallSensors[i][j].getMinValue() / hallSensors[i][j].getMM() + getError())) && (currentValue > checkValue))
                    {
                        checkValue = currentValue;
                        indexesOfWorkingSensors[0][0] = i;
                        indexesOfWorkingSensors[0][1] = j;
                    }
                }
            }

            if (checkValue == 0)
            {
                for (int i = 0; i < matrixSize; ++i)
                {
                    for (int j = 0; j < matrixSize; ++j)
                    {
                        currentValue = hallSensors[i][j].getValue();
                        if ((currentValue > (hallSensors[i][j].getMinValue() / hallSensors[i][j].getMM() + getError())) && (currentValue > checkValue))
                        {
                            checkValue = currentValue;
                            indexesOfWorkingSensors[0][0] = i;
                            indexesOfWorkingSensors[0][1] = j;
                        }
                    }
                }
            }

            if (indexesOfWorkingSensors[0][0] == 0)
            {
                if (indexesOfWorkingSensors[0][1] == 0)
                {
                    indexesOfWorkingSensors[1][0] = indexesOfWorkingSensors[0][0];
                    indexesOfWorkingSensors[1][1] = indexesOfWorkingSensors[0][1] + 1;
                    indexesOfWorkingSensors[2][0] = indexesOfWorkingSensors[0][0] + 1;
                    indexesOfWorkingSensors[2][1] = indexesOfWorkingSensors[0][1];
                }
                else if (indexesOfWorkingSensors[0][1] == 11)
                {
                    indexesOfWorkingSensors[1][0] = indexesOfWorkingSensors[0][0];
                    indexesOfWorkingSensors[1][1] = indexesOfWorkingSensors[0][1] - 1;
                    indexesOfWorkingSensors[2][0] = indexesOfWorkingSensors[0][0] + 1;
                    indexesOfWorkingSensors[2][1] = indexesOfWorkingSensors[0][1];
                }
                else
                {
                    indexesOfWorkingSensors[1][0] = indexesOfWorkingSensors[0][0];
                    if (hallSensors[indexesOfWorkingSensors[0][0]][indexesOfWorkingSensors[0][1] - 1].getValue() > hallSensors[indexesOfWorkingSensors[0][0]][indexesOfWorkingSensors[0][1] + 1].getValue())
                    {
                        indexesOfWorkingSensors[1][1] = indexesOfWorkingSensors[0][1] - 1;
                    }
                    else
                    {
                        indexesOfWorkingSensors[1][1] = indexesOfWorkingSensors[0][1] + 1;
                    }
                    indexesOfWorkingSensors[2][0] = indexesOfWorkingSensors[0][0] + 1;
                    indexesOfWorkingSensors[2][1] = indexesOfWorkingSensors[0][1];
                }
            }
            else if (indexesOfWorkingSensors[0][0] == 11)
            {
                if (indexesOfWorkingSensors[0][1] == 0)
                {
                    indexesOfWorkingSensors[1][0] = indexesOfWorkingSensors[0][0];
                    indexesOfWorkingSensors[1][1] = indexesOfWorkingSensors[0][1] + 1;
                    indexesOfWorkingSensors[2][0] = indexesOfWorkingSensors[0][0] - 1;
                    indexesOfWorkingSensors[2][1] = indexesOfWorkingSensors[0][1];
                }
                else if (indexesOfWorkingSensors[0][1] == 11)
                {
                    indexesOfWorkingSensors[1][0] = indexesOfWorkingSensors[0][0];
                    indexesOfWorkingSensors[1][1] = indexesOfWorkingSensors[0][1] - 1;
                    indexesOfWorkingSensors[2][0] = indexesOfWorkingSensors[0][0] - 1;
                    indexesOfWorkingSensors[2][1] = indexesOfWorkingSensors[0][1];
                }
                else
                {
                    indexesOfWorkingSensors[1][0] = indexesOfWorkingSensors[0][0];
                    if (hallSensors[indexesOfWorkingSensors[0][0]][indexesOfWorkingSensors[0][1] - 1].getValue() > hallSensors[indexesOfWorkingSensors[0][0]][indexesOfWorkingSensors[0][1] + 1].getValue())
                    {
                        indexesOfWorkingSensors[1][1] = indexesOfWorkingSensors[0][1] - 1;
                    }
                    else
                    {
                        indexesOfWorkingSensors[1][1] = indexesOfWorkingSensors[0][1] + 1;
                    }
                    indexesOfWorkingSensors[2][0] = indexesOfWorkingSensors[0][0] - 1;
                    indexesOfWorkingSensors[2][1] = indexesOfWorkingSensors[0][1];
                }
            }
            else if (indexesOfWorkingSensors[0][1] == 0)
            {
                indexesOfWorkingSensors[1][0] = indexesOfWorkingSensors[0][0];
                indexesOfWorkingSensors[1][1] = indexesOfWorkingSensors[0][1] + 1;
                if (hallSensors[indexesOfWorkingSensors[0][0] - 1][indexesOfWorkingSensors[0][1]].getValue() > hallSensors[indexesOfWorkingSensors[0][0] + 1][indexesOfWorkingSensors[0][1]].getValue())
                {
                    indexesOfWorkingSensors[2][0] = indexesOfWorkingSensors[0][0] - 1;
                }
                else
                {
                    indexesOfWorkingSensors[2][0] = indexesOfWorkingSensors[0][0] + 1;
                }
                indexesOfWorkingSensors[2][1] = indexesOfWorkingSensors[0][1];
            }
            else if (indexesOfWorkingSensors[0][1] == 11)
            {
                indexesOfWorkingSensors[1][0] = indexesOfWorkingSensors[0][0];
                indexesOfWorkingSensors[1][1] = indexesOfWorkingSensors[0][1] - 1;
                if (hallSensors[indexesOfWorkingSensors[0][0] - 1][indexesOfWorkingSensors[0][1]].getValue() > hallSensors[indexesOfWorkingSensors[0][0] + 1][indexesOfWorkingSensors[0][1]].getValue())
                {
                    indexesOfWorkingSensors[2][0] = indexesOfWorkingSensors[0][0] - 1;
                }
                else
                {
                    indexesOfWorkingSensors[2][0] = indexesOfWorkingSensors[0][0] + 1;
                }
                indexesOfWorkingSensors[2][1] = indexesOfWorkingSensors[0][1];
            }
            else
            {
                indexesOfWorkingSensors[1][0] = indexesOfWorkingSensors[0][0];
                if (hallSensors[indexesOfWorkingSensors[0][0]][indexesOfWorkingSensors[0][1] - 1].getValue() > hallSensors[indexesOfWorkingSensors[0][0]][indexesOfWorkingSensors[0][1] + 1].getValue())
                {
                    indexesOfWorkingSensors[1][1] = indexesOfWorkingSensors[0][1] - 1;
                }
                else
                {
                    indexesOfWorkingSensors[1][1] = indexesOfWorkingSensors[0][1] + 1;
                }

                if (hallSensors[indexesOfWorkingSensors[0][0] - 1][indexesOfWorkingSensors[0][1]].getValue() > hallSensors[indexesOfWorkingSensors[0][0] + 1][indexesOfWorkingSensors[0][1]].getValue())
                {
                    indexesOfWorkingSensors[2][0] = indexesOfWorkingSensors[0][0] - 1;
                }
                else
                {
                    indexesOfWorkingSensors[2][0] = indexesOfWorkingSensors[0][0] + 1;
                }
                indexesOfWorkingSensors[2][1] = indexesOfWorkingSensors[0][1];
            }
        }
};