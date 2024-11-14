#include <iostream>
#include <string>
#include <vector>

using namespace std;

class IVehicle
{
public:
  virtual bool drive(const int kilometers) = 0;
  virtual void refuel() = 0;
  virtual void printStatus() const = 0;
};

class AbstractCar : public IVehicle
{
protected:
  AbstractCar() : m_fuelConsumptionLitresPerKm(0.0), m_fuelLevel(0.0), m_fuelLevelMax(0.0), m_milageKm(0)
  {}

public:
  bool drive(const int kilometers) override
  {
    const double requiredFuelLitres = ((double)kilometers) / 100.0 * m_fuelConsumptionLitresPerKm;
    if (requiredFuelLitres > m_fuelLevel)
    {
      cout << "Not enough fuel to drive " << kilometers << " km" << endl;
      return false;
    }
    cout << m_name << ": Drive " << kilometers << " km" << endl;
    m_milageKm += kilometers;
    m_fuelLevel -= requiredFuelLitres;
    return true;
  }

  void refuel() override
  {
    cout << "Refuel " << m_name << endl;
    m_fuelLevel = m_fuelLevelMax;
  }

  void printStatus() const override
  {
    cout << m_name << " status: Mileage(km)=" << m_milageKm << ", Fuel level(l)=" << m_fuelLevel << endl;
  }

protected:
  double m_fuelConsumptionLitresPerKm;
  double m_fuelLevel;
  double m_fuelLevelMax;
  int m_milageKm;
  string m_name;
};

class Sedan : public AbstractCar
{
public:
  Sedan()
  {
    m_fuelConsumptionLitresPerKm = 7.0;
    m_fuelLevelMax = 50.0;
    m_name = "Sedan";
  }
};

class Suv : public AbstractCar
{
public:
  Suv()
  {
    m_fuelConsumptionLitresPerKm = 11.0;
    m_fuelLevelMax = 80.0;
    m_name = "Suv";
  }
};

class Bus : public AbstractCar
{
public:
  Bus()
  {
    m_fuelConsumptionLitresPerKm = 25.0;
    m_fuelLevelMax = 120.0;
    m_name = "Bus";
  }
};

class Bicycle : public IVehicle
{
public:
  Bicycle() : m_milageKm(0), m_name("Bicycle") {}

  bool drive(const int kilometers) override
  {
    m_milageKm += kilometers;
    return true;
  }

  void refuel() override {}

  void printStatus() const override
  {
    cout << m_name << " status: Mileage(km)=" << m_milageKm << endl;
  }

private:
  int m_milageKm;
  string m_name;
};

struct RoutePoint
{
  int xKm, yKm;
  string name;

  RoutePoint(const int xKm, const int yKm, const string& name) : xKm(xKm), yKm(yKm), name(name) {}
};

class Route
{
public:
  Route() {}

  void addPoint(const RoutePoint& point)
  {
    m_path.push_back(point);
  }

  void run(IVehicle* vehicle)
  {
    if (m_path.size() < 2)
    {
      cout << "Path is incomplete" << endl;
      return;
    }

    vehicle->printStatus();

    for (size_t i = 1, t = m_path.size(); i < t; ++i)
    {
      const RoutePoint& prevPoint = m_path[i - 1];
      const RoutePoint& currPoint = m_path[i];
      const int xDistance = currPoint.xKm - prevPoint.xKm;
      const int yDistance = currPoint.yKm - prevPoint.yKm;
      const int distance = (int)sqrt(xDistance * xDistance + yDistance * yDistance);

      for (int j = 0; j < 2; ++j)
      {
        if (!vehicle->drive(distance))
        {
          if (j == 0)
          {
            vehicle->refuel();
            continue;
          }
          else
          {
            cout << "Unable to drive the rout" << endl;
            return;
          }
        }
        vehicle->printStatus();
        break;
      }
    }
  }

private:
  vector<RoutePoint> m_path;
};

int main()
{
  Route routes[2];

  routes[0].addPoint(RoutePoint(0, 0, "Point1"));
  routes[0].addPoint(RoutePoint(100, 0, "Point2"));
  routes[0].addPoint(RoutePoint(200, 100, "Point3"));
  routes[0].addPoint(RoutePoint(300, 300, "Point4"));
  routes[0].addPoint(RoutePoint(100, 400, "Point5"));
  routes[0].addPoint(RoutePoint(0, 450, "Point6"));

  routes[1].addPoint(RoutePoint(0, 0, "Moscow"));
  routes[1].addPoint(RoutePoint(0, 540, "Voronezh"));
  routes[1].addPoint(RoutePoint(420, 540, "Saratov"));

  IVehicle* vehicles[4] = {new Sedan, new Suv, new Bus, new Bicycle};
  for (int i = 0; i < 2; ++i)
  {
    cout << "Run route " << i << endl;
    for (int j = 0; j < 4; ++j)
    {
      IVehicle* vehicle = vehicles[j];
      routes[i].run(vehicle);
      cout << endl;
    }
    cout << endl;
  }
  for (int j = 0; j < 4; ++j)
    delete vehicles[j];
  return 0;
}
