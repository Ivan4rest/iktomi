#include <Arduino.h>
#include "HallSensor.cpp"
#include "Vector.cpp"

class HallSensorMatrix
{
    private:
        static const short matrixSize {12};
        const short error = 20;

    public:

        HallSensor hallSensors[matrixSize][matrixSize];

        short indexesOfWorkingSensors[3][2] { {1, 1}, {1, 2}, {2, 1} };

        HallSensorMatrix()
        {
            setHallSensors();
        }

        short getError()
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

        bool calibrate8mmValue()
        {
            for (int i = 0; i < matrixSize; ++i)
            {
                for (int j = 0; j < matrixSize; ++j)
                {
                    hallSensors[i][j].setValue8mm();
                }
            }
            return true;
        }

        bool calibrateMinValue()
        {
            for (int i = 0; i < matrixSize; ++i)
            {
                for (int j = 0; j < matrixSize; ++j)
                {
                    hallSensors[i][j].setMinValue();
                }
            }
            return true;
        }

        bool calibrateMaxValue()
        {
            for (int i = 0; i < matrixSize; ++i)
            {
                for (int j = 0; j < matrixSize; ++j)
                {
                    hallSensors[i][j].setMaxValue();
                }
            }
            return true;
        }

        void findWorkingSensors()
        {
            float checkValue = 0;
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
                   hallSensors[i][j].setValue();
                   if ((hallSensors[i][j].getValue() > (hallSensors[i][j].getMinValue() + getError())) && (hallSensors[i][j].getValue() > checkValue))
                   {
                       checkValue = hallSensors[i][j].getValue();
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
                        hallSensors[i][j].setValue();
                        if (hallSensors[i][j].getValue() > checkValue)
                        {
                            checkValue = hallSensors[i][j].getValue();
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

        Vector getVector()
        {
            double x {0};
            double y {0};
            double z {0};

            double x1 {hallSensors[indexesOfWorkingSensors[0][0]][indexesOfWorkingSensors[0][1]].getXCoordinate()};
            double y1 {hallSensors[indexesOfWorkingSensors[0][0]][indexesOfWorkingSensors[0][1]].getYCoordinate()};
            double a {hallSensors[indexesOfWorkingSensors[0][0]][indexesOfWorkingSensors[0][1]].getValueInMM()};

            double x2 {hallSensors[indexesOfWorkingSensors[1][0]][indexesOfWorkingSensors[1][1]].getXCoordinate()};
            double y2 {hallSensors[indexesOfWorkingSensors[1][0]][indexesOfWorkingSensors[1][1]].getYCoordinate()};
            double b {hallSensors[indexesOfWorkingSensors[1][0]][indexesOfWorkingSensors[1][1]].getValueInMM()};

            double x3 {hallSensors[indexesOfWorkingSensors[2][0]][indexesOfWorkingSensors[2][1]].getXCoordinate()};
            double y3 {hallSensors[indexesOfWorkingSensors[2][0]][indexesOfWorkingSensors[2][1]].getYCoordinate()};
            double c {hallSensors[indexesOfWorkingSensors[2][0]][indexesOfWorkingSensors[2][1]].getValueInMM()};

            x = ((a*a - b*b - x1*x1 + x2*x2 - y1*y1 + y2*y2) * (y2 - y3) + (c*c - b*b + x2*x2 - x3*x3 + y2*y2 - y3*y3) * (y1 - y2)) / (2 * (x3 * (y2 - y1) + x2 * (y1 - y3) + x1 * (y3 - y2)));

            y = (c*c * (x1 - x2) + a*a * x2 - x1*x1 * x2 + x1 * x2*x2 - a*a * x3 + x1*x1 * x3 - x2*x2 * x3 - x1 * x3*x3 + x2 * x3*x3 + b*b * (-x1 + x3) - x2 * y1*y1 + x3 * y1*y1 + x1 * y2*y2 - x3 * y2*y2 - x1 * y3*y3 + x2 * y3*y3) / (2 * (x3 * (y1 - y2) + x1 * (y2 - y3) + x2 * (-y1 + y3)));

            z = sqrt(a*a - (x - x1) * (x - x1) - (y - y1) * (y - y1));

            return Vector(x, y, z);
        }
};