#ifndef MATRICES_H
#define MATRICES_H
#include <SFML/Graphics/Transform.hpp>

sf::Transform translation_mat(float, float);

sf::Transform scale_mat(float);

sf::Transform rotation_mat(float, float, float);

#endif /* MATRICES_H */
