
// Prototype Design Patter In C++
//   For following cases we can go for the Prototype Design Pattern
//      1. Creating an object is expensive operation than copy an object.
//      2. All Objects Initial State is common and takes time to build.
//      3. Composition, creation and representation of objects
//          should be decoupled from the system.
//      4. Hide the complexity of creating new instance from user
//          Which classes to create are specified at runtime.

#include <iostream>
#include <map>
#include <string>
#include <memory>
using namespace std;

/** Bullet is the base Prototype */
class Bullet
{
protected:
    string _bulletName;
    float _speed;
    float _firePower;
    float _damagePower;
    float _direction;

public:
    Bullet() {}
    Bullet(string bulletName, float speed, float firePower, float damagePower)
        : _bulletName(bulletName), _speed(speed), _firePower(firePower), _damagePower(damagePower) {}

    // Virtual function of the bullet should be redefined into derived class.
    virtual unique_ptr<Bullet> clone() = 0;
    void fire(float direction)
    {
        _direction = direction;

        cout << "Name        : " << _bulletName << endl
             << "Speed       : " << _speed << endl
             << "FirePower   : " << _firePower << endl
             << "DamagePower : " << _damagePower << endl
             << "Direction   : " << _direction << endl
             << endl;
    }
    virtual ~Bullet() {}
};

/** ConcretSimpleBullet is a Concrete Prototype */
class ConcretSimpleBullet : public Bullet
{

public:
    ConcretSimpleBullet(string bulletName, float speed, float firePower, float damagePower) : Bullet(bulletName, speed, firePower, damagePower)
    {
    }

    unique_ptr<Bullet> clone() override
    {
        return make_unique<ConcretSimpleBullet>(*this);
    }
};

/** ConcretGoodBullet is the Concrete Prototype */
class ConcretGoodBullet : public Bullet
{

public:
    ConcretGoodBullet(string bulletName, float speed, float firePower, float damagePower)
        : Bullet(bulletName, speed, firePower, damagePower)
    {
    }

    unique_ptr<Bullet> clone() override
    {
        return make_unique<ConcretGoodBullet>(*this);
    }
};

/** Opaque Bullet type, avoids exposing concrete implementations */
enum BulletType
{
    SIMPLE,
    GOOD
};

/** BulletFactory is the client */
class BulletFactory
{
private:
    map<BulletType, Bullet *> m_Bullets;

public:
    BulletFactory()
    {
        m_Bullets[SIMPLE] = new ConcretSimpleBullet("Simple Bullet", 50, 75, 75);
        m_Bullets[GOOD] = new ConcretGoodBullet("Good Bullet", 75, 100, 100);
    }

    unique_ptr<Bullet> createBullet(BulletType bulletType)
    {
        map<BulletType, Bullet *>::iterator it = m_Bullets.begin();

        return m_Bullets[bulletType]->clone();
    }
};

int main()
{
    BulletFactory bulletFactory;

    auto Bullet = bulletFactory.createBullet(SIMPLE);
    Bullet->fire(90);

    Bullet = bulletFactory.createBullet(GOOD);
    Bullet->fire(100);
}
