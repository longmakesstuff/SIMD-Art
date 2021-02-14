#include <array>
#include <SFML/Graphics.hpp>

int main() {

    const float start = 100;
    const float width = 200;

    sf::RenderWindow win{ sf::VideoMode{ 600, 600 }, "using VertexBuffer" };
    win.setFramerateLimit( 60 );

    sf::VertexBuffer v_buffer{ sf::Points, sf::VertexBuffer::Static };
    v_buffer.create( 4 );

    std::array< sf::Vertex, 4 > v_arr;
    v_arr.at( 0 ).position = sf::Vector2f{ start, start + width };
    v_arr.at( 1 ).position = sf::Vector2f{ start, start };
    v_arr.at( 2 ).position = sf::Vector2f{ start + width, start };
    v_arr.at( 3 ).position = sf::Vector2f{ start + width, start + width };

    v_arr.at( 0 ).color = sf::Color::Blue;
    v_arr.at( 1 ).color = sf::Color::Red;
    v_arr.at( 2 ).color = sf::Color::Green;
    v_arr.at( 3 ).color = sf::Color::White;

    v_buffer.update( v_arr.data() );

    while( win.isOpen() )
    {
        sf::Event event;
        while( win.pollEvent( event ) )
        {
            if ( event.type == sf::Event::Closed ) win.close();
            if ( ( event.type == sf::Event::KeyPressed ) && ( event.key.code == sf::Keyboard::Escape ) ) win.close();
        }

        win.clear();
        win.draw( v_buffer );
        win.display();
    }
}