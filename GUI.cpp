#include "GUI.h"

constexpr int32_t WIDGET_POS_X = WINDOW_WIDTH + 5;
constexpr int32_t WIDGET_WIDTH = 350;

GUI::GUI(sf::RenderWindow *window, sf::Font *font, tgui::GuiSFML *widgets) : window(window), font(font),
                                                                             widgets(widgets) {


    int32_t current_widget_pos = 0;

    auto get_next_label_position = [&current_widget_pos]() mutable ->int32_t {
        current_widget_pos += 50;
        return current_widget_pos;
    };

    auto get_next_widget_position = [&current_widget_pos]() mutable ->int32_t {
        current_widget_pos += 20;
        return current_widget_pos;
    };

    // Texture mapping
    texture_mapping = tgui::CheckBox::create("Texture mapping");
    texture_mapping->setPosition(WIDGET_POS_X, get_next_widget_position());
    texture_mapping->setChecked(field.isTextureMapping());
    texture_mapping->onChange([&](){
        field.setTextureMapping(texture_mapping->isChecked());
    });

    // Minimal distance
    minimal_distance_label = tgui::Label::create("Minimal distance");
    minimal_distance_label->setPosition(WIDGET_POS_X, get_next_label_position());
    minimal_distance_label->setWidth(WIDGET_WIDTH);

    minimal_distance = tgui::Slider::create(5, 150);
    minimal_distance->setStep(2);
    minimal_distance->setPosition(WIDGET_POS_X, get_next_widget_position());
    minimal_distance->setHeight(10);
    minimal_distance->setWidth(WIDGET_WIDTH);
    minimal_distance->setValue(field.getMinimalDistance());
    minimal_distance->onValueChange([&]() {
        field.setMinimalDistance(minimal_distance->getValue());
    });

    // Drag coefficient
    drag_coefficient_label = tgui::Label::create("Drag coefficient");
    drag_coefficient_label->setPosition(WIDGET_POS_X, get_next_label_position());
    drag_coefficient_label->setWidth(WIDGET_WIDTH);

    drag_coefficient = tgui::Slider::create(0.01, 0.1);
    drag_coefficient->setStep(0.01);
    drag_coefficient->setPosition(WIDGET_POS_X, get_next_widget_position());
    drag_coefficient->setHeight(10);
    drag_coefficient->setWidth(WIDGET_WIDTH);
    drag_coefficient->setValue(field.getDragCoefficient());
    drag_coefficient->onValueChange([&]() {
        field.setDragCoefficient(drag_coefficient->getValue());
    });

    // Mouse mass
    mouse_mass_label = tgui::Label::create("Mouse's mass");
    mouse_mass_label->setPosition(WIDGET_POS_X, get_next_label_position());
    mouse_mass_label->setWidth(WIDGET_WIDTH);

    mouse_mass = tgui::Slider::create(10000000.0f, 1000000000.0f);
    mouse_mass->setStep(10000000.0f);
    mouse_mass->setPosition(WIDGET_POS_X, get_next_widget_position());
    mouse_mass->setHeight(10);
    mouse_mass->setWidth(WIDGET_WIDTH);
    mouse_mass->setValue(field.getMouseMass());
    mouse_mass->onValueChange([&]() {
        field.setMouseMass(mouse_mass->getValue());
    });

    // Particles mass
    particles_mass_label = tgui::Label::create("Particles' mass");
    particles_mass_label->setPosition(WIDGET_POS_X, get_next_label_position());
    particles_mass_label->setWidth(WIDGET_WIDTH);

    particles_mass = tgui::Slider::create(100.0f, 10000.0f);
    particles_mass->setStep(500.0f);
    particles_mass->setPosition(WIDGET_POS_X, get_next_widget_position());
    particles_mass->setHeight(10);
    particles_mass->setWidth(WIDGET_WIDTH);
    particles_mass->setValue(field.getParticleMass());
    particles_mass->onValueChange([&]() {
        field.setMouseMass(particles_mass->getValue());
    });

    // List box
    textures_label = tgui::Label::create("Texture list");
    textures_label->setPosition(WIDGET_POS_X, get_next_label_position());
    textures_label->setWidth(WIDGET_WIDTH);

    textures = tgui::ListBox::create();
    textures->setPosition(WIDGET_POS_X, get_next_widget_position());
    textures->setWidth(WIDGET_WIDTH);
    textures->setSelectedItem(field.getTextureFile());
    textures->onItemSelect([&](){
        field.setTextureFile(textures->getSelectedItem().toStdString());
    });

    // Search for every texture
    for (const auto & entry : std::filesystem::directory_iterator(".")) {
        std::string file_path = entry.path().string();
        if(file_path.ends_with(".png") || file_path.ends_with(".jpg") || file_path.ends_with(".jpeg")){
            textures->addItem(file_path);
        }
    }

    widgets->add(texture_mapping);
    widgets->add(minimal_distance_label);
    widgets->add(minimal_distance);
    widgets->add(drag_coefficient_label);
    widgets->add(drag_coefficient);
    widgets->add(mouse_mass_label);
    widgets->add(mouse_mass);
    widgets->add(particles_mass_label);
    widgets->add(particles_mass);
    widgets->add(textures_label);
    widgets->add(textures);
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