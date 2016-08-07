//
//  Author: Shervin Aflatooni
//

#include "Sphere.h"

#include "../Engine.h"

Sphere::Sphere(float radius)
{
  m_radius = radius;
  m_properties["radius"] = Property{FLOAT, &m_radius, 0, 10};
}

Sphere::~Sphere(void)
{
}

void Sphere::registerWithEngine(Engine *engine)
{
  engine->getPhysicsManager()->registerCollider(this);
}

void Sphere::deregisterFromEngine(Engine *engine)
{
  engine->getPhysicsManager()->deregisterCollider(this);
}

float Sphere::getRadius(void) const
{
  return m_radius;
}
