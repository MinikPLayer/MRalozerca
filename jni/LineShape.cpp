#include <SFML/Graphics.hpp>
#include <cmath>

class sfLine : public sf::Drawable
{
public:
    sfLine(sf::Vector2f point1t, sf::Vector2f point2t) :
        color(sf::Color::Yellow), thickness(5.f)
    {
        point1 = point1t;
        point2 = point2t;
        sf::Vector2f direction = point2 - point1;
        sf::Vector2f unitDirection = direction / std::sqrt(direction.x*direction.x + direction.y*direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

        sf::Vector2f offset = (thickness / 2.f)*unitPerpendicular;

        vertices[0].position = point1 + offset;
        vertices[1].position = point2 + offset;
        vertices[2].position = point2 - offset;
        vertices[3].position = point1 - offset;

        for (int i = 0; i<4; ++i)
            vertices[i].color = color;
    }

    sfLine(sf::Vector2i point1t, sf::Vector2i point2t) :
        color(sf::Color::Yellow), thickness(5.f)
    {
        point1.x = point1t.x;
        point2.x = point2t.x;
        point1.y = point1t.y;
        point2.y = point2t.y;
        sf::Vector2f direction = point2 - point1;
        sf::Vector2f unitDirection = direction / std::sqrt(direction.x*direction.x + direction.y*direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

        sf::Vector2f offset = (thickness / 2.f)*unitPerpendicular;

        vertices[0].position = point1 + offset;
        vertices[1].position = point2 + offset;
        vertices[2].position = point2 - offset;
        vertices[3].position = point1 - offset;

        for (int i = 0; i<4; ++i)
            vertices[i].color = color;
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.draw(vertices, 4, sf::Quads);
    }
    void setThickness(float thicknessT)
    {
        thickness = thicknessT;
        sf::Vector2f direction = point2 - point1;
        sf::Vector2f unitDirection = direction / std::sqrt(direction.x*direction.x + direction.y*direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

        sf::Vector2f offset = (thickness / 2.f)*unitPerpendicular;

        vertices[0].position = point1 + offset;
        vertices[1].position = point2 + offset;
        vertices[2].position = point2 - offset;
        vertices[3].position = point1 - offset;
    }
    void setColor(sf::Color colorT)
    {
        color = colorT;
        for (int i = 0; i<4; ++i)
            vertices[i].color = color;
    }


private:
    sf::Vertex vertices[4];
    float thickness;
    sf::Color color;
    sf::Vector2f point1;
    sf::Vector2f point2;
};
