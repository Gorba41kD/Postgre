#include <string>
#include <iostream>
#include <pqxx/pqxx>
#include "AirLine.h"
#include"functionality.h"
#include"enum_Attribute.h"
#include"enum_Program.h"
class Person
{
private:
    std::string _name;
    std::string _surname;
public:
    std::string getName()const
    {
        return _name;
    }
    std::string getSurName()const
    {
        return _surname;
    }
    friend std::istream& operator >>(std::istream& ist, Person& person)
    {
        person._name = func::getString("first name");
        person._surname = func::getString("last name");
        return ist;
    }
};
class Passport
{
private:
    int _serialNumber;
    std::string _registration;
public:
    int getSerialNumber()const
    {
        return _serialNumber;
    }
    std::string getRegistration()const
    {
        return _registration;
    }
    friend std::istream& operator >>(std::istream& ist, Passport& passport)
    {

        do
        {
            std::cout << "Please enter your serial_number = ";
            passport._serialNumber = func::getNumber();
        } while (!func::сheckLogic(passport._serialNumber));

        passport._registration = func::getString("your registration");
        return ist;
    }
};
class Information;
class Irepository
{
public:
    virtual void save(const Information& information) = 0;
    virtual void save(const Passport& passport) = 0;
    virtual void save(const Person& person) = 0;
    virtual std::map<int, AirLine> read() = 0;
    virtual void print() = 0;
    virtual void printInformation() = 0;
    virtual std::map<int, AirLine> get() const = 0;
    virtual ~Irepository() {};
};
class Information
{
public:
    int getNumberOfAirLine()const
    {
        return _numberOfAirLine;
    }
    Information(std::unique_ptr<Irepository>& repository)
        :_repository(repository)
    {

    }
private:
    int _numberOfAirLine;
    std::unique_ptr<Irepository>& _repository;
    friend std::istream& operator >>(std::istream& ist, Information& information)
    {
        std::cout << "Which airline do you want to chose?" << std::endl;
        information._repository->print();
        auto container = information._repository->get();
        auto begin = container.begin()->second.getId();
        auto end = (--container.end())->second.getId();

        do
        {
            std::cout << "Please Enter a number of airline! = ";

            std::cin >> information._numberOfAirLine;
        } while (information._numberOfAirLine < begin || information._numberOfAirLine > end);
        std::cout << std::endl;
        return ist;
    }
};
class Repository :public Irepository
{
private:
    pqxx::connection _connection;
    pqxx::result _response;
    pqxx::nontransaction _nonTran;
    std::string _sql;
    std::map<int, AirLine> _containerAirLine;
public:
    void save(const Information& information) override
    {
        _sql = "INSERT INTO information (flight_id) VALUES (";
        _sql += std::to_string(information.getNumberOfAirLine()) + ")";
        _nonTran.exec(_sql.c_str());
    }
    void save(const Passport& passport) override
    {
        _sql = "INSERT INTO passport (serial_number,registration) VALUES (";
        _sql += std::to_string(passport.getSerialNumber()) + ", '" + passport.getRegistration() + "')";
        _nonTran.exec(_sql.c_str());
    }
    void save(const Person& person)override
    {
        _sql = "INSERT INTO person (first_name,last_name) VALUES ('";
        _sql += person.getName() + "', '" + person.getSurName() + "')";
        _nonTran.exec(_sql.c_str());
    }
    void print() override
    {
        for (auto it = _containerAirLine.begin(); it != _containerAirLine.end(); ++it)
        {
            std::cout << it->second;
        }
    }
    void printInformation() override
    {
        _response = _nonTran.exec("SELECT * FROM history");
        int j = 1;
        for (size_t i = 0; i < _response.size(); i++)
        {
            std::cout << j++ << ": " << _response[i][FIRST_DB] << " " << _response[i][SECOND_DB] << " Flight id: " << _response[i][THIRD_DB] << " AirLine: " << _response[i][FOURTH_DB]
                << " Departure airport: " << _response[i][FIFTH_DB] << " Arrival airport: " << _response[i][SIXTH_DB]
                << " Price: " << _response[i][SEVENTH_DB] << std::endl;
        }
    }
    std::map<int, AirLine> get()const override
    {
        return _containerAirLine;
    }
    std::map<int, AirLine> read()
    {
        _response = _nonTran.exec("SELECT * FROM flight");
        AirLine airLine;
        for (pqxx::result::const_iterator c = _response.begin(); c != _response.end(); ++c)
        {
            airLine.setId(c[FIRST_DB].as<int>());
            airLine.setAirLine(c[SECOND_DB].as<std::string>());
            airLine.setDepAir(c[THIRD_DB].as<std::string>());
            airLine.setArrAir(c[FOURTH_DB].as<std::string>());
            airLine.setDepDate(c[FIFTH_DB].as<std::string>());
            airLine.setDepTime(c[SIXTH_DB].as<std::string>());
            airLine.setPrice(c[SEVENTH_DB].as<double>());
            _containerAirLine.emplace(airLine.getId(), airLine);
        }
        return _containerAirLine;
    }
    Repository(const char* connectionString = "host=localhost port=5432 dbname=program user=postgres password =12081962")
        : _connection(connectionString), _nonTran(_connection)
    {

    }
    ~Repository()
    {
        _connection.close();
    }
};
void secondAttempt();
void printGuest();

int main()
{
    secondAttempt();

    return 0;
}

void printGuest()
{
    std::map<int, AirLine> mapOfAirlines;
    int value = 0;
    std::unique_ptr<Irepository> repository = std::make_unique<Repository>();
    do
    {
        std::cout << "Please Enter 1 to see all list or 2 to add a new passage or 3 to quit the program" << std::endl;
        value = func::getNumber();
        system("cls");
        mapOfAirlines = repository->read();
        if (value == FIRST_CASE)
        {
            repository->print();
        }
        if (value == SECOND_CASE)
        {
            Passport passport;
            std::cin >> passport;
            Person person;
            std::cin >> person;
            Information information(repository);
            std::cin >> information;
            repository->save(passport);
            repository->save(person);
            repository->save(information);

        }
        if (value == THIRD_CASE)
        {
            std::cout << "You quit the program" << std::endl;
            return;
        }
    } while (true);
}
void secondAttempt()
{
    std::string login;
    bool success = false;
    std::string password;
    do
    {
        system("cls");
        std::cout << "Enter login = guest: " << std::endl;
        std::cin >> login;
        std::cout << std::endl;
        std::cout << "Enter password = quest: " << std::endl;
        std::cin >> password;
        std::cout << std::endl;
        if (login == "guest" && password == "guest")
        {
            success = true;
            std::cout << "Welcome " << login << "!!!" << std::endl << std::endl;
            try
            {
                printGuest();
            }
            catch (const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
            catch (...)
            {
                std::cerr << "catch (...)" << std::endl;
            }
        }
        else if (login == "admin" && password == "1111")
        {
            success = true;
            std::cout << "Welcome " << login << "!!!" << std::endl << std::endl;

            try
            {
                std::unique_ptr<Irepository> repository = std::make_unique<Repository>();
                repository->printInformation();
            }
            catch (const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
            catch (...)
            {
                std::cerr << "catch (...)" << std::endl;
            }
        }
    } while (!success);
}
