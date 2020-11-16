#include <Breakout/GameObject.hpp>


namespace Breakout
{

    GameObject::GameObject(
        glm::vec2 const & position,
        glm::vec2 const & velocity,
        glm::vec2 const & size,
        Texture2D texture,
        glm::vec3 const & color,
        bool isSolid
    )
        : Position(position)
        , Velocity(velocity)
        , Size(size)
        , Texture(texture)
        , Color(color)
        , IsSolid(isSolid)
    { }

    void GameObject::Draw(SpriteRenderer & renderer)
    {
        renderer.DrawSprite(Texture, Position, Size, 0.0, Color);
    }

}