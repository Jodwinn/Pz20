#include <iostream>
#include <vector>
#include <string>
#include <memory> 
using namespace std;

class Airplane {
protected:
    string model;
    double fuelCapacity;   
    double fuelConsump; 
    double maintenancecost; 

public:
    Airplane(string model, double fuelCapacity, double fuelConsump, double maintenancecost)
        : model(model), fuelCapacity(fuelCapacity), fuelConsump(fuelConsump), maintenancecost(maintenancecost) {
    }

    virtual void displayInfo() const {
        cout << "Model: " << model << endl;
        cout << "Fuel Capacity: " << fuelCapacity << " L" << endl;
        cout << "Fuel Consumption: " << fuelConsump << " L/1100 km" << endl;
        cout << "Maintenance Cost: " << maintenancecost << " $/1250 km" << endl;
    }

    virtual double calculateCost(double distance, double fuelPrice) const {
        return (distance / 100.0) * fuelConsump * fuelPrice;
    }

    double operator()(double distance, double fuelPrice) const {
        double fuelCost = calculateCost(distance, fuelPrice);
        double maintenanceCostForDistance = (distance / 1000.0) * maintenancecost;
        return fuelCost + maintenanceCostForDistance;
    }

    virtual ~Airplane() {}
};


class PassengerAirplane : public Airplane {
private:
    int passengerCap;

public:
    PassengerAirplane(string model, double fuelCapacity, double fuelConsump, double maintenancecost, int passengerCap)
        : Airplane(model, fuelCapacity, fuelConsump, maintenancecost), passengerCap(passengerCap) {
    }

    void displayInfo() const override {
        Airplane::displayInfo();
        cout << "Passenger Capacity: " << passengerCap << endl;
    }
};

class CargoAirplane : public Airplane {
private:
    double cargoCapacity;

public:
    CargoAirplane(string model, double fuelCapacity, double fuelConsump, double maintenancecost, double cargoCapacity)
        : Airplane(model, fuelCapacity, fuelConsump, maintenancecost), cargoCapacity(cargoCapacity) {
    }

    void displayInfo() const override {
        Airplane::displayInfo();
        cout << "Cargo Capacity: " << cargoCapacity << " t" << endl;
    }
};

class CrewRole {
public:
    virtual void performDuty() const = 0;
    virtual ~CrewRole() {}
};

class Pilot : public CrewRole {
public:
    void performDuty() const override {
        cout << "Pilot Ensures safe flight operation." << endl;
    }
};

class FlightAttendant : public CrewRole {
public:
    void performDuty() const override {
        cout << "Flight Attendant: Assists passengers and ensures safety during the flight." << endl;
    }
};

class Flight {
private:
    unique_ptr<Airplane> airplane;
    vector<unique_ptr<CrewRole>> crew;
    double distance; 
    double fuelPrice; 

public:
    Flight(unique_ptr<Airplane> airplane, double distance, double fuelPrice)
        : airplane(move(airplane)), distance(distance), fuelPrice(fuelPrice) {
    }

    void addCrewMemb(unique_ptr<CrewRole> member) {
        crew.push_back(move(member));
    }

    void displayFlightDetails() const {
        cout << "=== Flight Details ===" << endl;
        airplane->displayInfo();
        cout << "Distance: " << distance << " km" << endl;
        cout << "Fuel Price: " << fuelPrice << " $ per L" << endl;

        cout << "\n=== Crew Roles ===" << endl;
        for (const auto& member : crew) {
            member->performDuty();
        }

        double totalCost = (*airplane)(distance, fuelPrice);
        cout << "\nTotal Flight Cost: " << totalCost << " $" << endl;
    }
};
int main() {
    unique_ptr<Airplane> passengerPlane = make_unique<PassengerAirplane>("Boeing 849", 183380, 12.0, 800, 416);
    unique_ptr<Airplane> cargoPlane = make_unique<CargoAirplane>("Airbus A531", 139090, 10.5, 700, 70);
    Flight passengerFlight(move(passengerPlane), 1200, 1.5); 
    passengerFlight.addCrewMemb(make_unique<Pilot>());
    passengerFlight.addCrewMemb(make_unique<FlightAttendant>());

    passengerFlight.displayFlightDetails();

    cout << endl;

    Flight cargoFlight(move(cargoPlane), 800, 1.4); 
    cargoFlight.addCrewMemb(make_unique<Pilot>());

    cargoFlight.displayFlightDetails();

    return 0;
}
