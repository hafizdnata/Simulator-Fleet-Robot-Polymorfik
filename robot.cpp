// robot.cpp
// Mini-Simulator Fleet Kendaraan Robotik
// Compile: g++ -std=c++17 robot.cpp -o fleet_sim

#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <string>
#include <iomanip>

// ---------------------- Vehicle (abstract) ----------------------
class Vehicle {
public:
    struct Position {
        double x, y, z;
        Position() : x(0), y(0), z(0) {}
        Position(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
    };
    
protected:
    std::string id;
    Position position;
    double battery; // percentage 0..100

public:
    Vehicle(const std::string& id_, Position p = Position(), double battery_ = 100.0)
        : id(id_), position(p), battery(battery_) {}
    virtual ~Vehicle() = default;

    virtual void move(double dx, double dy, double dz) = 0;
    virtual void status() const {
        std::cout << "[" << id << "] pos=("
                  << position.x << "," << position.y << "," << position.z
                  << ") battery=" << std::fixed << std::setprecision(1) << battery << "%\n";
    }

    virtual std::unique_ptr<Vehicle> clone() const = 0;

    bool consumeBattery(double amount) {
        if (amount < 0) amount = 0;
        if (battery <= 0.0) {
            battery = 0.0;
            std::cout << "[" << id << "] battery habis. Tidak bisa bergerak.\n";
            return false;
        }
        if (battery - amount <= 0.0) {
            battery = 0.0;
            std::cout << "[" << id << "] battery menurun ke 0% setelah konsumsi " << amount << "\n";
            return false;
        }
        battery -= amount;
        return true;
    }

    virtual bool obstacleAhead(double /*range*/) { return false; }

    std::string getId() const { return id; }
    Position getPos() const { return position; }
    double getBattery() const { return battery; }
};

// ---------------------- GroundRobot ----------------------
class GroundRobot : public virtual Vehicle {
protected:
    std::vector<Position> obstacles;
    double maxSlope;

public:
    GroundRobot(const std::string& id_, Position p = Position(), double battery_ = 100.0, double maxSlope_ = 0.5)
        : Vehicle(id_, p, battery_), maxSlope(maxSlope_) {
        obstacles.push_back(Position{3.0, 3.0, 0.0});
        obstacles.push_back(Position{6.0, 1.5, 0.0});
    }

    bool obstacleAhead(double range) override {
        for (const auto &o : obstacles) {
            double dist = std::hypot(o.x - position.x, o.y - position.y);
            if (dist <= range) return true;
        }
        return false;
    }

    void move(double dx, double dy, double dz) override {
        (void)dz; // ignored for ground
        double attemptDist = std::hypot(dx, dy);
        if (attemptDist <= 0.0) {
            std::cout << "[" << id << "] tidak ada pergerakan.\n";
            return;
        }

        double slope = (std::abs(dx) < 1e-6) ? std::abs(dy) : std::abs(dy / dx);
        if (slope > maxSlope) {
            std::cout << "[" << id << "] slope terlalu curam. Gerakan dibatasi.\n";
            dx *= 0.5; dy *= 0.5;
        }

        if (obstacleAhead(1.0)) {
            std::cout << "[" << id << "] obstacle terdeteksi. Gerakan ditolak.\n";
            return;
        }

        double batteryCost = attemptDist * 0.8;
        if (!consumeBattery(batteryCost)) {
            std::cout << "[" << id << "] gagal gerak karena battery tidak cukup.\n";
            return;
        }

        position.x += dx; position.y += dy;
        std::cout << "[" << id << "] ground bergerak ke (" << position.x << "," << position.y << ")\n";
    }

    // Overload: heading + distance
    void move(double headingDeg, double distance) {
        double rad = headingDeg * M_PI / 180.0;
        double dx = std::cos(rad) * distance;
        double dy = std::sin(rad) * distance;
        move(dx, dy, 0.0);
    }

    std::unique_ptr<Vehicle> clone() const override {
        return std::make_unique<GroundRobot>(*this);
    }
};

// ---------------------- AerialDrone ----------------------
class AerialDrone : public virtual Vehicle {
public:
    AerialDrone(const std::string& id_, Position p = Position(), double battery_ = 100.0)
        : Vehicle(id_, p, battery_) {}

