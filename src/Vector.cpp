class Vector
{
    private:
        double x;
        double y;
        double z;

    public:
        Vector(){}

        Vector(double x, double y, double z)
        {
            setX(x);
            setY(y);
            setZ(z);
        }

        void setX(double x)
        {
            this->x = x;
        }

        double getX()
        {
            return this->x;
        }

        void setY(double y)
        {
            this->y = y;
        }

        double getY()
        {
            return this->y;
        }

        void setZ(double z)
        {
            this->z = z;
        }

        double getZ()
        {
            return this->z;
        }
};