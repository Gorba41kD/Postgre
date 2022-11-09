#pragma once
#include<string>
#include<iostream>
class AirLine
{
public:
    void setId(int id);
    void setAirLine(std::string airLine);
    void setDepAir(std::string depAir);
    void setArrAir(std::string arrAir);
    void setDepDate(std::string depDate);
    void setDepTime(std::string depTime);
    void setPrice(double price);
    int getId() const;
private:
    int _id;
    std::string _airLine;
    std::string _depAir;
    std::string _arrAir;
    std::string _depDate;
    std::string _depTime;
    double _price;
    friend std::ostream& operator << (std::ostream& ost,const AirLine& airLine);
};