    void move(double dx, double dy, double dz) override {
        double dist = std::sqrt(dx*dx + dy*dy + dz*dz);
        if (dist <= 0.0) {
            std::cout << "[" << id << "] tidak ada pergerakan (drone).\n";
            return;
        }

        double batteryCost = dist * 1.5;
        if (!consumeBattery(batteryCost)) {
            std::cout << "[" << id << "] battery insufisien, mencoba naik ketinggian.\n";
            if (dz > 0.0 && battery > 1.0) {
                double climb = std::min(1.0, battery / 2.0);
                position.z += climb;
                consumeBattery(1.0);
                std::cout << "[" << id << "] fallback: naik ke altitude " << position.z << "\n";
            }
            return;
        }

        position.x += dx; position.y += dy; position.z += dz;
        std::cout << "[" << id << "] drone bergerak ke (" << position.x << "," << position.y << "," << position.z << ")\n";
    }

    // Overload: move to absolute target
    void move(const Position &target) {
        double dx = target.x - position.x;
        double dy = target.y - position.y;
        double dz = target.z - position.z;
        move(dx, dy, dz);
    }

    void status() const override {
        std::cout << "[" << id << "] Drone pos=(" << position.x << "," << position.y << "," << position.z
                  << ") altitude=" << position.z
                  << " battery=" << std::fixed << std::setprecision(1) << battery << "%\n";
    }

    std::unique_ptr<Vehicle> clone() const override {
        return std::make_unique<AerialDrone>(*this);
    }
};

// ---------------------- Amphibious (multiple inheritance) ----------------------
class Amphibious : public GroundRobot, public AerialDrone {
private:
    bool inAirMode{false};

public:
    Amphibious(const std::string& id_, Position p = Position(), double battery_ = 100.0)
        : Vehicle(id_, p, battery_), GroundRobot(id_, p, battery_), AerialDrone(id_, p, battery_) {}

    void move(double dx, double dy, double dz) override {
        if (dz != 0.0) {
            inAirMode = true;
            std::cout << "[" << id << "] Amphibious switching to AIR mode.\n";
            AerialDrone::move(dx, dy, dz);
        } else {
            inAirMode = false;
            std::cout << "[" << id << "] Amphibious in GROUND mode.\n";
            GroundRobot::move(dx, dy, dz);
        }
    }

    // Overload: heading + distance (ground mode only)
    void move(double headingDeg, double distance) {
        if (inAirMode) {
            std::cout << "[" << id << "] amph di udara, heading move tidak tersedia.\n";
            return;
        }
        GroundRobot::move(headingDeg, distance);
    }

    // Overload: absolute position (air mode)
    void move(const Position &target) {
        inAirMode = true;
        std::cout << "[" << id << "] Amphibious switching to AIR mode for target.\n";
        AerialDrone::move(target);
    }

    void status() const override {
        std::cout << "[" << id << "] Amphibious pos=(" << position.x << "," << position.y << "," << position.z
                  << ") mode=" << (inAirMode ? "AIR" : "GROUND")
                  << " battery=" << std::fixed << std::setprecision(1) << battery << "%\n";
    }

