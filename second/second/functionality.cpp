#include<vector>
#include"AirLine.h"
#include"pqxx/pqxx"
#include"enum_Program.h"
namespace func
{
    std::string connectionString = "host=localhost port=5432 dbname= program user=postgres password =12081962";

    void printAirlines(const std::vector<AirLine>& vec)
    {
        for (int i = 0; i < vec.size(); ++i)
        {
            std::cout << vec[i];
        }
        std::cout << std::endl;
    }
    std::vector<AirLine> firstFunctionality()
    {
        pqxx::connection connectionObject(connectionString.c_str());

        pqxx::work worker(connectionObject);

        pqxx::result response = worker.exec("SELECT * FROM flight");

        std::vector<AirLine> vecOfAirlines;
        AirLine airLine;

        for (pqxx::result::const_iterator c = response.begin(); c != response.end(); ++c)
        {
            airLine.setId(c[0].as<int>());
            airLine.setAirLine(c[1].as<std::string>());
            airLine.setDepAir(c[2].as<std::string>());
            airLine.setArrAir(c[3].as<std::string>());
            airLine.setDepDate(c[4].as<std::string>());
            airLine.setDepTime(c[5].as<std::string>());
            airLine.setPrice(c[6].as<double>());
            vecOfAirlines.push_back(airLine);
        }
        return vecOfAirlines;
    }
    bool ñheckLogic(int value)
    {
        const int count = 6;
        int i = 0;
        for (value % 10; value > 0; value /= 10)
        {
            i++;
        }

        return (i == count ? true : false);
    }
    int getNumber()
    {
        int value = 0;

        std::cin >> value;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(32767, '\n');
        }
        std::cin.ignore(32767, '\n');
        return value;
    }
    std::string getString(const std::string& line)
    {
        std::string str;
        std::cout << "Please enter your " << line << " = " << std::endl;
        std::cin >> str;
        return str;
    }
    void secondFunctionality(const std::vector<AirLine>& vecOfAirlines)
    {
        std::string firstName = getString("first name");
        std::string lastName = getString("last name");

        int serialNumber = 0;
        do
        {
            std::cout << "Please enter your serial_number = ";
            serialNumber = getNumber();
        } while (!ñheckLogic(serialNumber));

        std::string registration = getString("your registration");
        int numberOfAirLine = 0;

        std::cout << "Which airline do you want to chose?" << std::endl;
        printAirlines(vecOfAirlines);
        do
        {
            std::cout << "Please Enter a number of airline! = ";

            std::cin >> numberOfAirLine;
        } while (numberOfAirLine < vecOfAirlines[0].getId() || numberOfAirLine > vecOfAirlines[vecOfAirlines.size() - 1].getId());
        std::cout << std::endl;

        pqxx::connection connectionObject(connectionString.c_str());
        pqxx::work worker(connectionObject);

        std::string passport_sql = "INSERT INTO passport (serial_number,registration) VALUES (";
        passport_sql += std::to_string(serialNumber) + ", '" + registration + "')";
        worker.exec(passport_sql.c_str());
        worker.commit();

        pqxx::nontransaction secondWork(connectionObject);
        std::string person_sql = "INSERT INTO person (first_name,last_name) VALUES ('";
        person_sql += firstName + "', '" + lastName + "')";
        secondWork.exec(person_sql.c_str());
        secondWork.commit();

        pqxx::nontransaction thirdWork(connectionObject);
        std::string information_sql = "INSERT INTO information (flight_id) VALUES (";
        information_sql += std::to_string(numberOfAirLine) + ")";
        thirdWork.exec(information_sql.c_str());
        thirdWork.commit();
    }
    void printInformation()
    {
        pqxx::connection connectionObject(connectionString.c_str());

        pqxx::work worker(connectionObject);

        pqxx::result response = worker.exec("SELECT * FROM history");
        int j = 1;
        for (size_t i = 0; i < response.size(); i++)
        {
            std::cout << j++ << ": " << response[i][0] << " " << response[i][1] << " Flight id: " << response[i][2] << " AirLine: " << response[i][3]
                << " Departure airport: " << response[i][4] << " Arrival airport: " << response[i][5]
                << " Price: " << response[i][6] << std::endl;
        }
    }
    void printGuest()
    {
        std::vector<AirLine> vecOfAirlines;
        int value = 0;

        do
        {
            std::cout << "Please Enter 1 to see all list or 2 to add a new passage or 3 to quit the program" << std::endl;
            value = getNumber();
            system("cls");

            vecOfAirlines = firstFunctionality();

            if (value == FIRST_CASE)
            {
                printAirlines(vecOfAirlines);
            }
            if (value == SECOND_CASE)
            {
                secondFunctionality(vecOfAirlines);

            }
            if (value == THIRD_CASE)
            {
                std::cout << "You quit the program" << std::endl;
                return;
            }
        } while (true);
    }
    void firstAttempt()
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
                    printInformation();
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
}