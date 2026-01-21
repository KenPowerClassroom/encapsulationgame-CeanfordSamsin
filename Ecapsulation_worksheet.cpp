#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

class Weapon {
private:
    std::string name;
    int damage;

public:
    Weapon(const std::string& weaponName, int weaponDamage)
        : name(weaponName), damage(weaponDamage) {
    }

    std::string getName() const { return name; }
    int getDamage() const { return damage; }

    void setDamage(int newDamage) { damage = newDamage; }
};

class Character {
protected:
    std::string name;
    int health;

private:
    int strength;
    Weapon* currentWeapon;

public:
    Character(const std::string& characterName, int characterHealth, int characterStrength)
        : name(characterName),
        health(characterHealth),
        strength(characterStrength),
        currentWeapon(nullptr) {
    }

    Weapon* getWeapon() const { return currentWeapon; }

    void equipWeapon(Weapon* weapon) 
    {
        currentWeapon = weapon;
    }

    int getStrength() const { return strength; }

    std::string getName() const { return name; }

    int getHealth() const { return health; }

    void setHealth(int newHealth) { health = newHealth; }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;

        std::cout << name << "take damage " << damage << "\n";
    }

    bool isAlive() const { return health > 0; }

    void attack(Character& target) {
        if (currentWeapon) {
            applyDamage(target, currentWeapon->getDamage() * strength);
        }
    }

protected:
    void applyDamage(Character& target, int damage) {
        std::cout << name << " attacks " << target.getName() << " with " << currentWeapon->getName() << "\n";
        target.takeDamage(damage);
        std::cout << target.getName() << " health: " << target.getHealth() << "\n";
    }
};

class Player : public Character {
public:
    Player(const std::string& playerName, int playerHealth, int characterStrength)
        : Character(playerName, playerHealth, characterStrength) {
    }

    void heal(int amount) {
        setHealth(getHealth() + amount);
        std::cout << getName() << " healed by " << amount << " points.\n";
    }

    void healRandomly() {
        int healAmount = std::rand() % 50 + 1;
        heal(healAmount);
    }

    void fight(Character& enemy) {
        attack(enemy);
        healRandomly();
    }
};

class Enemy : public Character {
public:
    Enemy(const std::string& EnemyName, int EnemyHealth, int characterStrength)
        : Character(EnemyName, EnemyHealth, characterStrength) {
    }

    void fight(Character& player) {
        attack(player);
    }
};

class WeaponManager {
private:
    std::vector<Weapon> weapons;
public:
    void addWeapon(const Weapon& weapon) {
        weapons.push_back(weapon);
    }

    Weapon* getWeapon(int index) {
        if (index >= 0 && index < weapons.size())
            return &weapons[index];
        return nullptr;
    }

    Weapon* equipRandomWeapon(Character& character) {
        if (weapons.empty()) return nullptr;
        Weapon* selectedWeapon = &weapons[std::rand() % weapons.size()];
        character.equipWeapon(selectedWeapon);
        return selectedWeapon;
    }
};

class GameManager {
private:
    Player player;
    Enemy enemy;
    WeaponManager weaponManager;

    void fightRound() {
        player.fight(enemy);
        if (enemy.isAlive()) {
            enemy.fight(player);
        }
    }

public:
    GameManager(const Player& p, const Enemy& e)
        : player(p), enemy(e) {
        std::srand(std::time(0));
    }

    void addWeapon(const Weapon& weapon) {
        weaponManager.addWeapon(weapon);
    }

    int startGame() {
        std::cout << "Game started: " << player.getName() << " vs " << enemy.getName() << "\n";

        while (player.isAlive() && enemy.isAlive()) {
            fightRound();
        }

        if (!player.isAlive()) {
            std::cout << player.getName() << " has been defeated.\n";
            return 1;
        }
        else {
            std::cout << enemy.getName() << " has been defeated.\n";
            return 0;
        }
    }

    void equipPlayerWeapon(int weaponIndex) {
        Weapon* weapon = weaponManager.getWeapon(weaponIndex);
        if (weapon) player.equipWeapon(weapon);
    }

    void equipEnemyWeapon(int weaponIndex) {
        Weapon* weapon = weaponManager.getWeapon(weaponIndex);
        if (weapon) enemy.equipWeapon(weapon);
    }

    void equipRandomWeapon(Character& character) {
        weaponManager.equipRandomWeapon(character);
    }
};

// Main Function
int main() {

    Player player("Hero", 300, 2);
    Enemy enemy("Goblin", 150, 4);

    Weapon sword("Sword", 15);
    Weapon axe("Axe", 20);
    Weapon dagger("Dagger", 10);
    Weapon bow("Bow", 25);

    GameManager game(player, enemy);

    game.addWeapon(sword);
    game.addWeapon(axe);
    game.addWeapon(dagger);
    game.addWeapon(bow);

    // Equip weapons
    game.equipPlayerWeapon(0); // Equip sword to player
    game.equipEnemyWeapon(1);  // Equip axe to enemy

    game.startGame();

    return 0;
}