    std::unique_ptr<Vehicle> clone() const override {
        return std::make_unique<Amphibious>(*this);
    }
};

// ---------------------- Fleet manager: executeMove overloads ----------------------
void executeMove(Vehicle &v, double dx, double dy, double dz) {
    v.move(dx, dy, dz);
}

void executeMove(Vehicle &v, double heading, double distance) {
    // Attempt dynamic_cast to GroundRobot or Amphibious (which supports heading overload)
    if (auto *gr = dynamic_cast<GroundRobot*>(&v)) {
        gr->move(heading, distance);
    } else if (auto *am = dynamic_cast<Amphibious*>(&v)) {
        // Amphibious has heading overload implemented
        am->move(heading, distance);
    } else {
        std::cout << "[" << v.getId() << "] tidak mendukung move(heading,distance)\n";
    }
}

void executeMove(Vehicle &v, const Vehicle::Position &target) {
    if (auto *ad = dynamic_cast<AerialDrone*>(&v)) {
        ad->move(target);
    } else if (auto *am = dynamic_cast<Amphibious*>(&v)) {
        am->move(target);
    } else {
        Vehicle::Position cur = v.getPos();
        v.move(target.x - cur.x, target.y - cur.y, 0.0);
    }
}

// ---------------------- Utilities for mission logging ----------------------
void logStatusAll(const std::vector<std::unique_ptr<Vehicle>> &fleet) {
    std::cout << "=== Fleet status ===\n";
    for (const auto &v : fleet) v->status();
    std::cout << "====================\n";
}

// ---------------------- Main: scenario & tests ----------------------
int main() {
    std::cout << "Fleet Robot Simulator (mini)\n\n";

    // Create fleet: 1 GroundRobot, 1 AerialDrone, 1 Amphibious
    std::vector<std::unique_ptr<Vehicle>> fleet;
    fleet.push_back(std::make_unique<GroundRobot>("GR1", Vehicle::Position{0,0,0}, 50.0, 1.0)); // lower battery to show drain
    fleet.push_back(std::make_unique<AerialDrone>("AD1", Vehicle::Position{0,0,5}, 80.0));
    fleet.push_back(std::make_unique<Amphibious>("AMP1", Vehicle::Position{0, -2, 0}, 90.0));

    // Initial status
    logStatusAll(fleet);

    // Waypoints mission (3 waypoints). Each vehicle attempts to traverse in order.
    std::vector<Vehicle::Position> waypoints = {
        {2.0, 1.0, 0.0},
        {5.0, 3.0, 2.0},   // includes altitude -> drone & amph can use z
        {8.0, 0.0, 0.0}
    };

    std::cout << "\n=== Start mission: " << waypoints.size() << " waypoints ===\n";
    for (size_t i = 0; i < waypoints.size(); ++i) {
        std::cout << "\n-- Waypoint " << (i+1) << ": (" << waypoints[i].x << "," << waypoints[i].y << "," << waypoints[i].z << ")\n";

        for (auto &vptr : fleet) {
            std::cout << ">>> Perintah ke " << vptr->getId() << ":\n";
            // log before
            vptr->status();

            // Decide how to call executeMove based on type:
            if (dynamic_cast<AerialDrone*>(vptr.get())) {
                // drone: move to absolute target (uses overload)
                executeMove(*vptr, waypoints[i]);
            } else if (dynamic_cast<GroundRobot*>(vptr.get())) {
                // GroundRobot: try use heading+distance if possible
                Vehicle::Position cur = vptr->getPos();
                double dx = waypoints[i].x - cur.x;
                double dy = waypoints[i].y - cur.y;
                double heading = std::atan2(dy, dx) * 180.0 / M_PI;
                double dist = std::hypot(dx, dy);
                executeMove(*vptr, heading, dist);
            } else {
                // default: dx/dy/dz
                Vehicle::Position cur = vptr->getPos();
                executeMove(*vptr, waypoints[i].x - cur.x, waypoints[i].y - cur.y, waypoints[i].z - cur.z);
            }

            // Log after
            vptr->status();
        } // end for each vehicle

        std::cout << "\n-- End waypoint " << (i+1) << "\n";
    } // end for each waypoint

    // Final status
    std::cout << "\n=== Mission complete: final status ===\n";
    logStatusAll(fleet);

    // Tests for specific required cases:
    std::cout << "\n=== Additional minimal tests ===\n";
    // 1) GroundRobot: move 1,0,0 in area without obstacle
    GroundRobot testGR("GR_TEST", Vehicle::Position{10,10,0}, 20.0);
    std::cout << "Test GR move in free area:\n";
    testGR.status();
    testGR.move(1.0, 0.0, 0.0);
    testGR.status();

    // 2) GroundRobot: move towards obstacle
    GroundRobot testGR2("GR_OBS", Vehicle::Position{2.9,2.9,0}, 50.0);
    std::cout << "Test GR move toward obstacle (should detect):\n";
    testGR2.status();
    testGR2.move(1.0, 0.0, 0.0); // obstacle at (3,3)
    testGR2.status();

    // 3) AerialDrone: move(targetPosition) distance battery drain
    AerialDrone testAD("AD_TEST", Vehicle::Position{0,0,1}, 10.0);
    std::cout << "Test AD move to target (check battery drain / fallback):\n";
    testAD.status();
    testAD.move(Vehicle::Position{2.0,2.0,4.0});
    testAD.status();

    // 4) Fleet mission already logged above

    std::cout << "\n=== End of simulation ===\n";
    return 0;
}