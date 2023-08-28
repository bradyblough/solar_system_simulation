#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>

const int windowWidth = 1500;
const int windowHeight = 1000;
const float sunRadius = 60.0f;
const int numPlanets = 5;

struct Planet {
    float semiMajorAxis;
    float eccentricity;
    float angle;
    float angularVelocity;
    sf::CircleShape shape;
    sf::Text label;

    Planet(float a, float e, float initialAngle, sf::Color color, const std::string& planetName, const sf::Font& font)
        : semiMajorAxis(a), eccentricity(e), angle(initialAngle), angularVelocity(2.0f * M_PI / a) {
        shape.setRadius(10.0f);
        shape.setFillColor(color);
        float distance = semiMajorAxis * (1.0f - eccentricity);
        float xPos = windowWidth / 2 + distance * cos(angle);
        float yPos = windowHeight / 2 + distance * sin(angle);
        shape.setPosition(xPos, yPos);

        label.setFont(font);
        label.setCharacterSize(20);
        label.setString(planetName);
        label.setFillColor(sf::Color::White);
        label.setPosition(xPos + 20.0f, yPos - 15.0f);
    }

    void update(float deltaTime) {
        angle += angularVelocity * deltaTime;

        float distance = semiMajorAxis * (1.0f - eccentricity * cos(angle));
        float xPos = windowWidth / 2 + distance * cos(angle);
        float yPos = windowHeight / 2 + distance * sin(angle);
        shape.setPosition(xPos, yPos);
        label.setPosition(xPos + 20.0f, yPos - 15.0f);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Solar System Simulation");

    sf::CircleShape sun(sunRadius);
    sun.setFillColor(sf::Color::Yellow);
    sun.setPosition(windowWidth / 2 - sunRadius, windowHeight / 2 - sunRadius);

       sf::Font font;
    if (!font.loadFromFile("/System/Library/Fonts/Courier.ttc")) {
        return 1; 
    }

    Planet planets[numPlanets] = {
        {57.91f, 0.2056f, 0.0f, sf::Color(139, 139, 139), "Mercury", font},
        {108.2f, 0.0068f, 0.0f, sf::Color(218, 165, 32), "Venus", font},
        {149.6f, 0.0167f, 0.0f, sf::Color::Blue, "Earth", font},
        {227.9f, 0.0934f, 0.0f, sf::Color::Red, "Mars", font},
        {778.3f, 0.0485f, 0.0f, sf::Color(238, 180, 34), "Jupiter", font},
    };

    sf::Text daysText;
    daysText.setFont(font);
    daysText.setCharacterSize(24);
    daysText.setPosition(20.0f, 20.0f);
    daysText.setFillColor(sf::Color::White);

    sf::Clock clock;
    float days = 0.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        float deltaTime = clock.restart().asSeconds();
        days += deltaTime;
        for (int i = 0; i < numPlanets; ++i) {
            planets[i].update(deltaTime);
            if (days == 2){
                days = 0;
            }
    }
        window.clear();
        window.draw(sun);
        for (int i = 0; i < numPlanets; ++i) {
            window.draw(planets[i].shape);
            window.draw(planets[i].label);
        }

        daysText.setString("Days: " + std::to_string(static_cast<int>(days)));
        window.draw(daysText);

        window.display();
    }

    return 0;
}
