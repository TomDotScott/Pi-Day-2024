#include <iostream>
#include <SFML/Graphics.hpp>
#include <complex>

class Circle final : public sf::CircleShape
{
public:
    explicit Circle(const float curvature, float x, float y) :
		m_curvature(curvature)
    {
        setRadius(std::abs(1.f / m_curvature));

        setOrigin(getRadius(), getRadius());

        m_centre = std::complex<float>(x, y);

        setPosition({ m_centre.real(), m_centre.imag() });
        setFillColor(sf::Color::Transparent);
        setOutlineColor(sf::Color::Black);
        setOutlineThickness(1.f);
    }

    float GetCurvature() const
    {
        return m_curvature;
    }

    std::complex<float> GetCentre() const
    {
        return m_centre;
    }

private:
    float m_curvature;
    std::complex<float> m_centre;
};

struct DescartesValue
{
    float m_PositiveValue;
    float m_NegativeValue;
};

DescartesValue Descartes(const Circle& c1, const Circle& c2, const Circle& c3)
{
    const float k1 = c1.GetCurvature();
    const float k2 = c2.GetCurvature();
    const float k3 = c3.GetCurvature();

    const float sum = k1 + k2 + k3;
    const float root = std::sqrt(k1 * k2 + k2 * k3 + k1 * k3);

    return { sum + 2 * root, sum - 2 * root };
}

struct ComplexDescartesValue
{
    std::pair<std::complex<float>, std::complex<float>> m_k0;
    std::pair<std::complex<float>, std::complex<float>> m_k1;
};

ComplexDescartesValue ComplexDescartes(const Circle& c1, const Circle& c2, const Circle& c3, const DescartesValue& k4)
{
    const std::complex<float> k1 = c1.GetCurvature();
    const std::complex<float> k2 = c2.GetCurvature();
    const std::complex<float> k3 = c3.GetCurvature();
    const std::complex<float> z1 = c1.GetCentre();
    const std::complex<float> z2 = c2.GetCentre();
    const std::complex<float> z3 = c3.GetCentre();

    const std::complex<float> zk1 = z1 * k1;
    const std::complex<float> zk2 = z2 * k2;
    const std::complex<float> zk3 = z3 * k3;
    const std::complex<float> sum = zk1 + zk2 + zk3;

    std::complex<float> root = zk1 * zk2 + zk2 * zk3 + zk1 * zk3;
    root = std::sqrt(root) * 2.f;

    return {
        { (sum + root) / k4.m_PositiveValue, (sum - root) / k4.m_PositiveValue },
        { (sum + root) / k4.m_NegativeValue, (sum - root) / k4.m_NegativeValue }
    };
}

std::vector<Circle> GenerateCircles(const DescartesValue& dv, const ComplexDescartesValue& cdv)
{
	const std::complex<float> centre1 = cdv.m_k0.first;
	const std::complex<float> centre2 = cdv.m_k0.second;
	const std::complex<float> centre3 = cdv.m_k1.first;
	const std::complex<float> centre4 = cdv.m_k1.second;

    return {
        Circle(dv.m_PositiveValue, centre1.real(), centre1.imag()),
        Circle(dv.m_PositiveValue, centre2.real(), centre2.imag()),
        Circle(dv.m_NegativeValue, centre3.real(), centre3.imag()),
        Circle(dv.m_NegativeValue, centre4.real(), centre4.imag()),
    };
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(400, 400), "Apollonian Gasket");

    window.setFramerateLimit(60);

    Circle c1(-1.f / 200.f, 200, 200.f);
    Circle c2(1.f / 100.f, 100.f, 200.f);
    Circle c3(1.f / 100.f, 300.f, 200.f);

    auto k4 = Descartes(c1, c2, c3);
    auto circles = GenerateCircles(k4, ComplexDescartes(c1, c2, c3, k4));

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        window.draw(c1);
        window.draw(c2);
        window.draw(c3);

        for (const auto & circle : circles)
        {
            window.draw(circle);
        }

        window.display();
    }
}