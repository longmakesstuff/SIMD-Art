#include "GUI.h"

constexpr int32_t WIDGET_POS_X = WINDOW_WIDTH + 5;
constexpr int32_t WIDGET_WIDTH = 350;

GUI::GUI(sf::RenderWindow *window, sf::Font *font, tgui::GuiSFML *widgets) : window(window), font(font),
                                                                             widgets(widgets) {


    int32_t current_widget_pos = 0;

    auto get_next_label_position = [&current_widget_pos]() mutable ->int32_t {
        current_widget_pos += 35;
        return current_widget_pos;
    };

    auto get_next_widget_position = [&current_widget_pos]() mutable ->int32_t {
        current_widget_pos += 20;
        return current_widget_pos;
    };

    texture_mapping = tgui::CheckBox::create("Texture mapping");
    texture_mapping->setPosition(WIDGET_POS_X, get_next_widget_position());
    texture_mapping->setChecked(field.isTextureMapping());
    texture_mapping->onChange([&](){
        field.setTextureMapping(texture_mapping->isChecked());
    });

    minimal_distance_label = tgui::Label::create("Minimal distance");
    minimal_distance_label->setPosition(WIDGET_POS_X, get_next_label_position());
    minimal_distance_label->setWidth(WIDGET_WIDTH);

    minimal_distance = tgui::Slider::create(5, 100);
    minimal_distance->setStep(2);
    minimal_distance->setPosition(WIDGET_POS_X, get_next_widget_position());
    minimal_distance->setHeight(10);
    minimal_distance->setWidth(WIDGET_WIDTH);
    minimal_distance->setValue(field.getMinimalDistance());
    minimal_distance->onValueChange([&]() {
        field.setMinimalDistance(minimal_distance->getValue());
    });

    widgets->add(texture_mapping);
    widgets->add(minimal_distance_label);
    widgets->add(minimal_distance);
}


void GUI::info_text() {
    std::stringstream ss = std::stringstream{};
    fps.update();
    ss << "FPS: " << fps.getFPS();
    sf::Text fpsText{ss.str(), *font, 10};
    fpsText.setPosition(10, 10);
    fpsText.setFillColor(sf::Color::Black);
    window->draw(fpsText);
}

void GUI::main_loop() {
    sf::Event event;
    while (window->isOpen()) {
        while (window->pollEvent(event)) {
            widgets->handleEvent(event);
            if (event.type == sf::Event::EventType::Closed) {
                std::exit(0);
            }
            if (event.type == sf::Event::EventType::MouseButtonPressed) {
                if (!field.isMousePressed()) {
                    field.setMousePressed(true);
                }
            }
            if (event.type == sf::Event::EventType::MouseButtonReleased) {
                if (field.isMousePressed()) {
                    field.setMousePressed(false);
                }
            }
        }

        // Turn on this if your CPU does not support the operation
        //naive_simulate();
        if (field.isTextureMapping()) {
            window->clear(sf::Color::White);
        } else {
            window->clear(sf::Color::Black);
            sf::RectangleShape rectangle;
            rectangle.setFillColor(sf::Color::White);
            rectangle.setPosition(WINDOW_WIDTH, 0);
            rectangle.setSize(sf::Vector2f{400, WINDOW_HEIGHT});
            window->draw(rectangle);
        }

        widgets->draw();
        info_text();
        this->field.run(this->window);
    }
}