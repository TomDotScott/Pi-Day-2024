#include <SFML/Graphics.hpp>
#include <complex>

static constexpr float SCREEN_SIZE = 960.f;
static constexpr float SCREEN_SIZE_OVER_TWO = SCREEN_SIZE / 2.f;
static constexpr float SCREEN_SIZE_OVER_FOUR = SCREEN_SIZE_OVER_TWO / 2.f;
static constexpr float HACKY_EPSILON = SCREEN_SIZE / 1000.f;


float Distance(const sf::Vector2f& a, const sf::Vector2f& b)
{
	const sf::Vector2f c = b - a;
	return std::sqrt(c.x * c.x + c.y * c.y);
}

class Circle final
{
public:
	Circle(const float curvature, const float x, const float y) :
		m_curvature(curvature)
	{
		m_radius = std::abs(1.f / m_curvature);
		m_centre = std::complex<float>(x, y);
	}

	float GetCurvature() const
	{
		return m_curvature;
	}

	std::complex<float> GetCentre() const
	{
		return m_centre;
	}

	sf::Vector2f GetCentreAsVector() const
	{
		return { m_centre.real(), m_centre.imag() };
	}

	float GetRadius() const
	{
		return m_radius;
	}


private:
	float m_curvature;
	float m_radius;
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
	std::pair<std::complex<float>, std::complex<float>> m_PositiveValue;
	std::pair<std::complex<float>, std::complex<float>> m_NegativeValue;
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
	const std::complex<float> centre1 = cdv.m_PositiveValue.first;
	const std::complex<float> centre2 = cdv.m_PositiveValue.second;
	const std::complex<float> centre3 = cdv.m_NegativeValue.first;
	const std::complex<float> centre4 = cdv.m_NegativeValue.second;

	return {
		Circle(dv.m_PositiveValue, centre1.real(), centre1.imag()),
		Circle(dv.m_PositiveValue, centre2.real(), centre2.imag()),
		Circle(dv.m_NegativeValue, centre3.real(), centre3.imag()),
		Circle(dv.m_NegativeValue, centre4.real(), centre4.imag()),
	};
}

struct CircleTriplet
{
	Circle c1;
	Circle c2;
	Circle c3;
};

bool IsTangent(const Circle& c1, const Circle& c2)
{
	const float d = Distance(c1.GetCentreAsVector(), c2.GetCentreAsVector());
	const float r1 = c1.GetRadius();
	const float r2 = c2.GetRadius();
	const bool a = std::abs(d - (r1 + r2)) < HACKY_EPSILON;
	const bool b = std::abs(d - abs(r2 - r1)) < HACKY_EPSILON;
	return a || b;
}

bool ValidateCircle(const std::vector<Circle>& allCircles, const Circle& newCircle, const CircleTriplet& currentTriplet)
{
	if (newCircle.GetRadius() < HACKY_EPSILON * 2) return false;

	for (auto& other : allCircles)
	{
		const float dist = Distance(newCircle.GetCentreAsVector(), other.GetCentreAsVector());
		const float radiusDiff = abs(newCircle.GetRadius() - other.GetRadius());

		if (dist < HACKY_EPSILON && radiusDiff < HACKY_EPSILON)
		{
			return false;
		}
	}

	// Check if the circles are mutually tangential
	if (!IsTangent(newCircle, currentTriplet.c1)) return false;
	if (!IsTangent(newCircle, currentTriplet.c2)) return false;
	if (!IsTangent(newCircle, currentTriplet.c3)) return false;

	return true;
}

void AddTriplet(std::vector<Circle>& allCircles, std::vector<CircleTriplet>& circleQueue)
{
	std::vector<CircleTriplet> newVector;
	for (auto& triplet : circleQueue)
	{
		Circle& c1 = triplet.c1;
		Circle& c2 = triplet.c2;
		Circle& c3 = triplet.c3;

		DescartesValue k4 = Descartes(c1, c2, c3);
		ComplexDescartesValue complexDescartesValue = ComplexDescartes(c1, c2, c3, k4);

		auto newCircles = GenerateCircles(k4, complexDescartesValue);

		for (auto& new_ : newCircles)
		{
			if (ValidateCircle(allCircles, new_, triplet))
			{
				allCircles.emplace_back(new_);

				CircleTriplet newTriplet1{ c1, c2, new_ };
				CircleTriplet newTriplet2{ c1, c3, new_ };
				CircleTriplet newTriplet3{ c2, c3, new_ };

				newVector.push_back(newTriplet1);
				newVector.push_back(newTriplet2);
				newVector.push_back(newTriplet3);
			}
		}
	}

	circleQueue = newVector;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(static_cast<unsigned>(SCREEN_SIZE), static_cast<unsigned>(SCREEN_SIZE)), "Apollonian Gasket");

	window.setFramerateLimit(60);


	Circle c1(-1.f / SCREEN_SIZE_OVER_TWO, SCREEN_SIZE_OVER_TWO, SCREEN_SIZE_OVER_TWO);
	Circle c2(1.f / SCREEN_SIZE_OVER_FOUR, SCREEN_SIZE_OVER_FOUR, SCREEN_SIZE_OVER_TWO);
	Circle c3(1.f / SCREEN_SIZE_OVER_FOUR, SCREEN_SIZE_OVER_TWO + SCREEN_SIZE_OVER_FOUR, SCREEN_SIZE_OVER_TWO);

	std::vector<Circle> allCircles{ c1, c2, c3 };

	std::vector<CircleTriplet> circleQueue;
	circleQueue.push_back(CircleTriplet{ c1, c2, c3 });

	sf::CircleShape circleShape;

	circleShape.setFillColor(sf::Color::Transparent);
	circleShape.setOutlineColor(sf::Color::Black);
	circleShape.setOutlineThickness(1.f);

	while (window.isOpen())
	{
		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Key::Space)
				{
					AddTriplet(allCircles, circleQueue);
				}
			}
		}

		window.clear(sf::Color::White);

		for (const auto& circle : allCircles)
		{
			circleShape.setRadius(circle.GetRadius());
			circleShape.setPosition(circle.GetCentreAsVector());
			circleShape.setOrigin(circle.GetRadius(), circle.GetRadius());

			window.draw(circleShape);
		}

		window.display();
	}
}
