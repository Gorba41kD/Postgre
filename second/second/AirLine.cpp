#include "AirLine.h"
void AirLine::setId(int id)
{
    _id = id;
}
void AirLine::setAirLine(std::string airLine)
{
    _airLine = airLine;
}
void AirLine::setDepAir(std::string depAir)
{
    _depAir = depAir;
}
void AirLine::setArrAir(std::string arrAir)
{
    _arrAir = arrAir;
}
void AirLine::setDepDate(std::string depDate)
{
    _depDate = depDate;
}
void AirLine::setDepTime(std::string depTime)
{
    _depTime = depTime;
}
void AirLine::setPrice(double price)
{
    _price = price;
}
int AirLine::getId() const
{
    return _id;
}

std::ostream& operator<<(std::ostream& ost,const AirLine& airLine)
{
    ost << "Id: " << airLine._id << " Airline: " << airLine._airLine << " Depaurture airport: " << airLine._depAir << " Arrival airport: " << airLine._arrAir << " Departure date: " << airLine._depDate << " Departure time: "
        << airLine._depTime << " Price: " << airLine._price << std::endl;
    return ost;
}
